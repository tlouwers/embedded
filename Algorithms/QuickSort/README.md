
# QuickSort
Implementation of the QuickSort algorithm with template functions.

## Description
This code is intended as non-STL version of the QuickSort algorithm, making it better suited for embedded use.
Use it for large sets.

Worst case:      O(n^2)
Best case:       O(n)
Average case:    O(n)
Auxiliary space: O(Log n)

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
Inspiration from: <https://www.geeksforgeeks.org/quick-sort/> and <http://www.cplusplus.com/forum/beginner/119660/>
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

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
