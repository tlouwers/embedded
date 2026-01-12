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
 * \version 1.3
 * \date    01-2026
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
    ~Ringbuffer() = default;

    bool Reserve(const size_t size) noexcept;

    bool TryPush(const T* src, const size_t size = 1) noexcept;
    bool TryPop(T* &dest, const size_t size = 1) noexcept;

    size_t Size() const noexcept;
    size_t Capacity() const noexcept;

    void Clear() noexcept;

    bool IsLockFree() const noexcept;

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
};


/**
 * \brief Default constructor.
 * \details Initializes an empty ring buffer. Call 'Reserve()' to set the buffer size.
 */
template<typename T>
Ringbuffer<T>::Ringbuffer() noexcept :
    mWrite(0), mRead(0), mCapacity(0)
{ }

/**
 * \brief   Reserves capacity for the ring buffer.
 * \details Allocates storage for the specified number of elements. Frees any
 *          existing memory first, then allocates new memory. The actual capacity
 *          is increased by 1 internally to distinguish between 'full' and 'empty' states.
 *          Calling this method multiple times is permitted and handled similarly.
 * \param   size    The number of elements to allocate (must be greater than 0).
 * \returns True if allocation is successful; false if size is 0 or allocation fails.
 * \warning This method is NOT thread-safe and must NOT be called concurrently
 *          with any other buffer operations. It should only be called during
 *          initialization or after ensuring all producer and consumer threads have stopped.
 * \note    Follows the naming convention of std::vector::reserve().
 */
template<typename T>
bool Ringbuffer<T>::Reserve(const size_t size) noexcept
{
    if (0 == size) {
        return false;                                           // Invalid size
    }

    mCapacity = size + 1;                                       // +1 for distinguishing full/empty
    mElements = std::make_unique<T[]>(mCapacity);               // Allocate new buffer

    // Direct initialization is faster than Clear() - no concurrent access during Reserve
    mWrite.store(0, std::memory_order_relaxed);
    mRead.store(0, std::memory_order_relaxed);

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
bool Ringbuffer<T>::TryPush(const T* src, const size_t size) noexcept
{
    if (0 == size || size >= mCapacity || nullptr == src)
    {
        return false;                                           // Early exit for invalid conditions
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read = mRead.load(std::memory_order_acquire);    // Acquire: see consumer's updates

    // Robustness check: write should never exceed capacity
    if (write >= mCapacity)
    {
        return false;
    }

    // Calculate available space
    const size_t available = (write >= read)
        ? ((write == read) ? (mCapacity - 1) : (mCapacity - write + read - 1))
        : (read - write - 1);

    if (size > available)
    {
        return false;                                           // Not enough space
    }

    // Copy data, wrapping if necessary
    const size_t first_chunk = std::min(size, mCapacity - write);
    std::copy(src, src + first_chunk, mElements.get() + write);

    if (size > first_chunk)
    {
        std::copy(src + first_chunk, src + size, mElements.get());
    }

    // Update write pointer - avoid expensive modulo with conditional wrap
    const size_t new_write = write + size;
    mWrite.store((new_write >= mCapacity) ? (new_write - mCapacity) : new_write,
                 std::memory_order_release);

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
bool Ringbuffer<T>::TryPop(T* &dest, const size_t size) noexcept
{
    if (0 == size || size >= mCapacity || nullptr == dest)
    {
        return false;                                           // Early exit for invalid conditions
    }

    const auto write = mWrite.load(std::memory_order_acquire);  // Acquire: see producer's updates
    const auto read = mRead.load(std::memory_order_relaxed);

    // Robustness check: read should never exceed capacity
    if (read >= mCapacity)
    {
        return false;
    }

    // Early exit for empty buffer
    if (write == read)
    {
        return false;                                           // Buffer is empty
    }

    // Calculate available data
    const size_t available = (write > read)
        ? (write - read)
        : (mCapacity - read + write);

    if (size > available)
    {
        return false;                                           // Not enough elements available
    }

    // Copy data, wrapping if necessary
    const size_t first_chunk = std::min(size, mCapacity - read);
    std::copy(mElements.get() + read, mElements.get() + read + first_chunk, dest);

    if (size > first_chunk)
    {
        std::copy(mElements.get(), mElements.get() + (size - first_chunk), dest + first_chunk);
    }

    // Update read pointer - avoid expensive modulo with conditional wrap
    const size_t new_read = read + size;
    mRead.store((new_read >= mCapacity) ? (new_read - mCapacity) : new_read,
                std::memory_order_release);

    return true;
}

/**
 * \brief   Returns the number of elements in the buffer.
 * \details Uses relaxed memory ordering for optimal performance since this
 *          method provides a best-effort snapshot that doesn't require
 *          strict synchronization guarantees.
 * \remark  This value is a snapshot and may be slightly inaccurate due to
 *          concurrent read or write operations. The approximate nature allows
 *          the use of relaxed atomics for better performance.
 * \return  The total number of elements currently in the buffer.
 */
template<typename T>
size_t Ringbuffer<T>::Size() const noexcept
{
    // Use relaxed ordering: Size() provides a best-effort snapshot and doesn't
    // require synchronization guarantees. Each atomic read is still atomic (no
    // torn reads), but we avoid expensive memory barriers since approximate values
    // are acceptable per the documented behavior.
    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read  = mRead.load(std::memory_order_relaxed);

    // Calculate the number of elements based on the positions of write and read
    return (write >= read) ? (write - read) : (mCapacity - (read - write));
}

/**
 * \brief   Returns the maximum number of elements the buffer can hold.
 * \returns The maximum capacity of the buffer, accounting for the extra
 *          element used to distinguish between 'full' and 'empty' states.
 */
template<typename T>
size_t Ringbuffer<T>::Capacity() const noexcept
{
    return mCapacity - 1;
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write and read pointers to their initial states,
 *          effectively emptying the buffer.
 */
template<typename T>
void Ringbuffer<T>::Clear() noexcept
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
}

/**
 * \brief   Checks if the buffer's atomic operations are lock-free.
 * \returns True if all atomic operations are lock-free; otherwise, false.
 */
template<typename T>
bool Ringbuffer<T>::IsLockFree() const noexcept
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

#endif // RING_BUFFER_HPP_
