#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"

class TEST_WriteAndRead : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void TearDown() override
    {
        mRingBuffer.Clear();
    };

    // Helper method to add an element to buffer
    bool AddOne(int val) {
        bool result = true;
        int* data = nullptr;
        size_t size = 1;

        result &= mRingBuffer.Poke(data, size);
        EXPECT_TRUE(result);

        if (result) {
            data[0] = val;
            result &= mRingBuffer.Write(1);
            EXPECT_TRUE(result);
        }

        return result;
    };

    // Helper method to remove an element from buffer
    bool RemoveOne(int& val) {
        bool result = true;
        int* data = nullptr;
        size_t size = 1;

        result &= mRingBuffer.Peek(data, size);

        if (result) {
            val = data[0];
            result &= mRingBuffer.Read(1);
            EXPECT_TRUE(result);
        }

        return result;
    };
};


TEST_F(TEST_WriteAndRead, WriteAndReadOperations_BlockSize1) {
    EXPECT_TRUE(mRingBuffer.Resize(10));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;
    size_t size = 0;

    for (int i = 1; i <= 100; i++) {
        size = 1;
        EXPECT_TRUE(mRingBuffer.Poke(data, size));
        data[0] = i;
        EXPECT_TRUE(mRingBuffer.Write(1));

        size = 1;
        EXPECT_TRUE(mRingBuffer.Peek(data, size));
        EXPECT_EQ(data[0], i);
        EXPECT_TRUE(mRingBuffer.Read(1));
    }

    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_BlockSize2) {
    EXPECT_TRUE(mRingBuffer.Resize(10));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;
    size_t size = 0;

    for (int i = 1; i <= 100; i += 2) {
        size = 2;
        EXPECT_TRUE(mRingBuffer.Poke(data, size));
        data[0] = i;
        data[1] = i + 1;
        EXPECT_TRUE(mRingBuffer.Write(2));

        size = 2;
        EXPECT_TRUE(mRingBuffer.Peek(data, size));
        EXPECT_EQ(data[0], i);
        EXPECT_EQ(data[1], (i + 1));
        EXPECT_TRUE(mRingBuffer.Read(2));
    }

    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_BlockSize3) {
    EXPECT_TRUE(mRingBuffer.Resize(10));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data;
    size_t size = 0;

    for (int i = 1; i <= 100; i += 3) {
        size = 3;
        EXPECT_TRUE(mRingBuffer.Poke(data, size));
        data[0] = i;
        data[1] = i + 1;
        data[2] = i + 2;
        EXPECT_TRUE(mRingBuffer.Write(3));

        size = 3;
        EXPECT_TRUE(mRingBuffer.Peek(data, size));
        EXPECT_EQ(data[0], i);
        EXPECT_EQ(data[1], (i + 1));
        EXPECT_EQ(data[2], (i + 2));
        EXPECT_TRUE(mRingBuffer.Read(3));
    }

    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_BlockSize4) {
    EXPECT_TRUE(mRingBuffer.Resize(10));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;
    size_t size = 0;

    for (int i = 1; i <= 100; i += 4) {
        size = 4;
        EXPECT_TRUE(mRingBuffer.Poke(data, size));
        data[0] = i;
        data[1] = i + 1;
        data[2] = i + 2;
        data[3] = i + 3;
        EXPECT_TRUE(mRingBuffer.Write(4));

        size = 4;
        EXPECT_TRUE(mRingBuffer.Peek(data, size));
        EXPECT_EQ(data[0], i);
        EXPECT_EQ(data[1], (i + 1));
        EXPECT_EQ(data[2], (i + 2));
        EXPECT_EQ(data[3], (i + 3));
        EXPECT_TRUE(mRingBuffer.Read(4));
    }

    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_DeepInspection) {
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int index = 1;
    int val = -1;

    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));
    EXPECT_EQ(val, 1);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(mRingBuffer.CheckState(2, 1, 4));

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(mRingBuffer.CheckState(2, 2, 4));
    EXPECT_EQ(val, 2);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 2, 4));

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3, 4));
    EXPECT_EQ(val, 3);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));
    EXPECT_EQ(val, 4);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));
    EXPECT_EQ(val, 5);

    mRingBuffer.Clear();
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_DataMustBeContiguous) {
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(3, 3, 4); // Set mWrite(3), mRead(3), mWrap(4)

    int* data = nullptr;
    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // 3 elements, 1 at end, other 2 at start not contiguous
    EXPECT_TRUE(mRingBuffer.Write(1));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));

    mRingBuffer.SetState(0, 0, 4); // Set mWrite(0), mRead(0), mWrap(4)

    size = 3;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // 3 elements available at start
    EXPECT_EQ(mRingBuffer.Size(), 0); // Nothing in buffer yet
    EXPECT_TRUE(mRingBuffer.Write(3));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 4));

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Buffer full: 3 elements placed
    EXPECT_EQ(mRingBuffer.Size(), 3);

    EXPECT_TRUE(mRingBuffer.Read(1)); // Remove 1 element, 2 elements remain in the middle

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // 1 element available at end
    EXPECT_EQ(size, 1);
    EXPECT_TRUE(mRingBuffer.Write(1));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 1, 4));
}

TEST_F(TEST_WriteAndRead, WriteAndReadOperations_WithWrap) {
    EXPECT_TRUE(mRingBuffer.Resize(5));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(AddOne(1)); // Add 5 elements
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(AddOne(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(AddOne(3));
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(AddOne(4));
    EXPECT_EQ(mRingBuffer.Size(), 4);
    EXPECT_TRUE(AddOne(5));
    EXPECT_EQ(mRingBuffer.Size(), 5);

    int val = -1;
    EXPECT_TRUE(RemoveOne(val)); // Remove 3 elements
    EXPECT_EQ(val, 1);
    EXPECT_EQ(mRingBuffer.Size(), 4);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 2);
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 3);
    EXPECT_EQ(mRingBuffer.Size(), 2);

    EXPECT_TRUE(AddOne(6)); // Write wraps, value at end of buffer
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 6) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 4);
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 5);
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 6);
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_WriteAndRead, RegressionCheck_ThreadingIssue) {
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(AddOne(1)); // Add 3 elements
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(AddOne(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(AddOne(3));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    int val = -1;
    EXPECT_TRUE(RemoveOne(val)); // Remove 2 elements
    EXPECT_EQ(val, 1);
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 2);
    EXPECT_EQ(mRingBuffer.Size(), 1);

    EXPECT_TRUE(AddOne(4));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // Had threading issue here: depending on the value of read the available space is calculated incorrectly.
    size_t size = 1;
    int* data;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);
    data[0] = 5;
    EXPECT_TRUE(mRingBuffer.Write(1));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 2, 4));
}
