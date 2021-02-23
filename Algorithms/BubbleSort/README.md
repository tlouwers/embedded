
# BubbleSort
Implementation of the BubbleSort algorithm with template functions.

## Description
This code is intended as non-STL version of the BubbleSort algorithm, making it better suited for embedded use.
Use it for small sets only.

Worst case:      O(n^2)
Best case:       O(n)
Average case:    O(n^2)
Auxiliary space: O(1)

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
Inspiration from: <https://www.geeksforgeeks.org/bubble-sort/>
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Example
```cpp
// Define an array to sort:
int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };

// Sort the array:
BubbleSort(array, sizeof(array));

// At this point 'array' is sorted.
```

## Careful
One can only prevent so much, in this case the check to see if the range of the array to sort is valid is omitted in the example for clarity. The algorithm is not thread safe. Performance is rather poor when using this algorithm on larger sets.
