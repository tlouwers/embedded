#include <gtest/gtest.h>
#include "../QuickSort.hpp"

class QuickSortIntegerTest : public ::testing::Test {
protected:
    static constexpr int SIZE = 10;
    static constexpr int refArrayPos[SIZE] = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 };
    static constexpr int refArrayNeg[SIZE] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1 };
    static constexpr int refArrayMix[SIZE] = { -4, -3, -2, -1,  0,  1,  2,  3,  4,  5 };

    bool CompareArrays(const int* reference, int* sorted, size_t length) {
        for (size_t i = 0; i < length; i++) {
            if (reference[i] != sorted[i]) {
                return false;
            }
        }
        return true;
    }
};

TEST_F(QuickSortIntegerTest, PositiveNumbers) {
    int arrayPos[SIZE] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

    EXPECT_FALSE(CompareArrays(refArrayPos, arrayPos, SIZE));

    QuickSort(arrayPos, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayPos, arrayPos, SIZE));
}

TEST_F(QuickSortIntegerTest, NegativeNumbers) {
    int arrayNeg[SIZE] = { -4, -5, -1, -7, -10, -2, -8, -3, -6, -9 };

    EXPECT_FALSE(CompareArrays(refArrayNeg, arrayNeg, SIZE));

    QuickSort(arrayNeg, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayNeg, arrayNeg, SIZE));
}

TEST_F(QuickSortIntegerTest, MixedNumbers) {
    int arrayMix[SIZE] = { -1, 4, -2, -3, 3, 2, 0, 1, -4, 5 };

    EXPECT_FALSE(CompareArrays(refArrayMix, arrayMix, SIZE));

    QuickSort(arrayMix, 0, SIZE - 1);

    EXPECT_TRUE(CompareArrays(refArrayMix, arrayMix, SIZE));
}
