#include <gtest/gtest.h>
#include "../BubbleSort.hpp"
#include <cstddef> // for size_t

const int SIZE = 10;
const int refArrayPos[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
const int refArrayNeg[SIZE] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1 };
const int refArrayMix[SIZE] = { -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };

class BubbleSortIntegertTest : public ::testing::Test {
protected:
    // Method to compare two arrays
    bool CompareArrays(const int* reference, const int* sorted, size_t length)
    {
        // Check for null pointers and valid length
        if (reference == nullptr || sorted == nullptr) {
            return false; // Cannot sort non-existing arrays
        }
        if (length == 0) {
            return true; // Two empty arrays are considered equal
        }

        for (size_t i = 0; i < length; ++i) {
            if (reference[i] != sorted[i]) {
                return false; // Return false on first mismatch
            }
        }
        return true; // All elements matched
    }
};


TEST_F(BubbleSortIntegertTest, PositiveNumbers)
{
    int arrayPos[SIZE] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

    EXPECT_FALSE(CompareArrays(refArrayPos, arrayPos, SIZE));

    BubbleSort(arrayPos, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayPos, arrayPos, SIZE));
}

TEST_F(BubbleSortIntegertTest, NegativeNumbers)
{
    int arrayNeg[SIZE] = { -4, -5, -1, -7, -10, -2, -8, -3, -6, -9 };

    EXPECT_FALSE(CompareArrays(refArrayNeg, arrayNeg, SIZE));

    BubbleSort(arrayNeg, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayNeg, arrayNeg, SIZE));
}

TEST_F(BubbleSortIntegertTest, MixedNumbers)
{
    int arrayMix[SIZE] = { -1, 4, -2, -3, 3, 2, 0, 1, -4, 5 };

    EXPECT_FALSE(CompareArrays(refArrayMix, arrayMix, SIZE));

    BubbleSort(arrayMix, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayMix, arrayMix, SIZE));
}
