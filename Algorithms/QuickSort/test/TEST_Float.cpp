#include <gtest/gtest.h>
#include "../QuickSort.hpp"
#include <cmath>
#include <limits>

class QuickSortFloatTest : public ::testing::Test {
protected:
    static constexpr int SIZE = 10;
    static constexpr float refArrayPos[SIZE] = {  1.0f,   2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f, 10.0f };
    static constexpr float refArrayNeg[SIZE] = { -10.0f, -9.0f, -8.0f, -7.0f, -6.0f, -5.0f, -4.0f, -3.0f, -2.0f, -1.0f };
    static constexpr float refArrayMix[SIZE] = { -4.0f,  -3.0f, -2.0f, -1.0f,  0.0f,  1.0f,  2.0f,  3.0f,  4.0f,  5.0f };

    bool AreSame(float a, float b) {
        return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
    }

    bool CompareArrays(const float* reference, float* sorted, size_t length) {
        for (size_t i = 0; i < length; i++) {
            if (!AreSame(reference[i], sorted[i])) {
                return false;
            }
        }
        return true;
    }
};

TEST_F(QuickSortFloatTest, PositiveNumbers) {
    float arrayPos[SIZE] = { 6.0f, 3.0f, 8.0f, 7.0f, 10.0f, 2.0f, 1.0f, 4.0f, 5.0f, 9.0f };

    EXPECT_FALSE(CompareArrays(refArrayPos, arrayPos, SIZE));

    QuickSort(arrayPos, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayPos, arrayPos, SIZE));
}

TEST_F(QuickSortFloatTest, NegativeNumbers) {
    float arrayNeg[SIZE] = { -4.0f, -5.0f, -7.0f, -3.0f, -10.0f, -2.0f, -8.0f, -1.0f, -6.0f, -9.0f };

    EXPECT_FALSE(CompareArrays(refArrayNeg, arrayNeg, SIZE));

    QuickSort(arrayNeg, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayNeg, arrayNeg, SIZE));
}

TEST_F(QuickSortFloatTest, MixedNumbers) {
    float arrayMix[SIZE] = { -1.0f, -3.0f, 4.0f, -2.0f, 3.0f, 0.0f, 2.0f, 1.0f, 5.0f, -4.0f };

    EXPECT_FALSE(CompareArrays(refArrayMix, arrayMix, SIZE));

    QuickSort(arrayMix, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayMix, arrayMix, SIZE));
}
