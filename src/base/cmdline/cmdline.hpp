#pragma once

#include <Iris.hpp>
#include <base/cvar/cvar.hpp>

/*
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    cmdline.cpp: Implements the command-line (wow!)
*/

namespace Iris
{
    #define STRING_MAX_COMMAND_LINE             1024

    class CommandLine
    {
    public:
        /// @brief Parses the command line
        /// @param argc The argc parameter, passed from main.
        /// @param argv The argv parameter, passed from main.
        static void Parse(int32_t argc, char** argv);

        /// @brief Checks if a command string was supplied.
        /// @param commandString The command string to check.
        /// @return A boolean value determining if this command string exists or not.
        static bool Present(const char* commandString);

        /// @brief Get a specific arg number 
        /// @param c The argument to get.
        static constexpr char* Argv(int32_t c);

    private: 
        inline static int32_t argc;
       
        inline static std::vector<std::array<char, STRING_MAX_COMMAND_LINE>> commands;
    }; 
}