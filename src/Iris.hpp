// iris(tm)
// Copyright (C) 2026 starfrost

// Standard includes
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

// should be included everywhere as it's standalone
#include <platform/logging/logging.hpp>

#define APP_NAME            "iris"
#define APP_SIGNON          "The SGI Emulator"
#define APP_VERSION         "@ " __DATE__ " " __TIME__

#define EXIT_SUCCESS        0
#define EXIT_FAILURE        1

// Log prefixes
#define LOG_PREFIX_CORE     "Core"

// Some string lengths
#define STRING_MAX_SHORT    48
#define STRING_MAX_LONG     256
#define STRING_MAX_PATH     260