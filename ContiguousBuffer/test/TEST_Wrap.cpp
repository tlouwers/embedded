
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer wrap operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;

    REQUIRE( ringBuff.Size() == 0 );


    SECTION( "smallest wrap possible" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        size = 2;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 2);

        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.CheckState(2, 3, 3) == true);
        REQUIRE(ringBuff.Size() == 2);

        REQUIRE(ringBuff.Read(1) == true);              // Read 1 element, restores wrap
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 1);

        // --

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.CheckState(2, 3, 3) == true);
        REQUIRE(ringBuff.Size() == 2);

        REQUIRE(ringBuff.Read(2) == true);              // Read 2 elements, restores wrap
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // --

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        size = 3;
        REQUIRE(ringBuff.Poke(data, size) == false);
        REQUIRE(size == 0);
    }

    SECTION( "wrap with small buffer - read starts unequal to wrap" )
    {
        REQUIRE(ringBuff.Resize(4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 3, 5);                     // Set mWrite(3), mRead(3), mWrap(5) - 2 elements available at end, 2 elements available at start

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 2);
        data[0] = 4;                                    // Actually adding data, will be checked later
        REQUIRE(ringBuff.Write(1) == true);

        size = 2;
        REQUIRE(ringBuff.Poke(data, size) == true);     // Only 1 element available at end, but 2 available at start
        REQUIRE(size == 2);
        data[0] = 1;                                    // Actually adding data, will be checked later
        data[1] = 2;
        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 3);

        size = 2;
        REQUIRE(ringBuff.Peek(data, size) == false);    // Only 1 element present at end, 2 elements present at start, buffer wrapped
        REQUIRE(size == 0);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);
        REQUIRE(data[0] == 4);
        REQUIRE(ringBuff.Read(1) == true);              // Read 1 element, restores wrap
        REQUIRE(ringBuff.CheckState(2, 0, 5) == true);
        REQUIRE(ringBuff.Size() == 2);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);     // 2 elements present at start
        REQUIRE(size == 2);

        size = 3;
        REQUIRE(ringBuff.Peek(data, size) == false);    // 2 elements present at start
        REQUIRE(size == 0);

        size = 2;
        REQUIRE(ringBuff.Peek(data, size) == true);     // 2 elements present at start
        REQUIRE(size == 2);
        REQUIRE(data[0] == 1);
        REQUIRE(data[1] == 2);
        REQUIRE(ringBuff.Read(2) == true);              // Read 2 elements
        REQUIRE(ringBuff.CheckState(2, 2, 5) == true);
        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "wrap with small buffer - read starts equal to wrap" )
    {
        REQUIRE(ringBuff.Resize(4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        // Can get here by filling buffer with 4 elements, then removing 4 elements
        ringBuff.SetState(4, 4, 5);                     // Set mWrite(4), mRead(4), mWrap(5) - 1 element available at end, 3 elements available at start

        size = 2;
        REQUIRE(ringBuff.Poke(data, size) == true);
        data[0] = 7;                                    // Actually adding data, will be checked later
        data[1] = 8;
        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.CheckState(2, 4, 4) == true);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);     // 2 elements present at start, buffer wrapped
        REQUIRE(size == 2);

        size = 3;
        REQUIRE(ringBuff.Peek(data, size) == false);    // 2 elements present at start
        REQUIRE(size == 0);

        REQUIRE(ringBuff.Size() == 2);

        size = 2;
        REQUIRE(ringBuff.Peek(data, size) == true);     // 2 elements present at start
        REQUIRE(size == 2);
        REQUIRE(data[0] == 7);
        REQUIRE(data[1] == 8);
        REQUIRE(ringBuff.Read(2) == true);              // Read 2 elements, restores wrap
        REQUIRE(ringBuff.CheckState(2, 2, 5) == true);
    }
}
