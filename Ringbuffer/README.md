# Ringbuffer
A thread-safe, lock-free, single-producer, single-consumer ring buffer.

## Introduction
This code is designed for use as a single-producer, single-consumer ring buffer, particularly on Cortex-M4 microcontrollers, but it can be applied in various scenarios. Please refer to the documentation for details on its unique behavior.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test   | A CMake project with tests written using the Google Test framework. |

## Notes
Inspired by:
- [Lock-Free Single-Producer Single-Consumer Circular Queue](https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular)
- [Boost Lockfree SPSC Queue](https://www.boost.org/doc/libs/1_54_0/doc/html/boost/lockfree/spsc_queue.html)
- [MoodyCamel's Fast Lock-Free Queue](http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++)

If you encounter any issues or have suggestions for improvements, please reach out with a reproducible scenario or a proposed fix.

## Example
```cpp
// Producer fills the buffer, Consumer empties it.
// Note: Check result values; the example omits them for clarity.

// Declare the buffer
Ringbuffer<int> ringBuff;

// Resize the buffer to hold elements
ringBuff.Resize(5);

// Check the number of elements in the buffer
size_t nr_elements = ringBuff.Size();

// Producer's data source
int src_arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Writing data to the buffer
size_t size = 4;                        // Number of elements to add
bool result = ringBuff.TryPush(src_arr, size);

// Consumer's destination for retrieved data
int dest_arr[10] = { };

// Reading data from the buffer
size = 4;                               // Number of elements to retrieve
result = ringBuff.TryPop(dest_arr, size);
```

## Intended Use
The Ringbuffer serves as a standard ring buffer with the added advantage of being thread-safe for single-producer, single-consumer scenarios. It is particularly suitable for use with an Interrupt Service Routine (ISR) as the producer and the main application loop as the consumer.

In this setup, the producer uses `TryPush()` to insert data into the buffer, while the consumer utilizes `TryPop()` to retrieve it. Both operations are copy actions, meaning that data is copied into and out of the buffer. If either operation cannot be completed, `TryPush()` or `TryPop()` will return `false`. This situation may arise if the buffer is filling up faster than the consumer can process the data.

The `Size()` method provides a snapshot of the current number of elements in the buffer. However, due to the concurrent nature of operations, the buffer's state may change before the method completes.

Thread safety is maintained by ensuring that the write pointer does not overtake or equal the read pointer, while allowing them to be equal. If `TryPush()` uses an outdated read pointer, it indicates that the buffer is fuller than expected, potentially limiting the amount of data that can be inserted. Conversely, if `TryPop()` uses an outdated write pointer, it suggests that the buffer is emptier than expected, limiting the amount of data that can be removed.

### Caution
When using `TryPush()` or `TryPop()`, ensure to check the return values to handle cases where the operations may fail due to buffer constraints.
