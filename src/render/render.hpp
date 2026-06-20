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
    public:
        virtual void Init() { };

        /// @brief Pumps the event queue for this renderer. Main Emulator and UI can be rendered before this.
        virtual void FramePreRender() { };

        /// @brief Performs the actual rendering.
        virtual void FramePostRender() { };
        virtual void Shutdown() { };
    };
}