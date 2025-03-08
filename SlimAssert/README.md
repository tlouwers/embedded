# SlimAssert
Slim replacement of the assert macro for use in embedded environments.

## Description
SlimAssert is designed to replace the standard `assert` macro, including those from the Standard Template Library and newlib. The primary goals are to:

- Preserve the call stack without including `__PRETTY_FUNCTION__` or demangled method names in text memory, which helps reduce application size, especially in C++.
- Eliminate unnecessary output to `stderr` (such as from `printf()`) within the macro, which is typically not used in embedded environments.

Assertions are disabled when `NDEBUG` is defined. When debugging, the call stack is preserved, allowing for easier identification of assertion failures.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test   | A CMake project with a simple manual test demonstrating how to use SlimAssert. |

## Notes
Inspiration for SlimAssert was drawn from various sources, including:
- [Barr Group: Define Assert Macro](https://barrgroup.com/Embedded-Systems/How-To/Define-Assert-Macro)
- [C. Nicholson: Stupid C Tricks - Adventures in Assert](http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert)
- [Stack Overflow: Custom C Assert Macro](https://stackoverflow.com/questions/5252375/custom-c-assert-macro)

If you encounter any issues or have suggestions for improvements, please provide a reproducible scenario. Contributions for fixes or refactoring are welcome!

## Example
To use SlimAssert, include the header file instead of the standard assert header:

```cpp
// Include the SlimAssert header
#include "utility/SlimAssert.h"

// Use the upper-case ASSERT macro where needed
ASSERT(3 > 5);      // This will trigger the assertion failure
```

## Caution
SlimAssert is not a complete replacement for the standard assert functionality and is specifically tailored for embedded platforms where code size is a critical concern.