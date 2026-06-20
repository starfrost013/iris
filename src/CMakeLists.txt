# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

# Main /src directory makefile

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/external/sdl3/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)

add_executable(motion

    # Dear Imgui
    ../external/imgui/imgui_demo.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui.cpp
    ../external/imgui/backends/imgui_impl_sdl3.cpp
    ../external/imgui/backends/imgui_impl_sdlgpu3.cpp

    # Motion Emulator
    main.cpp
    base/component/component.cpp
    base/emulation.cpp
    base/filesystem/filesystem.cpp
    cpu/mc68020_core.cpp
    base/component/addrspace.cpp
    base/component/memory.cpp
    coherent/coherent_core.cpp
    coherent/coherent_gui.cpp
    render/sdl3/render_sdl3_core.cpp
)

