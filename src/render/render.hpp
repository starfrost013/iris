/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost
*/

#pragma once
#include <Iris.hpp>

namespace Iris
{
    #define RENDER_LOG_PREFIX       "Render - Core"

    /// @brief Base renderer class. Other renderers inherit from this
    class Renderer
    {
        // THis is the iris 3130's screen resolution. DON'T HARDCODE IT!
        #define WINDOW_SIZE_X       1024
        #define WINDOW_SIZE_Y       768

    public:
        virtual void Init() { };

        /// @brief Pumps the event queue for this renderer. Main Emulator and UI can be rendered before this.
        virtual void FramePreRender() { };

        /// @brief Performs the actual rendering.
        virtual void FramePostRender() { };
        virtual void Shutdown() { };

        // Getters for private fields
        int32_t GetWindowSizeX() { return windowSizeX; }; 
        int32_t GetWindowSizeY() { return windowSizeY; }; 

        // Setters for private fields
        virtual void SetWindowSize(int32_t x, int32_t y) { };
        
    protected: 
        int32_t windowSizeX = WINDOW_SIZE_X, windowSizeY = WINDOW_SIZE_Y;

    };
}