#include <gtest/gtest.h>
#include "../MovingAverage.hpp"

class MovingAverageResizeTest : public ::testing::Test {
protected:
    const int SIZE = 5;
    MovingAverage<int> movAvg;
};

TEST_F(MovingAverageResizeTest, Resize) {
    EXPECT_TRUE(movAvg.Resize(SIZE));
    EXPECT_EQ(movAvg.GetAverage(), 0);

    EXPECT_TRUE(movAvg.Add(4));         // Add 1st item
    EXPECT_EQ(movAvg.GetAverage(), 4);  // Average of 1 item

    EXPECT_TRUE(movAvg.Resize(SIZE));   // Clears the internal buffer
    EXPECT_EQ(movAvg.GetAverage(), 0);

    EXPECT_TRUE(movAvg.Add(4));         // Add 1st item
    EXPECT_TRUE(movAvg.Add(2));         // Add 2nd item
    EXPECT_EQ(movAvg.GetAverage(), 3);  // Average of 2 items

    EXPECT_TRUE(movAvg.Resize(SIZE));   // Clears the internal buffer
    EXPECT_EQ(movAvg.GetAverage(), 0);
}

TEST_F(MovingAverageResizeTest, ResizeNotPossible) {
    EXPECT_TRUE(movAvg.Resize(SIZE));
    EXPECT_FALSE(movAvg.Resize(0));     // Not allowed
    EXPECT_TRUE(movAvg.Resize(SIZE));   // Resize back to valid size
}

TEST_F(MovingAverageResizeTest, TypeDoubleNotAllowed) {
    MovingAverage<double> movAvgDouble;
    EXPECT_FALSE(movAvgDouble.Resize(SIZE));    // Type 'double' not allowed
    EXPECT_FALSE(movAvgDouble.Resize(0));       // Type 'double' not allowed
}

TEST_F(MovingAverageResizeTest, TypeInt64NotAllowed) {
    MovingAverage<int64_t> movAvgInt64;
    EXPECT_FALSE(movAvgInt64.Resize(SIZE));     // Type 'int64_t' not allowed
    EXPECT_FALSE(movAvgInt64.Resize(0));        // Type 'int64_t' not allowed
}

TEST_F(MovingAverageResizeTest, TypeUint64NotAllowed) {
    MovingAverage<uint64_t> movAvgUint64;
    EXPECT_FALSE(movAvgUint64.Resize(SIZE));    // Type 'uint64_t' not allowed
    EXPECT_FALSE(movAvgUint64.Resize(0));       // Type 'uint64_t' not allowed
}
