# Motion - An SGI Emulator!
# Copyright (C) 2026 starfrost

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/${CMAKE_BUILD_TYPE})

# mingw-w64 doesn't support static linking against the UCRT

# Install path
set(ROM_SRC_DIR "${CMAKE_SOURCE_DIR}/roms")
set(ROM_DST_DIR "${CMAKE_BINARY_DIR}/output/${CMAKE_BUILD_TYPE}/roms")

project(motion)

# Build SDL static, we like static libraries (esp. on Linux, where the system SDL3 might not be the same version!)

set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)

# Add our submodule directories

add_subdirectory(external/sdl3 EXCLUDE_FROM_ALL)

# Also allow us to use our own headers. At some point, we need to have a public/private header distinction...
add_subdirectory(src)


if(WIN32 AND MINGW)
    target_link_options(motion PRIVATE 
        -static -static-libgcc -static-libstdc++ 
        -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
    )
else()
    # if you statically link everything basically pipewrie will KILL SDL lol
    target_link_options(motion PRIVATE -static-libstdc++ -static-libgcc)             # static stdc++
endif()

target_link_libraries(motion PRIVATE SDL3::SDL3-static) # static sdl3


# install
add_custom_target(InstallAssets 
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${ROM_SRC_DIR}" "${ROM_DST_DIR}"
    COMMENT "Updating ROMs..."
)

add_dependencies(motion InstallAssets)