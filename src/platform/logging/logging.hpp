// 
// Starfrost Shared Logging System-NG (NextGen) Version 5.0
// Copyright © 2023-2026 starfrost
//
// This is a completely rewritten, C++ header only version of SSLS. 
// It adds the capacity to have custom channels, custom prefixes, custom date formats, stderr suppot and uses C++ concepts. 
//
// For C projects, use SSLS 4.7
//

#pragma once

// Includes
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <ios>
#include <iostream>
#include <vector>
#include <unordered_map>

// Defines

#define LOGGER_USE_NAMESPACE                                                    // Comment out to disable namespaces

#define LOGGER_NAMESPACE                Iris                                    // Namespace to use if namespace enabled
#define LOGGER_MAX_PATH                 260                                     // Max path
#define LOGGER_MAX_STRING_SHORT         48                                      // Generic maximum length for "short" strings
#define LOGGER_MAX_STRING_LONG          256                                     // Generic maximum length for "long" strings

// Strings, change to localise or whatever

#define STRING_VERSION                  "5.0 (May 18, 2026)"                    // Version number as a string (we don't need it in any other form)
#define STRING_SIGN_ON                  "SSLS-NG (Starfrost Shared Logging System - Next Gen) " STRING_VERSION " initialised" 

#define CONSOLE_TERMINAL_COMMAND_PREFIX "\x1B["                                 // Some console terminal command prefixes use this

#ifdef LOGGER_USE_NAMESPACE
namespace LOGGER_NAMESPACE
{
#endif

    /// @brief A logger channel.
    class LogChannel
    {
        friend class Logger;

        char name[LOGGER_MAX_STRING_SHORT];     // the name to use for this log channel.
        size_t mask;                            // The mask to use for selecting this log channel.

    public:
        LogChannel(const char* name, size_t mask)
        {
            strncpy(this->name, name, LOGGER_MAX_STRING_SHORT);
            this->mask = mask; 
        }
    };

    /// @brief Prebuilt log channels
    enum LogChannels
    {
        Message = 1,                                    // A normal message.
        Warning = 2,                                    // A warning message.
        Error = 4,                                      // An error message.
        FatalError = 8,                                 // A fatal error message.
        UnsafeShutdown = 16,                            // The memory or some other critical resource is hosed.
        LastPrebuilt = ((UnsafeShutdown << 1) - 1),     // Sentinel value for custom channels
    };

    /// @brief Enumerates possible log destinations.
    enum LogDestination
    {
        Stdout = 1,                                     // Log to standard out.
        Stderr = 1 << 1,                                // Log to standard error.
        File = 1 << 2,                                  // Log to a log file.
        MaxValid = (File << 1) - 1,                     // Log to file
    }; 

    class LoggerSettings
    {
        friend class Logger;

    public:
        void SetAppName(const char* appName) { strncpy(this->appName, appName, sizeof(appName)); };
        void SetDestinations(LogDestination destinations) { this->destinations = destinations; };
        void SetChannelMask(LogChannels channelMask) { this->channelMask = channelMask; }; 

        /// @brief Set the filename for the logging system to use.
        /// @param fileName The filename to use.
        void SetFileName(const char* fileName) 
        { 
            strncpy(this->appName, appName, sizeof(appName)); 
            overrideDefaultFileName = true; 
        };


    private: 

        char appName[LOGGER_MAX_STRING_SHORT] = {0};    // Application name to use for log file
        void (*fatalFunc)();                                // Function to call on a fatal error log.
        void (*lastChanceUnsafeFunc)();                     // "Last chance" for unsafe function
        bool overrideDefaultFileName;                       // If true, override the default filename
        char fileName[LOGGER_MAX_PATH] = {0};           // If overridedefaultfilename is true, use this filename
        bool hideDates;                                     // If the dates should be hidden or not.    
        char dateFormat[LOGGER_MAX_STRING_SHORT] = {0}; // Optional date format string to use. Otherwise yyyy-mm-dd hh:mm:ss is useed

        bool hideSignOnMessage;
        std::ofstream logStream;                             // The straem to open the log if LogDestination has FILE
        LogDestination destinations;                        // The destinations to send the log
        LogChannels channelMask;                            // Enabled channel mask
    }; 


    class Logger
    {
    private: 
        inline static std::vector<LogChannel> customChannels;       // custom channels 

        inline static bool initialised;                             // determines if we were initialised successfully

        /// @brief Send a message ot the log destination.
        /// @param msg The message to send.
        /// @param newline If a newline should be sent after.
        void LogOut(const char* msg, bool newline = false)
        {
            std::ostream* stream;

            if (settings.destinations & LogDestination::Stdout)
                stream = &std::cout;
            
            if (settings.destinations & LogDestination::Stderr)
                stream = &std::cerr;

            if (settings.destinations & LogDestination::File)
                stream = &settings.logStream;

            // go 
            *stream << msg;

            // send newline if we want
            if (newline)
                *stream << std::endl;
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
                settings.logStream.open(settings.fileName, std::ios_base::in | std::ios_base::out);

                if (settings.logStream.badbit)
                {
                    // turn off file and report error
                    std::cout << "SSLS Error 2: Failed to open file at " << settings.fileName << std::endl;
                    settings.destinations = (LogDestination)(settings.destinations & ~(LogDestination::File));
                }
            }

        skipfile:
            initialised = true;
        }

        /// @brief Send a single message to a destination. The internal log method called by all the others
        /// @param msg The mesasge to send
        /// @param channel The channel to send it to.
        /// @param sendChannelName if the channel name should be sent or not.
        /// @param newline Also send a newline.
        void Log(const char* msg, size_t channels, bool newline = true, bool sendChannelName = true)
        {
            // for easier checking
            size_t logDest = (size_t)(settings.destinations);
            
            // Keep the lines the same size
            // Message doesn't have one
            const char* messageNameStr = "[";
            const char* warningNameStr = "[WARNING ";
            const char* errorNameStr   = "[ERROR]  ";
            const char* fatalNameStr   = "[FATAL]  ";
            const char* unsafeNameStr  =  "**** UNSAFE CONDITION - EXITING ****"; // this one doesn't matter

            // send our new channel name
            if (sendChannelName)
            {
                if (channels & LogChannels::Message)
                    LogOut(messageNameStr);
                if (channels & LogChannels::Warning)
                    LogOut(warningNameStr);
                if (channels & LogChannels::Error)
                    LogOut(errorNameStr);
                if (channels & LogChannels::FatalError)
                    LogOut(fatalNameStr);
                if (channels & LogChannels::UnsafeShutdown)
                    LogOut(unsafeNameStr);

                // If there are some custom channels dump their names out
                if (channels > LogChannels::LastPrebuilt)
                {
                    for (LogChannel channel : customChannels)
                    {
                        if (((size_t)channels) & channel.mask)
                            LogOut(channel.name);
                    }
                }
            }
        
            // Log out the ] character

            if (settings.hideDates)
                LogOut("]");
            else // also the date
            {
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

            // Log out a colon
            LogOut(": ");
            
            // Go
            LogOut(msg);

            // done, dump newline
            if (newline)
                LogOut("", true);

            // It's a fatal so run the fatal function
            if (channels & LogChannels::FatalError)
            {
                // Striclty optional
                if (settings.fatalFunc)
                    settings.fatalFunc();
            }

            if (channels & LogChannels::UnsafeShutdown)
            {
                if (settings.lastChanceUnsafeFunc)
                    settings.lastChanceUnsafeFunc();

                // it's going down, it's going down!
                std::abort();
            }
        }


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