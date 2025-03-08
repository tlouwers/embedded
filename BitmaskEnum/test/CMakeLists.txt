cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TEST_Invalid.cpp
    TEST_Mixed.cpp
    TEST_AND.cpp
    TEST_NOT.cpp
    TEST_OR.cpp
    TEST_XOR.cpp
)

# Create an executable for the tests
add_executable(BitmaskEnumTest ${TEST_SOURCES})

# Link the BitmaskEnum library and Google Test libraries
target_link_libraries(BitmaskEnumTest BitmaskEnum gtest gtest_main)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME BitmaskEnumTest COMMAND BitmaskEnumTest)
