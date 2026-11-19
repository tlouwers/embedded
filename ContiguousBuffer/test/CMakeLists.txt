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
    TEST_StaticStorage.cpp
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

# The tests use SetState()/CheckState(): enable them in every configuration
target_compile_definitions(ContiguousRingbufferTest PRIVATE CONTIGUOUS_RINGBUFFER_TEST_HOOKS)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME ContiguousRingbufferTest COMMAND ContiguousRingbufferTest)
