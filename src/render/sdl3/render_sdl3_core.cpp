/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3_core.cpp: Core SDL3 renderer stuff (init, shutdown, etc)
*/

#include <base/emulation.hpp>
#include <render/sdl3/render_sdl3.hpp>

namespace Iris
{
    /// @brief Initialises the SDL renderer. A failure is reported as a FATAL_ERROR Log.
    void RendererSDL3::Init()
    {
        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL3 renderer...");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) //noreturn
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL3: {}!", SDL_GetError()).c_str(), LogChannels::FatalError);

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL window...", LogChannels::Debug);

        window = SDL_CreateWindow(WINDOW_TITLE_DEFAULT, windowSizeX, windowSizeY, 0);

        if (!window) // noreturn
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL Window!", SDL_GetError()).c_str(), LogChannels::FatalError);

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL GPU device...", LogChannels::Debug);

        gpuDevice = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV 
            | SDL_GPU_SHADERFORMAT_DXIL
            | SDL_GPU_SHADERFORMAT_MSL
            | SDL_GPU_SHADERFORMAT_METALLIB,
        #ifndef NDEBUG
            true,
        #else
            false,
        #endif
            nullptr // let sdl pick the optimal driver
        );

        if (!gpuDevice) // noreturn
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL GPU Device!", SDL_GetError()).c_str(), LogChannels::FatalError);

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising SDL GPU renderer...", LogChannels::Debug);

        renderer = SDL_CreateGPURenderer(gpuDevice, window);

        if (!renderer) // noreturn
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to initialise SDL GPU Renderer!", SDL_GetError()).c_str(), LogChannels::FatalError);

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Claiming window for GPU device...", LogChannels::Debug);

        // Claim the window for the gpu device
        if (!SDL_ClaimWindowForGPUDevice(gpuDevice, window)) // noreturn
            Logger::Log(RENDER_SDL3_LOG_PREFIX, std::format("Failed to claim SDL window for GPU device!", SDL_GetError()).c_str(), LogChannels::FatalError);
 
        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Setting swapchain parameters...", LogChannels::Debug);

        // Set SDR
        SDL_SetGPUSwapchainParameters(gpuDevice, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising IMGUI core...", LogChannels::Debug);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // setup input
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // setup style
        ImGui::StyleColorsDark();

        Logger::Log(RENDER_SDL3_LOG_PREFIX, "Initialising IMGUI backend...", LogChannels::Debug);

        ImGui_ImplSDL3_InitForSDLGPU(window);

        ImGui_ImplSDLGPU3_InitInfo initInfo = {};
        initInfo.Device = gpuDevice;
        initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpuDevice, window);
        initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
        initInfo.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
        initInfo.PresentMode = SDL_GPU_PRESENTMODE_VSYNC; // VSync enabled? Make it a convar?

        ImGui_ImplSDLGPU3_Init(&initInfo);
    }

    /// @brief Render a new frame.
    void RendererSDL3::FramePreRender()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            // quit if we need to
            if (event.type == SDL_EVENT_QUIT)
                Emulation::SetRunning(false);
        }
    
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void RendererSDL3::FramePostRender()
    {
    #ifndef NDEBUG
        ImGui::ShowDemoWindow();
    #endif    
        ImGui::Render();

        // Upload the IMGUI vertex and index buffers to the GPU
        // IMGUI is always on top of whatever the GPURenderer is rendereing

        ImDrawData* data = ImGui::GetDrawData();
        bool isMinimised = (data->DisplaySize.x < 0.0f | data->DisplaySize.y < 0.0f);
        SDL_GPUCommandBuffer* buffer = SDL_AcquireGPUCommandBuffer(gpuDevice);

        SDL_GPUTexture* swapchainTexture;
        SDL_WaitAndAcquireGPUSwapchainTexture(buffer, window, &swapchainTexture, nullptr, nullptr);

        if (swapchainTexture && !isMinimised)
        {
            ImGui_ImplSDLGPU3_PrepareDrawData(data, buffer);

            SDL_GPUColorTargetInfo targetInfo = {};
            targetInfo.texture = swapchainTexture;
            targetInfo.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f }; 
            targetInfo.load_op = SDL_GPU_LOADOP_CLEAR; // huh
            targetInfo.store_op = SDL_GPU_STOREOP_STORE;
            targetInfo.mip_level = 0; // 2D
            targetInfo.layer_or_depth_plane = 0; 
            targetInfo.cycle = false;

            SDL_GPURenderPass* imguiPass = SDL_BeginGPURenderPass(buffer, &targetInfo, 1, nullptr); 

            ImGui_ImplSDLGPU3_RenderDrawData(data, buffer, imguiPass);

            SDL_EndGPURenderPass(imguiPass);
        }
        
        SDL_SubmitGPUCommandBuffer(buffer);
    }

    // Allow resizing the window
    void RendererSDL3::SetWindowSize(int32_t x, int32_t y)
    {
        if (window)
            SDL_SetWindowSize(window, x, y);
    }

    /// @brief Shut down the renderer.
    void RendererSDL3::Shutdown()
    {
        // Wait for the gpu to shut down so we don't corrupt its state
        SDL_WaitForGPUIdle(gpuDevice);

        ImGui_ImplSDLGPU3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_ReleaseWindowFromGPUDevice(gpuDevice, window);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyGPUDevice(gpuDevice);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
}