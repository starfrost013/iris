/*
IRIS - A 68000 SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <Iris.hpp>
#include <iostream>

namespace Iris
{
    int main(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::Init();

        Logger::Log("SSLS Test!", LogChannels::Message);

        std::cout << APP_NAME << std::endl;
        std::cout << APP_SIGNON << " " << APP_VERSION << std::endl;
        std::cout << "Hi! I'm an SGI Iris 3130!!" << std::endl;
        return EXIT_SUCCESS;
    }
}

int main(int argc, char** argv)
{
    return Iris::main(argc, argv);
}