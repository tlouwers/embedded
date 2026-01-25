# QuickSort
Implementation of the QuickSort algorithm with template functions.

## Overview
This repository contains a non-STL implementation of the QuickSort algorithm using C++ template functions. Designed specifically for embedded systems, this version of QuickSort is optimized for environments where the Standard Template Library (STL) may not be available or suitable.

## Algorithm Complexity
- **Worst Case:** O(n²) — occurs when pivot selection is consistently poor
- **Best Case:** O(n log n)
- **Average Case:** O(n log n)
- **Auxiliary Space:** O(log n) due to the recursive stack space

## Optimization
This implementation includes tail-call optimization to reduce stack depth.

### Tail-Call Optimization (Iterative Larger Partition)
After partitioning, the algorithm recurses on the smaller partition and iterates (loops) on the larger partition. This ensures the maximum recursion depth remains O(log n) even in the worst case.

**Why it matters:** Reduces stack usage, making the algorithm more suitable for embedded systems with limited stack space.

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

// Sort the array (start index, end index):
QuickSort(array, 0, (sizeof(array) / sizeof(array[0])) - 1);

// At this point, 'array' is sorted.
```

### Important Notes
- The example above omits error checking for clarity. In practice, ensure that the range of the array to sort is valid before calling `QuickSort`.
- This implementation is **not thread-safe**. If you require thread safety, consider using synchronization mechanisms or alternative sorting methods.
- If the use of the STL is acceptable, it is recommended to use `std::sort()` from `<algorithm>` for sorting.

## Contributions
If you encounter any issues or have suggestions for improvements, please feel free to reach out. Contributions in the form of bug reports, fixes, or refactoring suggestions are welcome!

## Acknowledgments
This project is inspired by resources from:
- [GeeksforGeeks - QuickSort](https://www.geeksforgeeks.org/quick-sort/)
- [Cplusplus.com Forum](http://www.cplusplus.com/forum/beginner/119660/)
- Sedgewick, R. "Implementing Quicksort Programs" (median-of-three analysis)
