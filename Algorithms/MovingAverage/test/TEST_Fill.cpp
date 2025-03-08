#include <gtest/gtest.h>
#include "../MovingAverage.hpp"


class MovingAverageFillTest : public ::testing::Test {
protected:
    static constexpr int SIZE = 5; // Use constexpr for compile-time constant
    MovingAverage<int> movAvg;

    void SetUp() override {
        movAvg.Resize(SIZE);
    }
};


TEST_F(MovingAverageFillTest, FillInternalBuffer) {
    EXPECT_TRUE(movAvg.Resize(SIZE));   // Resize the internal buffer

    EXPECT_EQ(movAvg.GetAverage(), 0);  // Internal buffer empty

    EXPECT_TRUE(movAvg.Fill(2));        // Fill internal buffer with value 2

    EXPECT_EQ(movAvg.GetAverage(), 2);  // Internal buffer full

    EXPECT_TRUE(movAvg.Add(17));        // Replace 1 item, buffer was full

    EXPECT_EQ(movAvg.GetAverage(), 5);  // 4 items (2), 1 item (17)
}
