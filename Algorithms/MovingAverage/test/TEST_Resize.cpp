
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"


TEST_CASE( "resizing the internal buffer", "[MovingAverage]" )
{
    // For each section variables are anew
    const int SIZE = 5;
    MovingAverage<int> movAvg;

    SECTION( "resize" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(movAvg.GetAverage() == 0);


        REQUIRE(movAvg.Add(4) == true);         // Add 1st item

        REQUIRE(movAvg.GetAverage() == 4);      // Average of 1 item

        REQUIRE(movAvg.Resize(SIZE) == true);   // Clears the internal buffer

        REQUIRE(movAvg.GetAverage() == 0);


        REQUIRE(movAvg.Add(4) == true);         // Add 1st item
        REQUIRE(movAvg.Add(2) == true);         // Add 2nd item

        REQUIRE(movAvg.GetAverage() == 3);      // Average of 2 items

        REQUIRE(movAvg.Resize(SIZE) == true);   // Clears the internal buffer

        REQUIRE(movAvg.GetAverage() == 0);
    }
}
