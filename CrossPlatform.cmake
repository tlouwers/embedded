# CrossPlatform.cmake
# This helper file provides cross-platform utilities for Windows and Linux.

cmake_minimum_required(VERSION 3.10)

# Detect the current platform and set corresponding variables.
if(WIN32)
    set(CP_PLATFORM "Windows")
    set(CP_WINDOWS TRUE)
    set(EXE_EXT ".exe")
    set(CMAKE_GENERATOR "Ninja")  # Use Ninja on Windows
elseif(UNIX)
    # This block covers Linux and other Unix-like systems.
    set(CP_PLATFORM "Linux")
    set(CP_LINUX TRUE)
    set(EXE_EXT "")
    set(CMAKE_GENERATOR "Unix Makefiles")  # Use Make on Linux
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

message(STATUS "CrossPlatform: Detected platform ${CP_PLATFORM}")
message(STATUS "Using generator: ${CMAKE_GENERATOR}")

# Function to remove a directory.
function(cp_remove_directory dir_to_remove)
    if(EXISTS "${dir_to_remove}")
        message(STATUS "CrossPlatform: Removing directory ${dir_to_remove}")
        execute_process(COMMAND "${CMAKE_COMMAND}" -E remove_directory "${dir_to_remove}"
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE out
                        ERROR_VARIABLE err)
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to remove directory ${dir_to_remove}: ${err}")
        endif()
    else()
        message(STATUS "CrossPlatform: Directory ${dir_to_remove} does not exist; nothing to remove")
    endif()
endfunction()

# Function to create a directory using CMake's -E make_directory:
function(cp_make_directory dir_to_make)
    if(NOT EXISTS "${dir_to_make}")
        message(STATUS "CrossPlatform: Creating directory ${dir_to_make}")
        execute_process(COMMAND "${CMAKE_COMMAND}" -E make_directory "${dir_to_make}"
                        RESULT_VARIABLE result)
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to create directory ${dir_to_make}")
        endif()
    else()
        message(STATUS "CrossPlatform: Directory ${dir_to_make} already exists.")
    endif()
endfunction()
