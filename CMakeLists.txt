# IRIS - A 68000 SGI Emulator!
# Copyright (C) 2026 starfrost

cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 23)

project(motion)

set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

# try and find curses
find_package(Curses)

if (NOT CURSES_FOUND)
    message(FATAL_ERROR "Install curses!")
endif()

if (CURSES_FOUND AND NOT TARGET Curses::Curses)
    add_library(Curses::Curses INTERFACE IMPORTED)

    set_target_properties(Curses::Curses
    PROPERTIES
        IMPORTED_LOCATION "${CURSES_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
    )
endif()

add_subdirectory(src)

target_link_libraries(motion PRIVATE Curses::Curses)