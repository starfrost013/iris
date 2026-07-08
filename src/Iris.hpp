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
#define LOG_PREFIX_PROM     "Emulation - PROM"

// Some string lengths
#define STRING_MAX_SHORT    48
#define STRING_MAX_LONG     256
#define STRING_MAX_PATH     260