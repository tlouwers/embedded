


# BubbleSort
Implementation of the BubbleSort algorithm with template functions.

## Introduction
This code is intended as simple sort algorithm, used for small sets.

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
BubbleSort(array, sizeof(array));

// At this point 'array' is sorted.
```

## Careful
One can only prevent so much, in this case the check to see if the range of the array to sort is valid is omitted in the example for clarity. The algorithm is not thread safe. Performance is rather poor when using this algorithm on larger sets.
