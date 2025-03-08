#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

class RingbufferTestTryPushAndTryPop : public ::testing::Test {
protected:
    Ringbuffer<int> ringBuff;
    Ringbuffer<int> ringBuff_ext;

    void SetUp() override {
        EXPECT_TRUE(ringBuff.Resize(10));
        EXPECT_EQ(ringBuff.Size(), 0);
        EXPECT_TRUE(ringBuff_ext.Resize(3));
        EXPECT_EQ(ringBuff_ext.Size(), 0);
    }

    // Helper method to add an element to buffer
    bool AddOne(int val) {
        bool result = true;
        result &= ringBuff_ext.TryPush(&val);
        EXPECT_TRUE(result); // Changed from REQUIRE to EXPECT
        return result;
    }

    // Helper method to remove an element from buffer
    bool RemoveOne(int& val) {
        bool result = true;
        int dest[1] = { -1 };
        int* pDest = &dest[0];

        result &= ringBuff_ext.TryPop(pDest);
        EXPECT_TRUE(result); // Changed from REQUIRE to EXPECT

        val = (result) ? dest[0] : -1;
        return result;
    }
};

TEST_F(RingbufferTestTryPushAndTryPop, PushAndPopDataBlockSize1) {
    int src[10] = { };
    int dest[10] = { };
    int* pSrc = &src[0];
    int* pDest = &dest[0];

    for (int i = 1; i <= 100; i++) {
        pSrc[0] = i;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));
        EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
        EXPECT_EQ(pDest[0], i);
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferTestTryPushAndTryPop, PushAndPopDataBlockSize2) {
    int src[10] = { };
    int dest[10] = { };
    int* pSrc = &src[0];
    int* pDest = &dest[0];

    for (int i = 1; i <= 100; i++) {
        pSrc[0] = i;
        pSrc[1] = i + 1;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));
        EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
        EXPECT_EQ(pDest[0], i);
        EXPECT_EQ(pDest[1], (i + 1));
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferTestTryPushAndTryPop, PushAndPopDataBlockSize3) {
    int src[10] = { };
    int dest[10] = { };
    int* pSrc = &src[0];
    int* pDest = &dest[0];

    for (int i = 1; i <= 100; i++) {
        pSrc[0] = i;
        pSrc[1] = i + 1;
        pSrc[2] = i + 2;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 3));
        EXPECT_TRUE(ringBuff.TryPop(pDest, 3));
        EXPECT_EQ(pDest[0], i);
        EXPECT_EQ(pDest[1], (i + 1));
        EXPECT_EQ(pDest[2], (i + 2));
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferTestTryPushAndTryPop, PushAndPopDataBlockSize4) {
    int src[10] = { };
    int dest[10] = { };
    int* pSrc = &src[0];
    int* pDest = &dest[0];

    for (int i = 1; i <= 100; i++) {
        pSrc[0] = i;
        pSrc[1] = i + 1;
        pSrc[2] = i + 2;
        pSrc[3] = i + 3;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 4));
        EXPECT_TRUE(ringBuff.TryPop(pDest, 4));
        EXPECT_EQ(pDest[0], i);
        EXPECT_EQ(pDest[1], (i + 1));
        EXPECT_EQ(pDest[2], (i + 2));
        EXPECT_EQ(pDest[3], (i + 3));
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferTestTryPushAndTryPop, PushAndPopDataDeepInspection) {
    int index = 1;
    int val = -1;

    EXPECT_TRUE(ringBuff_ext.CheckState(0, 0) == true);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(ringBuff_ext.CheckState(1, 0) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(ringBuff_ext.CheckState(1, 1) == true);
    EXPECT_EQ(val, 1);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(ringBuff_ext.CheckState(2, 1) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(ringBuff_ext.CheckState(2, 2) == true);
    EXPECT_EQ(val, 2);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(ringBuff_ext.CheckState(3, 2) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(ringBuff_ext.CheckState(3, 3) == true);
    EXPECT_EQ(val, 3);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(ringBuff_ext.CheckState(0, 3) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(ringBuff_ext.CheckState(0, 0) == true);
    EXPECT_EQ(val, 4);

    EXPECT_TRUE(AddOne(index++));
    EXPECT_TRUE(ringBuff_ext.CheckState(1, 0) == true);

    EXPECT_TRUE(RemoveOne(val));
    EXPECT_TRUE(ringBuff_ext.CheckState(1, 1) == true);
    EXPECT_EQ(val, 5);

    ringBuff_ext.Clear();
}
