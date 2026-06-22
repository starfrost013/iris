/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    cvar.hpp: Console variables that allow global configuration 
*/

#pragma once
#include <Iris.hpp>
 
namespace Iris
{
    class Cvar
    {

    public: 
        // Statics (public API)
        static void Add(const char* name, char* value);
        static Cvar* Get(const char* name);
        static void Set(const char* name, char* value);

        // Getters for private fields

        float GetValue() { return value; };
        char* GetString() { return string; }

        // Settes for private fields 

    private: 
        const char* name;
        float value; 
        char* string;  // the linked list prevents whatever string we put here from being delete.d

        static inline Cvar* head; 
        static inline Cvar* tail; 
        Cvar* next;
    };
}