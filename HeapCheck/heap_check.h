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
 * \details This code is intended to be used to determine the heap usage at
 *          runtime. The code is implemented in 'C', to be usable in both 'C'
 *          and 'C++' projects.
 *
 *          As example:
 *          // Include the header as 'C' file
 *          extern "C"
 *          {
 *              #include "heap_check.h"
 *          }
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
 * \note    This code is not to be used 'as-is': be sure you know where the
 *          stack and heap are located in your project and modify the code to
 *          match these areas.
 *
 * \note    Inspiration from:
 *          https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction
 *          http://library.softwareverify.com/memory-fragmentation-your-worst-nightmare/
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    05-2018
 */

#ifndef HEAP_CHECK_H_
#define HEAP_CHECK_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdint.h>     // uint32_t


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
uint32_t get_used_heap(void);


#ifdef __cplusplus
}
#endif


#endif  // HEAP_CHECK_H_
