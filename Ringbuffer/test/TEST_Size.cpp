#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferSizeTest : public ::testing::Test {
protected:
    Ringbuffer<int> ringBuff;

    void SetUp() override {
        EXPECT_TRUE(ringBuff.Resize(3));
        EXPECT_EQ(ringBuff.Size(), 0);
    }
};

TEST_F(RingbufferSizeTest, BasicOperationsReadAt0) {
    ringBuff.SetState(0, 0); // Set mWrite(0), mRead(0) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(0, 0));
    EXPECT_EQ(ringBuff.Size(), 0);

    // -----

    ringBuff.SetState(1, 0); // Set mWrite(1), mRead(0) - 1 element at start
    EXPECT_TRUE(ringBuff.CheckState(1, 0));
    EXPECT_EQ(ringBuff.Size(), 1);

    // -----

    ringBuff.SetState(2, 0); // Set mWrite(2), mRead(0) - 2 elements at start
    EXPECT_TRUE(ringBuff.CheckState(2, 0));
    EXPECT_EQ(ringBuff.Size(), 2);

    // -----

    ringBuff.SetState(3, 0); // Set mWrite(3), mRead(0) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(3, 0));
    EXPECT_EQ(ringBuff.Size(), 3);
}

TEST_F(RingbufferSizeTest, BasicOperationsReadAt1) {
    ringBuff.SetState(0, 1); // Set mWrite(0), mRead(1) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(0, 1));
    EXPECT_EQ(ringBuff.Size(), 3);

    // -----

    ringBuff.SetState(1, 1); // Set mWrite(1), mRead(1) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(1, 1));
    EXPECT_EQ(ringBuff.Size(), 0);

    // -----

    ringBuff.SetState(2, 1); // Set mWrite(2), mRead(1) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(2, 1));
    EXPECT_EQ(ringBuff.Size(), 1);

    // -----

    ringBuff.SetState(3, 1); // Set mWrite(3), mRead(1) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(3, 1));
    EXPECT_EQ(ringBuff.Size(), 2);
}

TEST_F(RingbufferSizeTest, BasicOperationsReadAt2) {
    ringBuff.SetState(0, 2); // Set mWrite(0), mRead(2) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(0, 2));
    EXPECT_EQ(ringBuff.Size(), 2);

    // -----

    ringBuff.SetState(1, 2); // Set mWrite(1), mRead(2) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(1, 2));
    EXPECT_EQ(ringBuff.Size(), 3);

    // -----

    ringBuff.SetState(2, 2); // Set mWrite(2), mRead(2) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(2, 2));
    EXPECT_EQ(ringBuff.Size(), 0);

    // -----

    ringBuff.SetState(3, 2); // Set mWrite(3), mRead(2) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(3, 2));
    EXPECT_EQ(ringBuff.Size(), 1);
}

TEST_F(RingbufferSizeTest, BasicOperationsReadAt3) {
    ringBuff.SetState(0, 3); // Set mWrite(0), mRead(3) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(0, 3));
    EXPECT_EQ(ringBuff.Size(), 1);

    // -----

    ringBuff.SetState(1, 3); // Set mWrite(1), mRead(3) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(1, 3));
    EXPECT_EQ(ringBuff.Size(), 2);

    // -----

    ringBuff.SetState(2, 3); // Set mWrite(2), mRead(3) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(2, 3));
    EXPECT_EQ(ringBuff.Size(), 3);

    // -----

    ringBuff.SetState(3, 3); // Set mWrite(3), mRead(3) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(3, 3));
    EXPECT_EQ(ringBuff.Size(), 0);
}
