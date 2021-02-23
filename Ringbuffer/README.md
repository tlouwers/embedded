
# Ringbuffer
A thread-safe, lock-free, single producer, single consumer, ringbuffer.

## Introduction
This code is intended to be used as single-producer, single-consumer ringbuffer on a Cortex-M4 microcontroller, but can see its share of use in other scenarios as well. Be sure to read up on the documentation, as it has some other-than-usual behavior.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CodeBlocks 17.12 project, along with tests written with the Catch2 test framework. |

## Notes
Inspiration from: <https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular>, <https://www.boost.org/doc/libs/1_54_0/doc/html/boost/lockfree/spsc_queue.html> and <http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++>
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Example
```cpp
// Producer will fill the buffer, Consumer will empty the buffer.
// Note: do check for the result values, the example omits them for clarity.

// Declare the buffer
Ringbuffer<int> ringBuff;

// Reserve the size to hold the elements, 'int' in this example
ringBuff.Resize(5);

// If needed, check the number of elements in the buffer
size_t nr_elements = ringBuff.Size();

// Assuming the Producer has 'source', data to put in buffer
int src_arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Writing data to the buffer
size_t size = 4;                        // The amount of elements to add to buffer
bool result = TryPush(src_arr, size);

// Assuming the Consumer has a 'destination' to place retrieved data
int dest_arr[10] = { };

// Reading data from the buffer
size_t size = 4;                        // The amount of elements to retrieve from buffer
bool result = TryPop(dest_arr, size);
```

## Intended use
The Ringbuffer can be used a regular ringbuffer, with the addition that for a single consumer, single producer it is thread safe.  Assuming an Interrupt Service Routine (ISR) as thread 'Producer' and the main application loop as thread 'Consumer'. The Producer will push items to the buffer, where the consumer will read them. Putting items in the buffer is a copy action, so is retrieving them. If data cannot be put into buffer or retrieved from buffer the TryPush() and TryPop() will return with false. This can happen when the buffer is filling up and the Consumer does not read data from buffer fast enough. Although a Size() method is provided it is a snapshot or indication, due to the threaded nature the buffer contents can have changed before the method returns.

Thread safety is guaranteed by preventing the write pointer to overtake or become equal to the read pointer, preventing the read to overtake the write pointer (but they can become equal). If TryPush() uses an old value of the read pointer this would mean the buffer is 'more full' (or entirely full) at the time, allowing less data to be inserted. If TryPop() uses an old value of the write pointer this would mean the buffer is 'more empty' (or completely empty) at the time, allowing less data to be removed.

## Note
If TryPush() or TryPop() succeed, the data is copied using std::copy(). Behavior of objects with custom types has not been tested (yet).
