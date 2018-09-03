
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

    SECTION( "resize not possible" )
    {
        REQUIRE(movAvg.Resize(SIZE) == true);

        REQUIRE(movAvg.Resize(0) == false);     // Not allowed

        REQUIRE(movAvg.Resize(SIZE) == true);
    }

    SECTION( "Type 'double' not allowed" )
    {
        MovingAverage<double> movAvgDouble;

        REQUIRE(movAvgDouble.Resize(SIZE) == false);    // Type 'double' not allowed

        REQUIRE(movAvgDouble.Resize(0) == false);       // Type 'double' not allowed
    }

    SECTION( "Type 'int64_t' not allowed" )
    {
        MovingAverage<int64_t> movAvgInt64;

        REQUIRE(movAvgInt64.Resize(SIZE) == false);     // Type 'int64_t' not allowed

        REQUIRE(movAvgInt64.Resize(0) == false);        // Type 'int64_t' not allowed
    }

    SECTION( "Type 'uint64_t' not allowed" )
    {
        MovingAverage<uint64_t> movAvgUint64;

        REQUIRE(movAvgUint64.Resize(SIZE) == false);    // Type 'uint64_t' not allowed

        REQUIRE(movAvgUint64.Resize(0) == false);       // Type 'uint64_t' not allowed
    }
}
