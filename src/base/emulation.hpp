/*
    emulation.hpp : The core of the emulation thread ()
*/

#pragma once
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <base/machine/machine.hpp>

namespace Iris
{
    #define CONFIG_PATH "Iris.cfg"
    class Emulation
    {
    public: 
        static void Start();
        static void Frame();
        static void Tick();             // run at the tick for things that need to be regularly updated
        static void Shutdown();

        // Called by components to e.g. get the address space
        static Machine& GetMachine() { return machine; }
        
        static bool IsRunning() { return running; }    

    private: 
        inline static bool running = false;
        
        static FileStream config; 

        static Machine machine;
    };
}