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
 * \details QuickSort is a divide-and-conquer algorithm that sorts an array by
 *          partitioning it into smaller sub-arrays. The average time complexity
 *          is O(n log n), while the worst-case time complexity is O(n^2) when
 *          the smallest or largest element is always  chosen as the pivot.
 *          The space complexity is O(log n) due to the recursive stack space.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/QuickSort
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    03-2025
 */

#ifndef QUICKSORT_HPP_
#define QUICKSORT_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>

/******************************************************************************
 * Template methods                                                           *
 *****************************************************************************/
/**
 * \brief   Partitions the array around a pivot.
 * \param   arr     The array to partition.
 * \param   start   The starting index for partitioning.
 * \param   end     The ending index for partitioning.
 * \returns The index of the pivot after partitioning.
 */
template <typename T>
int32_t Partition(T arr[], int32_t start, int32_t end)
{
    // The pivot element is taken to be the element at the start of the subrange to be partitioned.
    T pivotValue = arr[start];
    int32_t pivotPosition = start;

    // Rearrange the rest of the array elements to partition the subrange from start to end.
    for (int32_t pos = start + 1; pos <= end; ++pos)
    {
        if (arr[pos] < pivotValue)
        {
            // arr[pos] is the "current" item.
            // Swap the current item with the item to the right of the pivot element.
            T temp = arr[++pivotPosition];
            arr[pivotPosition] = arr[pos];
            arr[pos] = temp;
        }
    }

    // Place pivot in the correct position
    T temp = arr[start];
    arr[start] = arr[pivotPosition];
    arr[pivotPosition] = temp;

    return pivotPosition;
}

/**
 * \brief   Sorts the array using the QuickSort algorithm.
 * \param   arr     The array to sort.
 * \param   start   The starting index for sorting.
 * \param   end     The ending index for sorting.
 * \returns True if successful, false if the range is invalid.
 */
template <typename T>
bool QuickSort(T arr[], int32_t start, int32_t end)
{
    if (start >= end)
    {
        return false; // Invalid range
    }

    while (start < end)
    {
        // Partition the array and get the pivot point.
        int32_t pivotIndex = Partition(arr, start, end);

        // If left part is smaller, then recurse for the left part and handle right part iteratively.
        if (pivotIndex - start < end - pivotIndex)
        {
            QuickSort(arr, start, pivotIndex - 1);
            start = pivotIndex + 1; // Handle right part iteratively
        }
        else
        {
            QuickSort(arr, pivotIndex + 1, end);
            end = pivotIndex - 1; // Handle left part iteratively
        }
    }

    return true;
}

#endif  // QUICKSORT_HPP_
