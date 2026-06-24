/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    cvar.cpp: Console variables that allow global configuration implementation
*/


#include <base/cvar/cvar.hpp>

namespace Iris
{
    void Cvar::Add(const char* name, char* value)
    {
        Cvar* cvar = new Cvar;

        Cvar::cvars[cvar->name] = cvar;

        cvar->name = name;
        cvar->SetInternal(value);
    }

    Cvar* Cvar::Get(const char* name)
    {
        auto count = Cvar::cvars.count(name);

        if (count > 1)
            Logger::Log(std::format("Cvar::Get - Duplicated convar {}. Any copies will be ignored.", name).c_str(), LogChannels::Warning);
    
        if (count >= 1)
            return Cvar::cvars[name];
            
        Logger::Log(std::format("Cvar::Get or Cvar::Set - The Convar by the name {} does not exist!", name).c_str(), LogChannels::Warning);

        return nullptr;
    }

    void Cvar::SetInternal(char* newValue)
    {
        string = newValue;
        value = atof(newValue);
    }

    void Cvar::Set(const char* name, char* value)
    {
        Cvar* cvar = Cvar::Get(name);

        // already logged
        if (!cvar)
            return;

        cvar->SetInternal(value);
    }
}