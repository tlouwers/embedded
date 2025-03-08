#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

struct TwoTypes {
    int16_t myInt16;
    float myFloat;
};

struct TypeAndArray {
    uint32_t myInt32;
    uint8_t myArray[3];
};

class RingbufferDifferentElementTypesTwoTypes : public ::testing::Test {
protected:
    Ringbuffer<TwoTypes> ringBuff;
    TwoTypes src[4] = {};
    TwoTypes dest[4] = {};
    TwoTypes* pSrc = &src[0];
    TwoTypes* pDest = &dest[0];

    void SetUp() override {
        ringBuff.Resize(3);
    }
};

TEST_F(RingbufferDifferentElementTypesTwoTypes, PushAndPopDataBlockSize1) {
    for (uint16_t i = 1; i <= 100; i++) {
        // Add block with size 1
        pSrc[0].myInt16 = i;
        pSrc[0].myFloat = i + 0.5f;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));

        EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
        EXPECT_EQ(pDest[0].myInt16, i);
        EXPECT_FLOAT_EQ(pDest[0].myFloat, i + 0.5f);
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferDifferentElementTypesTwoTypes, PushAndPopDataBlockSize2) {
    for (uint16_t i = 1; i <= 100; i++) {
        // Add block with size 2
        pSrc[0].myInt16 = i;
        pSrc[0].myFloat = i + 0.5f;
        pSrc[1].myInt16 = i + 1;
        pSrc[1].myFloat = i + 1.5f;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));

        EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
        EXPECT_EQ(pDest[0].myInt16, i);
        EXPECT_FLOAT_EQ(pDest[0].myFloat, i + 0.5f);
        EXPECT_EQ(pDest[1].myInt16, (i + 1));
        EXPECT_FLOAT_EQ(pDest[1].myFloat, i + 1.5f);
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

class RingbufferDifferentElementTypesTypeAndArray : public ::testing::Test {
protected:
    Ringbuffer<TypeAndArray> ringBuff;
    TypeAndArray src[4] = {};
    TypeAndArray dest[4] = {};
    TypeAndArray* pSrc = &src[0];
    TypeAndArray* pDest = &dest[0];

    void SetUp() override {
        ringBuff.Resize(3);
    }
};

TEST_F(RingbufferDifferentElementTypesTypeAndArray, PushAndPopDataBlockSize1) {
    for (uint32_t i = 1; i <= 100; i++) {
        // Add block with size 1
        pSrc[0].myInt32 = i;
        pSrc[0].myArray[0] = i + 1;
        pSrc[0].myArray[1] = i + 2;
        pSrc[0].myArray[2] = i + 3;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 1));

        EXPECT_TRUE(ringBuff.TryPop(pDest, 1));
        EXPECT_EQ(pDest[0].myInt32, i);
        EXPECT_EQ(pDest[0].myArray[0], (i + 1));
        EXPECT_EQ(pDest[0].myArray[1], (i + 2));
        EXPECT_EQ(pDest[0].myArray[2], (i + 3));
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

TEST_F(RingbufferDifferentElementTypesTypeAndArray, PushAndPopDataBlockSize2) {
    for (uint32_t i = 1; i <= 100; i++) {
        // Add block with size 2
        pSrc[0].myInt32 = i;
        pSrc[0].myArray[0] = i + 1;
        pSrc[0].myArray[1] = i + 2;
        pSrc[0].myArray[2] = i + 3;
        pSrc[1].myInt32 = i + 1;
        pSrc[1].myArray[0] = i + 2;
        pSrc[1].myArray[1] = i + 3;
        pSrc[1].myArray[2] = i + 4;
        EXPECT_TRUE(ringBuff.TryPush(pSrc, 2));

        EXPECT_TRUE(ringBuff.TryPop(pDest, 2));
        EXPECT_EQ(pDest[0].myInt32, i);
        EXPECT_EQ(pDest[0].myArray[0], (i + 1));
        EXPECT_EQ(pDest[0].myArray[1], (i + 2));
        EXPECT_EQ(pDest[0].myArray[2], (i + 3));
        EXPECT_EQ(pDest[1].myInt32, (i + 1));
        EXPECT_EQ(pDest[1].myArray[0], (i + 2));
        EXPECT_EQ(pDest[1].myArray[1], (i + 3));
        EXPECT_EQ(pDest[1].myArray[2], (i + 4));
    }

    EXPECT_EQ(ringBuff.Size(), 0);
}

