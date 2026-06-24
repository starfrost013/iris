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
        static Cvar* Add(const char* name, const char* value);
        static Cvar* Get(const char* name, const char* value);
        static Cvar* Set(const char* name, const char* value);

        // Getters for private fields

        float GetValue() { return value; };
        const char* GetString() { return string; }

        // Settes for private fields 

    private: 
        const char* name;
        float value; 
        const char* string;  // the linked list prevents whatever string we put here from being delete.d

        static inline std::unordered_map<const char*, Cvar*> cvars;

        /// @brief Internal method to set a convar
        /// @param name The name of the convar to set
        /// @param value The value
        void SetInternal(const char* newValue);
    };
}