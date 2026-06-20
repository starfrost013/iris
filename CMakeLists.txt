# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 23)

project(motion)

# Build SDL static, we like static libraries (esp. on Linux, where the system SDL3 might not be the same version!)

set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)

# Add our submodule directories

add_subdirectory(external/sdl3 EXCLUDE_FROM_ALL)

# Also allow us to use our own headers. At some point, we need to have a public/private header distinction...
add_subdirectory(src)

target_link_libraries(motion PRIVATE SDL3::SDL3-static)
