/**
 * \file    stack_painting.h
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

#ifndef STACK_PAINTING_H_
#define STACK_PAINTING_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdint.h>


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
void paint_stack(void);

uint32_t get_total_stack(void);
uint32_t get_used_stack(void);


#ifdef __cplusplus
}
#endif


#endif  // STACK_PAINTING_H_
