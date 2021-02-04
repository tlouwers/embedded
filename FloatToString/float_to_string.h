/**
 * \file float_to_string.h
 *
 * \licence Copyright (c) 2011-2020 Anton B. Gusev aka AHTOXA
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 * 
 * \brief   Convert a float or double integral type to string.
 * 
 * \details This code is intended to be used when no default conversion exists.
 *          A typical use case would be to print a sensor value (float) to a
 *          log file.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the code please let me know
 *          so I can update it.
 * 
 * \note    Code was originally created by <b>Anton Gusev</b>, published here:
 *          https://github.com/antongus/stm32tpl
 * 
 *          I cleaned up the code a little and added some comments.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    02-2021
 */

#ifndef FLOAT_TO_STRING_H_
#define FLOAT_TO_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
char* ftoa(double f, char* buf, int precision);


#ifdef __cplusplus
}
#endif


#endif // FLOAT_TO_STRING_H_
