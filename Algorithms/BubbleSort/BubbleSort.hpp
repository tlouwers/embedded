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
 * \details The BubbleSort algorithm has a worst-case and average-case time complexity
 *          of O(n^2), making it inefficient for large datasets. However, it performs
 *          well on small or nearly sorted datasets due to its early exit optimization
 *          when no swaps occur. The space complexity is O(1) since it sorts the array
 *          in place. This implementation is suitable for educational purposes and
 *          small-scale applications but is not recommended for production use with
 *          large datasets.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/BubbleSort
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    01-2026
 */

#ifndef BUBBLESORT_HPP_
#define BUBBLESORT_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>
#include <utility>

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
 * \brief Sorts an array using the optimized BubbleSort algorithm.
 *        Uses the "last swap" optimization to reduce the next pass length.
 * \param arr The array to sort.
 * \param length The length of the array.
 * \returns True if successful, false if the length is invalid.
 */
template <typename T>
bool BubbleSort(T arr[], size_t length)
{
    if (length < 2)
    {
        return length == 1; // Already sorted or invalid.
    }

    for (size_t i = 0; i < length - 1; ++i)
    {
        bool swapped = false;
        for (size_t j = 0; j < length - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                Swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
        {
            break; // No swaps means the array is sorted.
        }
    }
    return true; // Sorting completed successfully.
}

#endif  // BUBBLESORT_HPP_
