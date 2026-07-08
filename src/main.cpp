/*
IRIS - A 68000 SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <Iris.hpp>
#include <base/emulation.hpp>
#include <iostream>

namespace Iris
{
    /// @brief Fatal error function
    /// @return Emulator shuts down
    void fatal()
    {
        Emulation::SetRunning(false);
    }

    int main(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::settings.SetFatalFunction(fatal);
        Logger::settings.sendAnsiCodesToFile = false;
        Logger::settings.postLogMessageIgnoresAnsiCodes = true; //coherent
        Logger::Init();

        Logger::Log(APP_NAME " " APP_VERSION);
        Logger::Log(APP_SIGNON, LogChannels::Message);

        Emulation::Init();
        
        // run the emulation
        // todo: needs to run on its own thread

        while (Emulation::IsRunning())
        {            
            Emulation::Frame();

            // Each component ticks at its own speed. So we implement the tickrate in the Emulation class.
            // Emulation::Tick runs in its own thread. The renderer runs in a separate thread
            //Emulation::Tick();
                
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