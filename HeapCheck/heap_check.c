/**
 * \file    heap_check.c
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \brief   Heap check functions for Atmel Cortex-M4.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/HeapCheck
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    07-2018
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "heap_check.h"
#include <unistd.h>


/************************************************************************/
/* Externals                                                            */
/************************************************************************/
/**
 * \brief   Notify we use _estack provided by linker. It points to the first
 *          address after the top of the stack and marks the start of the heap.
 */
extern uint32_t _estack;

/**
 * \brief   External declaration for _sbrk, in newlib_stubs.c.
 * \param   incr    Size to increment with.
 * \return  Pointer to current used heap memory.
 */
extern caddr_t _sbrk(int incr);


/************************************************************************/
/* Static variables                                                     */
/************************************************************************/
/**
 * \brief   A 'magic' marker indicating the end of the heap. Must be equal
 *          to the one used in _sbrk.
 */
const uint32_t HEAP_END_MARKER = 0xFAFBFCFD;


/************************************************************************/
/* Public functions                                                     */
/************************************************************************/
/**
 * \brief   Get the used heap memory size.
 * \return  Used heap size in bytes.
 */
uint32_t get_used_heap(void)
{
    char* heap_end   = (char*)_sbrk(0);
    char* heap_start = (char*)&_estack;

    return (heap_end - heap_start);
}

/**
 * \brief   Get a pointer to the start of the heap.
 * \return  Start of the heap as address.
 */
uint32_t* get_start_of_heap(void)
{
    return (uint32_t*)&_estack;
}

/**
 * \brief   Check if the heap has been overwritten by the stack.
 * \note    It could be we never reach here as overwriting the heap could
 *          cause HardFaults as well.
 * \returns True if the heap is overwritten by the stack, else false.
 */
bool end_of_heap_overrun(void)
{
    char* heap_end = (char*)_sbrk(0);

    if (heap_end == (char*)-1)
    {
        return true;
    }

    // Note: the casting is to prevent compiler warnings.
    return ( *((uint32_t*)((void*)heap_end)) != HEAP_END_MARKER );
}
