#include <base/emulation.hpp>
#include <component/component.hpp>
#include <component/memory.hpp>
#include <component/cpu/mc68020.hpp>
#include <component/rom/prom.hpp>

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
        machine.AddComponent<PROM>();
        machine.Start();

        // enter the coherent debugger
        Coherent::Enter();

        Logger::Log("Starting emulation: Starting emulation thread...", LogChannels::Debug);
        running = true; 

        // start the thread
        emuThread = new std::thread(Emulation::Tick);
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
        while (running)
            machine.Tick();

    }

    void Emulation::Shutdown()
    {
        if (emuThread->joinable())
            emuThread->join();

        renderer->Shutdown();
        Coherent::Shutdown();
        machine.Shutdown();

    }
}