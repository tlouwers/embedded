#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumORTest : public ::testing::Test {
protected:
    ExampleEnum e1;
};

TEST_F(BitmaskEnumORTest, SimpleOROperations) {
    e1 = ExampleEnum::BLUE | ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x03);

    e1 = ExampleEnum::RED | ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x06);

    e1 = ExampleEnum::GREEN | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0C);

    e1 = ExampleEnum::WHITE | ExampleEnum::BLUE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x09);

    e1 = ExampleEnum::GREEN | ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x06);

    e1 = ExampleEnum::RED | ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x02);
}

TEST_F(BitmaskEnumORTest, AdvancedOROperations) {
    e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0B);

    e1 = ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::BLUE | ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0F);
}

TEST_F(BitmaskEnumORTest, AssignmentOROperations) {
    e1 = ExampleEnum::BLUE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x01);

    e1 |= ExampleEnum::GREEN;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x05);

    e1 |= ExampleEnum::WHITE;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0D);

    e1 |= ExampleEnum::RED;
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x0F);
}
