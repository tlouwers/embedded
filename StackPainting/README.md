
# StackPainting
Low level functions to determine stack usage during run time.

## Description
To check the stack usage, a method known as "stack painting" is used: upon startup of the application the entire stack memory is set to a specific value (magic number). When the application has run for a few minutes (hours, whatever it takes to touch all functionality) the stack memory is read and checked: where the magic number is replaced a real method has been executing - thus the stack here is used. Since the assumption is that the stack use will 'grow' when more of the application is used, the maximum stack usage can thus be determined.

## Requirements
- Atmel Studio, a recent version which supports C++11
- GCC compiler (untested on other compilers)
- Atmel SAM4E/S microcontroller (untested on other microcontrollers)

## Check
This code is not to be used 'as-is': be sure you know where the stack and heap are located in your project and modify the code tomatch these areas. In my (tested) case it was in a file called `flash.ld`.
Inspiration from: <https://ucexperiment.wordpress.com/2015/01/02/arduino-stack-painting/> and <https://embeddedgurus.com/stack-overflow/2009/03/computing-your-stack-size/>

## Intended use
This code is intended to be used on an Atmel Cortex-M4, but should be easily portable to other platforms.
The code is implemented in 'C', to be usable in both 'C' and 'C++' projects.

# Example
```cpp
// Include the header file
#include "stack_painting.h"

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
