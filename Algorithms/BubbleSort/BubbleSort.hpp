/**
 * \file    BubbleSort.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   BubbleSort
 *
 * \brief   Implementation of the BubbleSort algorithm with template functions.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/BubbleSort
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    08-2018
 */

#ifndef BUBBLESORT_HPP_
#define BUBBLESORT_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>
#include <algorithm>


/******************************************************************************
 * Template methods                                                           *
 *****************************************************************************/
/**
 * \brief   BubbleSort algorithm, sorts the given arr from start to end.
 * \details This is an optimized version where the inner loop is stopped when
 *          no elements are swapped anymore.
 * \param   arr     The array to sort.
 * \param   length  The length of the array to sort.
 * \returns True if successful, false if invalid length provided.
 */
template <typename T>
bool BubbleSort(T arr[], size_t length)
{
    if (length == 0) { return true; }   // Already sorted.

    if (length > 1)
    {
        bool swapped;

        for (size_t i = 0; i < (length - 1); i++)
        {
            swapped = false;
            for (size_t j = 0; j < (length - i - 1); j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }

            // If no two elements were swapped by inner loop, then break.
            if (swapped == false)
            {
                break;
            }
        }

        return true;
    }
    return false;   // Invalid range provided.
}


#endif  // BUBBLESORT_HPP_
