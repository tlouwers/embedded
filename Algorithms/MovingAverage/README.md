# MovingAverage
Implementation of a simple moving average with template functions.

## Overview
This repository contains a non-STL implementation of a simple moving average algorithm using C++ template functions. Designed specifically for embedded systems, this version of the MovingAverage class is optimized for environments where the Standard Template Library (STL) may not be available or suitable. It effectively smooths out data samples, making it particularly useful for applications like battery monitoring.

## Algorithm Complexity
- **Time Complexity:** O(1) for adding a new value and O(n) for calculating the average (where n is the number of items in the buffer).
- **Space Complexity:** O(n) for storing the elements in the internal buffer.

## Features
- **Dynamic Buffer Management:** The internal buffer can be resized to accommodate varying amounts of data.
- **Circular Buffer Logic:** The implementation uses a circular buffer to efficiently manage the addition of new elements while maintaining the moving average.
- **Type Safety:** The implementation restricts the use of larger types (e.g., `double`, `int64_t`, `uint64_t`) to ensure compatibility with smaller ranges suitable for embedded systems.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Usage
To use the MovingAverage class, include the header file in your project and create an instance of the class with the desired data type.

### Example
```cpp
#include "MovingAverage.hpp" // Include the MovingAverage header

// Declare the object and specify the type to use:
MovingAverage<int> movAvg;

// Use Resize() to define the length of the internal buffer:
movAvg.Resize(6);

// Add values:
movAvg.Add(2);
movAvg.Add(4);

// Calculate and get the averaged sum:
int result = movAvg.GetAverage();
```

### Important Notes
- The implementation checks for valid buffer sizes. If the size is less than 1, the `Resize()` method will return false for invalid input.
- The class is **not thread-safe**. Use caution when integrating into multi-threaded applications.
- If there are no elements in the internal buffer and `GetAverage()` is called, the result will be 0. Ensure to check the state of the buffer before performing operations that depend on its contents.

## Contributions
If you encounter any issues or have suggestions for improvements, please feel free to reach out. Contributions in the form of bug reports, fixes, or refactoring suggestions are welcome!
