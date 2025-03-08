cmake_minimum_required(VERSION 3.10)

project(BubbleSort)

# Include common settings (if any)
include(${CMAKE_SOURCE_DIR}/../../CMakeCommonSettings.cmake)
include(${CMAKE_SOURCE_DIR}/../../CrossPlatform.cmake)

# Check if the included files exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../../CMakeCommonSettings.cmake")
    message(FATAL_ERROR "CMakeCommonSettings.cmake not found!")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../../CrossPlatform.cmake")
    message(FATAL_ERROR "CrossPlatform.cmake not found!")
endif()

# The header file is used to build a header-only library.
set(SOURCES
    BubbleSort.hpp   # For testing we use some undisclosed interface methods
)

add_library(BubbleSort INTERFACE)
target_include_directories(BubbleSort INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTS)
    # Include the Google Test directory
    add_subdirectory(../../3rd-party/googletest googletest_build)

    # Add the test directory
    add_subdirectory(test)
else()
    # When not building tests, build main.cpp into a release executable.
    add_executable(BubbleSortMain Main.cpp)

    # Add the current source directory to the include path so Main.cpp can find the header.
    target_include_directories(BubbleSortMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(BubbleSortMain PRIVATE BubbleSort)
endif()

# -----------------------------------------------

# Use a variable for clarity
set(CLEAN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/CleanBuildDirectory.cmake")

# Write out the script that uses the CrossPlatform helper
file(WRITE ${CLEAN_SCRIPT}
"include(\"${CMAKE_SOURCE_DIR}/../../CrossPlatform.cmake\")\n"
"cp_remove_directory(\"${CMAKE_CURRENT_BINARY_DIR}\")\n"
)

# Print messages to ensure the script is generated as expected.
message(STATUS "CleanBuildDirectory.cmake generated at: ${CLEAN_SCRIPT}")
