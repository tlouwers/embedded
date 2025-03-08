#include <gtest/gtest.h>
#include "../MovingAverage.hpp"
#include <cmath>
#include <limits>

class MovingAverageFloatTest : public ::testing::Test {
protected:
    static constexpr int SIZE = 5; // Use constexpr for compile-time constant
    MovingAverage<float> movAvg;

    void SetUp() override {
        movAvg.Resize(SIZE);
    }

    bool AreSame(float a, float b) const {
        return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
    }
};

TEST_F(MovingAverageFloatTest, PositiveNumbers) {
    EXPECT_TRUE(movAvg.Resize(SIZE)); // Resize the internal buffer

    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 0.0f)); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(5.0f)); // Add 5.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 5.0f)); // 1 item to average

    EXPECT_TRUE(movAvg.Add(3.0f)); // Add 3.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 4.0f)); // 2 items to average

    EXPECT_TRUE(movAvg.Add(7.0f)); // Add 7.0
    EXPECT_TRUE(movAvg.Add(6.0f)); // Add 6.0
    EXPECT_TRUE(movAvg.Add(4.0f)); // Add 4.0

    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 5.0f)); // 5 items to average

    EXPECT_TRUE(movAvg.Add(15.0f)); // Add 15.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 7.0f)); // Average after adding 15.0
}

TEST_F(MovingAverageFloatTest, NegativeNumbers) {
    EXPECT_TRUE(movAvg.Resize(SIZE)); // Resize the internal buffer

    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 0.0f)); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(-5.0f)); // Add -5.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -5.0f)); // 1 item to average

    EXPECT_TRUE(movAvg.Add(-3.0f)); // Add -3.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -4.0f)); // 2 items to average

    EXPECT_TRUE(movAvg.Add(-7.0f)); // Add -7.0
    EXPECT_TRUE(movAvg.Add(-6.0f)); // Add -6.0
    EXPECT_TRUE(movAvg.Add(-4.0f)); // Add -4.0

    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -5.0f)); // 5 items to average

    EXPECT_TRUE(movAvg.Add(-15.0f)); // Add -15.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -7.0f)); // Average after adding -15.0
}

TEST_F(MovingAverageFloatTest, MixedNumbers) {
    EXPECT_TRUE(movAvg.Resize(3)); // Resize the internal buffer

    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 0.0f)); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(-6.0f)); // Add -6.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -6.0f)); // 1 item to average

    EXPECT_TRUE(movAvg.Add(-3.0f)); // Add -3.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -4.5f)); // 2 items to average

    EXPECT_TRUE(movAvg.Add(0.0f)); // Add 0.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), -3.0f)); // 3 items to average

    EXPECT_TRUE(movAvg.Add(3.0f)); // Add 3.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 0.0f)); // 3 items to average

    EXPECT_TRUE(movAvg.Add(6.0f)); // Add 6.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 3.0f)); // Average after adding 6.0

    EXPECT_TRUE(movAvg.Add(9.0f)); // Add 9.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 6.0f)); // Average after adding 9.0
}

TEST_F(MovingAverageFloatTest, Fractions) {
    EXPECT_TRUE(movAvg.Resize(3)); // Resize the internal buffer

    EXPECT_TRUE(movAvg.Add(1.0f)); // Add 1.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 1.0f)); // 1 item to average

    EXPECT_TRUE(movAvg.Add(2.0f)); // Add 2.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 1.5f)); // 2 items to average

    EXPECT_TRUE(movAvg.Add(2.0f)); // Add 2.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 1.6666666f)); // 3 items to average

    EXPECT_TRUE(movAvg.Add(8.0f)); // Add 8.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 4.0f)); // Average after adding 8.0

    EXPECT_TRUE(movAvg.Add(7.0f)); // Add 7.0
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 5.6666666f)); // Average after adding 7.0
}
