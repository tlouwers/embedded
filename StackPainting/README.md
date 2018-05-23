
# StackPainting
Low level functions to determine stack usage during run time.

## Introduction
This code is intended to be used on an Atmel Cortex-M4, but should be easily portable to other platforms.

## Requirements
 - Atmel Studio, a recent version which supports C++11
 - GCC compiler (untested on other compilers)
 - Atmel SAM4E/S microcontroller (untested on other microcontrollers)

## Check
Be sure you know where the stack is located. This is presented in the linker file, in our (tested) case it was in a file called `flash.ld`.

## Note
The code is written in "C", not C++ - as it performs a few tricks which may not work on every board. Also note that these methods are a rough indication - your mileage may vary.

# Example
To check the stack usage, a method known as "stack painting" is used: upon startup of the application the entire stack memory is set to a specific value (magic number). When the application has run for a few minutes (hours, whatever it takes to touch all functionality) the stack memory is read and checked: where the magic number is replaced a real method has been executing - thus the stack here is used. Since the assumption is that the stack use will 'grow' when more of the application is used, the maximum stack usage can thus be determined.

```cpp
// Include the header as 'C' file
extern "C"
{
	#include "stack_painting.h"
}

// In the main.cpp file, add the 'paint_stack()' function as close to the board startup as possible.
void main(void)
{
    paint_stack();		<-- here

    sysclk_init();
    board_init();

    // Remainder of the application	
    while(1)
    {
        // ...
    }
}

// At certain intervals log the used stack memory (not too often, every 10 seconds or so):
static volatile uint32_t used_stack = 0;			<-- global to store the (growing) stack value
void Application::GetStack()
{
    uint32_t tmp = get_used_stack();
    if (tmp > used_stack )
    {
        used_stack = tmp;
    }
}

// To get an idea of the total stack available:
void Application::GetTotalStack()
{
    uint32_t total_stack = get_total_stack();
}
```
