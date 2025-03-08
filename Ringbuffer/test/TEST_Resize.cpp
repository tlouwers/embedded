#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferResizeTest : public ::testing::Test {
protected:
    Ringbuffer<int> ringBuff;
    int src[5] = { 1, 2, 3 };
    int* pSrc = &src[0];

    void SetUp() override {
        // This function is called before each test.
        EXPECT_EQ(ringBuff.Size(), 0);
    }
};

TEST_F(RingbufferResizeTest, ResizeSmallToLarge) {
    EXPECT_TRUE(ringBuff.Resize(5));
    EXPECT_EQ(ringBuff.Size(), 0);
    ringBuff.Clear();
    EXPECT_EQ(ringBuff.Size(), 0);

    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_EQ(ringBuff.Size(), 2);

    // Now resize to large: discards data
    EXPECT_TRUE(ringBuff.Resize(50));
    EXPECT_EQ(ringBuff.Size(), 0);

    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_EQ(ringBuff.Size(), 2);

    ringBuff.Clear();
    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferResizeTest, ResizeLargeToSmall) {
    EXPECT_TRUE(ringBuff.Resize(50));
    EXPECT_EQ(ringBuff.Size(), 0);
    ringBuff.Clear();
    EXPECT_EQ(ringBuff.Size(), 0);

    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_EQ(ringBuff.Size(), 2);

    // Now resize to small: discards data
    EXPECT_TRUE(ringBuff.Resize(5));
    EXPECT_EQ(ringBuff.Size(), 0);

    EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
    EXPECT_EQ(ringBuff.Size(), 2);

    ringBuff.Clear();
    EXPECT_EQ(ringBuff.Size(), 0);
}
