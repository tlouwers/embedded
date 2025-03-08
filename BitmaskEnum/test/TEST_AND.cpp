#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumANDTest : public ::testing::Test {
protected:
    ExampleEnum e1;
    ExampleEnum e2;
};

TEST_F(BitmaskEnumANDTest, SimpleANDOperations) {
    e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0F);

    e2 = e1 & ExampleEnum::BLUE;
    EXPECT_EQ(static_cast<uint8_t>(e2), 0x01);

    e2 = e1 & ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e2), 0x02);

    e2 = e1 & ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e2), 0x04);

    e2 = e1 & ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e2), 0x08);
}

TEST_F(BitmaskEnumANDTest, AssignmentANDOperations) {
    e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0F);

    e1 &= ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x02);

    e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0F);

    e1 &= ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x04);
}
