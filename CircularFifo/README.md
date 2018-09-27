
# CircularFifo
A thread-safe, lock-free, single producer, single consumer, ringbuffer.

## Introduction
Small, simple, per-element based thread-safe ringbuffer.

## Requirements

 - C++11

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

## Example

```cpp
// Producer will fill the buffer, Consumer will empty the buffer.
// Note: do check for the result values.

// Declare the buffer and the size
CircularFifo<int, 4> mBuffer;

// Add an item
int i = 4;
result = mBuffer.push(i);

// Retrieve an item
int i;
result = mBuffer.pop(i);

// Note: only if the result is true, the action succeeded.
```

## Intended use
For single-element access, this is a simple and small thread-safe ringbuffer.
The buffer will copy the element when 'push' it inside and 'pop' it out.
The result values of these methods must be used to check if the action succeeded or not.
