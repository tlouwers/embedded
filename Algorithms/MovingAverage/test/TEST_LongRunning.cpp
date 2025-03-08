#include <gtest/gtest.h>
#include "../MovingAverage.hpp"
#include <cmath>
#include <limits>

class MovingAverageLongRunningTest : public ::testing::Test {
protected:
    // Non-static method to compare two floats
    bool AreSame(float a, float b) const {
        return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
    }
};

TEST_F(MovingAverageLongRunningTest, FloatNumbers) {
    const int SIZE = 5;
    MovingAverage<float> movAvgF;

    EXPECT_TRUE(movAvgF.Resize(SIZE));
    EXPECT_TRUE(AreSame(movAvgF.GetAverage(), 0.0));

    const float INCREMENT = 1.0000001f;

    // Add 50000 items, small buffer, thus 10000x the buffer is re-filled
    float val = INCREMENT;
    for (auto i = 0; i < 50000; i++) {
        movAvgF.Add(val);

        // Increment our count
        val += INCREMENT;

        // Reset count if it becomes too large
        if (val > (INCREMENT * 99)) {
            val = INCREMENT;
        }
    }

    EXPECT_TRUE(AreSame(movAvgF.GetAverage(), 3.00000048f));  // 5 items to average
}

TEST_F(MovingAverageLongRunningTest, IntegerNumbers) {
    const int SIZE = 5;
    MovingAverage<int> movAvg;

    EXPECT_TRUE(movAvg.Resize(SIZE));
    EXPECT_TRUE(AreSame(movAvg.GetAverage(), 0.0));

    const int INCREMENT = 1;

    // Add 50000 items, small buffer, thus 10000x the buffer is re-filled
    int val = INCREMENT;
    for (auto i = 0; i < 50000; i++) {
        movAvg.Add(val);

        // Increment our count
        val += INCREMENT;

        // Reset count if it becomes too large
        if (val > (INCREMENT * 99)) {
            val = INCREMENT;
        }
    }

    EXPECT_EQ(movAvg.GetAverage(), 3);  // 5 items to average
}
