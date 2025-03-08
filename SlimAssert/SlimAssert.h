/**
 * \file    SlimAssert.h
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 *
 * \brief   Slim replacement of assert macro for use in embedded environment.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/SlimAssert
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    03-2019
 */

#ifndef SLIM_ASSERT_H_
#define SLIM_ASSERT_H_


#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdlib.h>


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
/**
 * \brief   Handler for the assert.
 * \param   expression  The expression which failed as character string.
 * \param   line        The line on which the assert tripped.
 * \param   file        The file (name) in which the assert tripped.
  */
inline void _assert(const char* expression, int line, const char* file)
{
    abort();
}


/************************************************************************/
/* Macros                                                               */
/************************************************************************/
#ifdef NDEBUG
#define ASSERT( expr ) ( (void)(expr) )
#else
#define ASSERT( expr ) ( (expr) ? (void)(expr) : _assert(#expr, __LINE__, __FILE__) )
#endif


#ifdef __cplusplus
}
#endif


#endif  // SLIM_ASSERT_H_
