cmake_minimum_required(VERSION 3.10)

# No need to find_package(GTest REQUIRED) since we are building it from source

# Add the test source files
set(TEST_SOURCES
    TEST_Main.cpp
    TEST_Clear.cpp
    TEST_DifferentElementTypes.cpp
    TEST_Size.cpp
    TEST_Threading.cpp
    TEST_TryPop.cpp
    TEST_TryPush.cpp
    TEST_TryPushAndTryPop.cpp
)

# Create an executable for the tests
add_executable(RingbufferTest ${TEST_SOURCES})

# Link the Ringbuffer library and Google Test libraries
target_link_libraries(RingbufferTest Ringbuffer gtest gtest_main)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME RingbufferTest COMMAND RingbufferTest)
