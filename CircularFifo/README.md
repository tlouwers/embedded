# CircularFifo
A thread-safe, lock-free circular buffer designed for single-producer, single-consumer scenarios.

## Description
`CircularFifo` is a template implementation of a circular buffer, optimized for use in embedded systems where data streaming is common. It allows one producer (e.g., an interrupt handler) to write data and one consumer (e.g., the main application) to read data concurrently without locking, ensuring efficient and safe access.

## Requirements
- C++11 or later

## Features
- **Thread-Safe**: Designed for a single producer and a single consumer.
- **Lock-Free**: Utilizes atomic operations to avoid locking mechanisms.
- **Fixed Capacity**: The buffer size is defined at compile time.

## Usage Example
```cpp
// Create a circular buffer with a capacity of 4 elements
CircularFifo<int, 4> mBuffer;

// Producer: Add an item to the buffer
int itemToAdd = 4;
bool result = mBuffer.push(itemToAdd);

// Consumer: Retrieve an item from the buffer
int itemRetrieved;
result = mBuffer.pop(itemRetrieved);

// Check the result to ensure the operation succeeded
if (result) {
    // Successfully retrieved item
}
```

## Intended Use
This circular buffer is ideal for scenarios requiring single-element access in a thread-safe manner. The `push` method copies the element into the buffer, while the `pop` method retrieves it. Always check the return values of these methods to confirm successful operations.

## Contributions
If you encounter any issues or have suggestions for improvements, please provide a reproducible scenario. Contributions for fixes or refactoring are welcome to enhance the code further.
