/**
 * \file    heap_check.h
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
