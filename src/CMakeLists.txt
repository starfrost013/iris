# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

# Main /src directory

include_directories(.)

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
)

