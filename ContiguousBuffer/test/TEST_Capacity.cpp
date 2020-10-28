
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


TEST_CASE( "ContiguousRingbuffer Capacity() operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    SECTION( "after resize" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Capacity() == 3);
        REQUIRE(ringBuff.Resize(1) == true);
        REQUIRE(ringBuff.Capacity() == 1);
        REQUIRE(ringBuff.Resize(5) == true);
        REQUIRE(ringBuff.Capacity() == 5);
    }
}
