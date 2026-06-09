#include <base/emulation.hpp>


namespace Iris
{
    void Emulation::Start()
    {
        Logger::Log("Starting emulation...");
           
        machine.Start();

        running = true; 
    }   
    
    void Emulation::Frame()
    {

    }
    
    void Emulation::Tick()
    {
        if (!running)
            Shutdown();
    }

    void Emulation::Shutdown()
    {
        machine.Shutdown();

        running = false; 
    }
}