#include <base/emulation.hpp>
#include <base/component/component.hpp>
#include <base/component/memory.hpp>
#include <cpu/mc68020.hpp>

namespace Iris
{
    void Emulation::Start()
    {
        Logger::Log("Starting emulation...");
           
        machine.AddComponent<Memory>();
        machine.AddComponent<CPU_MC68020>();
        machine.Start();

        running = true; 
    }   
    
    void Emulation::Frame()
    {
        
    }
    
    void Emulation::Tick()
    {
        machine.Tick();
        
        if (!running)
            Shutdown();
    }

    void Emulation::Shutdown()
    {
        machine.Shutdown();

        running = false; 
    }
}