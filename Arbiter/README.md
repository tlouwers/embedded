# Arbiter
Arbiter class for I2C (master) implementation.

## Introduction
This code is intended to be used as Arbiter on an I2C (master) of a Cortex-M4 microcontroller. The concept is practically identical for SPI (and probably UART).

## Requirements
- C++11

## Contents
| Folder | Contents |
| ------ | -------- |
| doxygen | Doxygen configuration file. |
| src | Source code for the Arbiter showcase project (CodeBlocks 17.12). |

## Notes
Although care has been taken to make the code work like a charm, use it at your own risk.

## Intention
The code tries to mimic an embedded application, intended for a Cortex-M4. The `Application_Stub` is the upper layer, used to send 10 I2C requests over the bus (as master). The `I2CArbiter` will manage the requests and make sure no collisions occur on the bus. The `I2C` class stubs the I2C low level driver - although it has an identical interface to the Arbiter. This is intentional to allow the Arbiter to be added as layer between the Application and I2C driver. The I2C driver also stubs the DMA asynchronous behaviour by using a thread to wait a little while before calling the callback.

The Arbiter works by queueing the requests to the I2C driver and making sure they happen one after the other. The callbacks of the requests are rerouted to make sure the Arbiter can manage the requests.

## Example
The example project should be a clear enough showcase of how to use the Arbiter.

## Careful
One can only prevent so much, although this has been used in real products, there is no guarantee.