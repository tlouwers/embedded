
#ifndef EXAMPLE_ENUM_HPP_
#define EXAMPLE_ENUM_HPP_


#include <cstdint>          // uint8_t

// Add include to get to the macro to enable bitmask operations on the enum
#include "../BitmaskEnum.hpp"


enum class ExampleEnum : uint8_t            // Use an unsigned integer type to specify the bitmask values
{
    BLUE  = 0x01,   // bit 1                // Add a bit value per entry
    RED   = 0x02,   // bit 2
    GREEN = 0x04,   // bit 3
    WHITE = 0x08,   // bit 4
};
ENABLE_BITMASK_OPERATORS(ExampleEnum)       // No closing ';'


#endif // EXAMPLE_ENUM_HPP_
