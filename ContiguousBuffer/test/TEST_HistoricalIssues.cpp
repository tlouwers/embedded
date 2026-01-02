#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"


class TEST_HistoricalIssues : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override {
        EXPECT_TRUE(mRingBuffer.Reserve(10));
        EXPECT_EQ(mRingBuffer.Size(), 0);
    }

    void TearDown() override
    {
        mRingBuffer.Clear();
    };

    // Helper method to add an element to buffer
    bool AddOne(int val) {
        bool result = true;
        int* data = nullptr;
        size_t size = 1;

        result &= mRingBuffer.ReserveWrite(data, size);
        EXPECT_TRUE(result);

        if (result) {
            data[0] = val;
            result &= mRingBuffer.CommitWrite(1);
            EXPECT_TRUE(result);
        }

        return result;
    }

    // Helper method to remove an element from buffer
    bool RemoveOne(int& val) {
        bool result = true;
        int* data = nullptr;
        size_t size = 1;

        result &= mRingBuffer.ReserveRead(data, size);

        if (result) {
            val = data[0];
            result &= mRingBuffer.CommitRead(1);
            EXPECT_TRUE(result);
        }

        return result;
    }

    // Helper method to add a block of elements to buffer
    bool AddBlock(int index_start, size_t block_size) {
        bool result = true;
        int* data = nullptr;
        size_t size = block_size;

        result &= mRingBuffer.ReserveWrite(data, size);

        if (result) {
            // Fill the buffer with 'known' values
            for (size_t i = 0; i < block_size; i++) {
                data[i] = index_start++;
            }

            size = block_size;
            result &= mRingBuffer.CommitWrite(size);
        }

        return result;
    }

    // Helper method to remove a block of elements
    bool RemoveBlock(int index_start, size_t block_size) {
        bool result = true;
        int* data = nullptr;
        size_t size = block_size;

        result &= mRingBuffer.ReserveRead(data, size);

        if (result) {
            // Empty the buffer with 'known' values
            for (size_t i = 0; i < block_size; i++) {
                EXPECT_EQ(data[i], index_start);
                index_start++;
            }

            size = block_size;
            result &= mRingBuffer.CommitRead(size);
        }

        return result;
    }
};


TEST_F(TEST_HistoricalIssues, IndicateFirstFilledElementsAtEndThenAtStart) {
    EXPECT_TRUE(mRingBuffer.Reserve(8));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(5, 5, 9); // Set mWrite(5), mRead(5), mWrap(9)

    EXPECT_TRUE(AddOne(6)); // 1 filled, 3 available at end
    EXPECT_TRUE(AddOne(7)); // 2 filled, 2 available at end
    EXPECT_TRUE(mRingBuffer.CheckState(7, 5, 9));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    size_t size = 2;
    int* data = nullptr;
    int val = -1;

    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size)); // 2 elements available at end
    EXPECT_EQ(size, 2);

    size = 3;
    data = nullptr;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size)); // Larger than 2, thus 4 elements available at start
    EXPECT_EQ(size, 4);

    size = 5;
    data = nullptr;
    EXPECT_FALSE(mRingBuffer.ReserveWrite(data, size)); // Although 6 available, only 2 or 4 contiguous
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(7, 5, 9));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    size = 4;
    data = nullptr;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size)); // Largest contiguous block is 4
    EXPECT_EQ(size, 4);
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    EXPECT_TRUE(mRingBuffer.CommitWrite(3));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 5, 7));
    EXPECT_EQ(mRingBuffer.Size(), 5);

    size = 2;
    EXPECT_TRUE(mRingBuffer.ReserveRead(data, size)); // 2 filled at end, 3 at start
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.ReserveRead(data, size)); // Largest filled block is 2 at the end
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(RemoveOne(val)); // Remove 2 at the end, 3 at the start remaining
    EXPECT_EQ(val, 6);
    EXPECT_TRUE(RemoveOne(val));
    EXPECT_EQ(val, 7);
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 9));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    size = 3;
    EXPECT_TRUE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(size, 3);
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
    EXPECT_TRUE(mRingBuffer.CommitRead(3));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3, 9));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_HistoricalIssues, IndicateSpaceAvailableInVariousConditions) {
    EXPECT_TRUE(mRingBuffer.Reserve(4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(4, 0, 5); // Buffer full

    size_t size = 1;
    int* data = nullptr;

    EXPECT_FALSE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 0);
    EXPECT_TRUE(mRingBuffer.CheckState(4, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 4);

    mRingBuffer.SetState(3, 0, 5); // Space for 1 element available

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 1);
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(0, 0, 5); // Buffer empty

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 4);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(4, 4, 5); // Buffer empty, 1 element available at end, 3 at start

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 1);
    EXPECT_EQ(mRingBuffer.Size(), 0);

    size = 2;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 3);
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(mRingBuffer.CheckState(4, 4, 5));

    mRingBuffer.SetState(0, 4, 5); // Space for 3 elements available at start

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 4, 5));
    EXPECT_EQ(mRingBuffer.Size(), 1);
}

TEST_F(TEST_HistoricalIssues, FillingTheBufferReadWriteShiftedToEnd) {
    EXPECT_TRUE(mRingBuffer.Reserve(4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 0, 5); // Set mWrite(0), mRead(0), mWrap(5) - 4 elements available

    size_t size = 1;
    int* data = nullptr;

    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1, 0, 5); // 1 element filled at start, 3 available

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(3, 0, 5); // 3 elements filled at start, 1 available

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(4, 0, 5); // Buffer full

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(4, 0, 5));
    EXPECT_EQ(mRingBuffer.Size(), 4);

    mRingBuffer.SetState(5, 1, 5); // Buffer full

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CheckState(5, 1, 5));
    EXPECT_EQ(mRingBuffer.Size(), 4);
}

TEST_F(TEST_HistoricalIssues, PeekGeneratedFalsePositive) {
    EXPECT_TRUE(mRingBuffer.Reserve(1024));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(AddBlock(1, 1020)); // Add 1020 items, leave 4 remaining
    EXPECT_TRUE(mRingBuffer.CheckState(1020, 0, 1025));

    size_t size = 1;
    int* data = nullptr;

    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size)); // 4 elements available at end
    EXPECT_EQ(size, 4);

    EXPECT_TRUE(AddBlock(1021, 4)); // Fill the remaining 4 elements
    EXPECT_TRUE(mRingBuffer.CheckState(1024, 0, 1025));

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveWrite(data, size)); // Buffer full
    EXPECT_EQ(size, 0);
    EXPECT_TRUE(mRingBuffer.CheckState(1024, 0, 1025));
    EXPECT_EQ(mRingBuffer.Size(), 1024);

    EXPECT_TRUE(RemoveBlock(1, 1024)); // Empty the buffer
    EXPECT_TRUE(mRingBuffer.CheckState(1024, 1024, 1025));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // --

    mRingBuffer.SetState(1023, 1023, 1025); // Buffer empty

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1024, 1024, 1025); // Buffer empty

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 0, 1025);

    size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveRead(data, size)); // Empty buffer
    EXPECT_EQ(size, 0);
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_HistoricalIssues, CannotUseSingleBlockBufferSize) {
    EXPECT_TRUE(mRingBuffer.Reserve(128));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    EXPECT_TRUE(AddBlock(1, 128)); // Fill with a fixed block (size of buffer)
    EXPECT_TRUE(mRingBuffer.CheckState(128, 0, 129));

    EXPECT_TRUE(RemoveBlock(1, 128)); // Read the entire block
    EXPECT_TRUE(mRingBuffer.CheckState(128, 128, 129));

    size_t size = 128;
    int* data = nullptr;

    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size)); // Check a next block would fit (moves read pointer in this exceptional case)
    EXPECT_EQ(size, 128);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 129)); // Note: buffer was empty, Poke() moved mWrite and mRead!

    EXPECT_TRUE(AddBlock(1, 128)); // Fill with a fixed block (size of buffer)
    EXPECT_TRUE(mRingBuffer.CheckState(128, 0, 129));

    size = 128;
    EXPECT_TRUE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(size, 128);

    EXPECT_TRUE(RemoveBlock(1, 128)); // Read the entire block
    EXPECT_TRUE(mRingBuffer.CheckState(128, 128, 129));

    EXPECT_EQ(mRingBuffer.Size(), 0);
}
