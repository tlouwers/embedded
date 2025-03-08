
#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "AND operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;
    ExampleEnum e2;


    SECTION( "simple AND operations" )
    {
        e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0F );

        e2 = e1 & ExampleEnum::BLUE;
        REQUIRE( static_cast<uint8_t>(e2) == 0x01 );

        e2 = e1 & ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e2) == 0x02 );

        e2 = e1 & ExampleEnum::GREEN;
        REQUIRE( static_cast<uint8_t>(e2) == 0x04 );

        e2 = e1 & ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e2) == 0x08 );
    }

    SECTION( "assignment AND operations" )
    {
        e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0F );

        e1 &= ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e1) == 0x02 );

        e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0F );

        e1 &= ExampleEnum::GREEN;
        REQUIRE( static_cast<uint8_t>(e1) == 0x04 );
    }
}
