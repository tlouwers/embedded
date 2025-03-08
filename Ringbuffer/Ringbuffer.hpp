/**
 * \file    Ringbuffer.hpp
 * \brief   Header file for the Ringbuffer class.
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   Ringbuffer
 *
 * \brief   Single-Producer, Single-Consumer, lock-free, wait-free ring buffer.
 *          Suited for embedded use, see URLs below.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Ringbuffer
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    03-2025
 */

#ifndef RING_BUFFER_HPP_
#define RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>
#include <algorithm>
#include <memory>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename T>
class Ringbuffer
{
public:
    Ringbuffer() noexcept;
    ~Ringbuffer();

    bool Resize(const size_t size) noexcept;

    bool TryPush(const T* src, const size_t size = 1);
    bool TryPop(T* &dest, const size_t size = 1);

    size_t Size() const;
    size_t Capacity() const;

    void Clear();

    bool IsLockFree() const;

#ifdef DEBUG
    void Print() const;
    void SetState(size_t write, size_t read);
    bool CheckState(size_t write, size_t read);
#endif // DEBUG

private:
    std::atomic<size_t> mWrite{0};
    std::atomic<size_t> mRead{0};
    size_t mCapacity{0};
    std::unique_ptr<T[]> mElements{nullptr}; // Use unique_ptr for automatic memory management

    void DeleteBuffer();
};


/**
 * \brief Default constructor.
 * \details Initializes an empty ring buffer. Call 'Resize()' to set the buffer size.
 */
template<typename T>
Ringbuffer<T>::Ringbuffer() noexcept :
    mWrite(0), mRead(0), mCapacity(0), mElements(nullptr)
{ }

/**
 * \brief Destructor that frees the buffer memory.
 */
template<typename T>
Ringbuffer<T>::~Ringbuffer()
{
    // No need to explicitly delete the buffer; unique_ptr handles it
}

/**
 * \brief   Resizes the buffer to the specified size.
 * \details Frees any existing memory and allocates a new buffer of the requested size.
 *          The buffer allocates one additional element to distinguish between full and empty states.
 * \param   size    The desired size of the buffer (excluding the additional element).
 * \return  True if the buffer was successfully resized; otherwise, false.
 *          Returns false if the requested size is zero or if allocation fails.
 */
template<typename T>
bool Ringbuffer<T>::Resize(const size_t size) noexcept
{
    if (size == 0) {
        return false;                                           // Invalid size
    }

    mCapacity = size + 1;                                       // +1 for distinguishing full/empty
    mElements = std::make_unique<T[]>(mCapacity);               // Allocate new buffer

    if (!mElements) {
        mCapacity = 0;                                          // Reset capacity if allocation fails
        return false;                                           // Allocation failed
    }

    Clear();                                                    // Reset read and write indices

    return true;                                                // Successfully resized
}

/**
 * \brief Tries to copy 'size' elements from 'src' into the buffer.
 * \details Uses std::copy() to copy elements if there is enough space.
 *          Returns true if all elements are copied; false otherwise.
 * \returns True if all elements could be copied into the buffer; false if:
 *          - 'size' is 0 or larger than buffer capacity,
 *          - 'size' exceeds the remaining space,
 *          - 'src' is nullptr.
 */
template<typename T>
bool Ringbuffer<T>::TryPush(const T* src, const size_t size)
{
    if (size == 0 || size >= mCapacity || src == nullptr)
    {
        return false;                                           // Early exit for invalid conditions
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read = mRead.load(std::memory_order_relaxed);

    if (write >= mCapacity)
    {
        return false;                                           // Robustness check
    }

    size_t available;
    if (write >= read)                                          // Space at the end
    {
        available = (write == read) ? (mCapacity - 1) : (mCapacity - write + read - 1);
    }
    else                                                        // Space at the start
    {
        available = (read - write - 1);
    }

    if (size > available)                                       // Not enough space
    {
        return false;
    }

    const auto available_upto_end = std::min(size, mCapacity - write);
    std::copy(src, src + available_upto_end, mElements.get() + write);

    if (size > available_upto_end)
    {
        std::copy(src + available_upto_end, src + size, mElements.get());
    }

    mWrite.store((write + size) % mCapacity, std::memory_order_release);

    return true;
}

/**
 * \brief Tries to retrieve 'size' elements from the buffer to 'dest'.
 * \details Uses std::copy() to copy elements if there are enough available.
 *          Returns true if all elements are copied; false otherwise.
 * \returns True if all elements could be copied into 'dest'; false if:
 *          - 'size' is 0 or larger than buffer capacity,
 *          - 'size' exceeds the number of available elements,
 *          - 'dest' is nullptr.
 */
template<typename T>
bool Ringbuffer<T>::TryPop(T* &dest, const size_t size)
{
    if (size == 0 || size >= mCapacity || dest == nullptr)
    {
        return false;                                           // Early exit for invalid conditions
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read = mRead.load(std::memory_order_relaxed);

    if (read >= mCapacity)
    {
        return false;                                           // Robustness check
    }

    size_t available;
    if (write > read)                                           // Data at the start
    {
        available = write - read;
    }
    else if (write < read)                                      // Data at the end
    {
        available = (mCapacity - read) + write;
    }
    else
    {
        return false;                                           // Buffer is empty
    }

    if (size > available)
    {
        return false;                                           // Not enough elements available
    }

    const auto available_upto_end = std::min(size, mCapacity - read);
    std::copy(mElements.get() + read, mElements.get() + (read + available_upto_end), dest);

    if (size > available_upto_end)
    {
        std::copy(mElements.get(), mElements.get() + (size - available_upto_end), dest + available_upto_end);
    }

    mRead.store((read + size) % mCapacity, std::memory_order_release);

    return true;
}

/**
 * \brief   Returns the number of elements in the buffer.
 * \remark  This is a snapshot; the size may be slightly incorrect if read
 *          or write operations occur concurrently.
 * \return  The total number of elements currently in the buffer.
 */
template<typename T>
size_t Ringbuffer<T>::Size() const
{
    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read  = mRead.load(std::memory_order_acquire);

    // Calculate the number of elements based on the positions of write and read
    return (write >= read) ? (write - read) : (mCapacity - (read - write));
}

/**
 * \brief   Returns the usable capacity of the buffer.
 * \details The capacity is defined as the total number of elements that can be stored
 *          in the buffer, excluding one element used to distinguish between full and empty states.
 * \return  The number of elements that can be stored in the buffer.
 */
template<typename T>
size_t Ringbuffer<T>::Capacity() const
{
    return mCapacity - 1;
}

/**
 * \brief   Clears the buffer by resetting the read and write pointers.
 * \details The elements remain in memory until they are overwritten or the buffer is destructed.
 */
template<typename T>
void Ringbuffer<T>::Clear()
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
}

/**
 * \brief   Check if atomic operations in the buffer are truly lock-free.
 * \result  Returns true if the atomic operations are lock-free, else false.
 */
template<typename T>
bool Ringbuffer<T>::IsLockFree() const
{
    return (mWrite.is_lock_free() && mRead.is_lock_free());
}

#ifdef DEBUG
/**
 * \brief Sets the state of the ring buffer.
 * \param write Value to set for mWrite.
 * \param read Value to set for mRead.
 * \remarks Use with caution; no checks are performed.
 */
template<typename T>
void Ringbuffer<T>::SetState(size_t write, size_t read)
{
    #warning DEBUG method SetState() enabled - use with caution.

    mWrite.store(write, std::memory_order_release);
    mRead.store(read, std::memory_order_release);
}

/**
 * \brief Checks if the current state matches the given values.
 * \param write Value to compare with mWrite.
 * \param read Value to compare with mRead.
 * \returns True if both states match; otherwise, false.
 */
template<typename T>
bool Ringbuffer<T>::CheckState(size_t write, size_t read)
{
    #warning DEBUG method CheckState() enabled.

    return (write == mWrite.load(std::memory_order_acquire)) &&
           (read == mRead.load(std::memory_order_acquire));
}

/**
 * \brief Prints the contents of the buffer.
 * \details Displays the read and write pointers, the buffer elements, and the current size.
 */
template<typename T>
void Ringbuffer<T>::Print() const
{
    std::cout << "Read(" << mRead.load() << "), Write(" << mWrite.load() << "), Elements[";
    for (size_t i = 0; i < (mCapacity - 1); i++)
    {
        std::cout << mElements[i] << (i < (mCapacity - 2) ? "|" : "");
    }
    std::cout << "], Size(" << Size() << ")" << std::endl;
}
#endif // DEBUG

/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
/**
 * \brief   Delete the buffer, set pointer to nullptr.
 * \details No effect when buffer already deleted.
 */
template<class T>
void Ringbuffer<T>::DeleteBuffer()
{
    // No need to explicitly delete the buffer; unique_ptr handles it
    mElements.reset(); // Resetting unique_ptr to release memory
}

#endif // RING_BUFFER_HPP_
