
#include "../../Catch/catch.hpp"

#include "Ringbuffer.hpp"


TEST_CASE( "Ringbuffer Resize() operations", "[Ringbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    Ringbuffer<int> ringBuff;

    int src[5] = { 1, 2, 3 };
    int* pSrc = &src[0];

    REQUIRE( ringBuff.Size() == 0 );


    SECTION( "resize small to large" )
    {
        REQUIRE(ringBuff.Resize(5) == true);
        REQUIRE(ringBuff.Size() == 0);
        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);

        REQUIRE(ringBuff.TryPush(pSrc, 2) == true);
        REQUIRE(ringBuff.Size() == 2);

        // Now resize to large: discards data

        REQUIRE(ringBuff.Resize(50) == true);
        REQUIRE(ringBuff.Size() == 0);

        REQUIRE(ringBuff.TryPush(pSrc, 2) == true);
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

        REQUIRE(ringBuff.TryPush(pSrc, 2) == true);
        REQUIRE(ringBuff.Size() == 2);

        // Now resize to small: discards data

        REQUIRE(ringBuff.Resize(5) == true);
        REQUIRE(ringBuff.Size() == 0);

        REQUIRE(ringBuff.TryPush(pSrc, 2) == true);
        REQUIRE(ringBuff.Size() == 2);

        ringBuff.Clear();
        REQUIRE(ringBuff.Size() == 0);
    }
}
