#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"
#include <cstdint>      // uintptr_t, uint8_t
#include <type_traits>


// Storage is part of the object: the object size covers N + 1 elements
static_assert(sizeof(ContiguousRingbuffer<int, 64>) >= (65 * sizeof(int)), "Storage must be inside the object");
static_assert(sizeof(ContiguousRingbuffer<uint8_t, 64>) >= 65, "Storage must be inside the object");

// Trivially destructible for trivial element types: no destructor runs for a
// buffer with static storage duration
static_assert(std::is_trivially_destructible<ContiguousRingbuffer<int, 64>>::value, "Must be trivially destructible");


// Buffer with static storage duration, the intended embedded use
static ContiguousRingbuffer<int, 10> gStaticRingBuffer;


class TEST_StaticStorage : public ::testing::Test {
protected:
    ContiguousRingbuffer<int, 5> mRingBuffer;

    void TearDown() override
    {
        mRingBuffer.Clear();
        gStaticRingBuffer.Clear();
    };

    // Helper method to check a pointer lies within the given object
    template<typename Buffer>
    bool PointsIntoObject(const Buffer& buffer, const int* ptr, size_t size) {
        const auto begin = reinterpret_cast<uintptr_t>(&buffer);
        const auto end   = begin + sizeof(Buffer);
        const auto first = reinterpret_cast<uintptr_t>(ptr);
        const auto last  = reinterpret_cast<uintptr_t>(ptr + size);
        return (first >= begin) && (last <= end);
    }
};


TEST_F(TEST_StaticStorage, ReadyForUseAfterConstruction) {
    // No initialization call needed: empty buffer with wrap at N + 1
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 6));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    int* data = nullptr;
    size_t size = 1;
    EXPECT_FALSE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(data, nullptr);
    EXPECT_EQ(size, 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(size, 5);
    EXPECT_TRUE(mRingBuffer.CommitWrite(2));
    EXPECT_EQ(mRingBuffer.Size(), 2);
}

TEST_F(TEST_StaticStorage, StaticBufferReadyForUse) {
    EXPECT_TRUE(gStaticRingBuffer.CheckState(0, 0, 11));
    EXPECT_EQ(gStaticRingBuffer.Size(), 0);

    int* data = nullptr;
    size_t size = 1;
    EXPECT_TRUE(gStaticRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 10);
    EXPECT_TRUE(PointsIntoObject(gStaticRingBuffer, data, size));
}

TEST_F(TEST_StaticStorage, PointersPointIntoObject) {
    int* data = nullptr;

    // Block at the start
    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 5);
    EXPECT_TRUE(PointsIntoObject(mRingBuffer, data, size));
    EXPECT_TRUE(mRingBuffer.CommitWrite(4));

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveRead(data, size));
    EXPECT_EQ(size, 4);
    EXPECT_TRUE(PointsIntoObject(mRingBuffer, data, size));
    EXPECT_TRUE(mRingBuffer.CommitRead(4));

    // Block at the end, including the extra element
    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 2);
    EXPECT_TRUE(PointsIntoObject(mRingBuffer, data, size));
    EXPECT_TRUE(mRingBuffer.CommitWrite(2));
    EXPECT_TRUE(mRingBuffer.CheckState(0, 4, 6));
}

TEST_F(TEST_StaticStorage, ClearDiscardsData) {
    int* data = nullptr;
    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CommitWrite(3));
    EXPECT_EQ(mRingBuffer.Size(), 3);

    mRingBuffer.Clear();
    EXPECT_TRUE(mRingBuffer.CheckState(0, 0, 6));
    EXPECT_EQ(mRingBuffer.Size(), 0);

    size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_EQ(size, 5);
}

TEST_F(TEST_StaticStorage, InstancesAreIndependent) {
    ContiguousRingbuffer<int, 5> other;

    int* data = nullptr;
    size_t size = 1;
    EXPECT_TRUE(mRingBuffer.ReserveWrite(data, size));
    EXPECT_TRUE(mRingBuffer.CommitWrite(3));

    EXPECT_EQ(mRingBuffer.Size(), 3);
    EXPECT_EQ(other.Size(), 0);
    EXPECT_TRUE(other.CheckState(0, 0, 6));
}
