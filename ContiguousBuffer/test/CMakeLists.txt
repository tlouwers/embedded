cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TEST_Blocks.cpp
    TEST_Capacity.cpp
    TEST_Clear.cpp
    TEST_HistoricalIssues.cpp
    TEST_ReserveRead.cpp
    TEST_ReserveWrite.cpp
    TEST_CommitRead.cpp
    TEST_Resize.cpp
    TEST_Size.cpp
    TEST_Speed.cpp
    TEST_Threading.cpp
    TEST_Wrap.cpp
    TEST_CommitWrite.cpp
    TEST_WriteAndRead.cpp
)

# Create an executable for the tests
add_executable(ContiguousRingbufferTest ${TEST_SOURCES})
target_link_libraries(ContiguousRingbufferTest PRIVATE ContiguousRingbuffer gtest gtest_main)

# Define DEBUG for single-config generators (CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ContiguousRingbufferTest PRIVATE DEBUG)
endif()

# Also define DEBUG for multi-config generators when the Debug config is selected
target_compile_definitions(ContiguousRingbufferTest PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME ContiguousRingbufferTest COMMAND ContiguousRingbufferTest)
