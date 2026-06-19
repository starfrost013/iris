/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3_core.cpp: Core SDL3 renderer stuff (init, shutdown, etc)
*/

#include <render/sdl3/render_sdl3.hpp>

namespace Iris
{
    /// @brief Initialises the SDL renderer. A failure is reported as a FATAL_ERROR Log.
    void RendererSDL3::Init()
    {
        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL...");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
        {
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL3: {}!", SDL_GetError()).c_str(), LogChannels::FatalError);
            Shutdown();
        }

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL window and renderer...");

        if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE_DEFAULT, 1024, 768, 0, &window, &renderer))
        {
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL Window and Renderer!", SDL_GetError()).c_str(), LogChannels::FatalError);
            Shutdown();
        }
    }

    /// @brief Render a new frame.
    void RendererSDL3::Frame()
    {

    }

    /// @brief Shut down the renderer.
    void RendererSDL3::Shutdown()
    {
        SDL_Quit();
    }
}