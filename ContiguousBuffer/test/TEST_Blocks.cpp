#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"


class TEST_Blocks : public ::testing::Test {
protected:
    const size_t kBlockSize = 256;

    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override
    {
        EXPECT_TRUE(mRingBuffer.Resize(kBlockSize * 4));
        EXPECT_EQ(mRingBuffer.Size(), 0);
    };

    void TearDown() override
    {
        mRingBuffer.Clear();
    };

    // Helper method to add a block of elements to buffer
    bool AddBlock(int index_start, size_t kBlockSize) {
        int* data   = nullptr;
        size_t size = kBlockSize;

        if (mRingBuffer.Poke(data, size)) {
            // Fill the buffer with 'known' values
            for (size_t i = 0; i < kBlockSize; i++) {
                data[i] = index_start++;
            }

            size = kBlockSize;
            return mRingBuffer.Write(size);
        }
        return false;
    }

    // Helper method to remove a block of elements
    bool RemoveBlock(int index_start, size_t kBlockSize) {
        int* data   = nullptr;
        size_t size = kBlockSize;

        if (mRingBuffer.Peek(data, size)) {
            // Empty the buffer with 'known' values
            for (size_t i = 0; i < kBlockSize; i++) {
                EXPECT_EQ(data[i], index_start);
                index_start++;
            }

            size = kBlockSize;
            return mRingBuffer.Read(size);
        }
        return false;
    }
};

TEST_F(TEST_Blocks, LargeBlocksStartAtStart) {
    int index = 0;
    int* data = nullptr;
    size_t size = 0;

    EXPECT_TRUE(mRingBuffer.Resize(kBlockSize * 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // Add blocks and check state
    EXPECT_TRUE(AddBlock(index, kBlockSize));           // Block written at the start, wrap reduced from 1025 to 1024
    index += kBlockSize;                                // 1 block = 256 elements in buffer
    EXPECT_TRUE(AddBlock(index, kBlockSize));
    index += kBlockSize;                                // 2 blocks = 512 elements in buffer
    EXPECT_TRUE(AddBlock(index, kBlockSize));
    index += kBlockSize;                                // 3 blocks = 768 elements in buffer
    EXPECT_TRUE(AddBlock(index, kBlockSize));
    index += kBlockSize;                                // 4 blocks = 1024 elements in buffer

    EXPECT_EQ(mRingBuffer.CheckState(1024, 0, 1025), true);   // Wrapped

    size = kBlockSize;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));         // Cannot add another block
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.CheckState(1024, 0, 1025), true);

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));         // Cannot even add a single element
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.CheckState(1024, 0, 1025), true);

    size = kBlockSize;
    EXPECT_TRUE(mRingBuffer.Peek(data, size));          // Elements available from the start, 4 blocks
    EXPECT_EQ(size, 1024);
    EXPECT_EQ(mRingBuffer.Size(), 1024);

    index = 0;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Read 1 block, 3 remaining
    EXPECT_EQ(mRingBuffer.Size(), 768);
    index += kBlockSize;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Read another block, 2 remaining
    EXPECT_EQ(mRingBuffer.Size(), 512);
    index += kBlockSize;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Read another block, 1 remaining
    EXPECT_EQ(mRingBuffer.Size(), 256);
    index += kBlockSize;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_Blocks, LargeBlocksStartAtEnd) {
    int index = 0;
    int* data = nullptr;
    size_t size = 0;

    EXPECT_TRUE(mRingBuffer.Resize(kBlockSize * 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1024, 1024, 1025);            // Filled 4 blocks, removed 4 blocks

    EXPECT_TRUE(AddBlock(index, kBlockSize));           // Block written at the start, wrap reduced from 1025 to 1024
    EXPECT_EQ(mRingBuffer.Size(), 256);
    index += kBlockSize;                                // 1 block = 256 elements in buffer
    EXPECT_TRUE(AddBlock(index, kBlockSize));
    EXPECT_EQ(mRingBuffer.Size(), 512);
    index += kBlockSize;                                // 2 blocks = 512 elements in buffer
    EXPECT_TRUE(AddBlock(index, kBlockSize));
    EXPECT_EQ(mRingBuffer.Size(), 768);
    index += kBlockSize;                                // 3 blocks = 768 elements in buffer

    size = kBlockSize;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));         // Cannot add another block
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.CheckState(768, 1024, 1024), true);

    size = kBlockSize;
    EXPECT_TRUE(mRingBuffer.Peek(data, size));          // Elements available from the start, 3 blocks
    EXPECT_EQ(size, 768);
    EXPECT_EQ(mRingBuffer.Size(), 768);

    index = 0;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Read 1 block, 2 remaining
    index += kBlockSize;
    EXPECT_EQ(mRingBuffer.CheckState(768, 256, 1025), true);
    EXPECT_EQ(mRingBuffer.Size(), 512);                 // After read still 2 blocks available

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Read another block, 1 remaining
    EXPECT_EQ(mRingBuffer.Size(), 256);
    index += kBlockSize;

    EXPECT_TRUE(RemoveBlock(index, kBlockSize));        // Buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);
}
