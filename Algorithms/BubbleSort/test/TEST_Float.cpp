#include <gtest/gtest.h>
#include "../BubbleSort.hpp"
#include <cmath>
#include <limits>

const int SIZE = 10;
const float refArrayPos[SIZE] = {  1.0f,   2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f, 10.0f };
const float refArrayNeg[SIZE] = { -10.0f, -9.0f, -8.0f, -7.0f, -6.0f, -5.0f, -4.0f, -3.0f, -2.0f, -1.0f };
const float refArrayMix[SIZE] = { -4.0f,  -3.0f, -2.0f, -1.0f,  0.0f,  1.0f,  2.0f,  3.0f,  4.0f,  5.0f };

class BubbleSortFloatTest : public ::testing::Test {
protected:
    // Method to compare two floating-point numbers
    bool AreSame(float a, float b)
    {
        // Use a more appropriate epsilon for floating-point comparison
        return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
    }

    // Method to compare two arrays of floats
    bool CompareArrays(const float* reference, const float* sorted, size_t length)
    {
        // Check for null pointers and valid length
        if (reference == nullptr || sorted == nullptr) {
            return false; // Cannot sort non-existing arrays
        }
        if (length == 0) {
            return true; // Two empty arrays are considered equal
        }

        for (size_t i = 0; i < length; ++i) {
            if (!AreSame(reference[i], sorted[i])) {
                return false; // Return false on first mismatch
            }
        }
        return true; // All elements matched
    }
};


TEST_F(BubbleSortFloatTest, PositiveNumbers)
{
    float arrayPos[SIZE] = { 6.0f, 3.0f, 8.0f, 7.0f, 10.0f, 2.0f, 1.0f, 4.0f, 5.0f, 9.0f };

    EXPECT_FALSE(CompareArrays(refArrayPos, arrayPos, SIZE));

    BubbleSort(arrayPos, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayPos, arrayPos, SIZE));
}

TEST_F(BubbleSortFloatTest, NegativeNumbers)
{
    float arrayNeg[SIZE] = { -4.0f, -5.0f, -7.0f, -3.0f, -10.0f, -2.0f, -8.0f, -1.0f, -6.0f, -9.0f };

    EXPECT_FALSE(CompareArrays(refArrayNeg, arrayNeg, SIZE));

    BubbleSort(arrayNeg, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayNeg, arrayNeg, SIZE));
}

TEST_F(BubbleSortFloatTest, MixedNumbers)
{
    float arrayMix[SIZE] = { -1.0f, -3.0f, 4.0f, -2.0f, 3.0f, 0.0f, 2.0f, 1.0f, 5.0f, -4.0f };

    EXPECT_FALSE(CompareArrays(refArrayMix, arrayMix, SIZE));

    BubbleSort(arrayMix, SIZE);

    EXPECT_TRUE(CompareArrays(refArrayMix, arrayMix, SIZE));
}
