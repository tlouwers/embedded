# ftoa
A float or double integral to string conversion function.

## Introduction
This code is intended to be used when no default conversion exists. A typical use case would be to print a sensor value (float) to a log file.

## Requirements
 - C11

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

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
