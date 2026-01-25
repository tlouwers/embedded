# CrossPlatform.cmake
# ============================================================================
# Platform detection and helper functions for Windows and Linux
#
# Usage in your CMakeLists.txt:
#   include(${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake)
#
# Variables set by this file:
#   CP_PLATFORM  - "Windows" or "Linux"
#   CP_WINDOWS   - TRUE if Windows
#   CP_LINUX     - TRUE if Linux
# ============================================================================

cmake_minimum_required(VERSION 3.10)

# ----------------------------------------------------------------------------
# Platform Detection
# ----------------------------------------------------------------------------
if(WIN32)
    set(CP_PLATFORM "Windows")
    set(CP_WINDOWS TRUE)
    set(EXE_EXT ".exe")
elseif(UNIX)
    set(CP_PLATFORM "Linux")
    set(CP_LINUX TRUE)
    set(EXE_EXT "")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

message(STATUS "Platform: ${CP_PLATFORM}")

# ----------------------------------------------------------------------------
# Helper Functions
# ----------------------------------------------------------------------------

# Remove a directory (use for clean targets)
# Usage: cp_remove_directory("/path/to/dir")
function(cp_remove_directory dir_to_remove)
    if(EXISTS "${dir_to_remove}")
        file(REMOVE_RECURSE "${dir_to_remove}")
        message(STATUS "Removed: ${dir_to_remove}")
    endif()
endfunction()

# Create a directory if it doesn't exist
# Usage: cp_make_directory("/path/to/dir")
function(cp_make_directory dir_to_make)
    if(NOT EXISTS "${dir_to_make}")
        file(MAKE_DIRECTORY "${dir_to_make}")
        message(STATUS "Created: ${dir_to_make}")
    endif()
endfunction()
