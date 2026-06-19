/*
    emulation.hpp : The core of the emulation thread ()
*/

#pragma once
#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>
#include <base/machine/machine.hpp>

// render includes
#include <render/render.hpp>
#include <render/sdl3/render_sdl3.hpp>

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

        /// @brief get the name of this component. immutable const char*.
        virtual const char* GetName() { return "Motorola MC68020 CPU"; };

        static Renderer* GetRenderer() { return renderer; }; 
    private: 
        inline static bool running = false;
        
        inline static FileStream config; 

        /// @brief the machine that is being emulated
        inline static Machine machine;

        /// @brief TEMP
        inline static Renderer* renderer;
    };
}