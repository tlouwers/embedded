
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer wrap operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;

    REQUIRE( ringBuff.Size() == 0 );
    REQUIRE( ringBuff.ContiguousSize() == 0 );


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
        REQUIRE(ringBuff.ContiguousSize() == 2);
        REQUIRE(ringBuff.CheckState(2, 3, 3) == true);
        REQUIRE(ringBuff.Size() == 2);

        REQUIRE(ringBuff.Read(1) == true);              // Read 1 element, restores wrap
        REQUIRE(ringBuff.ContiguousSize() == 1);
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 1);

        // --

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.ContiguousSize() == 2);
        REQUIRE(ringBuff.CheckState(2, 3, 3) == true);
        REQUIRE(ringBuff.Size() == 2);

        REQUIRE(ringBuff.Read(2) == true);              // Read 2 elements, restores wrap
        REQUIRE(ringBuff.ContiguousSize() == 0);
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // --

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);

        size = 3;
        REQUIRE(ringBuff.Poke(data, size) == true);     // Exception: if write == read and size == read allow Poke() to reset mWrite and mRead
        REQUIRE(size == 3);
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
        REQUIRE(ringBuff.ContiguousSize() == 1);

        size = 2;
        REQUIRE(ringBuff.Poke(data, size) == true);     // Only 1 element available at end, but 2 available at start
        REQUIRE(size == 2);
        data[0] = 1;                                    // Actually adding data, will be checked later
        data[1] = 2;
        REQUIRE(ringBuff.Write(2) == true);             // Write block of 2 elements, shrinks wrap by 1
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        size = 2;
        REQUIRE(ringBuff.Peek(data, size) == false);    // Only 1 element present at end, 2 elements present at start, buffer wrapped
        REQUIRE(size == 0);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);
        REQUIRE(data[0] == 4);
        REQUIRE(ringBuff.Read(1) == true);              // Read 1 element, restores wrap
        REQUIRE(ringBuff.CheckState(2, 0, 5) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

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
        REQUIRE(ringBuff.ContiguousSize() == 0);
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

    SECTION( "wrap with large buffer" )
    {
        REQUIRE(ringBuff.Resize(20) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(14, 14, 21);                  // Set mWrite(14), mRead(14), mWrap(21) - 7 elements available at end, 13 elements available at start

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);     // 7 contiguous elements available at the end
        REQUIRE(size == 7);

        size = 13;
        REQUIRE(ringBuff.Poke(data, size) == true);     // And 13 contiguous elements available at the start
        REQUIRE(size == 13);

        REQUIRE(ringBuff.Write(10) == true);            // Wraps, leave 3 elements available
        REQUIRE(ringBuff.CheckState(10, 14, 14) == true);
        REQUIRE(ringBuff.Size() == 10);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);
        REQUIRE(size == 10);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);     // 3 elements available
        REQUIRE(size == 3);

        REQUIRE(ringBuff.Read(3) == true);              // Read 3 elements, restores wrap, leaving 7 remaining
        REQUIRE(ringBuff.CheckState(10, 3, 21) == true);
        REQUIRE(ringBuff.Size() == 7);

        size = 1;
        REQUIRE(ringBuff.Peek(data, size) == true);
        REQUIRE(size == 7);

        REQUIRE(ringBuff.Read(7) == true);            // Read remaining 7 elements
        REQUIRE(ringBuff.CheckState(10, 10, 21) == true);
        REQUIRE(ringBuff.Size() == 0);
    }
}
