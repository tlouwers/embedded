
# SlimAssert
Slim replacement of assert macro for use in embedded environment.

## Description
This code is intended to replace the 'assert' macro from the Standard Template Library, even the one from newlib. This to prevent two things:
- Keeping the `__PRETTY_FUNCTION__` (or demangled string method names in text memory, thereby reducing the application size (especially for C++), 
- And to remove the unneeded printf() inside the macro which prints to stderr, which in the embedded environment will not (typically) be used.
It is disabled when `NDEBUG` is defined. When debugging, the callstack is preserved.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, with a simple manual test to show how to use it. |

## Notes
Inspiration from: <https://barrgroup.com/Embedded-Systems/How-To/Define-Assert-Macro>, <http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert> and <https://stackoverflow.com/questions/5252375/custom-c-assert-macro>
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Example
```cpp
// Include the header (instead of <cassert> or <assert.h>)
#include "utility/SlimAssert.h"

// Use the upper-case assert where needed
ASSERT(3 > 5);      // Obviously this trips the assert
```

## Careful
This is not a full version of assert, and is intended for an embedded platform where code size is an issue.
