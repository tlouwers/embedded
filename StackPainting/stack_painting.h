/**
 * \file stack_painting.h
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
 * \details This code is intended to be used to determine the stack usage at
 *          runtime. The code is implemented in 'C', to be usable in both 'C'
 *          and 'C++' projects.
 *
 *          As example:
 *          // Include the header file
 *          #include "stack_painting.h"
 *
 *          // Right after the start of main(), 'paint' the stack:
 *          void main(void)
 *          {
 *              paint_stack();          <-- here
 *
 *              // Clocks, pins, remainder ...
 *          }
 *
 *          // At certain intervals log the used stack memory (not too often, every 10 seconds or so):
 *          static volatile uint32_t used_stack = 0;			<-- global to store the (growing) stack value
 *          void Application::GetUsedStack()
 *          {
 *              uint32_t tmp = get_used_stack();
 *              if (tmp > used_stack )
 *              {
 *                  used_stack = tmp;
 *              }
 *          }
 *
 *          // To get an idea of the total stack available:
 *          void Application::GetTotalStack()
 *          {
 *              uint32_t total_stack = get_total_stack();
 *          }
 *
 * \note    This code is not to be used 'as-is': be sure you know where the
 *          stack and heap are located in your project and modify the code to
 *          match these areas.
 *
 * \note    Inspiration from:
 *          https://ucexperiment.wordpress.com/2015/01/02/arduino-stack-painting/
 *          https://embeddedgurus.com/stack-overflow/2009/03/computing-your-stack-size/
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
