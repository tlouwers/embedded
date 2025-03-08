# BubbleSort
Implementation of the BubbleSort algorithm with template functions.

## Overview
This repository contains a non-STL implementation of the BubbleSort algorithm using C++ template functions. Designed specifically for embedded systems, this version of BubbleSort is optimized for environments where the Standard Template Library (STL) may not be available or suitable. It is particularly effective for sorting large datasets.

## Algorithm Complexity
- **Worst Case:** O(n²)
- **Best Case:** O(n) (when the array is already sorted)
- **Average Case:** O(n²)
- **Auxiliary Space:** O(1)

## Features
- **Optimized for Small Datasets:** The algorithm performs well on small or nearly sorted datasets due to its early exit optimization when no swaps occur.
- **In-Place Sorting:** The algorithm sorts the array in place, requiring no additional memory allocation.
- **Template Function:** The implementation is generic and can sort arrays of any data type that supports comparison.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Usage
To use the BubbleSort algorithm, include the header file in your project and call the `BubbleSort` function with the array you wish to sort.

### Example
```cpp
#include "BubbleSort.hpp" // Include the BubbleSort header

// Define an array to sort:
int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

// Sort the array:
BubbleSort(array, sizeof(array) / sizeof(array[0]));

// At this point, 'array' is sorted.
```

### Important Notes
- The implementation checks for valid array lengths. If the length is less than 2, it returns false for invalid input.
- The algorithm is **not thread-safe**. Use caution when integrating into multi-threaded applications.
- BubbleSort is not efficient for large datasets. Consider using more advanced sorting algorithms for larger arrays.

## Contributions
If you encounter any issues or have suggestions for improvements, please feel free to reach out. Contributions in the form of bug reports, fixes, or refactoring suggestions are welcome!

## Acknowledgments
This project is inspired by resources from:
- [GeeksforGeeks - Bubble Sort](https://www.geeksforgeeks.org/bubble-sort/)
