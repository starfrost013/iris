/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3.hpp: Implements SDL3 + IMGUI SDL3GPU 
*/

#pragma once
#include <Iris.hpp>
#include <render/render.hpp>

#include <SDL3/SDL.h>

namespace Iris
{
    #define RENDER_SDL3_LOG_PREFIX      "Render - SDL3"

    /// @brief Base renderer class. Other renderers inherit from this
    class RendererSDL3 : public Renderer
    {
        #define WINDOW_TITLE_DEFAULT APP_NAME " - SGI Emulator (c) 2026 Starfrost"

    public:
        void Init() override;
        void Frame() override;
        void Shutdown() override;

    private:
        SDL_Window* window;
        SDL_Renderer* renderer; 
    };
}