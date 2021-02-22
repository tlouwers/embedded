
# Arbiter
Arbiter class for I2C (master) implementation.

## Description
This code is intended to be used as Arbiter on an I2C (master) of a Cortex-M4 microcontroller. The concept is practically identical for SPI (and probably UART). It allows multiple concurrent users of the same bus.

As example:
In 'main.cpp' we call app.Test() 10x in quick succession, then await the handling of the callbacks. The Arbiter will make sure there are no collisions on the I2C bus by making sure each transactions is handled one after the other. The 'i2c_driver_stub.cpp' spawns a thread to mimic the hardware DMA handling of an I2C transaction by sleeping a little while, then calling the callback.
Note: do check for the result values, the example omits them for clarity.

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| doxygen | Doxygen configuration file. |
| src | Source code for the Arbiter showcase project (CodeBlocks 17.12). |

## Notes
If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

## Intention
The code tries to mimic an embedded application, intended for a Cortex-M4. The `Application_Stub` is the upper layer, used to send 10 I2C requests over the bus (as master). The `I2CArbiter` will manage the requests and make sure no collisions occur on the bus. The `I2C` class stubs the I2C low level driver - although it has an identical interface to the Arbiter. This is intentional to allow the Arbiter to be added as layer between the Application and I2C driver. The I2C driver also stubs the DMA asynchronous behaviour by using a thread to wait a little while before calling the callback.

The Arbiter works by queueing the requests to the I2C driver and making sure they happen one after the other. The callbacks of the requests are rerouted to make sure the Arbiter can manage the requests.

## Example
The example project should be a clear enough showcase of how to use the Arbiter.

## Careful
One can only prevent so much, although this has been used in real products, there is no guarantee.