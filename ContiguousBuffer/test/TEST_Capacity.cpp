#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"


class TEST_Capacity : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;
};


TEST_F(TEST_Capacity, CapacityOperationsAfterResize) {
    // Test resizing the ring buffer
    EXPECT_TRUE(mRingBuffer.Resize(3));
    EXPECT_EQ(mRingBuffer.Capacity(), 3);
    EXPECT_TRUE(mRingBuffer.Resize(1));
    EXPECT_EQ(mRingBuffer.Capacity(), 1);
    EXPECT_TRUE(mRingBuffer.Resize(5));
    EXPECT_EQ(mRingBuffer.Capacity(), 5);
}
