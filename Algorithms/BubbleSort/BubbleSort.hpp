/**
 * \file    BubbleSort.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \note    BubbleSort
 *
 * \brief   Implementation of the BubbleSort algorithm with template functions.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/BubbleSort
 *
 * \details This code is intended as non STL version of the BubbleSort algorithm,
 *          making it better suited for embedded use.
 *
 *          Use for small sets.
 *
 *          Worst case:      O(n^2)
 *          Best case:       O(n)
 *          Average case:    O(n^2)
 *          Auxiliary space: O(1)
 *
 *          Example:
 *          // Define an array to sort:
 *          int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };
 *
 *          // Sort the array:
 *          BubbleSort(array, sizeof(array));
 *
 *			// At this point 'array' is sorted.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://www.geeksforgeeks.org/bubble-sort/
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
#include <cstdint>      // int32_t
#include <algorithm>    // swap()


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
