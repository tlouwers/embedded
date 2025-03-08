#include <gtest/gtest.h>
#include "../MovingAverage.hpp"
#include <cmath>
#include <limits>

class MovingAverageLimitsTest : public ::testing::Test {
protected:
    // Non-static method to compare two floats
    bool AreSame(float a, float b) const {
        return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
    }
};

TEST_F(MovingAverageLimitsTest, MaxUnsignedInteger) {
    const uint16_t range_half = 0x7FFF;
    const uint32_t i_max = 0xFFFFFFFF;
    MovingAverage<uint32_t> movAvgInt;

    EXPECT_TRUE(movAvgInt.Resize(range_half));
    EXPECT_EQ(movAvgInt.GetAverage(), 0);

    for (uint16_t i = 0; i < range_half; i++) {
        EXPECT_TRUE(movAvgInt.Add(i_max));
    }

    EXPECT_EQ(movAvgInt.GetAverage(), i_max); // Only items of i_max in buffer

    for (uint16_t i = 0; i < range_half; i++) { // Add remainder
        EXPECT_TRUE(movAvgInt.Add(i_max));
    }

    EXPECT_EQ(movAvgInt.GetAverage(), i_max); // Only items of i_max in buffer

    EXPECT_TRUE(movAvgInt.Add(0)); // Add 1 item '0'
    EXPECT_EQ(movAvgInt.GetAverage(), 0xFFFDFFFA); // Only items of i_max and 1 item '0' in buffer
}

TEST_F(MovingAverageLimitsTest, MinFloat) {
    const uint16_t range_half = 0x7FFF;
    const float f_min = std::numeric_limits<float>::min(); // 1.17549435e-038f
    MovingAverage<float> movAvgFloat;

    EXPECT_TRUE(movAvgFloat.Resize(range_half));
    EXPECT_EQ(movAvgFloat.GetAverage(), 0);

    for (uint16_t i = 0; i < range_half; i++) {
        EXPECT_TRUE(movAvgFloat.Add(f_min));
    }

    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), f_min)); // Only items of f_min in buffer

    for (uint16_t i = 0; i < range_half; i++) { // Add remainder
        EXPECT_TRUE(movAvgFloat.Add(f_min));
    }

    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), f_min)); // Only items of f_min in buffer

    EXPECT_TRUE(movAvgFloat.Add(0)); // Add 1 item '0'
    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), 1.17545848e-038f)); // Only items of f_min and 1 item '0' in buffer
}

TEST_F(MovingAverageLimitsTest, MaxFloat) {
    const uint16_t range_half = 0x7FFF;
    const float f_max = std::numeric_limits<float>::max(); // 3.40282347e+038f
    MovingAverage<float> movAvgFloat;

    EXPECT_TRUE(movAvgFloat.Resize(range_half));
    EXPECT_EQ(movAvgFloat.GetAverage(), 0);

    for (uint16_t i = 0; i < range_half; i++) {
        EXPECT_TRUE(movAvgFloat.Add(f_max));
    }

    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), f_max)); // Only items of f_max in buffer

    for (uint16_t i = 0; i < range_half; i++) { // Add remainder
        EXPECT_TRUE(movAvgFloat.Add(f_max));
    }

    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), f_max)); // Only items of f_max in buffer

    EXPECT_TRUE(movAvgFloat.Add(0)); // Add 1 item '0'
    EXPECT_TRUE(AreSame(movAvgFloat.GetAverage(), 3.40271962e+038f)); // Only items of f_max and 1 item '0' in buffer
}
