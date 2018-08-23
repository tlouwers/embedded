
#include "../../../Catch/catch.hpp"

#include "../QuickSort.hpp"


const int SIZE = 10;
const float refArray[SIZE] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };


#include <cmath>
#include <limits>

bool AreSame(double a, double b)
{
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
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
    // For each section variables are anew.
    float array[SIZE] = { 6.0f, 3.0f, 7.0f, 8.0f, 2.0f, 10.0f, 1.0f, 4.0f, 5.0f, 9.0f };


    SECTION( "basic sort" )
    {
        REQUIRE(CompareArrays(refArray, array, SIZE) == false);

        QuickSort(array, 0, SIZE-1);

        REQUIRE(CompareArrays(refArray, array, SIZE) == true);
    }
}
