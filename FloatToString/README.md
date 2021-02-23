
# FloatToString
A float or double integral to string conversion function.

## Description
This code is intended to be used when no default conversion for float or double to string exists. A typical use case would be to print a sensor value (float) to a log file.
The code is implemented in 'C', to be usable in both 'C' and 'C++' projects.

## Requirements
- C99

## Notes
Code was originally created by <b>Anton Gusev</b>, published here: <https://github.com/antongus/stm32tpl>
I cleaned up the code a little and added some comments.
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Example
```cpp
#include "float_to_string.h"

// inside some method ...
{
    // Example float number
    constexpr float my_nr = 12.345f;
    
    // Create (and clear) a character array to store the result
    char buf[15] = {};

    // Convert the float to character string (null terminated) using 3 digits behind the dot
    ftoa(my_nr, buf, 3);
}
```

## Careful
One can only prevent so much, in this case when converting from float do not go outside the presicion of the float else behaviour is undefined.
