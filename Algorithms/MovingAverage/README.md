
# MovingAverage
Implementation of a simple moving average.

## Introduction
This code is intended to smooth out battery samples on an embedded device, using a simple moving average.

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
// Declare the object and type to use:
MovingAverage<int> movAvg;

// Use Resize() to define the length of the internal buffer:
movAvg.Resize(6);

// Add values:
movAvg.Add(2);
movAvg.Add(4);

// Calculate and get the averaged sum:
int result = movAvg.GetAverage();
```

## Notes
Since the intent is to use it on smaller ranges and embedded devices, the use of larger types 'T' is not allowed: double, int64_t and uint64_t are checked/blocked in Resize().

## Careful
One can only prevent so much, if the return values are not checked behavior might be undefined. One case in particular: if there are no elements in the internal buffer and GetAverage() is called, the result is 0. If this is used directly in a calculation it might trigger an unwanted divide-by-zero error.
