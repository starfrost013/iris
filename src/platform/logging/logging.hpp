//
// SSLS (Starfrost's Simple Logging System) Version 5.0 (C++ Only)
// Copyright (C) 2023-2026 starfrost
//

#pragma once

// Includes
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>
#include <unordered_map>

// Defines

#define LOGGER_USE_NAMESPACE                                                    // Comment out to disable namespaces
#define LOGGER_NAMESPACE                Iris                                    // Namespace to use if namespace enabled
#define LOGGER_MAX_PATH                 260                                     // Max path
#define LOGGER_MAX_STRING_SHORT         48                                      // Generic maximum length for "short" strings
#define LOGGER_MAX_STRING               256                                     // Generic maximum length for "long" strings

// Strings, change to localise or whatever

#define STRING_VERSION                  "5.0 (May 17, 2026)"                    // Version number as a string (we don't need it in any other form)
#define STRING_SIGN_ON                  "SSLS Version " STRING_VERSION " initilaised" 

#define CONSOLE_TERMINAL_COMMAND_PREFIX "\x1B["                                 // Some console terminal command prefixes use this

#ifdef LOGGER_USE_NAMESPACE
namespace LOGGER_NAMESPACE
{
#endif

    /// @brief A logger channel.
    class LogChannel
    {
        const char* name;                       // the name to use for this log channel.
        size_t mask;                            // The mask to use for selecting this log channel.

    public:
        LogChannel(const char* name, size_t mask)
        {
            this->name = name;
            this->mask = mask; 
        }
    };

    /// @brief Prebuilt log channels
    enum LogChannels
    {
        Message = 1,                    // A normal message.
        Warning = 2,                    // A warning message.
        Error = 4,                      // An error message.
        FatalError = 8,                 // A fatal error message.
        UnsafeShutdown = 16,            // The memory or some other critical resource is hosed.
    };

    /// @brief Enumerates possible log destinations.
    enum LogDestination
    {
        Stdin = 1,
        Stdout = 1 << 1,
        Stderr = 1 << 2, 
        File = 1 << 3,
    }; 

    class LoggerSettings
    {
        friend class Logger;

    public:
        void SetAppName(const char* appName) { strncpy(this->appName, appName, sizeof(appName)); };
        void SetDestinations(LogDestination destinations) { this->destinations = destinations; };

        /// @brief Set the filename for the logging system to use.
        /// @param fileName The filename to use.
        void SetFileName(const char* fileName) 
        { 
            strncpy(this->appName, appName, sizeof(appName)); 
            overrideDefaultFilename = true; 
        };


    private: 

        char appName[LOGGER_MAX_STRING_SHORT] = {0};    // Application name to use for log file
        void (*fatalFunc)();                                // Function to call on a fatal error log.
        bool overrideDefaultFilename;                       // If true, override the default filename
        char fileName[LOGGER_MAX_PATH] = {0};           // If overridedefaultfilename is true, use this filename
        char dateFormat[LOGGER_MAX_STRING_SHORT] = {0};     // Optional date format string to use. Otherwise yyyy-mm-dd hh:mm:ss is useed

        bool hideSignOnMessage;
        std::fstream logStream;                             // The straem to open the log if LogDestination has FILE
        LogDestination destinations;                        // The destinations to send the log
    }; 


    class Logger
    {
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

            // copy the filename
            if (!settings.overrideDefaultFilename)
                strncpy(settings.fileName, settings.appName, sizeof(settings.fileName));

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

    private: 
        inline static std::vector<LogChannel> customChannels;       // custom channels 

        inline static bool initialised;                             // determines if we were initialised successfully

    }; 

#ifdef LOGGER_USE_NAMESPACE
}
#endif