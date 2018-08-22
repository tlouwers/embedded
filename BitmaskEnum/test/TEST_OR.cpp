
#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "OR operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;


    SECTION( "simple OR operations" )
    {
        e1 = ExampleEnum::BLUE | ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e1) == 0x03 );

        e1 = ExampleEnum::RED | ExampleEnum::GREEN;
        REQUIRE( static_cast<uint8_t>(e1) == 0x06 );

        e1 = ExampleEnum::GREEN | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0C );

        e1 = ExampleEnum::WHITE | ExampleEnum::BLUE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x09 );

        e1 = ExampleEnum::GREEN | ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e1) == 0x06 );


        e1 = ExampleEnum::RED | ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e1) == 0x02 );
    }

    SECTION( "advanced OR operations" )
    {
        e1 = ExampleEnum::BLUE | ExampleEnum::RED | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0B );

        e1 = ExampleEnum::RED | ExampleEnum::GREEN | ExampleEnum::BLUE | ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0F );
    }

    SECTION( "assignment OR operations" )
    {
        e1 = ExampleEnum::BLUE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x01 );

        e1 |= ExampleEnum::GREEN;
        REQUIRE( static_cast<uint8_t>(e1) == 0x05 );

        e1 |= ExampleEnum::WHITE;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0D );

        e1 |= ExampleEnum::RED;
        REQUIRE( static_cast<uint8_t>(e1) == 0x0F );
    }
}
