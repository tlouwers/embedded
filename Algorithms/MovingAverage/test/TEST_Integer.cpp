
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"


TEST_CASE( "integer numbers", "[MovingAverage]" )
{
    // For each section variables are anew
    const int SIZE = 5;
    MovingAverage<int> movAvg;

    SECTION( "positive numbers" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(movAvg.GetAverage() == 0);

        REQUIRE(movAvg.Add(5) == true);

        REQUIRE(movAvg.GetAverage() == 5);  // 1 item to average

        REQUIRE(movAvg.Add(3) == true);

        REQUIRE(movAvg.GetAverage() == 4);  // 2 items to average

        REQUIRE(movAvg.Add(7) == true);
        REQUIRE(movAvg.Add(6) == true);
        REQUIRE(movAvg.Add(4) == true);

        REQUIRE(movAvg.GetAverage() == 5);  // 5 items to average

        REQUIRE(movAvg.Add(15) == true);

        REQUIRE(movAvg.GetAverage() == 7);
    }

    SECTION( "negative numbers" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(movAvg.GetAverage() == 0);

        REQUIRE(movAvg.Add(-5) == true);

        REQUIRE(movAvg.GetAverage() == -5);  // 1 item to average

        REQUIRE(movAvg.Add(-3) == true);

        REQUIRE(movAvg.GetAverage() == -4);  // 2 items to average

        REQUIRE(movAvg.Add(-7) == true);
        REQUIRE(movAvg.Add(-6) == true);
        REQUIRE(movAvg.Add(-4) == true);

        REQUIRE(movAvg.GetAverage() == -5);  // 5 items to average

        REQUIRE(movAvg.Add(-15) == true);

        REQUIRE(movAvg.GetAverage() == -7);  // 5 items to average
    }

    SECTION( "mixed numbers" )
    {
        REQUIRE(movAvg.Resize(3) == true);

        REQUIRE(movAvg.GetAverage() == 0);

        REQUIRE(movAvg.Add(-6) == true);
        REQUIRE(movAvg.GetAverage() == -6);  // 1 item to average

        REQUIRE(movAvg.Add(-3) == true);
        REQUIRE(movAvg.GetAverage() == -4);  // 2 items to average --> 4.5 becomes 4

        REQUIRE(movAvg.Add(0) == true);
        REQUIRE(movAvg.GetAverage() == -3);  // 3 items to average

        REQUIRE(movAvg.Add(3) == true);
        REQUIRE(movAvg.GetAverage() == 0);   // 3 items to average

        REQUIRE(movAvg.Add(6) == true);
        REQUIRE(movAvg.GetAverage() == 3);

        REQUIRE(movAvg.Add(9) == true);
        REQUIRE(movAvg.GetAverage() == 6);
    }

    SECTION( "fractions" )
    {
        REQUIRE(movAvg.Resize(3) == true);

        REQUIRE(movAvg.Add(1) == true);
        REQUIRE(movAvg.GetAverage() == 1);  // 1 item to average

        REQUIRE(movAvg.Add(2) == true);
        REQUIRE(movAvg.GetAverage() == 1);  // 2 items to average --> 1.5 becomes 1

        REQUIRE(movAvg.Add(2) == true);
        REQUIRE(movAvg.GetAverage() == 1);  // 3 items to average --> 1.66 becomes 1

        REQUIRE(movAvg.Add(8) == true);
        REQUIRE(movAvg.GetAverage() == 4);  // 3 items to average

        REQUIRE(movAvg.Add(7) == true);
        REQUIRE(movAvg.GetAverage() == 5);  // 3 items to average --> 5.66 becomes 5
    }
}
