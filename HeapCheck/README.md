# HeapCheck
Low level functions to determine heap usage during run time.

## Introduction
This code is intended to be used on an Atmel Cortex-M4, but should be easily portable to other platforms.

## Requirements
 - Atmel Studio, a recent version which supports C++11
 - GCC compiler (untested on other compilers)
 - Atmel SAM4E/S microcontroller (untested on other microcontrollers)

## Check
Be sure you know where the heap is located. This is presented in the linker file, in our (tested) case it was in a file called `flash.ld`.

## Change
To allow stack overflow detection (stack growing/running over allocated heap memory) a flag is added in the function `_sbrk()` which allocated the heap memory. Every time memory is allocated it flags the end of the claimed heap memory with a flag. Later on, when checking regularly, if the flag gets overwritten by the stack we can determine if a stack overflow occurred. It is an indication only, as there is a chance the stack has exactly the same value as the flag (very unlikely), or that the device enters a HardFault before we can check the condition.

## Note
The code is written in "C", not "C++" - as it performs a few tricks which may not work on every board. Also note that these methods are a rough indication - your mileage may vary.

# Example
To get an idea of the heap (dynamic memory currently in use), another trick is used: a low level method called `_sbrk()` is called with size 0. Usually this function is used by `malloc()` to request memory from the heap, but when we request nothing (size 0) we get the current heap address. Together with the start of the heap (address) we can determine how much heap memory is used. If the amount of memory increases over time (constantly), this indicates there is either a memory leak or memory fragmentation. Assuming the programmer handles the heap correctly (request and release memory) there should be no memory leaks, only fragmentation.

```cpp
// Include the header file
#include "heap_check.h"

// At a later point check where the block of memory can be allocated:
static volatile uint32_t used_heap = 0;			<-- global to store the (growing) heap value
void Application::GetUsedHeap()
{
    uint32_t tmp = get_used_heap();
    if (tmp > used_heap)
    {
        used_heap = tmp;
    }
}

// To check for stack overflow we can call a dedicated function (should be done regularly):
void Application::CheckForStackOverflow()
{
	if (end_of_heap_overrun())
	{
		// Log, or take action ...
	}
}
```
