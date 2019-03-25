/**
 * \file    SlimAssert.h
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \note    SlimAssert
 *
 * \brief   Slim replacement of assert macro for use in embedded environment.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/SlimAssert
 *
 * \details This code is intended to replace the 'assert' macro from the Standard
 *          Template Library, even the one from newlib. This to prevent two things:
 *          keeping the __PRETTY_FUNCTION__ (or demangled string method names in
 *          text memory, thereby reducing the application size (especially for C++),
 *          and to remove the unneeded printf() inside the macro which prints to stderr,
 *          which in the embedded environment will not (typically) be used.
 *          It is disabled when NDEBUG is defined. When debugging, the callstack is
 *          preserved.
 *
 *          Example:
 *          // Include the header (instead of <cassert> or <assert.h>)
 *          #include "utility/SlimAssert.h"
 *
 *          // Use the upper-case assert where needed
 *          ASSERT(3 > 5);      // Obviously this trips the assert
 *
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://barrgroup.com/Embedded-Systems/How-To/Define-Assert-Macro
 *          http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert
 *          https://stackoverflow.com/questions/5252375/custom-c-assert-macro
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
 * \param   file        The file (name) in which the assert tripped.
 * \param   line        The line on which the assert tripped.
 */
inline void _assert(const char* expression, const char* file, int line)
{
    abort();
}


/************************************************************************/
/* Macros                                                               */
/************************************************************************/
#ifdef NDEBUG
#define ASSERT( expr ) ( (void)0 )
#else
#define ASSERT( expr ) ( (expr) ? (void)0 : _assert(#expr, __FILE__, __LINE__) )
#endif


#ifdef __cplusplus
}
#endif


#endif  // SLIM_ASSERT_H_
