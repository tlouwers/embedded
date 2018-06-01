

# Ringbuffer
A thread-safe, lock-free, single producer, single consumer, ringbuffer.

## Introduction
This code is intended to be used as single-producer, single-consumer ringbuffer on a Cortex-M4 microcontroller, but can see its share of use in other scenarios as well. Be sure to read up on the documentation, as it has some other-than-usual behavior.

## Requirements

 - C++11

## Contents

| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with a main.cpp file containing tests for the buffer. |

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

## Example

```cpp
// Producer will fill the buffer, Consumer will empty the buffer.
// Note: do check for the result values, the example omits them for clarity.

// Declare the buffer
Ringbuffer<int> ringBuff;

// Reserve the size to hold the elements, 'int' in this example
ringBuff.Resize(5);

// < TBD >
```

## Intended use
< TBD >

