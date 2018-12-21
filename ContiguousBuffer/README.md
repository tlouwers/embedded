



# ContiguousRingbuffer
A thread-safe, lock-free, single producer, single consumer, contiguous ringbuffer.

## Introduction
This code is intended to be used to feed DMA handling of a Cortex-M4 microcontroller, but can see its share of use in other scenarios as well. Be sure to read up on the documentation, as it has some other-than-usual behavior.

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
There is no std::copy() in the buffer, meaning it is up to the user to make a copy of the data, the buffer will only manage the data here. This allows for DMA to fill the memory, or use a regular std::copy() or other construct to fill the buffer.

## Consideration
This buffer may not be as efficient in filling the full buffer since it works in blocks. The smaller the blocks the more efficiently the memory can be filled, but the larger the blocks the more effectively the buffer can be used (with DMA for instance). This is a trade-off and should be considered before choosing this buffer.

## Careful
One can only prevent so much, once the user get access to the data (the pointer), it is up to the user to not write/read beyond the boundaries given by the size.

## Explored options

 - Do not change the 'size' parameter in 'Poke()'. This is hard to do since it requires another method like 'ContiguousAvailable()', which will indicate the largest size of a contiguous block of elements available. Since there can be 1 or 2 blocks available: the 2 blocks where either the first or the last can be the biggest - it depends of the size of the block requested which is taken. Note that the 'dest' will point to the block in which 'size' will fit, which may be the first or second block. Decided not to implement this due to added complexity for the end user and a larger implementation.
 - Having a method called 'ContiguousSize()', which indicates the first contiguous block of elements available - if any. This information is already provided with 'Peek()', which then can be used immediate. Decided not to implement this due to added complexity for the end user and a larger implementation. Will be slower too as yet another call to 'Peek()' has to be made after calling 'ContiguousSize()'.

## More examples
Loop to move data from buffer to peripheral component, like Bluetooth or UART (also buffers).  The latter buffers may not be emptied quickly due to data being transmitted.
```cpp
// Assuming these buffers
ContiguousRingbuffer<uint8_t> buffRx;	// Used to hold received data from peripheral
ContiguousRingbuffer<uint8_t> buffRx;	// Used to hold data to send to peripheral

// <initialization, etc omitted for brevity>

// Method to get data from peripheral into buffer.
// Should be called many times from a main loop.
void ProcessRx(void)
{
	bool result = false;

	// Check if there is room in the buffer. Received now indicates the largest contiguous block available,
    // not necessarily the first block - this means some space may remain unused.
	uint8_t ptrDest = nullptr;
	size_t received = 1;
	if (buffRx.Poke(ptrDest, received)
	{
		// The Rx() is entered with a pointer where to store the data retrieved from Bluetooth, requesting
		// 'received' number of elements. Rx() will do the std::copy() to 'ptrDest'. Rx() will return with
		// the actual number of elements retrieved, maybe 0.
		received = mBluetooth.Rx(ptrDest, received);
	
		if (received > 0)
		{
			// 'commit' the written bytes to 'buffRx'.
			result = buffRx.Write(received);
			assert(result);
		}
	}

	// <do something with the read bytes>
}

// Method to get data from buffer into peripheral.
// Should be called many times from a main loop.
void ProcessTx(void)
{
	bool result = false;

	// Check if there is data in the buffer. Available now indicates the first contiguous block available,
	// up to the wrapping point.
	uint8_t ptrSrc = nullptr;
	size_t available = 1;
	if (buffRx.Peek(ptrSrc , available)
	{
		// The Tx() is entered with a pointer to the data to send via Bluetooth, along with the number of
		// available bytes. Tx() will do the std::copy() from 'ptrSrc' to Bluetooth internal buffer.
		// Tx() will return with the actual number of elements consumed, maybe 0.
		available = mBluetooth.Tx(ptrSrc, available);
	
		if (available> 0)
		{
			// 'release' the read bytes to 'buffTx'.
			result = buffTx.Read(available);
			assert(result);
		}
	}
}
```
