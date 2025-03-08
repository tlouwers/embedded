
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"

#include <cmath>
#include <limits>

static bool AreSame(float a, float b)
{
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}


TEST_CASE( "limits", "[MovingAverage]" )
{
    // For each section variables are anew
    const uint16_t range_half = 0x7FFF;


    SECTION( "max unsigned integer" )
    {
        const uint32_t i_max = 0xFFFFFFFF;
        MovingAverage<uint32_t> movAvgInt;


        REQUIRE(movAvgInt.Resize(range_half) == true);

        REQUIRE(movAvgInt.GetAverage() == 0);

        for (uint16_t i = 0; i < range_half; i++)
        {
            REQUIRE(movAvgInt.Add(i_max) == true);
        }

        REQUIRE(movAvgInt.GetAverage() == i_max);       // Only items of i_max in buffer

        for (uint16_t i = 0; i < range_half; i++)       // Add remainder
        {
            REQUIRE(movAvgInt.Add(i_max) == true);
        }

        REQUIRE(movAvgInt.GetAverage() == i_max);       // Only items of i_max in buffer

        REQUIRE(movAvgInt.Add(0) == true);              // Add 1 item '0'

        REQUIRE(movAvgInt.GetAverage() == 0xFFFDFFFA);  // Only items of i_max and 1 item '0' in buffer
    }

    SECTION( "min float" )
    {
        const float f_min = std::numeric_limits<float>::min();          // 1.17549435e-038f
        MovingAverage<float> movAvgFloat;

        REQUIRE(movAvgFloat.Resize(range_half) == true);

        REQUIRE(movAvgFloat.GetAverage() == 0);

        for (uint16_t i = 0; i < range_half; i++)
        {
            REQUIRE(movAvgFloat.Add(f_min) == true);
        }

        REQUIRE(AreSame(movAvgFloat.GetAverage(), f_min));              // Only items of f_min in buffer

        for (uint16_t i = 0; i < range_half; i++)                       // Add remainder
        {
            REQUIRE(movAvgFloat.Add(f_min) == true);
        }

        REQUIRE(AreSame(movAvgFloat.GetAverage(), f_min));              // Only items of f_min in buffer

        REQUIRE(movAvgFloat.Add(0) == true);                            // Add 1 item '0'

        REQUIRE(AreSame(movAvgFloat.GetAverage(), 1.17545848e-038f));   // Only items of f_min and 1 item '0' in buffer
    }

    SECTION( "max float" )
    {
        const float f_max = std::numeric_limits<float>::max();          // 3.40282347e+038f
        MovingAverage<float> movAvgFloat;

        REQUIRE(movAvgFloat.Resize(range_half) == true);

        REQUIRE(movAvgFloat.GetAverage() == 0);

        for (uint16_t i = 0; i < range_half; i++)
        {
            REQUIRE(movAvgFloat.Add(f_max) == true);
        }

        REQUIRE(AreSame(movAvgFloat.GetAverage(), f_max));              // Only items of f_max in buffer

        for (uint16_t i = 0; i < range_half; i++)                       // Add remainder
        {
            REQUIRE(movAvgFloat.Add(f_max) == true);
        }

        REQUIRE(AreSame(movAvgFloat.GetAverage(), f_max));              // Only items of f_max in buffer

        REQUIRE(movAvgFloat.Add(0) == true);                            // Add 1 item '0'

        REQUIRE(AreSame(movAvgFloat.GetAverage(), 3.40271962e+038f));   // Only items of f_max and 1 item '0' in buffer
    }
}
