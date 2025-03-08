cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TEST_Blocks.cpp
    TEST_Capacity.cpp
    TEST_Clear.cpp
    TEST_HistoricalIssues.cpp
    TEST_Peek.cpp
    TEST_Poke.cpp
    TEST_Read.cpp
    TEST_Resize.cpp
    TEST_Size.cpp
    TEST_Speed.cpp
    TEST_Threading.cpp
    TEST_Wrap.cpp
    TEST_Write.cpp
    TEST_WriteAndRead.cpp
)

# Create an executable for the tests
add_executable(ContiguousRingbufferTest ${TEST_SOURCES})

# Link the ContiguousRingbuffer library and Google Test libraries
target_link_libraries(ContiguousRingbufferTest ContiguousRingbuffer gtest gtest_main)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME ContiguousRingbufferTest COMMAND ContiguousRingbufferTest)
