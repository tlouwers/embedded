/**
 * \file    QuickSort.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   QuickSort
 *
 * \brief   Implementation of the QuickSort algorithm with template functions.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/QuickSort
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
    T       pivotValue    = arr[start];
    int32_t pivotPosition = start;

    // Rearrange the rest of the array elements to partition the subrange from start to end.
    for (int32_t pos = (start + 1); pos <= end; pos++)
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
 * \details Implementation uses tail call elimination to reduce stack
 *          size and number of recursions needed.
 * \param   arr     The array to sort.
 * \param   start   The starting point (index) for sorting.
 * \param   end     The end point (index) for sorting.
 * \returns True if successful, false if the start is before the end point.
 */
template <typename T>
bool QuickSort(T arr[], int32_t start, int32_t end)
{
    if (start < end)    // Range check
    {
        while (start < end)
        {
            // Partition the array and get the pivot point.
            int32_t p = Partition(arr, start, end);

            // If left part is smaller, then recurse for the left part and handle right part iteratively.
            if ((p - start) < (end - p))
            {
                QuickSort(arr, start, (p - 1));
                start = (p + 1);
            }
            // Recurse for the right part
            else
            {
                QuickSort(arr, (p + 1), end);
                end = (p - 1);
            }
        }
        return true;
    }
    return false;   // Invalid range provided.
}


#endif  // QUICKSORT_HPP_
