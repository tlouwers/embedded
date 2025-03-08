#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumMixedTest : public ::testing::Test {
protected:
    ExampleEnum e1;
};

TEST_F(BitmaskEnumMixedTest, MixedOperations) {
    e1 = ExampleEnum::RED | ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x06);

    e1 |= ExampleEnum::BLUE;                            // Add BLUE
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x07);

    e1 &= ExampleEnum::RED;                             // Filter, only if RED is set use it
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x02);

    e1 = ExampleEnum::BLUE | ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x05);

    e1 &= ExampleEnum::RED;                             // Filter, only if RED is set use it
    uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;      // Only use lower 4 bits of ExampleEnum
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x00);

    e1 ^= ExampleEnum::WHITE;                           // Toggle WHITE
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x08);

    e1 ^= ExampleEnum::WHITE;                           // Toggle WHITE
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x00);

    e1 = ~(ExampleEnum::GREEN | ExampleEnum::RED);      // All but GREEN and RED
    tmp = static_cast<uint8_t>(e1) & 0x0F;              // Only use lower 4 bits of ExampleEnum
    EXPECT_EQ(tmp, 0x09);

    e1 = ExampleEnum::BLUE | ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x05);

    e1 &= ~ExampleEnum::GREEN;                          // Remove GREEN
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x01);

    e1 |= ExampleEnum::GREEN;                           // Add GREEN
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x05);
}
