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
 *          This implementation uses tail-call optimization by recursing on the
 *          smaller partition and iterating on the larger partition, reducing
 *          stack usage for embedded systems.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/QuickSort
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    01-2026
 */

#ifndef QUICKSORT_HPP_
#define QUICKSORT_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>
#include <utility>
#include <type_traits>

/******************************************************************************
 * Internal helper functions                                                  *
 *****************************************************************************/
/** @brief Swaps two elements of any type, avoid pulling in <algorithm> for
 *         just std::swap.
 * \param a First element.
 * \param b Second element.
 */
template <typename T>
inline void Swap(T& a, T& b) noexcept
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

/******************************************************************************
 * Template methods                                                           *
 *****************************************************************************/
/**
 * \brief   Partitions the array around a pivot (Lomuto-style with pivot at start).
 * \param   arr     The array to partition.
 * \param   start   The starting index for partitioning (inclusive).
 * \param   end     The ending index for partitioning (inclusive).
 * \returns The index of the pivot after partitioning.
 */
template <typename T>
int32_t Partition(T arr[], int32_t start, int32_t end)
{
    // The pivot element is taken to be the element at the start of the subrange to be partitioned.
    T pivotValue = arr[start];
    int32_t pivotPosition = start; // final pivot position

    // Rearrange the rest of the array elements to partition the subrange from start to end.
    for (int32_t pos = start + 1; pos <= end; ++pos)
    {
        if (arr[pos] < pivotValue)
        {
            // Move smaller item next to the pivot region.
            ++pivotPosition;
            Swap(arr[pivotPosition], arr[pos]);
        }
    }

    // Place pivot in the correct position
    Swap(arr[start], arr[pivotPosition]);

    return pivotPosition;
}

/**
 * \brief   Sorts the array using the QuickSort algorithm.
 * \param   arr     The array to sort.
 * \param   start   The starting index for sorting (inclusive).
 * \param   end     The ending index for sorting (inclusive).
 * \note    This function is in-place and returns immediately for empty or
 *          single-element ranges.
 */
template <typename T>
void QuickSort(T arr[], int32_t start, int32_t end)
{
    if (start >= end)
    {
        return; // nothing to do for empty or single-element ranges
    }

    while (start < end)
    {
        // Partition the array and get the pivot point.
        int32_t pivotIndex = Partition(arr, start, end);

        // Recurse on the smaller partition to keep stack depth O(log n) on average.
        if (pivotIndex - start < end - pivotIndex)
        {
            QuickSort(arr, start, pivotIndex - 1);
            start = pivotIndex + 1; // tail-iterate on the larger partition
        }
        else
        {
            QuickSort(arr, pivotIndex + 1, end);
            end = pivotIndex - 1; // tail-iterate on the larger partition
        }
    }
}

#endif  // QUICKSORT_HPP_
