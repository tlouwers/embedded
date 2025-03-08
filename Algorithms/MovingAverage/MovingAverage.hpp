/**
 * \file    MovingAverage.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice, you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   MovingAverage
 *
 * \brief   Implementation of a simple moving average with template functions.
 *
 * \details This class provides a way to calculate the moving average of a series of values.
 *          The internal buffer can be resized, and values can be added incrementally.
 *          The average is computed based on the values currently in the buffer.
 *
 * \performance
 *          - The `Add` method has a time complexity of O(1) since it performs a constant
 *            number of operations.
 *          - The `GetAverage` method also has a time complexity of O(1) as it simply
 *            computes the average from the maintained sum.
 *          - The `Resize` method has a time complexity of O(n) due to the need to allocate
 *            new memory and copy existing values if the buffer is resized.
 *          - The space complexity is O(n), where n is the size of the internal buffer.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Algorithms/MovingAverage
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.3
 * \date    03-2025
 */

#ifndef MOVING_AVERAGE_HPP_
#define MOVING_AVERAGE_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstdint>

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
    T GetAverage() const;

private:
    uint16_t mCapacity{0};
    uint16_t mIndex{0};
    uint16_t mItemsInBuffer{0};
    double mSum{0.0};               // Use a known large type which supports fractional numbers
    T* mElements{nullptr};

    void DeleteBuffer();
    void CustomFill(T* begin, T* end, const T& value);
    bool IsTypeSupported() const;

    // Private type_trait implementation
    struct IsSupportedType {
        static const bool value = true; // Default to true
    };
};

// Specializations for unsupported types
template <>
struct MovingAverage<double>::IsSupportedType {
    static const bool value = false;
};

template <>
struct MovingAverage<int64_t>::IsSupportedType {
    static const bool value = false;
};

template <>
struct MovingAverage<uint64_t>::IsSupportedType {
    static const bool value = false;
};

/**
 * \brief Constructor.
 * \details Internal buffer needs to be set to a size with 'Resize()' before use.
 */
template<class T>
MovingAverage<T>::MovingAverage() noexcept = default;

/**
 * \brief Destructor.
 * \details Frees memory allocated to the buffer if needed.
 */
template<class T>
MovingAverage<T>::~MovingAverage()
{
    DeleteBuffer();
}

/**
 * \brief Resizes the internal buffer to the given size.
 * \param size Size of the memory to allocate.
 * \returns True if the requested size could be allocated, else false.
 */
template<class T>
bool MovingAverage<T>::Resize(uint16_t size) noexcept
{
    if (size == 0 || !IsTypeSupported())
    {
        return false;
    }

    DeleteBuffer();

    mCapacity = size;

    mElements = new(std::nothrow) T[size];

    // Check if memory allocation was successful
    if (nullptr == mElements)
    {
        return false;
    }

    // Fill the entire internal buffer with '0'
    CustomFill(mElements, mElements + mCapacity, T{});

    mSum = 0.0;
    mIndex = 0;
    mItemsInBuffer = 0;

    return true;
}

/**
 * \brief Fills the internal buffer with the given value.
 * \param value The value to fill the internal buffer with.
 * \returns True if the internal buffer could be filled, else false.
 */
template<class T>
bool MovingAverage<T>::Fill(T value)
{
    if (nullptr == mElements)
    {
        return false;
    }

    // Fill the entire internal buffer with 'value'
    CustomFill(mElements, mElements + mCapacity, value);

    // Update mSum accordingly
    mSum = static_cast<double>(mCapacity) * value;

    // Reset the counters
    mIndex = 0;
    mItemsInBuffer = mCapacity;

    return true;
}

/**
 * \brief Adds a value to the internal buffer.
 * \param value The value to add to the internal buffer.
 * \returns True if the value could be added, else false.
 */
template<class T>
bool MovingAverage<T>::Add(T value)
{
    // Check if the buffer is initialized
    if (nullptr == mElements)
    {
        return false;
    }

    // If the buffer is full, remove the oldest sample from the sum
    if (mItemsInBuffer == mCapacity)
    {
        mSum -= mElements[mIndex];
    }

    // Add the new value to the buffer and update the sum
    mElements[mIndex] = value;
    mSum += value;

    // Move to the next index, wrapping around if necessary
    mIndex = (mIndex + 1) % mCapacity;

    // Keep track of the number of items in the buffer, up to buffer full
    if (mItemsInBuffer < mCapacity)
    {
        ++mItemsInBuffer;
    }

    return true;
}

/**
 * \brief Gets the average of the elements in the internal buffer.
 * \returns The average if successful, 0 if the buffer has no elements.
 */
template<class T>
T MovingAverage<T>::GetAverage() const
{
    return mItemsInBuffer > 0 ? static_cast<T>(mSum / mItemsInBuffer) : T{};
}

/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
/**
 * \brief Deletes the buffer and sets the pointer to nullptr.
 * \details No effect when the buffer is already deleted.
 */
template<class T>
void MovingAverage<T>::DeleteBuffer()
{
    if (nullptr != mElements) {
        delete[] mElements;
        mElements = nullptr;
    }
}

/**
 * \brief Custom fill function to initialize a range of elements with a specified value.
 * \param begin Pointer to the beginning of the range.
 * \param end Pointer to the end of the range.
 * \param value The value to fill the range with.
 * \details This function iterates over the specified range and assigns the given value
 *          to each element. It is a simple alternative to std::fill.
 */
template<class T>
void MovingAverage<T>::CustomFill(T* begin, T* end, const T& value)
{
    for (T* ptr = begin; ptr != end; ++ptr) {
        *ptr = value;
    }
}

/**
 * \brief Checks if the type T is supported for moving average calculations.
 * \returns True if the type is supported, false otherwise.
 */
template<class T>
bool MovingAverage<T>::IsTypeSupported() const
{
    return IsSupportedType::value; // Accessing the static member directly
}

#endif  // MOVING_AVERAGE_HPP_
