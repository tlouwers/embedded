# BitmaskEnum
Template class to use strongly typed enum classes as bitmask,

## Introduction
This code is intended to use strongly typed enum classes as bitmasks. Doing so will improve readability and reduce the chance of errors, getting feedback at compile time.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

## Example
```cpp
// Include this file (only) to the header files listing the enum
#include "BitmaskEnum.hpp"

// Given the enum 'Dummy':
enum class Dummy : uint8_t
{
    BLUE  = 0x01,   // Bit 1
    RED   = 0x02,   // Bit 2
    GREEN = 0x04,   // Bit 3
};
ENABLE_BITMASK_OPERATORS(Dummy)		// Enable bitmask functionality

// After the 'enable', we can use the enum as bitmask:
Dummy d1 = ( Dummy::BLUE | Dummy::GREEN );      // d1: Bit 1 and 3
Dummy d2 = d1 & Dummy::BLUE;                    // d2: Bit 1
Dummy d3 = d1 | Dummy::RED;                     // d3: Bit 1 and 2
d1 |= Dummy::RED;                               // d1: Bit 1, 2 and 3
d1 &= ~Dummy::BLUE;                             // d1: Bit 2 and 3
```

## Careful
One can only prevent so much, if the enum does not fill the underlying type, like the uint8_t in the example, care must be taken with the 'not used' upper bits. The bitmask operations are valid, only when using the enum to compare these might be tricky if these upper bits are set by an earlier operation. Also, since the underlying type is set, this allows direct access to this type with a static_cast<> -- please do not do this, it defeats the purpose of the template.
