
#include "../../Catch/catch.hpp"

#include <cstdint>              // uint8_t
#include "ExampleEnum.hpp"


// Note: to properly validate the bitmask we cast back to uin8_t for number compares.


TEST_CASE( "XOR operations", "[BitmaskEnum]" )
{
    // For each section variables are anew.
    ExampleEnum e1;


    SECTION( "simple XOR operations" )
    {
        e1 = ExampleEnum::BLUE ^ ExampleEnum::RED;
        REQUIRE(static_cast<uint8_t>(e1) == 0x03);

        e1 = ExampleEnum::RED ^ ExampleEnum::GREEN;
        REQUIRE(static_cast<uint8_t>(e1) == 0x06);

        e1 = ExampleEnum::GREEN ^ ExampleEnum::WHITE;
        REQUIRE(static_cast<uint8_t>(e1) == 0x0C);

        e1 = ExampleEnum::BLUE ^ ExampleEnum::WHITE;
        REQUIRE(static_cast<uint8_t>(e1) == 0x09);
    }

    SECTION( "advanced XOR operations" )
    {
        e1 = ExampleEnum::BLUE ^ ExampleEnum::WHITE ^ ExampleEnum::WHITE;   // Yes, WHITE twice
        REQUIRE(static_cast<uint8_t>(e1) == 0x01);
    }

    SECTION( "assignment XOR operations" )
    {
        e1 = ExampleEnum::BLUE;
        REQUIRE(static_cast<uint8_t>(e1) == 0x01);

        e1 ^= ExampleEnum::RED;
        REQUIRE(static_cast<uint8_t>(e1) == 0x03);

        e1 ^= ExampleEnum::WHITE;
        REQUIRE(static_cast<uint8_t>(e1) == 0x0B);

        e1 ^= ExampleEnum::WHITE;                           // WHITE again removes WHITE
        REQUIRE(static_cast<uint8_t>(e1) == 0x03);

        e1 ^= ExampleEnum::RED;                             // RED again removes RED
        REQUIRE(static_cast<uint8_t>(e1) == 0x01);
    }
}
