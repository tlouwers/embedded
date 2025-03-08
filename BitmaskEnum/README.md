# BitmaskEnum
Template class for using strongly typed enum classes as bitmasks.

## Description
This implementation allows the use of strongly typed enum classes as bitmasks, enhancing code readability and reducing the likelihood of errors by providing compile-time feedback.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Notes
Originally created by **Anthony Williams** and published [here](https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html). **Andre Haupt** later introduced a macro to simplify enabling bitmask functionality for enums, detailed [here](http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/). This project consolidates the aforementioned code into a single header file, with added comments and examples. Contributions for bug reports, fixes, or refactoring suggestions are welcome!

## Example
```cpp
// Include this file in the header where the enum is defined
#include "BitmaskEnum.hpp"

// Define the enum 'Dummy':
enum class Dummy : uint8_t
{
    BLUE  = 0x01,   // Bit 1
    RED   = 0x02,   // Bit 2
    GREEN = 0x04,   // Bit 3
};
ENABLE_BITMASK_OPERATORS(Dummy) // Enable bitmask functionality

// Use the enum as a bitmask:
Dummy d1 = (Dummy::BLUE | Dummy::GREEN); // d1: Bit 1 and 3
Dummy d2 = d1 & Dummy::BLUE;              // d2: Bit 1
Dummy d3 = d1 | Dummy::RED;               // d3: Bit 1 and 2
d1 |= Dummy::RED;                         // d1: Bit 1, 2, and 3
d1 &= ~Dummy::BLUE;                       // d1: Bit 2 and 3
```

## Caution
Ensure that the enum values fill the underlying type (e.g., `uint8_t` in the example) to avoid issues with unused upper bits. While bitmask operations are valid, comparing enums may yield unexpected results if upper bits are set by previous operations. Direct access to the underlying type via `static_cast<>` is discouraged, as it undermines the purpose of using the template.
