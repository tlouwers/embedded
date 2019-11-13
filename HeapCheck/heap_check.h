/**
 * \file heap_check.h
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 *
 * \brief   Heap check functions for Atmel Cortex-M4.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/HeapCheck
 *
 * \details This code is intended to be used to determine the heap usage at
 *          runtime. The code is implemented in 'C', to be usable in both 'C'
 *          and 'C++' projects.
 *
 *          As example:
 *          // Include the header file
 *          #include "heap_check.h"
 *
 *          // At a later point check where the block of memory can be allocated:
 *          static volatile uint32_t used_heap = 0;         <-- global to store the (growing) heap value
 *          void Application::GetUsedHeap()
 *          {
 *              uint32_t tmp = get_used_heap();
 *              if (tmp > used_heap)
 *              {
 *                  used_heap = tmp;
 *              }
 *          }
 *
 *          // To check for stack overflow we can call a dedicated function (should be done regularly):
 *          void Application::CheckForStackOverflow()
 *          {
 *              if (end_of_heap_overrun())
 *              {
 *                  // Log, or take action ...
 *              }
 *          }
 *
 * \note    To use the 'end_of_heap_overrun()', a modification in the
 *          function '_sbrk()' needs to be made. For Atmel this is in the
 *          file 'syscalls.c', around line 65. A flag needs to be added to
 *          mark the end of the heap.
 *
 *          // Modified '_sbrk()' function:
 *
 *          extern caddr_t _sbrk(int incr)
 *          {
 *              static unsigned char *heap = NULL;
 *              unsigned char *prev_heap;
 *              int ramend = (int)\&__ram_end__;            // The '\' is needed for doxygen only!
 *
 *              if (heap == NULL)
 *              {
 *                  heap = (unsigned char *)\&_end;         // The '\' is needed for doxygen only!
 *              }
 *              prev_heap = heap;
 *
 *              if (((int)prev_heap + incr) > ramend)
 *              {
 *                  // Heap and stack collision
 *                  return (caddr_t) -1;
 *              }
 *
 *              heap += incr;
 *              *((uint32_t*)((void*)heap)) = 0xFAFBFCFD;      // Mark end of heap to detect stack overflow
 *
 *              return (caddr_t) prev_heap;
 *          }
 *
 * \note    This code is not to be used 'as-is': be sure you know where the
 *          stack and heap are located in your project and modify the code to
 *          match these areas.
 *
 * \note    Inspiration from:
 *          https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction
 *          http://library.softwareverify.com/memory-fragmentation-your-worst-nightmare/
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    07-2018
 */

#ifndef HEAP_CHECK_H_
#define HEAP_CHECK_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdint.h>
#include <stdbool.h>


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
uint32_t get_used_heap(void);
uint32_t* get_start_of_heap(void);
bool end_of_heap_overrun(void);


#ifdef __cplusplus
}
#endif


#endif  // HEAP_CHECK_H_
