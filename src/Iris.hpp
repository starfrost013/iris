// iris(tm)
// Copyright (C) 2026 starfrost

#pragma once

// Standard includes
#include <cctype>
#include <concepts>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <format>
#include <fstream>
#include <vector>
#include <unordered_map>

// should be included everywhere as it's standalone
#include <platform/logging/logging.hpp>

#define APP_NAME            "motion"
#define APP_SIGNON          "The SGI Emulator\nEmulation engine © 2026 starfrost\nOriginal hardware and software by Silicon Graphics, Inc. © 1981-1989"
#define APP_VERSION         "@ " __DATE__ " " __TIME__

#define EXIT_SUCCESS        0
#define EXIT_FAILURE        1

// Log prefixes
#define LOG_PREFIX_CORE     "Core"

// Some string lengths
#define STRING_MAX_SHORT    48
#define STRING_MAX_LONG     256
#define STRING_MAX_PATH     260