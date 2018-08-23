
#include "../../../Catch/catch.hpp"

#include "../QuickSort.hpp"


const int SIZE = 10;
const int refArray[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };


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


TEST_CASE( "integer sort", "[QuickSort]" )
{
    // For each section variables are anew.
    int array[SIZE] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };


    SECTION( "basic sort" )
    {
        REQUIRE(CompareArrays(refArray, array, SIZE) == false);

        QuickSort(array, 0, SIZE-1);

        REQUIRE(CompareArrays(refArray, array, SIZE) == true);
    }
}
