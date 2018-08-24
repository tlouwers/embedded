
#include "../../../Catch/catch.hpp"

#include "../BubbleSort.hpp"


const int SIZE = 10;
const int refArrayPos[SIZE] = {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10 };
const int refArrayNeg[SIZE] = { -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1 };
const int refArrayMix[SIZE] = {  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5 };


bool CompareArrays(const int* reference, int* sorted, size_t length)
{
    bool result = true;

    for (size_t i = 0; i < length; i++)
    {
        if (*reference != *sorted)
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


TEST_CASE( "positive numbers", "[BubbleSort]" )
{
    SECTION( "positive numbers" )
    {
        int arrayPos[SIZE] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

        REQUIRE(CompareArrays(refArrayPos, arrayPos, SIZE) == false);

        BubbleSort(arrayPos, SIZE);

        REQUIRE(CompareArrays(refArrayPos, arrayPos, SIZE) == true);
    }

    SECTION( "negative numbers" )
    {
        int arrayNeg[SIZE] = { -4, -5, -1, -7, -10, -2, -8, -3, -6, -9 };

        REQUIRE(CompareArrays(refArrayNeg, arrayNeg, SIZE) == false);

        BubbleSort(arrayNeg, SIZE);

        REQUIRE(CompareArrays(refArrayNeg, arrayNeg, SIZE) == true);
    }

    SECTION( "mixed numbers" )
    {
        int arrayMix[SIZE] = { -1, 4, -2, -3, 3, 2, 0, 1, -4, 5 };

        REQUIRE(CompareArrays(refArrayMix, arrayMix, SIZE) == false);

        BubbleSort(arrayMix, SIZE);

        REQUIRE(CompareArrays(refArrayMix, arrayMix, SIZE) == true);
    }
}
