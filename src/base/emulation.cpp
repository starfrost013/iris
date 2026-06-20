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

        Coherent::Init();

        Logger::Log("Starting emulation...");
           
        machine.AddComponent<Memory>();
        machine.AddComponent<CPU_MC68020>();
        machine.Start();

        // enter the coherent debugger
        Coherent::Enter();

        running = true; 
    }   
    
    void Emulation::Frame()
    {
        renderer->FramePreRender();

        if (Coherent::active)
            Coherent::Frame();
            
        renderer->FramePostRender();
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
        Coherent::Shutdown();
        machine.Shutdown();

        running = false; 
    }
}