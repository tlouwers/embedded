#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"
#include <cstddef>      // size_t
#include <cstdint>      // uint32_t
#include <chrono>

class TEST_Speed : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override {
        EXPECT_TRUE(mRingBuffer.Resize(40));
        EXPECT_EQ(mRingBuffer.Size(), 0);
    }

    void TearDown() override {
        mRingBuffer.Clear();
    }

    // Helper method to add a block of elements to buffer
    bool AddBlock(int index_start, size_t block_size) {
        int* data = nullptr;
        size_t size = block_size;

        if (mRingBuffer.Poke(data, size)) {
            // Fill the buffer with 'known' values
            for (size_t i = 0; i < block_size; i++) {
                data[i] = index_start++;
            }

            return mRingBuffer.Write(block_size);
        }
        return false;
    }

    // Helper method to remove a block of elements
    bool RemoveBlock(size_t block_size) {
        int* data = nullptr;
        size_t size = block_size;

        if (mRingBuffer.Peek(data, size)) {
            // Empty the buffer with 'known' values
            for (size_t i = 1; i < block_size; i++)
            {
                if (data[i] <= data[i-1])
                {
                    return false;
                }
            }

            return mRingBuffer.Read(size);
        }
        return false;
    }
};

TEST_F(TEST_Speed, SpeedCheck) {
    const size_t kBlockSize = 7;
    uint32_t count = 0;
    const uint32_t nrOfRuns = 2000000;

    // Speed check
    auto start = std::chrono::steady_clock::now();

    bool result = true;

    for (uint32_t i = 0; i < nrOfRuns; i++) {
        result &= AddBlock(count, kBlockSize);
        result &= RemoveBlock(kBlockSize);
        count += kBlockSize;

        if (!result) {
            break;
        }
    }

    auto end = std::chrono::steady_clock::now();

    EXPECT_TRUE(result);

#ifndef NDEBUG
    std::cerr << "Using DEBUG build - results are NOT accurate" << std::endl;
#endif // NDEBUG

    std::cerr << "Duration of speed check: "
              << (std::chrono::duration<double, std::milli>(end - start).count())
              << " milliseconds" << std::endl;
}
