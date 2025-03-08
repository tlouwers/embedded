cmake_minimum_required(VERSION 3.10)

project(SlimAssert)

# Include common settings (if any)
include(${CMAKE_SOURCE_DIR}/../CMakeCommonSettings.cmake)
include(${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake)

# Check if the included files exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../CMakeCommonSettings.cmake")
    message(FATAL_ERROR "CMakeCommonSettings.cmake not found!")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake")
    message(FATAL_ERROR "CrossPlatform.cmake not found!")
endif()

set(SOURCES
    Main.cpp
)

# Create an executable from the source files
add_executable(SlimAssertMain ${SOURCES})

# Include the current source directory to find SlimAssert.h
target_include_directories(SlimAssertMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------------------------

# Use a variable for clarity
set(CLEAN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/CleanBuildDirectory.cmake")

# Write out the script that uses the CrossPlatform helper
file(WRITE ${CLEAN_SCRIPT}
"include(\"${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake\")\n"
"cp_remove_directory(\"${CMAKE_CURRENT_BINARY_DIR}\")\n"
)

# Print messages to ensure the script is generated as expected.
message(STATUS "CleanBuildDirectory.cmake generated at: ${CLEAN_SCRIPT}")
