#include <gtest/gtest.h>
#include "../MovingAverage.hpp"

class MovingAverageIntegerTest : public ::testing::Test {
protected:
    static constexpr int SIZE = 5; // Use constexpr for compile-time constant
    MovingAverage<int> movAvg;

    void SetUp() override {
        movAvg.Resize(SIZE);
    }
};

TEST_F(MovingAverageIntegerTest, PositiveNumbers) {
    EXPECT_TRUE(movAvg.Resize(SIZE)); // Resize the internal buffer

    EXPECT_EQ(movAvg.GetAverage(), 0); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(5)); // Add 5
    EXPECT_EQ(movAvg.GetAverage(), 5); // 1 item to average

    EXPECT_TRUE(movAvg.Add(3)); // Add 3
    EXPECT_EQ(movAvg.GetAverage(), 4); // 2 items to average

    EXPECT_TRUE(movAvg.Add(7)); // Add 7
    EXPECT_TRUE(movAvg.Add(6)); // Add 6
    EXPECT_TRUE(movAvg.Add(4)); // Add 4

    EXPECT_EQ(movAvg.GetAverage(), 5); // 5 items to average

    EXPECT_TRUE(movAvg.Add(15)); // Add 15
    EXPECT_EQ(movAvg.GetAverage(), 7); // Average after adding 15
}

TEST_F(MovingAverageIntegerTest, NegativeNumbers) {
    EXPECT_TRUE(movAvg.Resize(SIZE)); // Resize the internal buffer

    EXPECT_EQ(movAvg.GetAverage(), 0); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(-5)); // Add -5
    EXPECT_EQ(movAvg.GetAverage(), -5); // 1 item to average

    EXPECT_TRUE(movAvg.Add(-3)); // Add -3
    EXPECT_EQ(movAvg.GetAverage(), -4); // 2 items to average

    EXPECT_TRUE(movAvg.Add(-7)); // Add -7
    EXPECT_TRUE(movAvg.Add(-6)); // Add -6
    EXPECT_TRUE(movAvg.Add(-4)); // Add -4

    EXPECT_EQ(movAvg.GetAverage(), -5); // 5 items to average

    EXPECT_TRUE(movAvg.Add(-15)); // Add -15
    EXPECT_EQ(movAvg.GetAverage(), -7); // Average after adding -15
}

TEST_F(MovingAverageIntegerTest, MixedNumbers) {
    EXPECT_TRUE(movAvg.Resize(3)); // Resize the internal buffer

    EXPECT_EQ(movAvg.GetAverage(), 0); // Internal buffer empty

    EXPECT_TRUE(movAvg.Add(-6)); // Add -6
    EXPECT_EQ(movAvg.GetAverage(), -6); // 1 item to average

    EXPECT_TRUE(movAvg.Add(-3)); // Add -3
    EXPECT_EQ(movAvg.GetAverage(), -4); // 2 items to average

    EXPECT_TRUE(movAvg.Add(0)); // Add 0
    EXPECT_EQ(movAvg.GetAverage(), -3); // 3 items to average

    EXPECT_TRUE(movAvg.Add(3)); // Add 3
    EXPECT_EQ(movAvg.GetAverage(), 0); // 3 items to average

    EXPECT_TRUE(movAvg.Add(6)); // Add 6
    EXPECT_EQ(movAvg.GetAverage(), 3); // Average after adding 6

    EXPECT_TRUE(movAvg.Add(9)); // Add 9
    EXPECT_EQ(movAvg.GetAverage(), 6); // Average after adding 9
}

TEST_F(MovingAverageIntegerTest, Fractions) {
    EXPECT_TRUE(movAvg.Resize(3)); // Resize the internal buffer

    EXPECT_TRUE(movAvg.Add(1)); // Add 1
    EXPECT_EQ(movAvg.GetAverage(), 1); // 1 item to average

    EXPECT_TRUE(movAvg.Add(2)); // Add 2
    EXPECT_EQ(movAvg.GetAverage(), 1); // 2 items to average

    EXPECT_TRUE(movAvg.Add(2)); // Add 2
    EXPECT_EQ(movAvg.GetAverage(), 1); // 3 items to average

    EXPECT_TRUE(movAvg.Add(8)); // Add 8
    EXPECT_EQ(movAvg.GetAverage(), 4); // Average after adding 8

    EXPECT_TRUE(movAvg.Add(7)); // Add 7
    EXPECT_EQ(movAvg.GetAverage(), 5); // Average after adding 7
}
