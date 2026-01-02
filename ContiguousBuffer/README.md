# ContiguousRingbuffer
A thread-safe, lock-free, single producer, single consumer contiguous ring buffer.

## Description
This lock-free, wait-free contiguous ring buffer is designed for embedded use, particularly for DMA handling in Cortex-M4 microcontrollers. It functions similarly to a bip-buffer. Refer to the documentation for unique behaviors.

### Usage
In this setup, an Interrupt Service Routine (ISR) acts as the producer, while the main application loop serves as the consumer. The producer uses `Poke()` to request a contiguous block of elements for DMA to fill. Once the DMA completes, it calls `Write()` to indicate the data is ready. The consumer checks for available data using `Peek()`, either specifying a size or using 1 to find the largest contiguous block. After processing, it releases memory with `Read()`.

Thread safety is ensured by preventing the write pointer from overtaking the read pointer, allowing them to be equal but not reversed. If `Poke()`/`Write()` uses an outdated read pointer, it indicates the buffer is fuller than expected, limiting data insertion. Conversely, if `Peek()`/`Read()` uses an outdated write pointer, it suggests the buffer is emptier, limiting data removal. The wrap pointer's race condition is mitigated by ensuring `Write()` and `Read()` do not overtake each other.

### Efficiency Comparison
The ContiguousRingbuffer provides enhanced efficiency compared to traditional thread-safe buffers in FreeRTOS when operating under single producer and single consumer conditions. By eliminating the need for data copying, it allows direct access to data during DMA operations, reducing CPU cycles and memory usage. In contrast, thread-safe buffers typically require locking mechanisms to manage concurrent access, which can introduce latency and complexity. This makes the ContiguousRingbuffer particularly well-suited for real-time applications in embedded systems.

## Requirements
- C++11 or later

## Contents
| Folder | Contents |
| ------ | -------- |
| test | A CMake project with tests using the Google Test framework. |

## Notes
Inspired by: [PEEK and POKE](https://en.wikipedia.org/wiki/PEEK_and_POKE), [Lock-Free Single-Producer Single-Consumer Circular](https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular), and [The Bip-Buffer](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist). If you encounter issues or have improvements, please share.

## Example
```cpp
// Producer fills the buffer, Consumer empties it.
// Check result values in actual code; omitted here for clarity.

// Declare the buffer
ContiguousRingbuffer<int> ringBuff;

// Reserve size for elements
ringBuff.Resize(5);

// Check for room and write 1 element
int* data = nullptr;
size_t size = 1;
int val = 42;
if (ringBuff.Poke(data, size)) {
    data[0] = val;
    ringBuff.Write(1);
}

// Check for at least 1 element and read it
size = 1;
if (ringBuff.Peek(data, size)) {
    val = data[0];
    ringBuff.Read(1);
}
```

## Note
There is no `std::copy()` in the buffer; users must manage data copying. This allows for DMA filling or using `std::copy()` to populate the buffer.

## Consideration
This buffer may not efficiently fill to capacity since it operates in blocks. Smaller blocks allow for more efficient memory filling, while larger blocks enhance buffer usage (e.g., with DMA). This trade-off should be considered.

## Caution
Once users access the data pointer, they must avoid reading or writing beyond the specified boundaries.

## Explored Options
- Not changing the 'size' parameter in `Poke()`, which would require a new method like `ContiguousAvailable()`. This added complexity was deemed unnecessary for users.
- Implementing a `ContiguousSize()` method to indicate the first contiguous block available. This information is already provided by `Peek()`, making an additional method redundant.

## More Examples
Loop to transfer data between the buffer and a peripheral component, such as Bluetooth or UART:
```cpp
// Assuming these buffers
ContiguousRingbuffer<uint8_t> buffRx; // Holds received data
ContiguousRingbuffer<uint8_t> buffTx; // Holds data to send

// <initialization omitted>

// Method to receive data from a peripheral into the buffer.
void ProcessRx(void) {
    bool result = false;
    uint8_t* ptrDest = nullptr;
    size_t received = 1;

    // Check if there is room in the buffer. Received now indicates the largest contiguous block available,
    // not necessarily the first block - this means some space may remain unused.
    if (buffRx.Poke(ptrDest, received)) {
        // The Rx() is entered with a pointer where to store the data retrieved from Bluetooth, requesting
        // 'received' number of elements. Rx() will do the std::copy() to 'ptrDest'. Rx() will return with
        // the actual number of elements retrieved, maybe 0.
        received = mBluetooth.Rx(ptrDest, received);

        if (received > 0) {
            // 'commit' the written bytes to 'buffRx'.
            result = buffRx.Write(received);
            assert(result);
        }
    }

    // <do something with the read bytes>
}

// Method to send data from the buffer to a peripheral.
void ProcessTx(void) {
    bool result = false;
    uint8_t* ptrSrc = nullptr;
    size_t available = 1;

    // Check if there is data in the buffer. Available now indicates the first contiguous block available,
    // up to the wrapping point.
    if (buffTx.Peek(ptrSrc, available)) {
        // The Tx() is entered with a pointer to the data to send via Bluetooth, along with the number of
        // available bytes. Tx() will do the std::copy() from 'ptrSrc' to Bluetooth's internal buffer.
        // Tx() will return with the actual number of elements consumed, maybe 0.
        available = mBluetooth.Tx(ptrSrc, available);

        if (available > 0) {
            // 'release' the read bytes to 'buffTx'.
            result = buffTx.Read(available);
            assert(result);
        }
    }
}
```