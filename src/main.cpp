/*
IRIS - A 68000 SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <Iris.hpp>
#include <base/emulation.hpp>
#include <iostream>

namespace Iris
{
    int main(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::Init();

        Logger::Log(APP_NAME " " APP_VERSION);
        Logger::Log(APP_SIGNON, LogChannels::Message);

        Emulation::Start();
        
        // run the emulation
        // todo: needs to run on its own thread

        while (Emulation::IsRunning())
        {            
            Emulation::Frame();

            // TODO: IMPLEMENT TICKRATE
            Emulation::Tick();

        }   

        Logger::Log("Shutting down...");

        Logger::Shutdown(); 

        return EXIT_SUCCESS;
    }
}

int main(int argc, char** argv)
{
    return Iris::main(argc, argv);
}