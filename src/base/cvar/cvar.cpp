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

        if (!Cvar::head)
        {
            Cvar::head = Cvar::tail = cvar;
        }
        else
        {
            Cvar::tail->next = cvar;
            Cvar::tail = cvar; 
        }

        cvar->name = name;
        cvar->string = value;
        cvar->value = atof(value);
    }

    Cvar* Cvar::Get(const char* name)
    {
        Cvar* cvar = Cvar::head;

        while (cvar)
        {
            if (!strcmp(cvar->name, name))
                return cvar;
                
            cvar = cvar->next;
        }
    }
}