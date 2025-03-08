#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"

class TEST_Size : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override {
        EXPECT_TRUE(mRingBuffer.Resize(3));
        EXPECT_EQ(mRingBuffer.Size(), 0);
    }

    void TearDown() override
    {
        mRingBuffer.Clear();
    };
};

TEST_F(TEST_Size, BasicOperations_ReadAt0) {
    mRingBuffer.SetState(0, 0, 4); // Set mWrite(0), mRead(0), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(1, 0, 4); // Set mWrite(1), mRead(0), mWrap(4) - 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(2, 0, 4); // Set mWrite(2), mRead(0), mWrap(4) - 2 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(2, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(3, 0, 4); // Set mWrite(3), mRead(0), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
}

TEST_F(TEST_Size, BasicOperations_ReadAt1) {
    mRingBuffer.SetState(0, 1, 4); // Set mWrite(0), mRead(1), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(0, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(1, 1, 4); // Set mWrite(1), mRead(1), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(2, 1, 4); // Set mWrite(2), mRead(1), mWrap(4) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(2, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(3, 1, 4); // Set mWrite(3), mRead(1), mWrap(4) - 2 elements
    EXPECT_TRUE(mRingBuffer.CheckState(3, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);
}

TEST_F(TEST_Size, BasicOperations_ReadAt2) {
    mRingBuffer.SetState(0, 2, 4); // Set mWrite(0), mRead(2), mWrap(4) - 2 elements
    EXPECT_TRUE(mRingBuffer.CheckState(0, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(1, 2, 4); // Set mWrite(1), mRead(2), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(1, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(2, 2, 4); // Set mWrite(2), mRead(2), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(2, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(3, 2, 4); // Set mWrite(3), mRead(2), mWrap(4) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(3, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);
}

TEST_F(TEST_Size, BasicOperations_ReadAt3) {
    mRingBuffer.SetState(0, 3, 4); // Set mWrite(0), mRead(3), mWrap(4) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(1, 3, 4); // Set mWrite(1), mRead(3), mWrap(4) - 2 elements
    EXPECT_TRUE(mRingBuffer.CheckState(1, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(2, 3, 4); // Set mWrite(2), mRead(3), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(2, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(3, 3, 4); // Set mWrite(3), mRead(3), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}
