

#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "NOT operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;


    SECTION( "simple NOT operations" )
    {
        e1 = ~ExampleEnum::BLUE;

        uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;  // Get lower 4 bits: no more bits in enum
        REQUIRE( static_cast<uint8_t>(tmp) == 0x0E );


        e1 = ~ExampleEnum::GREEN;

        tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
        REQUIRE( static_cast<uint8_t>(tmp) == 0x0B );


        e1 = ~ExampleEnum::RED;

        tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
        REQUIRE( static_cast<uint8_t>(tmp) == 0x0D );
    }

    SECTION( "advanced NOT operations" )
    {
        e1 = ~(ExampleEnum::BLUE | ExampleEnum::WHITE);

        uint8_t tmp = static_cast<uint8_t>(e1) & 0x0F;  // Get lower 4 bits: no more bits in enum
        REQUIRE( static_cast<uint8_t>(tmp) == 0x06 );


        e1 = ~(ExampleEnum::RED | ExampleEnum::GREEN);

        tmp = static_cast<uint8_t>(e1) & 0x0F;          // Get lower 4 bits: no more bits in enum
        REQUIRE( static_cast<uint8_t>(tmp) == 0x09 );
    }
}
