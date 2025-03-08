#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumXORTest : public ::testing::Test {
protected:
    ExampleEnum e1;
};

TEST_F(BitmaskEnumXORTest, SimpleXOROperations) {
    e1 = ExampleEnum::BLUE ^ ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x03);

    e1 = ExampleEnum::RED ^ ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x06);

    e1 = ExampleEnum::GREEN ^ ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0C);

    e1 = ExampleEnum::BLUE ^ ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x09);
}

TEST_F(BitmaskEnumXORTest, AdvancedXOROperations) {
    e1 = ExampleEnum::BLUE ^ ExampleEnum::WHITE ^ ExampleEnum::WHITE;   // Yes, WHITE twice
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x01);
}

TEST_F(BitmaskEnumXORTest, AssignmentXOROperations) {
    e1 = ExampleEnum::BLUE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x01);

    e1 ^= ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x03);

    e1 ^= ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0B);

    e1 ^= ExampleEnum::WHITE;                           // WHITE again removes WHITE
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x03);

    e1 ^= ExampleEnum::RED;                             // RED again removes RED
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x01);
}
