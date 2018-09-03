
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"

#include <cmath>
#include <limits>

static bool AreSame(float a, float b)
{
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}


TEST_CASE( "float numbers", "[MovingAverage]" )
{
    // For each section variables are anew
    const int SIZE = 5;
    MovingAverage<float> movAvg;

    SECTION( "positive numbers" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 0.0) == true);

        REQUIRE(movAvg.Add(5.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 5.0) == true);  // 1 item to average

        REQUIRE(movAvg.Add(3.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 4.0) == true);  // 2 items to average

        REQUIRE(movAvg.Add(7.0) == true);
        REQUIRE(movAvg.Add(6.0) == true);
        REQUIRE(movAvg.Add(4.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 5.0) == true);  // 5 items to average

        REQUIRE(movAvg.Add(15.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 7.0) == true);
    }

    SECTION( "negative numbers" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 0.0) == true);

        REQUIRE(movAvg.Add(-5.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), -5.0) == true);  // 1 item to average

        REQUIRE(movAvg.Add(-3.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), -4.0) == true);  // 2 items to average

        REQUIRE(movAvg.Add(-7.0) == true);
        REQUIRE(movAvg.Add(-6.0) == true);
        REQUIRE(movAvg.Add(-4.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), -5.0) == true);  // 5 items to average

        REQUIRE(movAvg.Add(-15.0) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), -7.0) == true);  // 5 items to average
    }

    SECTION( "mixed numbers" )
    {
        REQUIRE(movAvg.Resize(3) == true);

        REQUIRE(AreSame(movAvg.GetAverage(), 0.0) == true);

        REQUIRE(movAvg.Add(-6.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), -6.0) == true);  // 1 item to average

        REQUIRE(movAvg.Add(-3.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), -4.5) == true);  // 2 items to average

        REQUIRE(movAvg.Add(0.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), -3.0) == true);  // 3 items to average

        REQUIRE(movAvg.Add(3.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 0.0) == true);   // 3 items to average

        REQUIRE(movAvg.Add(6.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 3.0) == true);

        REQUIRE(movAvg.Add(9.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 6.0) == true);
    }

    SECTION( "fractions" )
    {
        REQUIRE(movAvg.Resize(3) == true);

        REQUIRE(movAvg.Add(1.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 1.0) == true);         // 1 item to average

        REQUIRE(movAvg.Add(2.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 1.5) == true);         // 2 items to average

        REQUIRE(movAvg.Add(2.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 1.6666666) == true);   // 3 items to average

        REQUIRE(movAvg.Add(8.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 4.0) == true);         // 3 items to average

        REQUIRE(movAvg.Add(7.0) == true);
        REQUIRE(AreSame(movAvg.GetAverage(), 5.6666666) == true);   // 3 items to average --> 5.66 becomes 5
    }
}
