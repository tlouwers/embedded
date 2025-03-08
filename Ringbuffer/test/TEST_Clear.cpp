#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferTestClear : public ::testing::Test {
protected:
    Ringbuffer<int> mRingBuffer;

    void SetUp() override {
        mRingBuffer.Resize(3);
    }
};

TEST_F(RingbufferTestClear, BasicOperationsReadAt0) {
    mRingBuffer.SetState(0, 0); // Set mWrite(0), mRead(0) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(1, 0); // Set mWrite(1), mRead(0) - 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(2, 0); // Set mWrite(2), mRead(0) - 2 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(2, 0));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(3, 0); // Set mWrite(3), mRead(0) - 3 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));
}

TEST_F(RingbufferTestClear, BasicOperationsReadAt1) {
    mRingBuffer.SetState(0, 1); // Set mWrite(0), mRead(1) - 3 elements at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 1));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(1, 1); // Set mWrite(1), mRead(1) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(2, 1); // Set mWrite(2), mRead(1) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(2, 1));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(3, 1); // Set mWrite(3), mRead(1) - 2 elements
    EXPECT_TRUE(mRingBuffer.CheckState(3, 1));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));
}

TEST_F(RingbufferTestClear, BasicOperationsReadAt2) {
    mRingBuffer.SetState(0, 2); // Set mWrite(0), mRead(2) - 2 elements at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 2));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(1, 2); // Set mWrite(1), mRead(2) - 2 elements at end, 1 element at front
    EXPECT_TRUE(mRingBuffer.CheckState(1, 2));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(2, 2); // Set mWrite(2), mRead(2) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(2, 2));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(3, 2); // Set mWrite(3), mRead(2) - 1 element
    EXPECT_TRUE(mRingBuffer.CheckState(3, 2));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));
}

TEST_F(RingbufferTestClear, BasicOperationsReadAt3) {
    mRingBuffer.SetState(0, 3); // Set mWrite(0), mRead(3) - 1 element at end
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(1, 3); // Set mWrite(1), mRead(3) - 1 element at end, 1 element at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 3));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(2, 3); // Set mWrite(2), mRead(3) - 1 element at end, 2 elements at start
    EXPECT_TRUE(mRingBuffer.CheckState(2, 3));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));

    // -----

    mRingBuffer.SetState(3, 3); // Set mWrite(3), mRead(3) - buffer empty
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3));

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0));
}
