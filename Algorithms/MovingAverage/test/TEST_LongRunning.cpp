
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"

#include <cmath>
#include <limits>

static bool AreSame(float a, float b)
{
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}


TEST_CASE( "long running", "[MovingAverage]" )
{
    // For each section variables are anew
    const int SIZE = 5;
    MovingAverage<float> movAvgF;
    MovingAverage<int>   movAvg;

    SECTION( "float numbers" )
    {
        REQUIRE(movAvgF.Resize(SIZE) == true);

        REQUIRE(AreSame(movAvgF.GetAverage(), 0.0) == true);

        const float INCREMENT = 1.0000001f;

        // Add 50000 items, small buffer, thus 10000x the buffer is re-filled
        float val = INCREMENT;
        for (auto i = 0; i < 50000; i++)
        {
            movAvgF.Add(val);

            // Increment our count
            val += INCREMENT;

            // Reset count if it becomes too large
            if (val > (INCREMENT * 99))
            {
                val = INCREMENT;
            }
        }

        REQUIRE(AreSame(movAvgF.GetAverage(), 3.00000048f) == true);  // 5 items to average
    }

    SECTION( "integer numbers" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 0.0) == true);

        const int INCREMENT = 1;

        // Add 50000 items, small buffer, thus 10000x the buffer is re-filled
        int val = INCREMENT;
        for (auto i = 0; i < 50000; i++)
        {
            movAvg.Add(val);

            // Increment our count
            val += INCREMENT;

            // Reset count if it becomes too large
            if (val > (INCREMENT * 99))
            {
                val = INCREMENT;
            }
        }

        REQUIRE(movAvg.GetAverage() == 3);      // 5 items to average
    }
}
