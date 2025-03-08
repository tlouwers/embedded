# embedded
Various C/C++ code fragments for use on an embedded platform, like a Cortex-M4.

## Contents
| Folder | Contents |
| ------ | -------- |
| 3rd-party | CMake file to download and build recent google test framework. |
| Algorithms / BubbleSort | Implementation of the BubbleSort algorithm with template functions. |
| Algorithms / MovingAverage | Implementation of a simple moving average. |
| Algorithms / QuickSort | Implementation of the QuickSort algorithm with template functions. |
| Arbiter | Example of an I2C Arbiter class to manage shared/asynchronous access to a bus (I2C, SPI, ...). |
| BitmaskEnum | Template to enable bitmask operations using a strongly typed enum classes. |
| CircularFifo | A thread-safe, lock-free, single producer, single consumer, ringbuffer. Works on per-element basis.  |
| ContiguousRingbuffer | A thread-safe, lock-free, single producer, single consumer, contiguous ringbuffer. |
| FloatToString | A float or double integral to string conversion function. |
| HeapCheck | Example code to determine heap usage on Cortex-M4. |
| Ringbuffer | A thread-safe, lock-free, single producer, single consumer, ringbuffer. |
| Sawtooth | Simple sawtooth generator class. |
| SlimAssert | Slim replacement of assert macro for use in embedded environment. |
| SoftTimer | Flexible software timer helper class. |
| StackPainting | Example code to determine stack usage on Cortex-M4. |
