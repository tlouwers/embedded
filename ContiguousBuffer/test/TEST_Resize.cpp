
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Resize() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;

    REQUIRE( ringBuff.Size() == 0 );
    REQUIRE( ringBuff.ContiguousSize() == 0 );


    SECTION( "resize small to large" )
    {
        REQUIRE(ringBuff.Resize(5) == true);
        REQUIRE(ringBuff.Size() == 0);
        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 5);
        REQUIRE(ringBuff.CheckState(0, 0, 6) == true);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);

        // Now resize to large: discards data

        REQUIRE(ringBuff.Resize(50) == true);
        REQUIRE(ringBuff.Size() == 0);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 50);
        REQUIRE(ringBuff.CheckState(0, 0, 51) == true);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "resize large to small" )
    {
        REQUIRE(ringBuff.Resize(50) == true);
        REQUIRE(ringBuff.Size() == 0);
        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 50);
        REQUIRE(ringBuff.CheckState(0, 0, 51) == true);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);

        // Now resize to small: discards data

        REQUIRE(ringBuff.Resize(5) == true);
        REQUIRE(ringBuff.Size() == 0);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 5);
        REQUIRE(ringBuff.CheckState(0, 0, 6) == true);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);
        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);
    }
}
