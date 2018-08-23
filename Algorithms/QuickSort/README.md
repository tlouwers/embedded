


# QuickSort
Implementation of the QuickSort algorithm with template functions.

## Introduction
This code is intended as non-STL version of the QuickSort algorithm, making it better suited for embedded use.

## Requirements

 - C++11

## Contents

| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

## Example
```cpp
// Define an array to sort:
int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

// Sort the array:
QuickSort(array, 0, (sizeof(array) - 1));

// At this point 'array' is sorted.
```

## Careful
One can only prevent so much, in this case the check to see if the range of the array to sort is valid is omitted in the example for clarity. The algorithm is not thread safe. If the use of the STL (standard template library) is not an issue, the use of std::qsort() is recommended (in \<cstdlib\>).
