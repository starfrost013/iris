# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

# Main /src directory

include_directories(.)

add_executable(iris
    main.cpp
    base/component/component.cpp
    cpu/mc68020_core.cpp
)