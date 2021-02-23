/**
 * \file    stack_painting.c
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 *
 * \brief   Stack painting functions for Atmel Cortex-M4.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/StackPainting
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    05-2018
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "stack_painting.h"
#include <interrupt.h>


/************************************************************************/
/* Externals                                                            */
/************************************************************************/
/**
 * \brief   Use bottom of stack as specified in the linker control script.
 */
extern uint32_t _sstack;

/**
 * \brief   Use top of stack as specified in the linker control script.
 */
extern uint32_t _estack;


/************************************************************************/
/* Constants                                                            */
/************************************************************************/
/**
 * \brief   A 'magic' number to 'paint' the stack. Although theoretically
 *          possible a stack value is the same as the number, it is
 *          very unlikely and not repeated for long.
 */
const uint32_t PAINT_VALUE = 0xC5C5C5C5;


/************************************************************************/
/* Static variables                                                     */
/************************************************************************/
static uint32_t total_stack_size = 0;
static uint32_t used_stack_size  = 0;


/************************************************************************/
/* Public functions                                                     */
/************************************************************************/
/**
 * \brief   Fills all of the stack with a defined value (PAINT_VALUE).
 * \note    Should be done as one of the first things in main().
 */
void paint_stack(void)
{
    // Find the top and bottom of the stack as specified in the linker control script
    uint32_t* bottom_of_stack = &_sstack;

    // Get the space occupied by the application: stack_pointer since start of SRAM
    uint32_t application = __get_MSP();

    // Find out what needs to be 'painted' - in sizeof(uint32_t)
    uint32_t area_to_paint = (application - (uint32_t)bottom_of_stack) / 4;

    // Paint that area
    for (uint32_t i = 0; i < area_to_paint; i++)
    {
        *bottom_of_stack++ = PAINT_VALUE;
    }
}

/**
 * \brief   Get the total amount of stack available.
 * \return  Total stack size in bytes.
 */
uint32_t get_total_stack(void)
{
    // Could be we never called 'get_used_stack' before.
    if (total_stack_size == 0)
    {
        get_used_stack();
    }

    return total_stack_size;
}

/**
 * \brief   Get the (once) used stack size.
 * \return  Used stack size in bytes.
 */
uint32_t get_used_stack(void)
{
    // Prevent interrupts during this section
    irqflags_t irq_state = cpu_irq_save();

    // Find the top and bottom of the stack as specified in the linker control script
    uint32_t* bottom_of_stack = &_sstack;
    uint32_t* top_of_stack    = &_estack;

    // Find out what needs to be searched - in sizeof(uint32_t)
    uint32_t area_to_search = (top_of_stack - bottom_of_stack);

    uint32_t i;
    for (i = 0; i < area_to_search; i++)
    {
        if (*bottom_of_stack++ != PAINT_VALUE)
        {
            break;
        }
    }

    // Restore interrupts
    cpu_irq_restore(irq_state);

    total_stack_size = (area_to_search * 4);            // * 4: uint32_t to byte
    used_stack_size  = (total_stack_size) - (i * 4);

    return used_stack_size;
}
