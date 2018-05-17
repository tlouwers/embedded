
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Read() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    REQUIRE( ringBuff.Size() == 0 );


    SECTION( "basic operations - read at 0" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 0, 4);                     // Set mWrite(0), mRead(0), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(0, 0, 4);
        REQUIRE(ringBuff.Read(1) == false);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(1, 0, 4);                     // Set mWrite(1), mRead(0), mWrap(4) - 1 element at start
        REQUIRE(ringBuff.CheckState(1, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(1, 0, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(1, 0, 4);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(2, 0, 4);                     // Set mWrite(2), mRead(0), mWrap(4) - 2 elements at start
        REQUIRE(ringBuff.CheckState(2, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(2, 0, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(2, 0, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(2, 0, 4);
        REQUIRE(ringBuff.Read(3) == false);
        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(3, 0, 4);                     // Set mWrite(3), mRead(0), mWrap(4) - 3 elements at start
        REQUIRE(ringBuff.CheckState(3, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(3, 0, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(3, 0, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(3, 0, 4);
        REQUIRE(ringBuff.Read(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(3, 0, 4);
        REQUIRE(ringBuff.Read(4) == false);
        REQUIRE(ringBuff.Size() == 3);
    }

    SECTION( "basic operations - read at 1" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 1, 4);                     // Set mWrite(0), mRead(1), mWrap(4) - 3 elements at end
        REQUIRE(ringBuff.CheckState(0, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(0, 1, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(0, 1, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(0, 1, 4);
        REQUIRE(ringBuff.Read(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(0, 1, 4);
        REQUIRE(ringBuff.Read(4) == false);
        REQUIRE(ringBuff.Size() == 3);

        // -----

        ringBuff.SetState(1, 1, 4);                     // Set mWrite(1), mRead(1), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(1, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(1, 1, 4);
        REQUIRE(ringBuff.Read(1) == false);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(2, 1, 4);                     // Set mWrite(2), mRead(1), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(2, 1, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(2, 1, 4);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(3, 1, 4);                     // Set mWrite(3), mRead(1), mWrap(4) - 2 elements
        REQUIRE(ringBuff.CheckState(3, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(3, 1, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(3, 1, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(3, 1, 4);
        REQUIRE(ringBuff.Read(3) == false);
        REQUIRE(ringBuff.Size() == 2);
    }

    SECTION( "basic operations - read at 2" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 2, 4);                     // Set mWrite(0), mRead(2), mWrap(4) - 2 elements at end
        REQUIRE(ringBuff.CheckState(0, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(0, 2, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(0, 2, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(0, 2, 4);
        REQUIRE(ringBuff.Read(3) == false);
        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(1, 2, 4);                     // Set mWrite(1), mRead(2), mWrap(4) - 2 elements at end, 1 element at start
        REQUIRE(ringBuff.CheckState(1, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available element at end
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Read(2) == true);              // Uses the available element at end
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Read(3) == false);             // Cannot read over the wrap, must be in 2 iterations
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available element at start
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(2, 2, 4);                     // Set mWrite(2), mRead(2), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(2, 2, 4);
        REQUIRE(ringBuff.Read(1) == false);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 2, 4);                     // Set mWrite(3), mRead(2), mWrap(4) - 1 element
        REQUIRE(ringBuff.CheckState(3, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(3, 2, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(3, 2, 4);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);
    }

    SECTION( "basic operations - read at 3" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 3, 4);                     // Set mWrite(0), mRead(3), mWrap(4) - 1 element at end
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(0, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(0, 3, 4);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);

        // -----

        ringBuff.SetState(1, 3, 4);                     // Set mWrite(1), mRead(3), mWrap(4) - 1 element at end, 1 element at start
        REQUIRE(ringBuff.CheckState(1, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available element at end
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Read(2) == false);             // Cannot read over the wrap, must be in 2 iterations
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available element at start
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Read(2) == false);
        REQUIRE(ringBuff.Size() == 1);

         // -----

        ringBuff.SetState(2, 3, 4);                     // Set mWrite(2), mRead(3), mWrap(4) - 1 element at end, 2 elements at front
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available element at end
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Read(2) == false);             // Cannot read over the wrap, must be in 2 iterations
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(1) == true);              // Uses the available elements at start
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(2) == true);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Read(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Read(3) == false);
        REQUIRE(ringBuff.Size() == 2);

        // -----

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - buffer empty
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Read(0) == false);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(3, 3, 4);
        REQUIRE(ringBuff.Read(1) == false);
        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "basic operations - invalid states" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 4, 4);                     // Set mWrite(0), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
        REQUIRE(ringBuff.CheckState(0, 4, 4) == true);

        REQUIRE(ringBuff.Read(1) == false);             // Handle as: buffer empty

        // -----

        ringBuff.SetState(1, 4, 4);                     // Set mWrite(1), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
        REQUIRE(ringBuff.CheckState(1, 4, 4) == true);

        REQUIRE(ringBuff.Read(1) == false);             // Handle as: buffer empty

        // -----

        ringBuff.SetState(2, 4, 4);                     // Set mWrite(2), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
        REQUIRE(ringBuff.CheckState(2, 4, 4) == true);

        REQUIRE(ringBuff.Read(1) == false);             // Handle as: buffer empty

        // -----

        ringBuff.SetState(3, 4, 4);                     // Set mWrite(3), mRead(4), mWrap(4) - 1 element more than possible - buffer empty
        REQUIRE(ringBuff.CheckState(3, 4, 4) == true);

        REQUIRE(ringBuff.Read(1) == false);             // Handle as: buffer empty

        // -----

        ringBuff.SetState(0, 5, 4);                     // Set mWrite(0), mRead(5), mWrap(4) - 2 elements more than possible - buffer empty
        REQUIRE(ringBuff.CheckState(0, 5, 4) == true);

        REQUIRE(ringBuff.Read(1) == false);             // Handle as: buffer empty
    }
}
