/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    cvar.cpp: Console variables that allow global configuration implementation
*/


#include <base/cvar/cvar.hpp>

namespace Iris
{
    Cvar* Cvar::Add(const char* name, const char* value)
    {
        Cvar* cvar = new Cvar;

        Cvar::cvars[cvar->name] = cvar;

        cvar->name = name;
        cvar->SetInternal(value);
       
        return cvar;
    }

    Cvar* Cvar::Get(const char* name, const char* value = nullptr)
    {
        auto count = Cvar::cvars.count(name);

        if (count > 1)
            Logger::Log(std::format("Cvar::Get - Duplicated convar {}. Any copies will be ignored.", name).c_str(), LogChannels::Warning);
    
        if (count >= 1)
            return Cvar::cvars[name];
                    
        if (value == nullptr)
            value = "0";

        return Cvar::Add(name, value);
    }

    void Cvar::SetInternal(const char* newValue)
    {
        string = newValue;
        value = atof(newValue);
    }

    Cvar* Cvar::Set(const char* name, const char* value)
    {
        Cvar* cvar = Cvar::Get(name);

        // already logged
        // shouldn't happen but just in case
        if (!cvar)
            Add(name, value);

        cvar->SetInternal(value);

        return cvar; 
    }
}