# Sawtooth
Implementation of a simple sawtooth generator class.

## Overview
This repository contains a non-STL implementation of a sawtooth generator designed to produce a simple sawtooth waveform based on specified parameters. The range and steepness of the waveform can be easily configured.

## Algorithm Complexity
- **Time Complexity:** O(1) for generating the next value.
- **Space Complexity:** O(1) as it operates in place.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Usage
To use the Sawtooth generator, include the header file in your project and create an instance of the `Sawtooth` class with the desired parameters.

### Example
```cpp
#include "Sawtooth.hpp" // Include the Sawtooth header

// Create sawtooth objects with desired parameters:
Sawtooth mSawtooth1(79, 5);   // 0..79 in 5 steps of 16
Sawtooth mSawtooth2(99, 8);   // 0..99 in 8 steps of 12

// Generate values with 'Next()':
for (auto i = 0; i < 10; i++)
{
    uint16_t tmp1 = mSawtooth1.Next();
    uint16_t tmp2 = mSawtooth2.Next();
}

// This results in values for mSawtooth1: 16, 32, 48, 64, 0, 16, 32, 48, 64, 0.
// This results in values for mSawtooth2: 12, 24, 36, 48, 60, 72, 84, 96, 8, 20.

// Use 'Reset()' to start at value '0' again:
mSawtooth1.Reset();
```

## Careful
When calculating the step size, rounding will occur: the remainder of the factional number is discarded.
When wrapping, the remainder before the wrapping point is taken into account.
