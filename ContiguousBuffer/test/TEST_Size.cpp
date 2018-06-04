
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Size() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    REQUIRE(ringBuff.Resize(3) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "basic operations - read at 0" )
    {
        ringBuff.SetState(0, 0, 4);                     // Set mWrite(0), mRead(0), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(1, 0, 4);                     // Set mWrite(1), mRead(0), mWrap(4) - 1 element at start
        REQUIRE(ringBuff.CheckState(1, 0, 4) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(2, 0, 4);                     // Set mWrite(2), mRead(0), mWrap(4) - 2 elements at start
        REQUIRE(ringBuff.CheckState(2, 0, 4) == true);

        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(3, 0, 4);                     // Set mWrite(3), mRead(0), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(3, 0, 4) == true);

        REQUIRE(ringBuff.Size() == 3);
    }

    SECTION( "basic operations - read at 1" )
    {
        ringBuff.SetState(0, 1, 4);                     // Set mWrite(0), mRead(1), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(0, 1, 4) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(1, 1, 4);                    // Set mWrite(1), mRead(1), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(1, 1, 4) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(2, 1, 4);                    // Set mWrite(2), mRead(1), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(3, 1, 4);                    // Set mWrite(3), mRead(1), mWrap(4) - 2 elements
        REQUIRE(ringBuff.CheckState(3, 1, 4) == true);

        REQUIRE(ringBuff.Size() == 2);
    }

    SECTION( "basic operations - read at 2" )
    {
        ringBuff.SetState(0, 2, 4);                    // Set mWrite(0), mRead(2), mWrap(4) - 2 elements
        REQUIRE(ringBuff.CheckState(0, 2, 4) == true);

        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(1, 2, 4);                    // Set mWrite(1), mRead(2), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(1, 2, 4) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(2, 2, 4);                    // Set mWrite(2), mRead(2), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 2, 4);                    // Set mWrite(3), mRead(2), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(3, 2, 4) == true);

        REQUIRE(ringBuff.Size() == 1);
    }

    SECTION( "basic operations - read at 3" )
    {
        ringBuff.SetState(0, 3, 4);                    // Set mWrite(0), mRead(3), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(1, 3, 4);                    // Set mWrite(1), mRead(3), mWrap(4) - 2 elements
        REQUIRE(ringBuff.CheckState(1, 3, 4) == true);

        REQUIRE(ringBuff.Size() == 2);

         // -----

        ringBuff.SetState(2, 3, 4);                    // Set mWrite(2), mRead(3), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(3, 3, 4);                    // Set mWrite(3), mRead(3), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);

        REQUIRE(ringBuff.Size() == 0);
    }
}
