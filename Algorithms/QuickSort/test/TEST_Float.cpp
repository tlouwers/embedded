
#include "../../../Catch/catch.hpp"

#include "../QuickSort.hpp"


const int SIZE = 10;
const float refArrayPos[SIZE] = {  1.0f,   2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f, 10.0f };
const float refArrayNeg[SIZE] = { -10.0f, -9.0f, -8.0f, -7.0f, -6.0f, -5.0f, -4.0f, -3.0f, -2.0f, -1.0f };
const float refArrayMix[SIZE] = { -4.0f,  -3.0f, -2.0f, -1.0f,  0.0f,  1.0f,  2.0f,  3.0f,  4.0f,  5.0f };

#include <cmath>
#include <limits>

bool AreSame(float a, float b)
{
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

bool CompareArrays(const float* reference, float* sorted, size_t length)
{
    bool result = true;

    for (size_t i = 0; i < length; i++)
    {
        if (!AreSame(*reference, *sorted))
        {
            result = false;
            break;
        }
        else
        {
            reference++;
            sorted++;
        }
    }

    return result;
}


TEST_CASE( "float sort", "[QuickSort]" )
{
    SECTION( "positive numbers" )
    {
        float arrayPos[SIZE] = { 6.0f, 3.0f, 8.0f, 7.0f, 10.0f, 2.0f, 1.0f, 4.0f, 5.0f, 9.0f };

        REQUIRE(CompareArrays(refArrayPos, arrayPos, SIZE) == false);

        QuickSort(arrayPos, 0, SIZE-1);

        REQUIRE(CompareArrays(refArrayPos, arrayPos, SIZE) == true);
    }

    SECTION( "negative numbers" )
    {
        float arrayNeg[SIZE] = { -4.0f, -5.0f, -7.0f, -3.0f, -10.0f, -2.0f, -8.0f, -1.0f, -6.0f, -9.0f };

        REQUIRE(CompareArrays(refArrayNeg, arrayNeg, SIZE) == false);

        QuickSort(arrayNeg, 0, SIZE-1);

        REQUIRE(CompareArrays(refArrayNeg, arrayNeg, SIZE) == true);
    }

    SECTION( "mixed numbers" )
    {
        float arrayMix[SIZE] = { -1.0f, -3.0f, 4.0f, -2.0f, 3.0f, 0.0f, 2.0f, 1.0f, 5.0f, -4.0f };

        REQUIRE(CompareArrays(refArrayMix, arrayMix, SIZE) == false);

        QuickSort(arrayMix, 0, SIZE-1);

        REQUIRE(CompareArrays(refArrayMix, arrayMix, SIZE) == true);
    }
}
