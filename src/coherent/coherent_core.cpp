#include <coherent/coherent.hpp>

namespace Iris
{
    //
    // Cvars
    //
    Cvar* startPaused;

    /// getters for private fields
    CoherentSystem::RunState CoherentSystem::GetRunState() { return runState; };

    /// setters for private fields

    void CoherentSystem::SetRunState(CoherentSystem::RunState runState)
    {
        this->runState = runState;

        switch (runState)
        {
            case CoherentSystem::RunState::Reset:
                Emulation::Reset();
                break;
            case CoherentSystem::RunState::SingleStep:
                Emulation::SingleStep();
                SetRunState(Paused);
                break;
            case CoherentSystem::RunState::Paused:
                if (!Emulation::GetPaused())
                    Emulation::SetPaused(true);

                break;
            case CoherentSystem::RunState::Running:
                if (Emulation::GetPaused())
                    Emulation::SetPaused(false);
        }
    }

    //
    // Coherent class
    //

    /// @brief a C trampoline. since the logging system is portable across projects which may use various paradigms, its function poitners use the lowest common 
    /// denominator (ie a function pointer.)
    /// @param str 
    void Coherent_CTrampolineForLog(const char* str)
    {
        Coherent::AddTextToLogWindowBuffer(str);
    }

    void Coherent::Init()
    {
        startPaused = Cvar::Set("startPaused", "1");

        Logger::Log(COHERENT_LOG_PREFIX, COHERENT_VERSION " initialised");
        Logger::settings.SetPostLogFunction(Coherent_CTrampolineForLog);

        initialised = true; 
    }

    void Coherent::Enter()
    {
        active = true;
    }

    void Coherent::Leave()
    {   
        active = false; 
    }

    void Coherent::Shutdown()
    {
        currentSystem->Shutdown();
        Logger::settings.SetPostLogFunction(nullptr);
    }

    //
    // Extension registration
    //

    void Coherent::RegisterExtension(CoherentExtension* extension)
    {
        if (!extension)
        {
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::RegisterExtension - extension == NULLPTR!", LogChannels::Error);
            return; 
        }

        if (!extension->component)
        {
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::RegisterExtension - extension->component == NULLPTR!", LogChannels::Error);
            return; 
        }

        extensions.push_back(extension);
        Logger::Log(COHERENT_LOG_PREFIX, std::format("Coherent registered an extension for the component {}", extension->component->GetName()).c_str(), LogChannels::Message);
    }

    //
    // Debug system
    //

    /// @brief Called when the coherent system was requested to remove a breakpoint.
    void Coherent::AddBreakpoint(Breakpoint bp)
    {
        if (breakpoints.count(bp.addr) > 0)
        {
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::AddBreakpoint - the breakpoint already exists!", LogChannels::Warning);
            return; 
        }

        breakpoints[bp.addr] = bp;
    }

    void Coherent::AddWatchpoint(Watchpoint wp)
    {
        if (watchpoints.count(wp.addr) > 0)
        {
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::AddWatchpoint - the watchpoint already exists!", LogChannels::Warning);
            return; 
        }
        
        watchpoints[wp.addr] = wp;
    }

    void Coherent::AddCatchpoint(Catchpoint cp)
    {
        if (catchpoints.count(cp.addr) > 0)
        {
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::AddCatchpoint - the catchpoint already exists!", LogChannels::Warning);
            return; 
        }

        catchpoints[cp.addr] = cp;
    }

    /// @brief Called when the coherent system was requested to remove a breakpoint.
    void Coherent::RemoveBreakpoint(Breakpoint bp)
    {
        if (breakpoints.count(bp.addr) > 0)
            breakpoints.erase(bp.addr);
        else
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::RemoveBreakpoint - the breakpoint doesn't exist!", LogChannels::Warning);
    }

    void Coherent::RemoveWatchpoint(Watchpoint wp)
    {
        if (watchpoints.count(wp.addr) > 0)
            watchpoints.erase(wp.addr);
        else
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::RemoveWatchpoint - the watchpoint doesn't exist!", LogChannels::Warning);
    }

    void Coherent::RemoveCatchpoint(Catchpoint cp)
    {
        if (catchpoints.count(cp.addr) > 0)
            catchpoints.erase(cp.addr);
        else
            Logger::Log(COHERENT_LOG_PREFIX, "Coherent::RemoveCatchpoint - the catchpoint doesn't exist!", LogChannels::Warning);
    }

    Coherent::Breakpoint Coherent::GetBreakpointByAddr(size_t addr)
    {
        return breakpoints[addr];
    }

    Coherent::Watchpoint Coherent::GetWatchpointByAddr(size_t addr)
    {
        return watchpoints[addr];
    }

    Coherent::Catchpoint Coherent::GetCatchpointByAddr(size_t addr)
    {
        return catchpoints[addr];
    }

}