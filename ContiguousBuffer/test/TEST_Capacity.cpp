#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"


// Capacity is a compile-time property of the type
static_assert(ContiguousRingbuffer<int, 1>::Capacity() == 1, "Capacity must equal N");
static_assert(ContiguousRingbuffer<int, 3>::Capacity() == 3, "Capacity must equal N");
static_assert(ContiguousRingbuffer<int, 5>::Capacity() == 5, "Capacity must equal N");


TEST(TEST_Capacity, CapacityEqualsTemplateArgument) {
    ContiguousRingbuffer<int, 3> ringBuffer3;
    EXPECT_EQ(ringBuffer3.Capacity(), 3);

    ContiguousRingbuffer<int, 1> ringBuffer1;
    EXPECT_EQ(ringBuffer1.Capacity(), 1);

    ContiguousRingbuffer<int, 5> ringBuffer5;
    EXPECT_EQ(ringBuffer5.Capacity(), 5);
}
