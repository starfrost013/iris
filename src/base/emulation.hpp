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
    // Temp until convars are a thi9ng
    #define CONFIG_PATH "Iris.cfg"

    class Emulation
    {
    public: 
        static void Init();             // initialise the emulation system
        static void Start();            // Start emulation
        static void Frame();            // render a frame
        static void Tick();             // run one tick of the system
        static void Reset();            // reset the emulation
        static void SingleStep();       // run one emulation tick
        static void Stop();      // shut down
        static void Shutdown();     

        // Called by components to e.g. get the address space
        static Machine& GetMachine() { return machine; }
        
        static bool IsRunning() { return running; }    
        static void SetRunning(bool value) { running = value; };

        // getters for private    
        static Renderer* GetRenderer() { return renderer; }; 
        static bool GetPaused() { return paused; };

        // setters for private fields
        static void SetPaused(bool paused) { Emulation::paused = paused; };

    private: 
        /// @brief determines if the emulator is running
        inline static bool running = false;

        /// @brief determines if the emulator is paused
        inline static bool paused = false; 
        
        inline static FileStream config; 

        /// @brief the machine that is being emulated
        inline static Machine machine;

        /// @brief TEMP
        inline static Renderer* renderer;

        inline static std::thread* emuThread;
    };
}