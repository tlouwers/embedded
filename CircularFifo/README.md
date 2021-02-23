
# CircularFifo
A thread-safe, lock-free, single producer, single consumer, ringbuffer.

## Description
This code is intended to provide a "Template" for a Circular buffer, its  main use is for embedded devices where streaming data is handled.
The code is thread safe for a single producer, single consumer.
An example of this is an interrupt producing data (read from input), main application code reading the data.

## Requirements
- C++11

## Notes
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

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
