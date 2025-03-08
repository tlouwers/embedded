/**
 * \file ContiguousRingbuffer.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   ContiguousRingbuffer
 *
 * \brief   Single-Producer, Single-Consumer, lock free, wait free, contiguous
 *          ringbuffer. Best described as a variant of the bip-buffer, suited
 *          for embedded use, see URLs below.
 *
 * \details This buffer is designed for efficient data transfer between
 *          producer and consumer, utilizing Direct Memory Access (DMA)
 *          or Interrupt Service Routines (ISR) for data input. The buffer
 *          reserves space for elements and allows for thread-safe
 *          operations to check, read, and write data. It maintains
 *          a wrap-around mechanism to efficiently manage memory and
 *          prevent race conditions. The buffer supports resizing and
 *          provides methods to access the size and available contiguous
 *          blocks of data. The block sizes to read and write need not be
 *          equal in size.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/ContiguousBuffer
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.5
 * \date    02-2025
 */

#ifndef CONTIGUOUS_RING_BUFFER_HPP_
#define CONTIGUOUS_RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstddef>
#include <atomic>
#include <memory>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename T>
class ContiguousRingbuffer
{
public:
    ContiguousRingbuffer() noexcept;

    bool Resize(const size_t size) noexcept;

    bool Poke(T* &dest, size_t& size);
    bool Write(const size_t size);

    bool Peek(T* &dest, size_t& size);
    bool Read(const size_t size);

    size_t Size() const;
    size_t Capacity() const;
    void Clear();
    bool IsLockFree() const;

#ifdef DEBUG
    void SetState(size_t write, size_t read, size_t wrap);
    bool CheckState(size_t write, size_t read, size_t wrap);
#endif // DEBUG

private:
    std::atomic<size_t> mWrite{0};
    std::atomic<size_t> mRead{0};
    std::atomic<size_t> mWrap{0};
    size_t mCapacity{0};
    std::unique_ptr<T[]> mElements;
};


/**
 * \brief   Default constructor.
 * \details Initializes the buffer with zero capacity. The buffer must be
 *          resized using 'Resize()' before use.
 */
template<typename T>
ContiguousRingbuffer<T>::ContiguousRingbuffer() noexcept :
    mWrite(0), mRead(0), mWrap(0), mCapacity(0)
{ }

/**
 * \brief   Resizes the ring buffer to the specified number of elements.
 * \details Frees existing memory and allocates new memory for the buffer,
 *          adding one extra element to distinguish between 'full' and 'empty' states.
 *          Resizing to a previous size is permitted and handled similarly.
 * \param   size    The number of elements to allocate (must be greater than 0).
 * \returns True if allocation is successful; false if size is 0 or allocation fails.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Resize(const size_t size) noexcept
{
    // Free existing memory
    mElements.reset();

    // Handle invalid size
    if (0 == size) {
        return false; // Requested size is zero
    }

    // Reset pointers and update capacity
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
    mWrap.store(size + 1, std::memory_order_release);
    mCapacity = size + 1;

    // Allocate new memory
    mElements = std::unique_ptr<T[]>(new(std::nothrow) T[size + 1]);

    return (nullptr != mElements); // Return true if allocation was successful
}

/**
 * \brief   Checks for available contiguous space in the buffer.
 * \details Returns a pointer to a contiguous block for writing data,
 *          either at the end or the start of the buffer. The user must
 *          call 'Write()' to commit the data. If the requested size exceeds
 *          the available space, the method will select the appropriate block
 *          and may leave unused elements at the end.
 * \param   dest    Reference to a pointer that will point to the start of
 *                  the contiguous block if found; otherwise, nullptr.
 * \param   size    Reference to the size of the requested block; updated
 *                  to the maximum available contiguous size if found, else 0.
 * \note    An exceptional case occurs when the buffer is empty and the
 *          requested size equals the current read pointer, allowing a reset
 *          of both pointers to enable writing.
 * \returns True if a contiguous block is found; false if size is invalid
 *          or no block is available. The 'dest' and 'size' parameters are
 *          updated accordingly.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Poke(T*& dest, size_t& size)
{
    // Handle invalid size
    if (0 == size || size >= mCapacity) {
        dest = nullptr;
        size = 0;
        return false; // Size is not within valid range
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read  = mRead.load(std::memory_order_acquire);

    // Case 1: Space available at the end
    if (write >= read) {
        if (write < mCapacity) {                                // Robustness check
            const size_t available = mCapacity - write - ((read > 0) ? 0 : 1);

            if (size <= available) {                            // Does the requested block fit?
                size = available;
                dest = &mElements[write];
                return true;
            }
            // Case 2: Space available at the start
            else if (size < read) {                             // Does the requested block fit?
                size = read - 1;
                dest = &mElements[0];
                return true;
            }
            // Exceptional case: buffer is empty and requested size equals read
            else if ((write == read) && (size == read)) {
                dest = &mElements[0];
                mRead.store(0, std::memory_order_release);      // Note: Poke() modifies mWrite and mRead!
                mWrite.store(0, std::memory_order_release);
                return true;
            }
        }
    }
    // Case 3: Space available at the start when write < read
    else { // write < read
        if ((write + size) < read) {                            // Does the requested block fit?
            size = read - write - 1;
            dest = &mElements[write];
            return true;
        }
    }

    // If none of the conditions were met, return false
    dest = nullptr;
    size = 0;
    return false;                                           // No contiguous block available
}

/**
 * \brief   Advances the write pointer by the specified size.
 * \details Increments the write pointer if a contiguous block of the given
 *          size is available. If space is not available at the end, but
 *          it is at the start, the wrap pointer is adjusted to prevent
 *          further allocation at the end. The write pointer cannot exceed
 *          the read pointer, preventing race conditions.
 * \param   size    The number of elements to advance the write pointer by.
 * \returns True if the write pointer was successfully advanced; false if
 *          the size is invalid or no space is available. Returns true if
 *          size is 0, as no update occurs.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Write(const size_t size)
{
    // Handle invalid size
    if (0 == size) {
        return true; // No update is done
    }
    if (size >= mCapacity) {
        return false; // Size is not within valid range
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read  = mRead.load(std::memory_order_acquire);

    // Case 1: Space at the end
    if (write >= read) {
        if (write < mCapacity) {                                // Robustness, condition should always be true
            // Calculate the size available at the end, take into account the extra element when the buffer is empty
            const size_t available = mCapacity - write - ((read > 0) ? 0 : 1);

            if (size <= available) {
                if (size < (mCapacity - write)) {               // Does the requested block fit?
                    mWrite.store(write + size, std::memory_order_release);
                    return true;
                } else if (size == (mCapacity - write)) {       // Exact fit, need to wrap
                    mWrite.store(0, std::memory_order_release);
                    return true;
                }
            }
            // Case 2: Space at the start
            if (size < read) {
                mWrap.store(write, std::memory_order_release);  // Shrink wrap to prevent claiming memory at the end
                mWrite.store(size, std::memory_order_release);
                return true;
            }
        }
    }
    // Case 3: Space at the start when write < read
    else if ((write + size) < read) {
        mWrite.store(write + size, std::memory_order_release);
        return true;
    }

    return false;                                               // No space available
}

/**
 * \brief   Checks for available filled contiguous data in the buffer.
 * \details Returns a pointer to a contiguous block of filled elements,
 *          either at the start or the end of the buffer. The user must
 *          call 'Read()' to release the data. If the requested size exceeds
 *          the available data, the method will return the size of the
 *          contiguous block at the end, while additional data may be
 *          available at the start for subsequent reads.
 * \param   dest    Reference to a pointer that will point to the start of
 *                  the filled contiguous block if found; otherwise, nullptr.
 * \param   size    Reference to the size of the requested block; updated
 *                  to the maximum available contiguous size if found, else 0.
 * \returns True if a filled contiguous block is found; false if size is
 *          invalid or no block is available. The 'dest' and 'size'
 *          parameters are updated accordingly.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Peek(T*& dest, size_t& size)
{
    // Handle invalid size
    if (0 == size || size >= mCapacity) {
        dest = nullptr;
        size = 0;
        return false; // Size is not within valid range
    }

    const auto read  = mRead.load(std::memory_order_relaxed);
    const auto write = mWrite.load(std::memory_order_acquire);

    // Case 1: Data available at the start
    if (write >= read) {
        if ((read + size) <= write) {                           // Requested size available?
            size = write - read;
            dest = &mElements[read];
            return true;
        }
    }
    // Case 2: Data available at the end
    else { // write < read
        if (read < mCapacity) {                                 // Robustness, condition should always be true
            const auto wrap = mWrap.load(std::memory_order_acquire);

            if ((read + size) <= wrap) {                        // Requested size available?
                size = wrap - read;
                dest = &mElements[read];
                return true;
            }
            // Exception: when read/write were equal at the end of the buffer and a large block was written,
        	//            this resulted in wrap being shrunk and becoming equal to read.
            else if (read == wrap) {                            // Data available at the start?
                if (size <= write) {                            // Requested size available?
                    size = write;
                    dest = &mElements[0];
                    return true;
                }
            }
        }
    }

    // If none of the conditions were met, return false
    dest = nullptr;
    size = 0;
    return false;                                               // No contiguous block available
}

/**
 * \brief   Advances the read pointer by the specified size.
 * \details Increments the read pointer if the requested size is available
 *          without exceeding the write pointer. If the read pointer wraps
 *          around, it restores the wrap pointer to prevent reading released
 *          data. The read pointer can only equal the write pointer,
 *          preventing race conditions.
 * \param   size    The number of elements to advance the read pointer by.
 * \returns True if the read pointer was successfully advanced; false if
 *          the size is invalid or no data is available. Returns true if
 *          size is 0, as no update occurs.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Read(const size_t size)
{
    // Handle invalid size
    if (0 == size) {
        return true; // No update is done
    }
    if (size >= mCapacity) {
        return false; // Size is not within valid range
    }

    const auto read  = mRead.load(std::memory_order_relaxed);
    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read_and_size = read + size;

    // Case 1: Data available at the start
    if (read < write) {
        if (read_and_size <= write) {                           // Requested size available?
            mRead.store(read_and_size, std::memory_order_release);
            return true;
        }
    }
    // Case 2: Data available at the end
    else if (read > write) {
        if (read < mCapacity) {                                 // Robustness, condition should always be true
            const auto wrap = mWrap.load(std::memory_order_acquire);

            if (read_and_size < wrap) {                         // Requested size available? And we do not wrap?
                mRead.store(read_and_size, std::memory_order_release);
                return true;
            }
            else if (read_and_size == wrap) {                   // Requested size available? And we do wrap?
                mWrap.store(mCapacity, std::memory_order_release);
                mRead.store(0, std::memory_order_release);
                return true;
            }
            // Exception: when read/write were equal at the end of the buffer and a large block was written,
            //            this resulted in wrap being shrunk and becoming equal to read.
            else if (read == wrap) {                            // Data available at the start?
                if (size <= write) {                            // Requested size available?
                    mWrap.store(mCapacity, std::memory_order_release);
                    mRead.store(size, std::memory_order_release);
                    return true;
                }
            }
        }
    }

    // If none of the conditions were met, return false
    return false;                                               // Buffer empty or invalid size
}

/**
 * \brief   Returns the number of elements currently in the buffer.
 * \remark  This value is a snapshot and may be slightly inaccurate due to
 *          concurrent read or write operations.
 * \returns The total number of elements in the buffer, or 0 if the buffer
 *          is empty or not resized.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Size() const
{
    if (0 == mCapacity) {
        return 0; // Buffer not resized yet
    }

    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read  = mRead.load(std::memory_order_acquire);
    const auto wrap  = mWrap.load(std::memory_order_acquire);

    // Sanity checks: administration out-of-bounds, thus return a 'sane' value
    if (write >= wrap) {
        return mCapacity - 1;           // Write out of bounds
    }
    if (read > wrap) {
        return 0;                       // Read out of bounds
    }
    if ((read == wrap) && (read == mCapacity) && (write > 0)) {
        return 0; // More elements than specified in mCapacity
    }

    // Calculate the number of elements in the buffer
    if (write > read) {
        return write - read;            // Case 1: Data available in the middle
    } else if (write < read) {
        return (wrap - read) + write;   // Case 2: Data wraps around
    }

    // Else: write == read --> buffer empty, return 0
    return 0;
}

/**
 * \brief   Returns the maximum number of elements the buffer can hold.
 * \returns The maximum capacity of the buffer, accounting for the extra
 *          element used to distinguish between 'full' and 'empty' states.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Capacity() const
{
    return mCapacity - 1;
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write, read, and wrap pointers to their initial states,
 *          effectively emptying the buffer.
 */
template<typename T>
void ContiguousRingbuffer<T>::Clear()
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
    mWrap.store(mCapacity, std::memory_order_release);
}

/**
 * \brief   Checks if the buffer's atomic operations are lock-free.
 * \returns True if all atomic operations are lock-free; otherwise, false.
 */
template<typename T>
bool ContiguousRingbuffer<T>::IsLockFree() const
{
    return (mWrite.is_lock_free() && mRead.is_lock_free() && mWrap.is_lock_free());
}

#ifdef DEBUG
/**
 * \brief   Debug method to force a state to be set to the mWrite/mRead/mWrap
 *          pointers.
 * \param   write   Value to set mWrite to.
 * \param   read    Value to set mRead to.
 * \param   wrap    Value to set mWrap to.
 * \remarks There are no checks, so know what you are doing!
 */
template<typename T>
void ContiguousRingbuffer<T>::SetState(size_t write, size_t read, size_t wrap)
{
    #warning DEBUG method SetState() enabled - carefull, there be dragons here.

    mWrite.store(write, std::memory_order_release);
    mRead.store(read, std::memory_order_release);
    mWrap.store(wrap, std::memory_order_release);
}

/**
 * \brief   Debug method to check the state of the mWrite/mRead/mWrap pointers.
 * \param   write   Value to check mWrite against.
 * \param   read    Value to check mRead against.
 * \param   wrap    Value to check mWrap against.
 * \returns True if the state matches, else false.
 */
template<typename T>
bool ContiguousRingbuffer<T>::CheckState(size_t write, size_t read, size_t wrap)
{
    #warning DEBUG method CheckState() enabled.

    const auto current_write = mWrite.load(std::memory_order_acquire);
    const auto current_read  = mRead.load(std::memory_order_acquire);
    const auto current_wrap  = mWrap.load(std::memory_order_acquire);

    return ( ( write == current_write ) &&
             ( read  == current_read  ) &&
             ( wrap  == current_wrap  ) );
}
#endif // DEBUG

#endif // CONTIGUOUS_RING_BUFFER_HPP_
