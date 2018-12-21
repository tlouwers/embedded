
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Write() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;


    REQUIRE(ringBuff.Resize(3) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "basic operations - read at 0" )
    {
        ringBuff.SetState(0, 0, 4);                     // Set mWrite(0), mRead(0), mWrap(4) - 3 elements available at start
        REQUIRE(ringBuff.CheckState(0, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        ringBuff.SetState(0, 0, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(0, 0, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(0, 0, 4);
        REQUIRE(ringBuff.Write(3) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(0, 0, 4);
        REQUIRE(ringBuff.Write(4) == false);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        // -----

        ringBuff.SetState(1, 0, 4);                     // Set mWrite(1), mRead(0), mWrap(4) - 2 elements available at end
        REQUIRE(ringBuff.CheckState(1, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(1, 0, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(1, 0, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(1, 0, 4);
        REQUIRE(ringBuff.Write(3) == false);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        // -----

        ringBuff.SetState(2, 0, 4);                     // Set mWrite(2), mRead(0), mWrap(4) - 1 element available at end
        REQUIRE(ringBuff.CheckState(2, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(2, 0, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(2, 0, 4);
        REQUIRE(ringBuff.Write(2) == false);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        // -----

        ringBuff.SetState(3, 0, 4);                     // Set mWrite(3), mRead(0), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(3, 0, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(3, 0, 4);
        REQUIRE(ringBuff.Write(1) == false);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);
    }

    SECTION( "basic operations - read at 1" )
    {
        ringBuff.SetState(0, 1, 4);                     // Set mWrite(0), mRead(1), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(0, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(0, 1, 4);
        REQUIRE(ringBuff.Write(1) == false);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        // -----

        ringBuff.SetState(1, 1, 4);                     // Set mWrite(1), mRead(1), mWrap(4) - 3 elements available at end
        REQUIRE(ringBuff.CheckState(1, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        ringBuff.SetState(1, 1, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(1, 1, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(1, 1, 4);
        REQUIRE(ringBuff.Write(3) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(1, 1, 4);
        REQUIRE(ringBuff.Write(4) == false);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        // -----

        ringBuff.SetState(2, 1, 4);                     // Set mWrite(2), mRead(1), mWrap(4) - 2 elements available at end
        REQUIRE(ringBuff.CheckState(2, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(2, 1, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(2, 1, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(2, 1, 4);
        REQUIRE(ringBuff.Write(3) == false);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        // -----

        ringBuff.SetState(3, 1, 4);                     // Set mWrite(3), mRead(1), mWrap(4) - 1 element available at end
        REQUIRE(ringBuff.CheckState(3, 1, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(3, 1, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 3);

        ringBuff.SetState(3, 1, 4);
        REQUIRE(ringBuff.Write(2) == false);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);
    }

    SECTION( "basic operations - read at 2" )
    {
        ringBuff.SetState(0, 2, 4);                     // Set mWrite(0), mRead(2), mWrap(4) - 1 element available at start
        REQUIRE(ringBuff.CheckState(0, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(0, 2, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(0, 2, 4);
        REQUIRE(ringBuff.Write(2) == false);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        // -----

        ringBuff.SetState(1, 2, 4);                     // Set mWrite(1), mRead(2), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(1, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(1, 2, 4);
        REQUIRE(ringBuff.Write(1) == false);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        // -----

        ringBuff.SetState(2, 2, 4);                     // Set mWrite(2), mRead(2), mWrap(4) - 2 elements available at end
        REQUIRE(ringBuff.CheckState(2, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        ringBuff.SetState(2, 2, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(2, 2, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(2, 2, 4);
        REQUIRE(ringBuff.Write(3) == false);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        // -----

        ringBuff.SetState(3, 2, 4);                     // Set mWrite(3), mRead(2), mWrap(4) - 1 element available at end
        REQUIRE(ringBuff.CheckState(3, 2, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(3, 2, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(3, 2, 4);
        REQUIRE(ringBuff.Write(2) == false);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);
    }

    SECTION( "basic operations - read at 3" )
    {
        ringBuff.SetState(0, 3, 4);                     // Set mWrite(0), mRead(3), mWrap(4) - 2 elements available at start
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(0, 3, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(0, 3, 4);
        REQUIRE(ringBuff.Write(2) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(0, 3, 4);
        REQUIRE(ringBuff.Write(3) == false);
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        // -----

        ringBuff.SetState(1, 3, 4);                     // Set mWrite(1), mRead(3), mWrap(4) - 1 element available at start
        REQUIRE(ringBuff.CheckState(1, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(1, 3, 4);
        REQUIRE(ringBuff.Write(2) == false);
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.ContiguousSize() == 1);

         // -----

        ringBuff.SetState(2, 3, 4);                     // Set mWrite(2), mRead(3), mWrap(4) - buffer full
        REQUIRE(ringBuff.CheckState(2, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(2, 3, 4);
        REQUIRE(ringBuff.Write(1) == false);
        REQUIRE(ringBuff.Size() == 3);
        REQUIRE(ringBuff.ContiguousSize() == 1);

        // -----

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start
        REQUIRE(ringBuff.CheckState(3, 3, 4) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.Write(0) == true);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);

        ringBuff.SetState(3, 3, 4);
        REQUIRE(ringBuff.Write(1) == true);             // Uses the available element at the end
        REQUIRE(ringBuff.Size() == 1);
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);  // Does not wrap
        REQUIRE(ringBuff.ContiguousSize() == 1);

        ringBuff.SetState(3, 3, 4);
        REQUIRE(ringBuff.Write(2) == true);             // Uses the available elements at the start
        REQUIRE(ringBuff.Size() == 2);
        REQUIRE(ringBuff.CheckState(2, 3, 3) == true);  // Does wrap
        REQUIRE(ringBuff.ContiguousSize() == 2);

        ringBuff.SetState(3, 3, 4);
        REQUIRE(ringBuff.Write(3) == false);
        REQUIRE(ringBuff.Size() == 0);
        REQUIRE(ringBuff.ContiguousSize() == 0);
    }

    SECTION( "basic operations - invalid states" )
    {
        ringBuff.SetState(4, 0, 4);                     // Set mWrite(4), mRead(0), mWrap(4) - 1 element more than possible - buffer full
        REQUIRE(ringBuff.CheckState(4, 0, 4) == true);

        REQUIRE(ringBuff.Write(1) == false);            // Handle as: buffer full
        REQUIRE(ringBuff.ContiguousSize() == 3);

        // -----

        ringBuff.SetState(4, 1, 4);                     // Set mWrite(4), mRead(1), mWrap(4) - 1 element more than possible - buffer full
        REQUIRE(ringBuff.CheckState(4, 1, 4) == true);

        REQUIRE(ringBuff.Write(1) == false);            // Handle as: buffer full
        REQUIRE(ringBuff.ContiguousSize() == 3);

        // -----

        ringBuff.SetState(4, 2, 4);                     // Set mWrite(4), mRead(2), mWrap(4) - 1 element more than possible - buffer full
        REQUIRE(ringBuff.CheckState(4, 2, 4) == true);

        REQUIRE(ringBuff.Write(1) == false);            // Handle as: buffer full
        REQUIRE(ringBuff.ContiguousSize() == 2);

        // -----

        ringBuff.SetState(4, 3, 4);                     // Set mWrite(4), mRead(3), mWrap(4) - 1 element more than possible - buffer full
        REQUIRE(ringBuff.CheckState(4, 3, 4) == true);

        REQUIRE(ringBuff.Write(1) == false);            // Handle as: buffer full
        REQUIRE(ringBuff.ContiguousSize() == 1);

        // -----

        ringBuff.SetState(5, 0, 4);                     // Set mWrite(5), mRead(0), mWrap(4) - 2 elements more than possible - buffer full
        REQUIRE(ringBuff.CheckState(5, 0, 4) == true);

        REQUIRE(ringBuff.Write(1) == false);            // Handle as: buffer full
        REQUIRE(ringBuff.ContiguousSize() == 3);
    }
}
