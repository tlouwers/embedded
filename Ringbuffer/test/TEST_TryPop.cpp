#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferTryPopTest : public ::testing::Test {
protected:
    Ringbuffer<int> ringBuff;
    int dest[3] = { 0, 0, 0 };
    int* pDest = &dest[0];

    void SetUp() override {
        EXPECT_TRUE(ringBuff.Resize(3));
    }
};

TEST_F(RingbufferTryPopTest, BasicOperationsReadAt0) {
    EXPECT_EQ(ringBuff.Size(), 0);

    ringBuff.SetState(0, 0);  // Set mWrite(0), mRead(0) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(0, 0);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));

    // Default: size == 1
    ringBuff.SetState(0, 0);
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(1, 0);  // Set mWrite(1), mRead(0) - 1 element at start
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(1, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    // Default: size == 1
    ringBuff.SetState(1, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    ringBuff.SetState(1, 0);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 2));

    // -----

    ringBuff.SetState(2, 0);  // Set mWrite(2), mRead(0) - 2 elements at start
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(2, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    // Default: size == 1
    ringBuff.SetState(2, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    ringBuff.SetState(2, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    ringBuff.SetState(2, 0);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 3));

    // -----

    ringBuff.SetState(3, 0);  // Set mWrite(3), mRead(0) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(3, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    // Default: size == 1
    ringBuff.SetState(3, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    ringBuff.SetState(3, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    ringBuff.SetState(3, 0);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 3));
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    ringBuff.SetState(3, 0);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 4));
}

TEST_F(RingbufferTryPopTest, BasicOperationsReadAt1) {
    ringBuff.SetState(0, 1);  // Set mWrite(0), mRead(1) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(0, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(0,     2));

    // Default: size == 1
    ringBuff.SetState(0, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    ringBuff.SetState(0, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    ringBuff.SetState(0, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 3));
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    ringBuff.SetState(0, 1);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 4));

    // -----

    ringBuff.SetState(1, 1);  // Set mWrite(1), mRead(1) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(1, 1);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));

    // Default: size == 1
    ringBuff.SetState(1, 1);
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(2, 1);  // Set mWrite(2), mRead(1) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(2, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    // Default: size == 1
    ringBuff.SetState(2, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    ringBuff.SetState(2, 1);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 2));

    // -----

    ringBuff.SetState(3, 1);  // Set mWrite(3), mRead(1) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(3, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    // Default: size == 1
    ringBuff.SetState(3, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    ringBuff.SetState(3, 1);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    ringBuff.SetState(3, 1);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 3));
}

TEST_F(RingbufferTryPopTest, BasicOperationsReadAt2) {
    ringBuff.SetState(0, 2);  // Set mWrite(0), mRead(2) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(0, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    // Default: size == 1
    ringBuff.SetState(0, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    ringBuff.SetState(0, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    ringBuff.SetState(0, 2);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 3));

    // -----

    ringBuff.SetState(1, 2);  // Set mWrite(1), mRead(2) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(1, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    // Default: size == 1
    ringBuff.SetState(1, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    ringBuff.SetState(1, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    ringBuff.SetState(1, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 3));
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    ringBuff.SetState(1, 2);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 4));

    // -----

    ringBuff.SetState(2, 2);  // Set mWrite(2), mRead(2) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(2, 2);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));

    // Default: size == 1
    ringBuff.SetState(2, 2);
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(3, 2);  // Set mWrite(3), mRead(2) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(3, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    // Default: size == 1
    ringBuff.SetState(3, 2);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    ringBuff.SetState(3, 2);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 2));
}

TEST_F(RingbufferTryPopTest, BasicOperationsReadAt3) {
    ringBuff.SetState(0, 3);  // Set mWrite(0), mRead(3) - 1 element
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(0, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    // Default: size == 1
    ringBuff.SetState(0, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    ringBuff.SetState(0, 3);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 2));

    // -----

    ringBuff.SetState(1, 3);  // Set mWrite(1), mRead(3) - 2 elements
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(1, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    // Default: size == 1
    ringBuff.SetState(1, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    ringBuff.SetState(1, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    ringBuff.SetState(1, 3);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 3));

    // -----

    ringBuff.SetState(2, 3);  // Set mWrite(2), mRead(3) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(2, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    // Default: size == 1
    ringBuff.SetState(2, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest));
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    ringBuff.SetState(2, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    ringBuff.SetState(2, 3);
    EXPECT_TRUE(ringBuff.TryPop(pDest, 3));
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    ringBuff.SetState(2, 3);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 4));

    // -----

    ringBuff.SetState(3, 3);  // Set mWrite(3), mRead(3) - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 0));

    ringBuff.SetState(3, 3);
    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));

    // Default: size == 1
    ringBuff.SetState(3, 3);
    EXPECT_FALSE(ringBuff.TryPop(pDest));
}

TEST_F(RingbufferTryPopTest, InvalidStates) {
    // Invalid states tests
    ringBuff.SetState(0, 4);  // Set mWrite(0), mRead(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(0, 4));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));  // Handle as: buffer empty
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(1, 4);  // Set mWrite(1), mRead(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(1, 4));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));  // Handle as: buffer empty
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(2, 4);  // Set mWrite(2), mRead(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(2, 4));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));  // Handle as: buffer empty
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(3, 4);  // Set mWrite(3), mRead(4) - 1 element more than possible - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(3, 4));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));  // Handle as: buffer empty
    EXPECT_FALSE(ringBuff.TryPop(pDest));

    // -----

    ringBuff.SetState(0, 5);  // Set mWrite(0), mRead(5) - 2 elements more than possible - buffer empty
    EXPECT_TRUE(ringBuff.CheckState(0, 5));

    EXPECT_FALSE(ringBuff.TryPop(pDest, 1));  // Handle as: buffer empty
    EXPECT_FALSE(ringBuff.TryPop(pDest));
}
