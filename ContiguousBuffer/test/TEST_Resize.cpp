#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"

class TEST_Resize : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void SetUp() override {
        EXPECT_EQ(mRingBuffer.Size(), 0);
    }

    void TearDown() override
    {
        mRingBuffer.Clear();
    };
};

TEST_F(TEST_Resize, ResizeSmallToLarge) {
    EXPECT_TRUE(mRingBuffer.Resize(5));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    mRingBuffer.Clear();
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;

    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 5);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 6));
    EXPECT_TRUE(mRingBuffer.Write(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // Now resize to large: discards data

    EXPECT_TRUE(mRingBuffer.Resize(50));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 50);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 51));
    EXPECT_TRUE(mRingBuffer.Write(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.Clear();
    EXPECT_EQ(mRingBuffer.Size(), 0);
}

TEST_F(TEST_Resize, ResizeLargeToSmall) {
    EXPECT_TRUE(mRingBuffer.Resize(50));
    EXPECT_EQ(mRingBuffer.Size(), 0);
    mRingBuffer.Clear();
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;

    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 50);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 51));
    EXPECT_TRUE(mRingBuffer.Write(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    // Now resize to small: discards data

    EXPECT_TRUE(mRingBuffer.Resize(5));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 5);
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 6));
    EXPECT_TRUE(mRingBuffer.Write(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);

    mRingBuffer.Clear();
    EXPECT_EQ(mRingBuffer.Size(), 0);
}
