
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Clear() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;

    REQUIRE( ringBuff.Size() == 0 );


    SECTION( "basic operations - read at 0" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 0, 4);                     // Set mWrite(0), mRead(0), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(1, 0, 4);                     // Set mWrite(1), mRead(0), mWrap(4) - 1 element at start
        REQUIRE(ringBuff.CheckState(1, 0, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);


        // -----

        ringBuff.SetState(2, 0, 4);                     // Set mWrite(2), mRead(0), mWrap(4) - 2 elements at start
        REQUIRE(ringBuff.CheckState(2, 0, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(3, 0, 4);                     // Set mWrite(3), mRead(0), mWrap(4) - 3 elements at start
        REQUIRE(ringBuff.CheckState(3, 0, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
    }

    SECTION( "basic operations - read at 1" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 1, 4);                     // Set mWrite(0), mRead(1), mWrap(4) - 3 elements at end
        REQUIRE(ringBuff.CheckState(0, 1, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(1, 1, 4);                     // Set mWrite(1), mRead(1), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(1, 1, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);


        // -----

        ringBuff.SetState(2, 1, 4);                     // Set mWrite(2), mRead(1), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(3, 1, 4);                     // Set mWrite(3), mRead(1), mWrap(4) - 2 elements
        REQUIRE(ringBuff.CheckState(3, 1, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
    }

    SECTION( "basic operations - read at 2" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 2, 4);                     // Set mWrite(0), mRead(2), mWrap(4) - 2 elements at end
        REQUIRE(ringBuff.CheckState(0, 2, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(1, 2, 4);                     // Set mWrite(1), mRead(2), mWrap(4) - 2 elements at end, 1 element at front
        REQUIRE(ringBuff.CheckState(1, 2, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);


        // -----

        ringBuff.SetState(2, 2, 4);                     // Set mWrite(2), mRead(2), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(3, 2, 4);                     // Set mWrite(3), mRead(2), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(3, 2, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
    }

    SECTION( "basic operations - read at 3" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 3, 4);                     // Set mWrite(0), mRead(3), mWrap(4) - 1 element at end
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(1, 3, 4);                     // Set mWrite(1), mRead(3), mWrap(4) - 1 element at end, 1 element at start
        REQUIRE(ringBuff.CheckState(1, 3, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);


        // -----

        ringBuff.SetState(2, 3, 4);                     // Set mWrite(2), mRead(3), mWrap(4) - 1 element at end, 2 elements at start
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        // -----

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
    }

    SECTION( "clear when wrapped also restores wrap" )
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

        ringBuff.Clear();
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
    }
}
