# QuickSort
Implementation of the QuickSort algorithm with template functions.

## Overview
This repository contains a non-STL implementation of the QuickSort algorithm using C++ template functions. Designed specifically for embedded systems, this version of QuickSort is optimized for environments where the Standard Template Library (STL) may not be available or suitable. It is particularly effective for sorting large datasets.

## Algorithm Complexity
- **Worst Case:** O(n²) (occurs when the smallest or largest element is consistently chosen as the pivot)
- **Best Case:** O(n log n) (occurs when the pivot divides the array into two equal halves)
- **Average Case:** O(n log n)
- **Auxiliary Space:** O(log n) (due to the recursive stack space)

## Description
- **Divide-and-Conquer Algorithm** QuickSort efficiently sorts an array by partitioning it into smaller sub-arrays, allowing for faster sorting compared to simpler algorithms.
- **Average Time Complexity** The algorithm has an average time complexity of O(n log n), making it suitable for large datasets.
- **Worst-Case Handling** While the worst-case time complexity is O(n²) (occurring when the smallest or largest element is consistently chosen as the pivot), the implementation mitigates this by using a hybrid approach that handles smaller partitions recursively and larger ones iteratively.
- **In-Place Sorting** QuickSort sorts the array in place, requiring minimal additional memory, with a space complexity of O(log n) due to the recursive stack space.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Usage
To use the QuickSort implementation, include the `QuickSort.hpp` header file in your project. Below is a simple example demonstrating how to sort an array:

### Example
```cpp
#include "QuickSort.hpp"

// Define an array to sort:
int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

// Sort the array:
QuickSort(array, 0, (sizeof(array) / sizeof(array[0]) - 1));

// At this point, 'array' is sorted.
```

### Important Notes
- The example above omits error checking for clarity. In practice, ensure that the range of the array to sort is valid before calling `QuickSort`.
- This implementation is **not thread-safe**. If you require thread safety, consider using synchronization mechanisms or alternative sorting methods.
- If the use of the STL is acceptable, it is recommended to use `std::qsort()` from `<cstdlib>` for sorting.

## Contributions
If you encounter any issues or have suggestions for improvements, please feel free to reach out. Contributions in the form of bug reports, fixes, or refactoring suggestions are welcome!

## Acknowledgments
This project is inspired by resources from:
- [GeeksforGeeks - QuickSort](https://www.geeksforgeeks.org/quick-sort/)
- [Cplusplus.com Forum](http://www.cplusplus.com/forum/beginner/119660/)
