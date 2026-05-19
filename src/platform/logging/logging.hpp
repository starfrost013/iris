// 
// Starfrost Shared Logging System-NG (NextGen) Version 5.0
// Copyright © 2023-2026 starfrost
//
// This is a completely rewritten, C++ header only version of SSLS. 
// It adds the capacity to have custom channels, custom prefixes, custom date formats, stderr support and uses C++ concepts. 
// It almost eliminates fixed-sized buffers and uses streams. It's fully self-contained.
//
// For C projects, use SSLS 4.7
//

#pragma once

// Includes
#include <cstring>
#include <fstream>
#include <ostream>
#include <ios>
#include <iostream>
#include <vector>
#include <unordered_map>

// Defines

#define LOGGER_USE_NAMESPACE                                                    // Comment out to disable namespaces

#define LOGGER_NAMESPACE                Iris                                    // Namespace to use if namespace is enabled
#define LOGGER_MAX_PATH                 260                                     // Max path
#define LOGGER_MAX_STRING_SHORT         48                                      // Generic maximum length for "short" strings
#define LOGGER_MAX_STRING_LONG          256                                     // Generic maximum length for "long" strings

// Strings, change to localise or whatever

#define STRING_VERSION                  "5.0 (May 19, 2026)"                    // Version number as a string (we don't need it in any other form)
#define STRING_SIGN_ON                  "SSLS-NG (Starfrost Shared Logging System - Next Gen) " STRING_VERSION " initialised" 
#define STRING_ANSI_PREFIX              "\x1B["                                 // Some ANSI command prefixes use this

#ifdef LOGGER_USE_NAMESPACE
namespace LOGGER_NAMESPACE
{
#endif

    /// @brief Enumerates the available console colours
    enum ConsoleColor
    {
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		BrightBlack,
    	FirstBright = BrightBlack,
		BrightRed,
		BrightGreen,
		BrightYellow,
		BrightBlue,
		BrightMagenta,
		BrightCyan,
		BrightWhite,
    }; 

    /// @brief Defines a custom log channel that you can add.
    class LogChannel
    {
        friend class Logger;

        char name[LOGGER_MAX_STRING_SHORT];             // the name to use for this log channel.
        size_t mask;                                    // The mask to use for selecting this log channel.

        ConsoleColor colorFg;                           // The current foreground colour
        ConsoleColor colorBg;                           // The current background colour 

    public:
        LogChannel(const char* name, size_t mask, ConsoleColor colorFg = ConsoleColor::Black, ConsoleColor colorBg = ConsoleColor::White)
        {
            strncpy(this->name, name, LOGGER_MAX_STRING_SHORT);
            this->mask = mask; 
            this->colorBg = colorBg;
            this->colorFg = colorFg;
        }
    };

    /// @brief Built-in log channels
    enum LogChannels
    {
        Message = 1,                                    // A normal message.
        Debug = 1 << 1,                                 // A debug message. Not used on
        Warning = 1 << 2,                               // A warning message.
        Error = 1 << 3,                                 // An error message.
        FatalError = 1 << 4,                            // A fatal error message.
        UnsafeShutdown = 1 << 5,                        // The memory or some other critical resource is hosed.
        LastPrebuilt = ((UnsafeShutdown << 1) - 1),     // Sentinel value for custom channels
    };

    /// @brief Enumerates possible log destinations.
    enum LogDestination
    {
        Stdout = 1,                                     // Log to standard out.
        Stderr = 1 << 1,                                // Log to standard error.
        File = 1 << 2,                                  // Log to a log file.
        MaxValid = (File << 1) - 1,                     // Maximum valid mask
    }; 

    /// @brief The settings that the logging system will use. SET BEFORE CALLING Logging::Init!
    class LoggerSettings
    {
        friend class Logger;

    public:
    
        /// @brief Sets the application name; the application name is used as the default log file name
        /// @param appName The application name to set.
        void SetAppName(const char* appName) { strncpy(this->appName, appName, LOGGER_MAX_STRING_SHORT); };
        
        /// @brief Sets the date format; the default is ISO 8601.
        /// @param dateFormat The date format to set. If this method is nevr called, ISO 8601 date format is used.
        void SetDateFormat(const char* dateFormat) { strncpy(this->dateFormat, dateFormat, LOGGER_MAX_STRING_SHORT); };
        
        /// @brief Sets the mask of log destinations.
        /// @param destinations The mask of the log destinations to set. See the LogDesitnations enum for further information
        void SetDestinations(LogDestination destinations) { this->destinations = destinations; };
        
        /// @brief Sets the mask of log channels.
        /// @param channelMask  The mask of the log destinations to set. See the LogDesitnations enum for further information
        void SetChannelMask(LogChannels channelMask) { this->channelMask = channelMask; }; 

        /// @brief Set the filename for the logging system to use.
        /// @param fileName The filename to use.
        void SetFileName(const char* fileName) 
        { 
            strncpy(this->fileName, fileName, LOGGER_MAX_PATH); 
            overrideDefaultFileName = true; 
        };

        /// @brief Allows you to set the function that is run on a fatal error.
        /// @param fatalFunc Function pointer to the function that handles a fatal error.
        void SetFatalFunction(void (*fatalFunc)()) { this->fatalFunc = fatalFunc; }; 

        /// @brief Allows you to set a last-chance unsafe function run on a superfatal / unsafe termination error.
        /// @param lastChanceUnsafeFunc Function pointer to be run. NOTE: std::abort will be run RIGHT AFTER YOUR FUNCTION COMPLETES!
        void SetLastChanceUnsafeFunction(void (*lastChanceUnsafeFunc)()) { this->lastChanceUnsafeFunc = fatalFunc; }; 

        // These are safe to set directly
        bool hideSignOnMessage;
        bool hideDates;                                     // If the dates should be hidden or not.    

    private: 

        char appName[LOGGER_MAX_STRING_SHORT] = {0};    // Application name to use for log file
        void (*fatalFunc)();                                // Function to call on a fatal error log.
        void (*lastChanceUnsafeFunc)();                     // "Last chance" for unsafe function. Called right before std::abort so you cand o anything you can
        bool overrideDefaultFileName;                       // If true, override the default filename
        char fileName[LOGGER_MAX_PATH] = {0};           // If overridedefaultfilename is true, use this filename
        char dateFormat[LOGGER_MAX_STRING_SHORT] = {0}; // Optional date format string to use. Otherwise yyyy-mm-dd hh:mm:ss is useed

        std::ofstream logStream;                            // The straem to open the log if LogDestination has FILE
        LogDestination destinations;                        // The destinations to send the log
        LogChannels channelMask;                            // Enabled channel mask

        ConsoleColor colorForeground;                       // The current foreground colour
        ConsoleColor colorBackground;                       // The current background colour 
    }; 

    /// @brief Container for Logger static methods.
    class Logger
    {
    private: 

     /// @brief Maps console colours to ANSI escape codes for foreground colours
        inline static std::unordered_map<ConsoleColor, const char*> colorToAnsiTableFg =
        {
            { Black,    STRING_ANSI_PREFIX "30m" },
            { Red,      STRING_ANSI_PREFIX "31m" },
            { Green,    STRING_ANSI_PREFIX "32m" },
            { Yellow,   STRING_ANSI_PREFIX "33m" },
            { Blue,     STRING_ANSI_PREFIX "34m" },
            { Magenta,  STRING_ANSI_PREFIX "35m" },
            { Cyan,     STRING_ANSI_PREFIX "36m" },
            { White,    STRING_ANSI_PREFIX "37m" },
            { BrightBlack,    STRING_ANSI_PREFIX "90m" },
            { BrightRed,      STRING_ANSI_PREFIX "91m" },
            { BrightGreen,    STRING_ANSI_PREFIX "92m" },
            { BrightYellow,   STRING_ANSI_PREFIX "93m" },
            { BrightBlue,     STRING_ANSI_PREFIX "94m" },
            { BrightMagenta,  STRING_ANSI_PREFIX "95m" },
            { BrightCyan,     STRING_ANSI_PREFIX "96m" },
            { BrightWhite,    STRING_ANSI_PREFIX "97m" },
        };

        /// @brief Maps console colours to ANSI escape codes for background colours
        inline static std::unordered_map<ConsoleColor, const char*> colorToAnsiTableBg =
        {
            { Black,    STRING_ANSI_PREFIX "40m" },
            { Red,      STRING_ANSI_PREFIX "41m" },
            { Green,    STRING_ANSI_PREFIX "42m" },
            { Yellow,   STRING_ANSI_PREFIX "43m" },
            { Blue,     STRING_ANSI_PREFIX "44m" },
            { Magenta,  STRING_ANSI_PREFIX "45m" },
            { Cyan,     STRING_ANSI_PREFIX "46m" },
            { White,    STRING_ANSI_PREFIX "47m" },
            { BrightBlack,    STRING_ANSI_PREFIX "100m" },
            { BrightRed,      STRING_ANSI_PREFIX "101m" },
            { BrightGreen,    STRING_ANSI_PREFIX "102m" },
            { BrightYellow,   STRING_ANSI_PREFIX "103m" },
            { BrightBlue,     STRING_ANSI_PREFIX "104m" },
            { BrightMagenta,  STRING_ANSI_PREFIX "105m" },
            { BrightCyan,     STRING_ANSI_PREFIX "106m" },
            { BrightWhite,    STRING_ANSI_PREFIX "107m" },
        };

        inline static const char* colorResetFgStr = STRING_ANSI_PREFIX "39m";
        inline static const char* colorResetBgStr = STRING_ANSI_PREFIX "49m";
        
        inline static std::vector<LogChannel> customChannels;       // internal vector of custom channels 

        inline static bool initialised;                             // determines if we were initialised successfully

        /// @brief Internal method to send to multiple streams if we need to
        /// @param msg The message to send
        /// @param stream The std::ostream to send the msg too
        /// @param newline The line to send to.
        inline static void LogOutToStream(const char* msg, std::ostream* stream, bool newline = false)
        {
            *stream << msg;

            if (newline)
                *stream << std::endl;
        }

        /// @brief Send a message ot the log destination.
        /// @param msg The message to send.
        /// @param newline If a newline should be sent after.
        inline static void LogOut(const char* msg, bool newline = false)
        {
            if (settings.destinations & LogDestination::Stdout)
                LogOutToStream(msg, &std::cout, newline);
            
            if (settings.destinations & LogDestination::Stderr)
                LogOutToStream(msg, &std::cerr, newline);

            if (settings.destinations & LogDestination::File)
                LogOutToStream(msg, &settings.logStream, newline);
        }
       
    public: 
        inline static LoggerSettings settings;                // the settings

        inline static void Init()
        {
            customChannels = std::vector<LogChannel>();

            if (!settings.hideSignOnMessage)
                std::cout << STRING_SIGN_ON << std::endl;

            if (!settings.destinations)
            {
                std::cout << "SSLS Error 1: No destinations specified! (settings.destinations == 0)" << std::endl;   
                return; 
            }

            // check if the file exists
            if (settings.destinations & LogDestination::File)
            {
                if (!settings.fileName[0])
                {
                    if (!settings.appName[0])
                    {
                        std::cout << "SSLS Error 0: Did not set an app name or file name, so can't open a log file" << std::endl;   
                        settings.destinations = (LogDestination)(settings.destinations & ~(LogDestination::File)); // turn it off
                        goto skipfile; // don't even bother trying
                    }
                }
            }

            // copy the filename and append .log to it
            if (!settings.overrideDefaultFileName)
                snprintf(settings.fileName, LOGGER_MAX_PATH, "%s%s", settings.appName, ".log");

            if (settings.destinations & LogDestination::File)
            {
                settings.logStream.open(settings.fileName, std::ios_base::out | std::ios_base::trunc);

                if (settings.logStream.bad())
                {
                    // turn off file and report error
                    std::cout << "SSLS Error 2: Failed to open file at " << settings.fileName << std::endl;
                    settings.destinations = (LogDestination)(settings.destinations & ~(LogDestination::File));
                }
            }

        skipfile:
            initialised = true;
        }
        
        /// @brief Adds a custom channel to the logger.
        /// @param channel The custom channel object you want to add. 
        inline static void AddChannel(LogChannel channel)
        {
            customChannels.push_back(channel);
        }

        /// @brief Send a single message to the log. Also the internal log method called by all the others
        /// @param prefix Component prefix
        /// @param msg The message to send to the log
        /// @param channel The channel to send it to.
        /// @param sendChannelName if the channel name should be sent or not.
        /// @param newline Also send a newline.
        inline static void Log(const char* prefix, const char* msg, size_t channelMask = LogChannels::Message, 
            bool newline = true, bool sendChannelName = true, bool sendDate = true)
        {
            // check if the logging was actually initialised properly
            if (!initialised)
            {
                std::cout << "SSLS Error 3: Call Logger::Init! (Or it failed)" << std::endl;
                return;
            }

            // for easier checking
            size_t logDest = (size_t)(settings.destinations);
            
            // Keep the lines the same size
            // Message doesn't have one
            const char* messageNameStr = "";
            const char* debugNameStr   = "[DEBUG";
            const char* warningNameStr = "[WARN ";
            const char* errorNameStr   = "[ERROR";
            const char* fatalNameStr   = "[FATAL";
            const char* unsafeNameStr  =  "**** UNSAFE CONDITION - EXITING ****"; // this one doesn't matter

            // needs to be universal for all project
            #ifdef NDEBUG
            if (channelMask == LogChannels::Debug)
                return;
            #endif

            // send out an optional prefix
            if (prefix != nullptr)
            {
                LogOut("[");
                LogOut(prefix);
                LogOut("] ");
            }

            // send our new channel name
            if (sendChannelName)
            {
                if (channelMask & LogChannels::Message)
                    LogOut(messageNameStr);

            // universal to all projects
            #ifndef NDEBUG 
                if (channelMask & LogChannels::Debug)
                {
                    LogOut(colorToAnsiTableFg[ConsoleColor::BrightBlue]);
                    LogOut(debugNameStr);
                }
            #endif

                if (channelMask & LogChannels::Warning)
                {
                    LogOut(colorToAnsiTableFg[ConsoleColor::BrightYellow]);
                    LogOut(warningNameStr);
                }

                // these will all use the same colour
                if (channelMask & LogChannels::Error 
                || channelMask & LogChannels::FatalError
                || channelMask & LogChannels::UnsafeShutdown)
                {
                    LogOut(colorToAnsiTableFg[ConsoleColor::BrightRed]);
                }

                if (channelMask & LogChannels::Error)
                    LogOut(errorNameStr);
                if (channelMask & LogChannels::FatalError)
                    LogOut(fatalNameStr);
                if (channelMask & LogChannels::UnsafeShutdown)
                    LogOut(unsafeNameStr);

                // If there are some custom channels dump their names out
                if (channelMask > LogChannels::LastPrebuilt)
                {
                    for (LogChannel channel : customChannels)
                    {
                        if (((size_t)channelMask) & channel.mask)
                        {
                            LogOut(colorToAnsiTableFg[channel.colorFg]);
                            LogOut(colorToAnsiTableBg[channel.colorBg]);
                            LogOut(channel.name);
                        }
                    }
                }
            }
        
            // Log out the ] character

            bool hideDates = settings.hideDates;

            // local override in case we want to use multiple log calls for one message
            if (!sendDate)
                hideDates = true;

            if (settings.hideDates) // if only message don't bother
            {
                if (channelMask != LogChannels::Message)
                    LogOut("]");
            }
            else // also the date
            {
                // custom handling for only message
                if (channelMask == LogChannels::Message)
                    LogOut("[");
                else
                    LogOut("] [");

                std::time_t t = std::time(nullptr);
                std::tm tm = *std::localtime(&t);

                char dateTimeBuf[LOGGER_MAX_STRING_SHORT] = {0};

                // format the date
                if (!settings.dateFormat[0])
                    std::strftime(dateTimeBuf, sizeof(dateTimeBuf), "%F %T", &tm); // don't want day of the week
                else
                    std::strftime(dateTimeBuf, sizeof(dateTimeBuf), settings.dateFormat, &tm);

                // send it out

                LogOut(dateTimeBuf);
                LogOut("]");
            }

            if (sendDate || sendChannelName || prefix != nullptr)
                LogOut(": "); // Log out a colon if we need to
            
            // finally log the real message
            LogOut(msg);

            // can't hurt
            // don't need to do it if it's a message only log, it'll save a little bit of time, so may as well do it

            if (channelMask != LogChannels::Message)
            {
                LogOut(colorResetFgStr);
                LogOut(colorResetBgStr);
            }

            // done, dump newline
            if (newline)
                LogOut("", true);

            // It's a fatal so run the fatal function
            if (channelMask & LogChannels::FatalError)
            {
                // Striclty optional
                if (settings.fatalFunc)
                    settings.fatalFunc();
            }

            if (channelMask & LogChannels::UnsafeShutdown)
            {
                if (settings.lastChanceUnsafeFunc)
                    settings.lastChanceUnsafeFunc();

                // it's going down, it's going down!
                std::abort();
            }
        }

        /// @brief Send a single message to the log with no prefix.
        /// @param msg The mesasge to send to the log.
        /// @param channel The channel to send it to.
        /// @param sendChannelName if the channel name should be sent or not.
        /// @param newline Also send a newline.
        inline static void Log(const char* msg, size_t channels = LogChannels::Message, bool newline = true, bool sendChannelName = true,
        bool sendDate = true)
        {
            Log(nullptr, msg, channels, newline, sendChannelName, sendDate);
        }

        /// @brief Shuts down the logging system
        inline static void Shutdown()
        {
            if (settings.destinations & LogDestination::File)
                settings.logStream.close();

            initialised = false;
        }
    }; 

#ifdef LOGGER_USE_NAMESPACE
}
#endif