# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

# Main /src directory makefile

include_directories(.)
include_directories(./external/sdl3/include)

add_executable(motion
    main.cpp
    base/component/component.cpp
    base/emulation.cpp
    base/filesystem/filesystem.cpp
    cpu/mc68020_core.cpp
    base/component/addrspace.cpp
    base/component/memory.cpp
    cpu/coherent/coherent_core.cpp
    cpu/coherent/coherent_gui.cpp
    render/sdl3/render_sdl3_core.cpp
)

