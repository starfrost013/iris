#include <base/emulation.hpp>
#include <base/component/component.hpp>
#include <base/component/memory.hpp>
#include <cpu/mc68020.hpp>

namespace Iris
{
    void Emulation::Start()
    {
        // TEMP
        renderer = new RendererSDL3();
        
        renderer->Init();

        Logger::Log("Starting emulation...");
           
        machine.AddComponent<Memory>();
        machine.AddComponent<CPU_MC68020>();
        machine.Start();

        running = true; 
    }   
    
    void Emulation::Frame()
    {
        renderer->Frame();
    }
    
    void Emulation::Tick()
    {
        machine.Tick();
        
        if (!running)
            Shutdown();
    }

    void Emulation::Shutdown()
    {
        renderer->Shutdown();
        machine.Shutdown();

        running = false; 
    }
}