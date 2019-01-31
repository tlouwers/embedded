/**
 * \file    MovingAverage.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \note    MovingAverage
 *
 * \brief   Implementation of a simple moving average with template functions.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/MovingAverage
 *
 * \details This code is intended to smooth out battery samples on an embedded
 *          device, using a simple moving average. It only averages on items in
 *          the internal buffer, returns 0 if none are present. The Fill()
 *          method can be used to completely fill the internal buffer with a
 *          defined value.
 *          Since the intent is to use it on smaller ranges and embedded
 *          devices, the use of larger types 'T' is not allowed:
 *          double, int64_t and uint64_t are checked/blocked in Resize().
 *
 *          Example:
 *          // Declare the object and type to use:
 *          MovingAverage<int> movAvg;
 *
 *          // Use Resize() to define the length of the internal buffer:
 *          movAvg.Resize(6);
 *
 *          // Add values:
 *          movAvg.Add(2);
 *          movAvg.Add(4);
 *
 *          // Calculate and get the averaged sum:
 *          int result = movAvg.GetAverage();
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    01-2019
 */

#ifndef MOVING_AVERAGE_HPP_
#define MOVING_AVERAGE_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>      // uint16_t
#include <algorithm>    // std::fill()
#include <type_traits>  // std::is_same()


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<class T>
class MovingAverage
{
public:
    MovingAverage() noexcept;
    ~MovingAverage();

    bool Resize(uint16_t size) noexcept;
    bool Fill(T value);

    bool Add(T value);

    T GetAverage();

private:
    uint16_t mCapacity;
    uint16_t mIndex;
    uint16_t mItemsInBuffer;
    double   mSum;              // Use a known large type which supports fractional numbers
    T* mElements;

    void DeleteBuffer();
};


/**
 * \brief   Constructor.
 * \details Internal buffer needs to be set to a size with 'Resize()' before
 *          it can be used.
 */
template<class T>
MovingAverage<T>::MovingAverage() noexcept :
    mCapacity(0), mIndex(0), mItemsInBuffer(0), mSum(0), mElements(nullptr)
{
    ;
}

/**
 * \brief   Destructor.
 * \details Frees memory allocated to the buffer if needed.
 */
template<class T>
MovingAverage<T>::~MovingAverage()
{
    DeleteBuffer();
}

/**
 * \brief   Resizes the internal buffer to the given size by freeing and
 *          (re)allocating memory.
 * \details Frees memory allocated to the buffer if needed. Then tries to
 *          allocate the requested memory size.
 *          Resizing to previous size is allowed, this frees and allocates
 *          memory like other Resize().
 * \note    The size is limited using the uint16_t to a maximum of 65535
 *          numbers. This to prevent overflow when calculating a sum of the
 *          available items.
 * \param   size    Size of the memory to allocate.
 * \returns True if the requested size could be allocated, else false. False if
 *          the requested size equals 0.
 *          False if the type T is too large: double, int64_t, uint64_t.
 */
template<class T>
bool MovingAverage<T>::Resize(const uint16_t size) noexcept
{
    // Disallow the use of larger types
    if (std::is_same<double,   T>::value) { return false; }
    if (std::is_same<int64_t,  T>::value) { return false; }
    if (std::is_same<uint64_t, T>::value) { return false; }

    DeleteBuffer();

    if (size > 0)
    {
        mIndex         = 0;
        mItemsInBuffer = 0;
        mCapacity      = size;
        mSum           = 0;

        mElements = new(std::nothrow) T[size];

        if (mElements != nullptr)
        {
            // Fill the entire internal buffer with '0'
            std::fill(mElements, (mElements + mCapacity), 0);

            return true;
        }
    }

    return false;
}

/**
 * \brief   Fills the entire internal buffer with the given value. Older
 *          values are overwritten.
 * \param   value   The value to fill the internal buffer with.
 * \returns True if the internal buffer could be filled, else false.
 */
template<class T>
bool MovingAverage<T>::Fill(T value)
{
    if (mElements != nullptr)
    {
        // Fill the entire internal buffer with 'value'
        std::fill(mElements, (mElements + mCapacity), value);

        // Update mSum accordingly
        mSum = mCapacity * value;

        // Reset the counters
        mIndex         = 0;
        mItemsInBuffer = mCapacity;

        return true;
    }
    return false;
}

/**
 * \brief   Add the given value to the internal buffer. If the buffer is full,
 *          the oldest value gets overwritten. It also updates the index of the
 *          number of elements in the buffer.
 * \details In 'mSum' the current accumulated contents of the buffer is kept,
 *          to prevent iterating over all elements in GetAverage().
 *          For floating point types: every time the buffer wraps 'mSum' is
 *          refilled with the buffer contents to prevent rounding issues
 *          accumulate over time.
 * \param   value   The value to add to internal buffer.
 * \returns True if the value could be added to internal buffer, else false.
 */
template<class T>
bool MovingAverage<T>::Add(T value)
{
    if (mElements != nullptr)
    {
        // If the buffer is already full...
        if (mItemsInBuffer == mCapacity)
        {
            // Remove the oldest sample from the sum
            mSum -= mElements[mIndex];
        }

        // Add to the moving average (ring) buffer
        mElements[mIndex] = value;
        mSum += value;

        // Increase the index and wrap around if needed
        if (++mIndex >= mCapacity)
        {
            mIndex = 0;

            // If type is float, reset mSum to buffer contents at wrap around to prevent accumulating rounding error
            if (std::is_same<float, T>::value)
            {
                mSum = 0;
                for (auto i = 0; i < mCapacity; i++)
                {
                    mSum += mElements[i];
                }
            }
        }

        // Keep track of the number of items in the buffer, up to buffer full
        if (mItemsInBuffer < mCapacity)
        {
            mItemsInBuffer++;
        }

        return true;
    }
    return false;
}

/**
 * \brief   Get the averaged sum of the elements in the internal buffer (if
 *          any).
 * \returns The averaged sum of the number of available elements if successful,
 *          0 if the buffer has no elements.
 */
template<class T>
T MovingAverage<T>::GetAverage()
{
    if (mItemsInBuffer > 0)
    {
        // Return average of the buffered items
        return static_cast<T>(mSum / mItemsInBuffer);
    }
    return 0;
}


/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
/**
 * \brief   Delete the buffer, set pointer to nullptr.
 * \details No effect when buffer already deleted.
 */
template<class T>
void MovingAverage<T>::DeleteBuffer()
{
    if (mElements != nullptr)
    {
        delete [] mElements;
        mElements = nullptr;
    }
}


#endif  // MOVING_AVERAGE_HPP_
