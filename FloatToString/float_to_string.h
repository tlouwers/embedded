/**
 * \file    float_to_string.h
 *
 * \licence Copyright (c) 2011-2020 Anton B. Gusev aka AHTOXA
 *
 *          Permission is hereby granted, free of charge, to any person obtaining a copy
 *          of this software and associated documentation files (the "Software"), to deal
 *          in the Software without restriction, including without limitation the rights
 *          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *          copies of the Software, and to permit persons to whom the Software is
 *          furnished to do so, subject to the following conditions:
 *
 *          The above copyright notice and this permission notice shall be included in
 *          all copies or substantial portions of the Software.
 *
 *          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *          IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *          FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *          AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *          LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *          OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *          THE SOFTWARE.
 * 
 * \brief   Convert a float or double integral type to string.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/FloatToString
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
