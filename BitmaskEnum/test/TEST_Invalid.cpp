#include <gtest/gtest.h>
#include <cstdint>
#include "ExampleEnum.hpp"

// Note: to properly validate the bitmask we cast back to uint8_t for number compares.

class BitmaskEnumInvalidTest : public ::testing::Test {
protected:
    ExampleEnum e1;
};

TEST_F(BitmaskEnumInvalidTest, InvalidOROperations) {
    e1 = static_cast<ExampleEnum>(0x10);            // To show this is allowed, but not correct as no bitmask is available
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x10);      // Upper 4 bits not part of ExampleEnum - what is expressed here?
}

TEST_F(BitmaskEnumInvalidTest, InvalidNOTOperations) {
    e1 = ~ExampleEnum::RED;

    uint8_t tmp = static_cast<uint8_t>(e1);
    EXPECT_EQ(tmp, 0xFD);                            // Upper 4 bits not part of ExampleEnum - what is expressed here?
}

TEST_F(BitmaskEnumInvalidTest, InvalidXOROperations) {
    e1 = ExampleEnum::BLUE ^ ExampleEnum::BLUE;     // Yes, BLUE twice
    EXPECT_EQ(static_cast<uint8_t>(e1), 0x00);      // Valid bitmask, but not part of the enum anymore
}
