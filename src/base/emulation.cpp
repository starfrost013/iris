#include <base/emulation.hpp>
#include <component/component.hpp>
#include <component/memory.hpp>
#include <component/cpu/mc68020.hpp>
#include <component/ip2/prom.hpp>
#include <component/ip2/prom_sram.hpp>
#include <component/ip2/ip2_mmu.hpp>
#include <component/ip2/ip2_rtc.hpp>
#include <component/ip2/ip2_duart.hpp>

namespace Iris
{
    void Emulation::Init()
    {
        // TEMP
        renderer = new RendererSDL3();
        renderer->Init();

        Coherent::Init();

        Start();
    }   

    void Emulation::Start()
    {
        Logger::Log("Starting emulation...");
           
        machine.AddComponent<Memory>();
        machine.AddComponent<MC68020>();
        machine.AddComponent<PROM>();
        machine.AddComponent<PROM_SRAM>();
        machine.AddComponent<MMU_IP2>();
        machine.AddComponent<DUART68681>();
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
    
    void Emulation::Reset()
    {
        Stop();
        Start();
    }

    void Emulation::SingleStep()
    {
        if (paused)
            machine.SingleStep();
    }

    void Emulation::Tick()
    {
        while (running)
        {
            // do this after processing everything for the current tick
            if (Coherent::active)
                Coherent::Tick();

            if (!paused)
                machine.Tick();
        }
    }

    void Emulation::Stop()
    {
        Logger::Log("Resetting emulation...");

        // make sure the machine is joinable
        SetRunning(false);
        SetPaused(false);

        if (emuThread->joinable())
            emuThread->join();

        Coherent::Leave();
        machine.Shutdown();
        AddrSpace::Shutdown();
    }

    void Emulation::Shutdown()
    {
        Coherent::Shutdown();
        Stop();
        renderer->Shutdown();
    }
}