#include <base/emulation.hpp>


namespace Iris
{
    void Emulation::Start()
    {
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
        
    }
}