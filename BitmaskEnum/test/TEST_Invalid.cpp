
#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "Invalid operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;


    SECTION( "invalid OR operations" )
    {
        e1 = static_cast<ExampleEnum>(0x10);            // To show this is allowed, but not correct as no bitmask is available
        REQUIRE( static_cast<uint8_t>(e1) == 0x10 );    // Upper 4 bits not part of ExampleEnum - what is expressed here?
    }

    SECTION( "invalid NOT operations" )
    {
        e1 = ~ExampleEnum::RED;

        uint8_t tmp = static_cast<uint8_t>(e1);
        REQUIRE( tmp == 0xFD );                         // Upper 4 bits not part of ExampleEnum - what is expressed here?
    }

    SECTION( "invalid XOR operations" )
    {
        e1 = ExampleEnum::BLUE ^ ExampleEnum::BLUE;     // Yes, BLUE twice
        REQUIRE(static_cast<uint8_t>(e1) == 0x00);      // Valid bitmask, but not part of the enum anymore
    }
}
