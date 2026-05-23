/*
IRIS - A 68000 SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <platform/logging/logging.hpp>
#include <Iris.hpp>
#include <iostream>

namespace Iris
{
    int main(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::Init();

        Logger::Log(APP_NAME);
        Logger::Log(APP_SIGNON " " APP_VERSION, LogChannels::Message);
        Logger::Log(LOG_PREFIX_CORE, "Hi! I'm an SGI Iris 3130!", LogChannels::Debug);

        Logger::Log("Shutting down...");

        Logger::Shutdown(); 

        return EXIT_SUCCESS;
    }
}

int main(int argc, char** argv)
{
    return Iris::main(argc, argv);
}