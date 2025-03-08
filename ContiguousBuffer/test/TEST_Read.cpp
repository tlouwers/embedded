#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"

class TEST_Read : public ::testing::Test {
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

TEST_F(TEST_Read, ReadOperations_ReadAt0) {
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 0, 4);   // Set mWrite(0), mRead(0), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 0, 4);
    EXPECT_FALSE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(1, 0, 4);   // Set mWrite(1), mRead(0), mWrap(4) - 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(1, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1, 0, 4);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(2, 0, 4);   // Set mWrite(2), mRead(0), mWrap(4) - 2 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(2, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(2, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(2, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(2, 0, 4);
    EXPECT_FALSE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(3, 0, 4);   // Set mWrite(3), mRead(0), mWrap(4) - 3 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(3, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(3, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(3, 0, 4);
    EXPECT_TRUE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(3, 0, 4);
    EXPECT_FALSE(mRingBuffer.Read(4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
}

TEST_F(TEST_Read, ReadOperations_ReadAt1) {
    mRingBuffer.SetState(0, 1, 4);   // Set mWrite(0), mRead(1), mWrap(4) - 3 elements at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(0, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(0, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(0, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 1, 4);
    EXPECT_FALSE(mRingBuffer.Read(4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(1, 1, 4);   // Set mWrite(1), mRead(1), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1, 1, 4);
    EXPECT_FALSE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(2, 1, 4);   // Set mWrite(2), mRead(1), mWrap(4) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(2, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(2, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(2, 1, 4);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(3, 1, 4);   // Set mWrite(3), mRead(1), mWrap(4) - 2 elements
    EXPECT_TRUE(mRingBuffer.CheckState(3, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(3, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(3, 1, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(3, 1, 4);
    EXPECT_FALSE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 2);
}

TEST_F(TEST_Read, ReadOperations_ReadAt2) {
    mRingBuffer.SetState(0, 2, 4); // Set mWrite(0), mRead(2), mWrap(4) - 2 elements at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(0, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(0, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 2, 4);
    EXPECT_FALSE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(1, 2, 4); // Set mWrite(1), mRead(2), mWrap(4) - 2 elements at end, 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(1, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available element at end
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(1, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(2)); // Uses the available element at end
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(1, 2, 4);
    EXPECT_FALSE(mRingBuffer.Read(3)); // Cannot read over the wrap, must be in 2 iterations
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(1, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available element at start
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(2, 2, 4); // Set mWrite(2), mRead(2), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(2, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(2, 2, 4);
    EXPECT_FALSE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(3, 2, 4); // Set mWrite(3), mRead(2), mWrap(4) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(3, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(3, 2, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(3, 2, 4);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);
}

TEST_F(TEST_Read, ReadOperations_ReadAt3) {
    mRingBuffer.SetState(0, 3, 4); // Set mWrite(0), mRead(3), mWrap(4) - 1 element at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(0, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(0, 3, 4);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(1, 3, 4); // Set mWrite(1), mRead(3), mWrap(4) - 1 element at end, 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(1, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available element at end
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(1, 3, 4);
    EXPECT_FALSE(mRingBuffer.Read(2)); // Cannot read over the wrap, must be in 2 iterations
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(1, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available element at start
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(1, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 1);
    EXPECT_FALSE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 1);

    // -----

    mRingBuffer.SetState(2, 3, 4); // Set mWrite(2), mRead(3), mWrap(4) - 1 element at end, 2 elements at front
    EXPECT_TRUE(mRingBuffer.CheckState(2, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(2, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available element at end
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.SetState(2, 3, 4);
    EXPECT_FALSE(mRingBuffer.Read(2)); // Cannot read over the wrap, must be in 2 iterations
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.SetState(2, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(1)); // Uses the available elements at start
    EXPECT_EQ(mRingBuffer.Size(), 1);

    mRingBuffer.SetState(2, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_TRUE(mRingBuffer.Read(2));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(2, 3, 4);
    EXPECT_TRUE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 2);
    EXPECT_FALSE(mRingBuffer.Read(3));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // -----

    mRingBuffer.SetState(3, 3, 4); // Set mWrite(3), mRead(3), mWrap(4) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    EXPECT_TRUE(mRingBuffer.Read(0));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    mRingBuffer.SetState(3, 3, 4);
    EXPECT_FALSE(mRingBuffer.Read(1));
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_Read, ReadOperations_InvalidStates) {
    mRingBuffer.SetState(0, 4, 4); // Set mWrite(0), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(0, 4, 4));
    EXPECT_FALSE(mRingBuffer.Read(1)); // Handle as: buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(1, 4, 4); // Set mWrite(1), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(1, 4, 4));
    EXPECT_FALSE(mRingBuffer.Read(1)); // Handle as: buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(2, 4, 4); // Set mWrite(2), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(2, 4, 4));
    EXPECT_FALSE(mRingBuffer.Read(1)); // Handle as: buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(3, 4, 4); // Set mWrite(3), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(3, 4, 4));
    EXPECT_FALSE(mRingBuffer.Read(1)); // Handle as: buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);

    // -----

    mRingBuffer.SetState(0, 5, 4); // Set mWrite(0), mRead(5), mWrap(4) - 2 elements more than possible - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(0, 5, 4));
    EXPECT_FALSE(mRingBuffer.Read(1)); // Handle as: buffer empty
    EXPECT_EQ(mRingBuffer.Size(), 0);
}
