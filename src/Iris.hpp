/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    Iris.hpp: Main file for motion (previously called Iris, but it was too generic) SGI Emulator
*/

#pragma once

// Standard includes
#include <any>
#include <cctype>
#include <chrono>
#include <concepts>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <format>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <thread>

// should be included everywhere as it's standalone
#include <platform/logging/logging.hpp>
#include <platform/util/util.hpp>

// Not standalone, but useful for everything
#include <base/cvar/cvar.hpp>

#define APP_NAME            "motion"
#define APP_SIGNON          "The SGI Emulator\nEmulation engine © 2026 starfrost\nOriginal hardware and software by Silicon Graphics, Inc. © 1981-1989"
#define APP_VERSION         "@ " __DATE__ " " __TIME__

#define EXIT_SUCCESS        0
#define EXIT_FAILURE        1

// Log prefixes
#define LOG_PREFIX_CORE     "Core"
#define LOG_PREFIX_MAPPING  "Emulation - Memory Mapping"
#define LOG_PREFIX_PROM     "Emulation - PROM"
#define LOG_PREFIX_IP2MMU   "Emulation - IP2 MMU"

// Some string lengths
#define STRING_MAX_SHORT    48
#define STRING_MAX_LONG     256
#define STRING_MAX_PATH     260

// Some useful macros for converting between different io sizes
#define READ_32TO8(x, addr) (uint8_t)(x >> ((addr & 3) << 3) & 0xFF)
#define READ_32TO16(x, addr)(uint16_t)(x >> ((addr & 3) << 3) & 0xFFFF)

#define WRITE_32TO8(x, old, addr)   x &= (~0xFF << (addr & 3) << 3); \
                                    x |= (old << ((addr & 3) << 3))
                            

#define WRITE_32TO16(x, old, addr)  x &= (~0xFFFF << (addr & 3) << 3); \
                                    x |= (old << ((addr & 3) << 3))     
                               