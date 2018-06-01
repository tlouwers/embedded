
# ContiguousRingbuffer
A thread-safe, lock-free, single producer, single consumer, contiguous ringbuffer.

## Introduction
This code is intended to be used to feed DMA handling of a Cortex-M4 microcontroller, but can see its share of use in other scenarios as well. Be sure to read up on the documentation, as it has some other-than-usual behavior.

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
ContiguousRingbuffer<int> ringBuff;

// Reserve the size to hold the elements, 'int' in this example
ringBuff.Resize(5);

// Check if there is room, then write 1 element
int* data = nullptr;
size_t size = 1;
int val = 42;
if (ringBuff.Poke(data, size))  // 'size' changes to the space available
{
    data[0] = val;
    ringBuff.Write(1);          // Administer the data is written, 1 element
}

// Check if there is at least 1 element in buffer, then read it
size_t size = 1;
if (ringBuff.Peek(data, size))  // 'size' changes to the elements available
{
    val = data[0];
    ringBuff.Read(1);           // Administer the data is read, 1 element
}
```

## Intended use
Assuming an Interrupt Service Routine (ISR) as thread 'Producer' and  the main application loop as thread 'Consumer'. Producer will use Poke() to request a contiguous block of elements, this can be passed on to the DMA (to fill the data). The CPU can perform another task. When DMA finishes it flags the data as written by calling Write() with the correct size. Consumer is expected to check if data is available in the buffer by calling Peek() - either with the size it can manage at the time or 1 to get an indication of the largest contiguous block available. When the is read/processed the memory is released to the buffer (for reuse) with a call to Read() with the correct size.

Thread safety is guaranteed by preventing the write pointer to overtake or become equal to the read pointer, preventing the read to overtake the write pointer (but they can become equal). If Poke() / Write() use an old value of the read pointer this would mean the buffer is 'more full' (or entirely full) at the time, allowing less data to be inserted.
If Peek() / Read() use an old value of the write pointer this would mean the buffer is 'more empty' (or completely empty) at the time, allowing less data to be removed.
The race condition on the wrap pointer is prevented partly by not allowing Write() and Read() to overtake each other. In addition, Write() will be the first to pass the wrapping point, Read() at this point will not use the wrap pointer. When Read() passes the wrapping point Write() will not use the wrap pointer. In this case it is irrelevant whether Write() or Read() use an 'old' or 'new' state, at the time they need the wrap pointer the other is guaranteed not to alter it.

## Note
There is no memcpy() in the buffer, meaning it is up to the user to make a copy of the data, the buffer will only manage the data here. This allows for DMA to fill the memory, or use a regular memcpy() or other construct to fill the buffer.

## Careful
One can only prevent so much, once the user get access to the data (the pointer), it is up to the user to not write/read beyond the boundaries given by the size.
