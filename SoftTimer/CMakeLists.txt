cmake_minimum_required(VERSION 3.10)

project(SoftTimer)

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

# The source files for the library
set(SOURCES
    SoftTimer.cpp
)

# Create a static library (or shared if needed)
add_library(SoftTimer STATIC ${SOURCES})

# Include the current source directory for the library
target_include_directories(SoftTimer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)

if(BUILD_TESTS)
    # Include the Google Test directory
    add_subdirectory(../3rd-party/googletest googletest_build)

    # Add the test directory
    add_subdirectory(test)
else()
    set(CMAKE_VERBOSE_MAKEFILE ON)  # For logging

    # Check if Main.cpp exists before adding it
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")
        # Build Main.cpp into a release executable.
        add_executable(SoftTimerMain Main.cpp)

        # Link the SoftTimer library to the SoftTimerMain executable
        target_link_libraries(SoftTimerMain PRIVATE SoftTimer)

        # Add the current source directory to the include path so Main.cpp can find the header.
        target_include_directories(SoftTimerMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(FATAL_ERROR "Main.cpp not found in the current source directory!")
    endif()
endif()

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
