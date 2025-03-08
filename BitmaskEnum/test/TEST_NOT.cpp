#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumNOTTest : public ::testing::Test {
protected:
    ExampleEnum e1;
};

TEST_F(BitmaskEnumNOTTest, SimpleNOTOperations) {
    e1 = ~ExampleEnum::BLUE;

    uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;  // Get lower 4 bits: no more bits in enum
    EXPECT_EQ(static_cast<uint8_t>(tmp), 0x0E);

    e1 = ~ExampleEnum::GREEN;

    tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
    EXPECT_EQ(static_cast<uint8_t>(tmp), 0x0B);

    e1 = ~ExampleEnum::RED;

    tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
    EXPECT_EQ(static_cast<uint8_t>(tmp), 0x0D);
}

TEST_F(BitmaskEnumNOTTest, AdvancedNOTOperations) {
    e1 = ~(ExampleEnum::BLUE | ExampleEnum::WHITE);

    uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;  // Get lower 4 bits: no more bits in enum
    EXPECT_EQ(static_cast<uint8_t>(tmp), 0x06);

    e1 = ~(ExampleEnum::RED | ExampleEnum::GREEN);

    tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
    EXPECT_EQ(static_cast<uint8_t>(tmp), 0x09);
}
