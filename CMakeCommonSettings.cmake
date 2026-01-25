# CMakeCommonSettings.cmake
# ============================================================================
# Common build settings for embedded C++ projects (GCC toolchain)
#
# Usage in your CMakeLists.txt:
#   include(${CMAKE_SOURCE_DIR}/../CMakeCommonSettings.cmake)
#
# Build commands:
#   Debug build:    cmake -DCMAKE_BUILD_TYPE=Debug ..
#   Release build:  cmake -DCMAKE_BUILD_TYPE=Release ..
#   Embedded build: cmake -DCMAKE_BUILD_TYPE=Release -DEMBEDDED_BUILD=ON ..
# ============================================================================

cmake_minimum_required(VERSION 3.10)

# ----------------------------------------------------------------------------
# Build Target: Hosted (Linux/Windows) or Embedded (bare-metal)
# ----------------------------------------------------------------------------
# OFF = Build for Linux/Windows with full C++ library (default)
# ON  = Build for bare-metal microcontroller without OS or STL
option(EMBEDDED_BUILD "Build for bare-metal embedded target" OFF)

# ----------------------------------------------------------------------------
# C++ Standard
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------------------------------------------
# Warning Flags
# ----------------------------------------------------------------------------
# -Wall        Enable common warnings
# -Wextra      Enable extra warnings
# -Wpedantic   Strict ISO C++ compliance
# -Wshadow     Warn when variable shadows another
# -Wconversion Warn on implicit type conversions
set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wconversion")

# ----------------------------------------------------------------------------
# Debug Build
# ----------------------------------------------------------------------------
# -O0     No optimization (easier debugging)
# -g      Include debug symbols
# -DDEBUG Define DEBUG macro for conditional code
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG ${WARNING_FLAGS}")

# ----------------------------------------------------------------------------
# Release Build
# ----------------------------------------------------------------------------
if(EMBEDDED_BUILD)
    # === Embedded/Bare-Metal Settings ===

    # -Os      Optimize for size (flash is usually limited)
    # -g       Keep debug symbols (strip later if needed)
    # -DNDEBUG Disable assert() for production
    set(CMAKE_CXX_FLAGS_RELEASE "-Os -g -DNDEBUG ${WARNING_FLAGS}")

    # Disable features that need runtime support
    string(APPEND CMAKE_CXX_FLAGS " -fno-exceptions")          # No try/catch
    string(APPEND CMAKE_CXX_FLAGS " -fno-rtti")                # No typeid/dynamic_cast
    string(APPEND CMAKE_CXX_FLAGS " -fno-threadsafe-statics")  # No mutex for static init
    string(APPEND CMAKE_CXX_FLAGS " -fno-use-cxa-atexit")      # No global destructors
    string(APPEND CMAKE_CXX_FLAGS " -ffreestanding")           # No hosted C++ library

    # Dead code elimination (reduces binary size)
    string(APPEND CMAKE_CXX_FLAGS " -ffunction-sections -fdata-sections")
    string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections")

    # Link-time optimization (further size reduction)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -flto")
    string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE " -flto")

    message(STATUS "Target: EMBEDDED (bare-metal, no STL)")
else()
    # === Hosted Linux/Windows Settings ===

    # -O3      Maximum optimization for speed
    # -DNDEBUG Disable assert() for production
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ${WARNING_FLAGS}")

    message(STATUS "Target: HOSTED (Linux/Windows with STL)")
endif()

# ----------------------------------------------------------------------------
# Print Summary
# ----------------------------------------------------------------------------
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
