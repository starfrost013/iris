#include <base/cmdline/cmdline.hpp>

namespace Iris
{
    void CommandLine::Parse(int32_t argc, char** argv)
    {
        CommandLine::argc = argc;
    
        for (int32_t i = 0; i < argc; i++)
        {
            std::array<char, STRING_MAX_COMMAND_LINE> cmd{};
            strncpy(cmd.data(), argv[i], STRING_MAX_COMMAND_LINE);
            commands.push_back(cmd);

            // parse early cvars
            if ((argc - i) > 2)
            {
                if (!strcmp(argv[i], "+set"))
                    Cvar::Add(argv[i + 1], argv[i + 2]);
            }

        }
    }

    bool CommandLine::Present(const char* commandString)
    {
        for (auto& command : commands)
        {
            if (!strcmp(command.data(), commandString))
                return true; 
        }

        return false; 
    }

    constexpr char* CommandLine::Argv(int32_t c)
    {
        if (c < 0
        || c >= argc)
            Logger::Log("Attempted to obtain invalid command-line argument ID!", LogChannels::Warning);

        return commands[c].data();
    }
}