#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"

class TEST_Wrap : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override {
        EXPECT_TRUE(mRingBuffer.Resize(40));
        EXPECT_EQ(mRingBuffer.Size(), 0);
    }

    void TearDown() override
    {
        mRingBuffer.Clear();
    };

    // Helper method to set the state of the ring buffer
    void SetRingBufferState(int write, int read, int wrap) {
        mRingBuffer.SetState(write, read, wrap);
    }

    // Helper method to check the state of the ring buffer
    bool CheckRingBufferState(int write, int read, int wrap) {
        return mRingBuffer.CheckState(write, read, wrap);
    }
};

TEST_F(TEST_Wrap, SmallestWrapPossible) {
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----
    SetRingBufferState(3, 3, 4); // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
    EXPECT_TRUE(CheckRingBufferState(3, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;

    size_t size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    EXPECT_TRUE(mRingBuffer.Write(2)); // Write block of 2 elements, shrinks wrap by 1
    EXPECT_TRUE(CheckRingBufferState(2, 3, 3));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    EXPECT_TRUE(mRingBuffer.Read(1)); // Read 1 element, restores wrap
    EXPECT_TRUE(CheckRingBufferState(2, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // --

    SetRingBufferState(3, 3, 4);   // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
    EXPECT_TRUE(CheckRingBufferState(3, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(mRingBuffer.Write(2)); // Write block of 2 elements, shrinks wrap by 1
    EXPECT_TRUE(CheckRingBufferState(2, 3, 3));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    EXPECT_TRUE(mRingBuffer.Read(2)); // Read 2 elements, restores wrap
    EXPECT_TRUE(CheckRingBufferState(2, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // --
    SetRingBufferState(3, 3, 4);   // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
    EXPECT_TRUE(CheckRingBufferState(3, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    size = 3;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // Exception: if write == read and size == read allow Poke() to reset mWrite and mRead
    EXPECT_EQ(size, 3);
}

TEST_F(TEST_Wrap, WrapWithSmallBufferReadStartsUnequalToWrap) {
    EXPECT_TRUE(mRingBuffer.Resize(4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----
    SetRingBufferState(3, 3, 5); // Set mWrite(3), mRead(3), mWrap(5) - 2 elements available at end, 2 elements available at start

    int* data = nullptr;

    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);
    data[0] = 4;                       // Actually adding data, will be checked later
    EXPECT_TRUE(mRingBuffer.Write(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // Only 1 element available at end, but 2 available at start
    EXPECT_EQ(size, 2);
    data[0] = 1;                     // Actually adding data, will be checked later
    data[1] = 2;
    EXPECT_TRUE(mRingBuffer.Write(2)); // Write block of 2 elements, shrinks wrap by 1
    EXPECT_TRUE(CheckRingBufferState(2, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Peek(data, size)); // Only 1 element present at end, 2 elements present at start, buffer wrapped
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Peek(data, size));
    EXPECT_EQ(data[0], 4);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Read 1 element, restores wrap
    EXPECT_TRUE(CheckRingBufferState(2, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Peek(data, size)); // 2 elements present at start
    EXPECT_EQ(size, 2);
    size = 3;
    EXPECT_FALSE(mRingBuffer.Peek(data, size)); // 2 elements present at start
    EXPECT_EQ(size, 0);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Peek(data, size)); // 2 elements present at start
    EXPECT_EQ(size, 2);
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_TRUE(mRingBuffer.Read(2)); // Read 2 elements
    EXPECT_TRUE(CheckRingBufferState(2, 2, 5));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_Wrap, WrapWithSmallBufferReadStartsEqualToWrap) {
    EXPECT_TRUE(mRingBuffer.Resize(4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----
    // Can get here by filling buffer with 4 elements, then removing 4 elements
    SetRingBufferState(4, 4, 5); // Set mWrite(4), mRead(4), mWrap(5) - 1 element available at end, 3 elements available at start

    int* data = nullptr;

    size_t size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    data[0] = 7;                   // Actually adding data, will be checked later
    data[1] = 8;
    EXPECT_TRUE(mRingBuffer.Write(2)); // Write block of 2 elements, shrinks wrap by 1
    EXPECT_TRUE(CheckRingBufferState(2, 4, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Peek(data, size)); // 2 elements present at start, buffer wrapped
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.Peek(data, size)); // 2 elements present at start
    EXPECT_EQ(size, 0);

    EXPECT_EQ(mRingBuffer.Size(), 2);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Peek(data, size)); // 2 elements present at start
    EXPECT_EQ(size, 2);
    EXPECT_EQ(data[0], 7);
    EXPECT_EQ(data[1], 8);
    EXPECT_TRUE(mRingBuffer.Read(2)); // Read 2 elements, restores wrap
    EXPECT_TRUE(CheckRingBufferState(2, 2, 5));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_Wrap, WrapWithLargeBuffer) {
    EXPECT_TRUE(mRingBuffer.Resize(20));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----
    SetRingBufferState(14, 14, 21); // Set mWrite(14), mRead(14), mWrap(21) - 7 elements available at end, 13 elements available at start

    int* data = nullptr;

    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // 7 contiguous elements available at the end
    EXPECT_EQ(size, 7);

    size = 13;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // And 13 contiguous elements available at the start
    EXPECT_EQ(size, 13);

    EXPECT_TRUE(mRingBuffer.Write(10)); // Wraps, leave 3 elements available
    EXPECT_TRUE(CheckRingBufferState(10, 14, 14));
    EXPECT_EQ(mRingBuffer.Size(), 10);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Peek(data, size));
    EXPECT_EQ(size, 10);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // 3 elements available
    EXPECT_EQ(size, 3);

    EXPECT_TRUE(mRingBuffer.Read(3)); // Read 3 elements, restores wrap, leaving 7 remaining
    EXPECT_TRUE(CheckRingBufferState(10, 3, 21));
    EXPECT_EQ(mRingBuffer.Size(), 7);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Peek(data, size));
    EXPECT_EQ(size, 7);

    EXPECT_TRUE(mRingBuffer.Read(7)); // Read remaining 7 elements
    EXPECT_TRUE(CheckRingBufferState(10, 10, 21));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}
