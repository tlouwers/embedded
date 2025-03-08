
#include "../../../Catch/catch.hpp"

#include "../MovingAverage.hpp"


TEST_CASE( "filling the entire internal buffer", "[MovingAverage]" )
{
    // For each section variables are anew
    const int SIZE = 5;
    MovingAverage<int> movAvg;

    SECTION( "fill" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(movAvg.GetAverage() == 0);      // Internal buffer empty

        REQUIRE(movAvg.Fill(2) == true);        // Fill internal buffer with value 2

        REQUIRE(movAvg.GetAverage() == 2);      // Internal buffer full

        REQUIRE(movAvg.Add(17) == true);        // Replace 1 item, buffer was full

        REQUIRE(movAvg.GetAverage() == 5);      // 4 items (2), 1 item (17)
    }
}
