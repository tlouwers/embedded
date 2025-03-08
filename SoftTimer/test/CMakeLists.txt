cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TestSoftTimer.cpp
)

# Create an executable for the tests
add_executable(SoftTimerTest ${TEST_SOURCES})

# Link the SoftTimer library and Google Test libraries
target_link_libraries(SoftTimerTest SoftTimer gtest gtest_main)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME SoftTimerTest COMMAND SoftTimerTest)
