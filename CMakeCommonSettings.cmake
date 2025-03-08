# CMakeCommonSettings.cmake
cmake_minimum_required(VERSION 3.10)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler optimization flags for Debug build (no optimization, maximum debug info)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG -Wall -Wextra -Wpedantic")

# Add compiler optimization flags for Release build

# Base settings:
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Additional flags for performance:
# -ftree-vectorize enables auto-vectorization.
# -funroll-loops enables loop unrolling.
# -finline-functions allows function inlining across translation units.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftree-vectorize -funroll-loops -finline-functions")

# Additional flags for robustness:
# -fstack-protector-strong adds extra protection against stack smashing.
# -fno-strict-overflow disables some optimizations that assume no signed overflow - sometimes useful for subtle bugs.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fstack-protector-strong -fno-strict-overflow")

# Optionally, if you want to push performance further, you might experiment with:
# -Ofast (be aware that it relaxes strict standards compliance and might impact floating point accuracy)
# But if you decide to use Ofast, ensure that your code is validated under these assumptions.
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

# Print the flags based on the chosen build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
else()
    message(STATUS "CMAKE_BUILD_TYPE is set to: ${CMAKE_BUILD_TYPE}. No specific flags to print.")
endif()
