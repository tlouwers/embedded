#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferTryPushTest : public ::testing::Test {
protected:
    Ringbuffer<int> ringBuff;
    int src[3] = { 1, 2, 3 };
    int* pSrc = &src[0];

    void SetUp() override {
        ringBuff.Resize(3);
    }
};

TEST_F(RingbufferTryPushTest, BasicOperationsReadAt0) {
    ringBuff.SetState(0, 0); // Set mWrite(0), mRead(0) - 3 elements available at start
    EXPECT_TRUE(ringBuff.CheckState(0, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(0, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    // Default: size == 1
    ringBuff.SetState(0, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    ringBuff.SetState(0, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    ringBuff.SetState(0, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 3));
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    ringBuff.SetState(0, 0);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 4));

    // -----

    ringBuff.SetState(1, 0); // Set mWrite(1), mRead(0) - 2 elements available at end
    EXPECT_TRUE(ringBuff.CheckState(1, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(1, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    // Default: size == 1
    ringBuff.SetState(1, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    ringBuff.SetState(1, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    ringBuff.SetState(1, 0);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 3));

    // -----

    ringBuff.SetState(2, 0); // Set mWrite(2), mRead(0) - 1 element available at end
    EXPECT_TRUE(ringBuff.CheckState(2, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(2, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    // Default: size == 1
    ringBuff.SetState(2, 0);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    ringBuff.SetState(2, 0);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 2));

    // -----

    ringBuff.SetState(3, 0); // Set mWrite(3), mRead(0) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(3, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(3, 0);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1));

    // Default: size == 1
    ringBuff.SetState(3, 0);
    EXPECT_FALSE(ringBuff.TryPush(pSrc));
}

TEST_F(RingbufferTryPushTest, BasicOperationsReadAt1) {
    ringBuff.SetState(0, 1); // Set mWrite(0), mRead(1) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(0, 1);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1));

    // Default: size == 1
    ringBuff.SetState(0, 1);
    EXPECT_FALSE(ringBuff.TryPush(pSrc));

    // -----

    ringBuff.SetState(1, 1); // Set mWrite(1), mRead(1) - 3 elements available at end
    EXPECT_TRUE(ringBuff.CheckState(1, 1));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(1, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    // Default: size == 1
    ringBuff.SetState(1, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    ringBuff.SetState(1, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    ringBuff.SetState(1, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 3));
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    ringBuff.SetState(1, 1);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 4));

    // -----

    ringBuff.SetState(2, 1); // Set mWrite(2), mRead(1) - 2 elements available at end
    EXPECT_TRUE(ringBuff.CheckState(2, 1));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(2, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    // Default: size == 1
    ringBuff.SetState(2, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    ringBuff.SetState(2, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    ringBuff.SetState(2, 1);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 3));

    // -----

    ringBuff.SetState(3, 1); // Set mWrite(3), mRead(1) - 1 element available at end
    EXPECT_TRUE(ringBuff.CheckState(3, 1));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(3, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    // Default: size == 1
    ringBuff.SetState(3, 1);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(0, 1));

    ringBuff.SetState(3, 1);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 2));
}

TEST_F(RingbufferTryPushTest, BasicOperationsReadAt2) {
    ringBuff.SetState(0, 2); // Set mWrite(0), mRead(2) - 1 element available at start
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(0, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    // Default: size == 1
    ringBuff.SetState(0, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    ringBuff.SetState(0, 2);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 2));

    // -----
    ringBuff.SetState(1, 2); // Set mWrite(1), mRead(2) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(1, 2);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1));

    // Default: size == 1
    ringBuff.SetState(1, 2);
    EXPECT_FALSE(ringBuff.TryPush(pSrc));

    // -----
    ringBuff.SetState(2, 2); // Set mWrite(2), mRead(2) - 2 elements available at end, 1 element available at start
    EXPECT_TRUE(ringBuff.CheckState(2, 2));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(2, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    // Default: size == 1
    ringBuff.SetState(2, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    ringBuff.SetState(2, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    ringBuff.SetState(2, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 3));
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    ringBuff.SetState(2, 2);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 4));

    // -----
    ringBuff.SetState(3, 2); // Set mWrite(3), mRead(2) - 1 element available at end, 1 element available at start
    EXPECT_TRUE(ringBuff.CheckState(3, 2));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(3, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    // Default: size == 1
    ringBuff.SetState(3, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(0, 2));

    ringBuff.SetState(3, 2);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(1, 2));

    ringBuff.SetState(3, 2);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 3));
}

TEST_F(RingbufferTryPushTest, BasicOperationsReadAt3) {
    ringBuff.SetState(0, 3); // Set mWrite(0), mRead(3) - 2 elements available at start
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(0, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    // Default: size == 1
    ringBuff.SetState(0, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    ringBuff.SetState(0, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    ringBuff.SetState(0, 3);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 3));

    // -----
    ringBuff.SetState(1, 3); // Set mWrite(1), mRead(3) - 1 element available at start
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(1, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    // Default: size == 1
    ringBuff.SetState(1, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    ringBuff.SetState(1, 3);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 2));

    // -----
    ringBuff.SetState(2, 3); // Set mWrite(2), mRead(3) - buffer full
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(2, 3);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1));

    // Default: size == 1
    ringBuff.SetState(2, 3);
    EXPECT_FALSE(ringBuff.TryPush(pSrc));

    // -----
    ringBuff.SetState(3, 3); // Set mWrite(3), mRead(3) - 1 element available at end, 2 elements available at start
    EXPECT_TRUE(ringBuff.CheckState(3, 3));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 0));

    ringBuff.SetState(3, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    // Default: size == 1
    ringBuff.SetState(3, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc));
    EXPECT_TRUE(ringBuff.CheckState(0, 3));

    ringBuff.SetState(3, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_TRUE(ringBuff.CheckState(1, 3));

    ringBuff.SetState(3, 3);
    EXPECT_TRUE(ringBuff.TryPush(pSrc, 3));
    EXPECT_TRUE(ringBuff.CheckState(2, 3));

    ringBuff.SetState(3, 3);
    EXPECT_FALSE(ringBuff.TryPush(pSrc, 4));
}

TEST_F(RingbufferTryPushTest, BasicOperationsInvalidStates) {
    ringBuff.SetState(4, 0); // Set mWrite(4), mRead(0) - 1 element more than possible - buffer full
    EXPECT_TRUE(ringBuff.CheckState(4, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1)); // Handle as: buffer full
    EXPECT_FALSE(ringBuff.TryPush(pSrc));    // Default: size == 1

    // -----
    ringBuff.SetState(4, 1); // Set mWrite(4), mRead(1) - 1 element more than possible - buffer full
    EXPECT_TRUE(ringBuff.CheckState(4, 1));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1)); // Handle as: buffer full
    EXPECT_FALSE(ringBuff.TryPush(pSrc));    // Default: size == 1

    // -----
    ringBuff.SetState(4, 2); // Set mWrite(4), mRead(2) - 1 element
    // -----
    ringBuff.SetState(4, 2); // Set mWrite(4), mRead(2) - 1 element more than possible - buffer full
    EXPECT_TRUE(ringBuff.CheckState(4, 2));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1)); // Handle as: buffer full
    EXPECT_FALSE(ringBuff.TryPush(pSrc));    // Default: size == 1

    // -----
    ringBuff.SetState(4, 3); // Set mWrite(4), mRead(3) - 1 element more than possible - buffer full
    EXPECT_TRUE(ringBuff.CheckState(4, 3));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1)); // Handle as: buffer full
    EXPECT_FALSE(ringBuff.TryPush(pSrc));    // Default: size == 1

    // -----
    ringBuff.SetState(5, 0); // Set mWrite(5), mRead(0) - 2 elements more than possible - buffer full
    EXPECT_TRUE(ringBuff.CheckState(5, 0));

    EXPECT_FALSE(ringBuff.TryPush(pSrc, 1)); // Handle as: buffer full
    EXPECT_FALSE(ringBuff.TryPush(pSrc));    // Default: size == 1
}
