cmake_minimum_required(VERSION 3.10)

project(Arbiter)

# Include common settings (if any)
include(${CMAKE_SOURCE_DIR}/../CMakeCommonSettings.cmake)
include(${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake)

# Check if the included files exist (fail early with helpful message)
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../CMakeCommonSettings.cmake")
    message(FATAL_ERROR "CMakeCommonSettings.cmake not found!")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake")
    message(FATAL_ERROR "CrossPlatform.cmake not found!")
endif()

set(SOURCES
    scr/main.cpp
    scr/Application_Stub.cpp
    scr/i2c_arbiter.cpp
    scr/i2c_drv_stub.cpp
)

add_executable(ArbiterMain ${SOURCES})

# Define DEBUG for single-config generators when Debug is selected
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(ArbiterMain PRIVATE DEBUG)
endif()

# Define DEBUG for multi-config generators when Debug config is built
target_compile_definitions(ArbiterMain PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(ArbiterMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/scr)

# Standard link libraries (none required for this example)

# Write out a clean script as used by other projects
set(CLEAN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/CleanBuildDirectory.cmake")
file(WRITE ${CLEAN_SCRIPT}
"include(\"${CMAKE_SOURCE_DIR}/../CrossPlatform.cmake\")\n"
"cp_remove_directory(\"${CMAKE_CURRENT_BINARY_DIR}\")\n"
)

message(STATUS "CleanBuildDirectory.cmake generated at: ${CLEAN_SCRIPT}")
