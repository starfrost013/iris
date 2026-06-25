# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/)

# Install path
set(ROM_SRC_DIR "${CMAKE_SOURCE_DIR}/roms")
set(ROM_DST_DIR "${CMAKE_BINARY_DIR}/output/roms")

project(motion)

# Build SDL static, we like static libraries (esp. on Linux, where the system SDL3 might not be the same version!)

set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)

# Add our submodule directories

add_subdirectory(external/sdl3 EXCLUDE_FROM_ALL)

# Also allow us to use our own headers. At some point, we need to have a public/private header distinction...
add_subdirectory(src)

target_link_libraries(motion PRIVATE SDL3::SDL3-static) # static sdl3
target_link_options(motion PRIVATE -static)             # static stdc++

# install
add_custom_target(InstallAssets 
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${ROM_SRC_DIR}" "${ROM_DST_DIR}"
    COMMENT "Updating ROMs..."
)

add_dependencies(motion InstallAssets)