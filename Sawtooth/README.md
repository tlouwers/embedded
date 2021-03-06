# Sawtooth
Simple sawtooth generator class.

## Description
This code is intended to generate a simple sawtooth within given parameters. The range and steepness can be set.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Example
```cpp
// Create a sawooth object and construct it with desired parameters:
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
