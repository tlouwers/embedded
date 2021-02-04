/**
 * \file float_to_string.c
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

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "float_to_string.h"

/************************************************************************/
/* Defines                                                              */
/************************************************************************/
/**
 * \def     MAX_PRECISION
 * \brief   The maximum precision supported.
 */
#define MAX_PRECISION (10)


/************************************************************************/
/* Static variables                                                     */
/************************************************************************/
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};


/************************************************************************/
/* Public functions                                                     */
/************************************************************************/
/**
 * \brief   Convert a floating point number to a string, with given precision.
 * \param   f           The number to convert.
 * \param   buf         The string buffer to put the result into.
 * \param   precision   The amount of digits behind the dot.
 */
char* ftoa(double f, char* buf, int precision)
{
	// check precision bounds, clip to maximum
	if (precision > MAX_PRECISION)
    {
		precision = MAX_PRECISION;
    }

    char* ptr = buf;

	// handle the sign of the float
	if (f < 0)
    {
		f = -f;
		*ptr++ = '-';
	}

    // negative precision -> perform automatic precision guess
	if (precision < 0)
	{
		     if (f <      1.0) { precision = 6; }
		else if (f <     10.0) { precision = 5; }
		else if (f <    100.0) { precision = 4; }
		else if (f <   1000.0) { precision = 3; }
		else if (f <  10000.0) { precision = 2; }
		else if (f < 100000.0) { precision = 1; }
		else                   { precision = 0; }
	}

	// round value according the precision
	if (precision)
    {
		f += rounders[precision];
    }

	// integer part...
	long intPart = f;
	f -= intPart;

	if (!intPart)
    {
		*ptr++ = '0';
    }
	else
	{
		// save start pointer
		char* p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		char* p1 = p;

		// reverse result
		while (p > ptr)
		{
			char c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			char c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// add terminating zero
	*ptr = 0;

	return buf;
}
