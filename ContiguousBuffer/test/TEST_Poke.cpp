#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"


class TEST_Poke : public ::testing::Test {
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

TEST_F(TEST_Poke, PokeOperations_ReadAt0) {
    mRingBuffer.SetState(0, 0, 4);   // Set mWrite(0), mRead(0), mWrap(4) - 3 elements available at start
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));

    int* data = nullptr;

    size_t size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 3;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 4;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4));

    // -----

    mRingBuffer.SetState(1, 0, 4);   // Set mWrite(1), mRead(0), mWrap(4) - 2 elements available at end
    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(1, 0, 4));

    // -----

    mRingBuffer.SetState(2, 0, 4);   // Set mWrite(2), mRead(0), mWrap(4) - 1 element available at end
    EXPECT_TRUE(mRingBuffer.CheckState(2, 0, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(2, 0, 4));

    // -----

    mRingBuffer.SetState(3, 0, 4);   // Set mWrite(3), mRead(0), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(3, 0, 4));
}

TEST_F(TEST_Poke, PokeOperations_ReadAt1) {
    mRingBuffer.SetState(0, 1, 4);   // Set mWrite(0), mRead(1), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(0, 1, 4));

    int* data = nullptr;

    size_t size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 1, 4));

    // -----

    mRingBuffer.SetState(1, 1, 4);   // Set mWrite(1), mRead(1), mWrap(4) - 3 elements available at end
    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 3;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 3);

    size = 4;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(1, 1, 4));

    // -----

    mRingBuffer.SetState(2, 1, 4);   // Set mWrite(2), mRead(1), mWrap(4) - 2 elements available at end
    EXPECT_TRUE(mRingBuffer.CheckState(2, 1, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(2, 1, 4));

    // -----

    mRingBuffer.SetState(3, 1, 4);   // Set mWrite(3), mRead(1), mWrap(4) - 1 element available at end
    EXPECT_TRUE(mRingBuffer.CheckState(3, 1, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(3, 1, 4));
}

TEST_F(TEST_Poke, PokeOperations_ReadAt2) {
    mRingBuffer.SetState(0, 2, 4);   // Set mWrite(0), mRead(2), mWrap(4) - 1 element available at start
    EXPECT_TRUE(mRingBuffer.CheckState(0, 2, 4));

    int* data = nullptr;

    size_t size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 2, 4));

    // -----

    mRingBuffer.SetState(1, 2, 4);   // Set mWrite(1), mRead(2), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(1, 2, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(1, 2, 4));

    // -----

    mRingBuffer.SetState(2, 2, 4);   // Set mWrite(2), mRead(2), mWrap(4) - 2 elements available at end
    EXPECT_TRUE(mRingBuffer.CheckState(2, 2, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(2, 2, 4));

    // -----

    mRingBuffer.SetState(3, 2, 4);   // Set mWrite(3), mRead(2), mWrap(4) - 1 element available at end
    EXPECT_TRUE(mRingBuffer.CheckState(3, 2, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(3, 2, 4));
}

TEST_F(TEST_Poke, PokeOperations_ReadAt3) {
    mRingBuffer.SetState(0, 3, 4);   // Set mWrite(0), mRead(3), mWrap(4) - 2 elements available at start
    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));

    int* data = nullptr;

    size_t size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 3, 4));

    // -----

    mRingBuffer.SetState(1, 3, 4);   // Set mWrite(1), mRead(3), mWrap(4) - 1 element available at start
    EXPECT_TRUE(mRingBuffer.CheckState(1, 3, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(1, 3, 4));

    // -----

    mRingBuffer.SetState(2, 3, 4);   // Set mWrite(2), mRead(3), mWrap(4) - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(2, 3, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(2, 3, 4));

    // -----

    mRingBuffer.SetState(3, 3, 4);   // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
    EXPECT_TRUE(mRingBuffer.CheckState(3, 3, 4));

    size = 0;
    EXPECT_FALSE(mRingBuffer.Poke(data, size));
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // Can use the available element at the end
    EXPECT_EQ(size, 1);

    size = 2;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // Can use the available elements at the start
    EXPECT_EQ(size, 2);

    size = 3;
    EXPECT_TRUE(mRingBuffer.Poke(data, size)); // Exception: if write == read and size == read allow Poke() to reset mWrite and mRead
    EXPECT_EQ(size, 3);

    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 4)); // Check the reset of mWrite and mRead
}

TEST_F(TEST_Poke, InvalidStates) {
    mRingBuffer.SetState(4, 0, 4);   // Set mWrite(4), mRead(0), mWrap(4) - 1 element more than possible - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(4, 0, 4));

    int* data = nullptr;

    size_t size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Handle as: buffer full
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(4, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(4, 1, 4);   // Set mWrite(4), mRead(1), mWrap(4) - 1 element more than possible - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(4, 1, 4));

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Handle as: buffer full
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(4, 1, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(4, 2, 4);   // Set mWrite(4), mRead(2), mWrap(4) - 1 element more than possible - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(4, 2, 4));

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Handle as: buffer full
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(4, 2, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(4, 3, 4);   // Set mWrite(4), mRead(3), mWrap(4) - 1 element more than possible - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(4, 3, 4));

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Handle as: buffer full
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(4, 3, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    // -----

    mRingBuffer.SetState(5, 0, 4);   // Set mWrite(5), mRead(0), mWrap(4) - 2 elements more than possible - buffer full
    EXPECT_TRUE(mRingBuffer.CheckState(5, 0, 4));

    size = 1;
    EXPECT_FALSE(mRingBuffer.Poke(data, size)); // Handle as: buffer full
    EXPECT_EQ(size, 0);

    EXPECT_TRUE(mRingBuffer.CheckState(5, 0, 4));
    EXPECT_EQ(mRingBuffer.Size(), 3);
}
