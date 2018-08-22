
#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "Mixed operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;


    SECTION( "mixed operations" )
    {
        e1 = ExampleEnum::RED | ExampleEnum::GREEN;
        REQUIRE(static_cast<uint8_t>(e1) == 0x06);

        e1 |= ExampleEnum::BLUE;                            // Add BLUE
        REQUIRE(static_cast<uint8_t>(e1) == 0x07);

        e1 &= ExampleEnum::RED;                             // Filter, only if RED is set use it
        REQUIRE(static_cast<uint8_t>(e1) == 0x02);


        e1 = ExampleEnum::BLUE | ExampleEnum::GREEN;
        REQUIRE(static_cast<uint8_t>(e1) == 0x05);

        e1 &= ExampleEnum::RED;                             // Filter, only if RED is set use it
        uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;      // Only use lower 4 bits of ExampleEnum
        REQUIRE(static_cast<uint8_t>(e1) == 0x00);


        e1 ^= ExampleEnum::WHITE;                           // Toggle WHITE
        REQUIRE(static_cast<uint8_t>(e1) == 0x08);

        e1 ^= ExampleEnum::WHITE;                           // Toggle WHITE
        REQUIRE(static_cast<uint8_t>(e1) == 0x00);


        e1 = ~(ExampleEnum::GREEN | ExampleEnum::RED);      // All  but GREEN and RED
        tmp = static_cast<uint8_t>(e1) & 0x0F;              // Only use lower 4 bits of ExampleEnum
        REQUIRE(tmp == 0x09);


        e1 = ExampleEnum::BLUE | ExampleEnum::GREEN;
        REQUIRE(static_cast<uint8_t>(e1) == 0x05);

        e1 &= ~ExampleEnum::GREEN;                          // Remove GREEN
        REQUIRE(static_cast<uint8_t>(e1) == 0x01);

        e1 |= ExampleEnum::GREEN;                           // Add GREEN
        REQUIRE(static_cast<uint8_t>(e1) == 0x05);
    }
}
