cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TEST_Fill.cpp
    TEST_Float.cpp
    TEST_Integer.cpp
    TEST_Limits.cpp
    TEST_LongRunning.cpp
    TEST_Resize.cpp
)

# Create an executable for the tests
add_executable(MovingAverageTest ${TEST_SOURCES})

# Link the MovingAverage library and Google Test libraries
target_link_libraries(MovingAverageTest MovingAverage gtest gtest_main)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME MovingAverageTest COMMAND MovingAverageTest)
