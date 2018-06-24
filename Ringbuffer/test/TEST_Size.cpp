
#include "../../Catch/catch.hpp"

#include "Ringbuffer.hpp"


TEST_CASE( "Ringbuffer Size() operations", "[Ringbuffer]" )
{
    // For each section, Ringbuffer ringBuff is anew:
    Ringbuffer<int> ringBuff;

    REQUIRE(ringBuff.Resize(3) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "basic operations - read at 0" )
    {
        ringBuff.SetState(0, 0);                        // Set mWrite(0), mRead(0) - buffer empty
        REQUIRE(ringBuff.CheckState(0, 0) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(1, 0);                        // Set mWrite(1), mRead(0) - 1 element at start
        REQUIRE(ringBuff.CheckState(1, 0) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(2, 0);                        // Set mWrite(2), mRead(0) - 2 elements at start
        REQUIRE(ringBuff.CheckState(2, 0) == true);

        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(3, 0);                        // Set mWrite(3), mRead(0) - buffer full
        REQUIRE(ringBuff.CheckState(3, 0) == true);

        REQUIRE(ringBuff.Size() == 3);
    }

    SECTION( "basic operations - read at 1" )
    {
        ringBuff.SetState(0, 1);                        // Set mWrite(0), mRead(1) - buffer full
        REQUIRE(ringBuff.CheckState(0, 1) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(1, 1);                        // Set mWrite(1), mRead(1) - buffer empty
        REQUIRE(ringBuff.CheckState(1, 1) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(2, 1);                        // Set mWrite(2), mRead(1) - 1 element
        REQUIRE(ringBuff.CheckState(2, 1) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(3, 1);                        // Set mWrite(3), mRead(1) - 2 elements
        REQUIRE(ringBuff.CheckState(3, 1) == true);

        REQUIRE(ringBuff.Size() == 2);
    }

    SECTION( "basic operations - read at 2" )
    {
        ringBuff.SetState(0, 2);                        // Set mWrite(0), mRead(2) - 2 elements
        REQUIRE(ringBuff.CheckState(0, 2) == true);

        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(1, 2);                        // Set mWrite(1), mRead(2) - buffer full
        REQUIRE(ringBuff.CheckState(1, 2) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(2, 2);                        // Set mWrite(2), mRead(2) - buffer empty
        REQUIRE(ringBuff.CheckState(2, 2) == true);

        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 2);                        // Set mWrite(3), mRead(2) - 1 element
        REQUIRE(ringBuff.CheckState(3, 2) == true);

        REQUIRE(ringBuff.Size() == 1);
    }

    SECTION( "basic operations - read at 3" )
    {
        ringBuff.SetState(0, 3);                        // Set mWrite(0), mRead(3) - 1 element
        REQUIRE(ringBuff.CheckState(0, 3) == true);

        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(1, 3);                        // Set mWrite(1), mRead(3) - 2 elements
        REQUIRE(ringBuff.CheckState(1, 3) == true);

        REQUIRE(ringBuff.Size() == 2);

         // -----

        ringBuff.SetState(2, 3);                        // Set mWrite(2), mRead(3) - buffer full
        REQUIRE(ringBuff.CheckState(2, 3) == true);

        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(3, 3);                        // Set mWrite(3), mRead(3) - buffer empty
        REQUIRE(ringBuff.CheckState(3, 3) == true);

        REQUIRE(ringBuff.Size() == 0);
    }
}
