/**
 * \file    QuickSort.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \note    QuickSort
 *
 * \brief   Implementation of the QuickSort algorithm with template functions.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/QuickSort
 *
 * \details This code is intended as non STL version of the QuickSort algorithms,
 *          making it better suited for embedded use.
 *
 *          Example:
 *          // Define an array to sort:
 *          int array[10] = { 6, 3, 7, 8, 2, 10, 1, 4, 5, 9 };
 *
 *          // Sort the array:
 *          QuickSort(array, 0, (sizeof(array) - 1));
 *
 *			// At this point 'array' is sorted.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://www.geeksforgeeks.org/quick-sort/
 *          http://www.cplusplus.com/forum/beginner/119660/
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    08-2018
 */

#ifndef QUICKSORT_HPP_
#define QUICKSORT_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>      // int32_t
#include <algorithm>    // swap()


/******************************************************************************
 * Template methods                                                           *
 *****************************************************************************/
/**
 * \brief   Take the first element as pivot, rearrange the elements to have
 *          all values greater than or equal to the pivot element are on the
 *          right side of the pivot element and all values less are on the
 *          left side.
 * \param   arr     The array to partition/rearrange.
 * \param   start   The starting point (index) for sorting.
 * \param   end     The end point (index) for sorting.
 * \note    Intended for internal use by the QuickSort algorithm.
 * \returns The updated pivot point.
 */
template <typename T>
int32_t Partition(T arr[], int32_t start, int32_t end)
{
    // The pivot element is taken to be the element at the start of the subrange to be partitioned.
    T      pivotValue    = arr[start];
    int32_t pivotPosition = start;

    // Rearrange the rest of the array elements to partition the subrange from start to end.
    for (int32_t pos = start + 1; pos <= end; pos++)
    {
        if (arr[pos] < pivotValue)
        {
            // arr[pos] is the "current" item.
            // Swap the current item with the item to the right of the pivot element.
            std::swap(arr[pivotPosition + 1], arr[pos]);

            // Swap the current item with the pivot element.
            std::swap(arr[pivotPosition], arr[pivotPosition + 1]);

            // Adjust the pivot position so it stays with the pivot element.
            pivotPosition++;
        }
    }
    return pivotPosition;
}

/**
 * \brief   QuickSort algorithm, sorts the given arr from start to end.
 * \param   arr     The array to sort.
 * \param   start   The starting point (index) for sorting.
 * \param   end     The end point (index) for sorting.
 * \returns True if successful, false if the start is before the end point.
 */
template <typename T>
bool QuickSort(T arr[], int32_t start, int32_t end)
{
    if (start < end)
    {
        // Partition the array and get the pivot point.
        int32_t p = Partition(arr, start, end);

        // Sort the portion before the pivot point.
        QuickSort(arr, start, p - 1);

        // Sort the portion after the pivot point.
        QuickSort(arr, p + 1, end);

        return true;
    }
    return false;   // Invalid range provided.
}


#endif  // QUICKSORT_HPP_
