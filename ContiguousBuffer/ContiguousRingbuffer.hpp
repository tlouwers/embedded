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
 *          prevent race conditions. The buffer uses a two-phase Reserve/Commit
 *          API pattern and provides methods to access the size and available
 *          contiguous blocks of data. The block sizes to read and write
 *          need not be equal in size.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/ContiguousBuffer
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.7
 * \date    06-2026
 */

#ifndef CONTIGUOUS_RING_BUFFER_HPP_
#define CONTIGUOUS_RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <cstddef>
#include <atomic>
#include <memory>


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename T>
class ContiguousRingbuffer
{
public:
    ContiguousRingbuffer() noexcept = default;

    bool Reserve(const size_t capacity) noexcept;

    bool ReserveWrite(T* &dest, size_t& size) noexcept;
    bool CommitWrite(const size_t size) noexcept;

    bool ReserveRead(T* &dest, size_t& size) noexcept;
    bool CommitRead(const size_t size) noexcept;

    size_t Size() const noexcept;
    size_t Capacity() const noexcept;
    void Clear() noexcept;
    bool IsLockFree() const noexcept;

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
 * \brief   Reserves capacity for the ring buffer.
 * \details Allocates storage for the specified number of elements. Frees any
 *          existing memory first, then allocates new memory. The actual capacity
 *          is increased by 1 internally to distinguish between 'full' and 'empty' states.
 *          Calling this method multiple times is permitted and handled similarly.
 * \param   capacity    The number of elements to allocate (must be greater than 0).
 * \returns True if allocation is successful; false if capacity is 0 or allocation fails.
 * \warning This method is NOT thread-safe and must NOT be called concurrently
 *          with any other buffer operations. It should only be called during
 *          initialization or after ensuring all producer and consumer threads have stopped.
 * \note    Follows the naming convention of std::vector::reserve().
 */
template<typename T>
bool ContiguousRingbuffer<T>::Reserve(const size_t capacity) noexcept
{
    // Free existing memory
    mElements.reset();

    // Handle invalid capacity
    if (0 == capacity) {
        return false; // Requested capacity is zero
    }

    // Reset pointers and update capacity
    // Use relaxed ordering: Reserve() is not called concurrently with read/write
    // operations. It's called during initialization or after all threads have stopped.
    // No synchronization is needed since there's no concurrent access.
    const size_t newCapacity = capacity + 1;
    mWrite.store(0, std::memory_order_relaxed);
    mRead.store(0, std::memory_order_relaxed);
    mWrap.store(newCapacity, std::memory_order_relaxed);
    mCapacity = newCapacity;

    // Allocate new memory
    mElements = std::unique_ptr<T[]>(new(std::nothrow) T[newCapacity]);

    if (nullptr == mElements) {
        // Keep state consistent so callers that ignore the return value
        // cannot accidentally dereference a null buffer.
        mCapacity = 0;
        mWrap.store(0, std::memory_order_relaxed);
        return false;
    }
    return true;
}

/**
 * \brief   Reserves contiguous space in the buffer for writing.
 * \details This is phase 1 of a two-phase write operation:
 *          1. ReserveWrite() - Get pointer to writable memory
 *          2. [User writes data to the pointer]
 *          3. CommitWrite() - Mark the data as available for reading
 *
 *          Returns a pointer to a contiguous block for writing data,
 *          either at the end or the start of the buffer. If the requested
 *          size exceeds the available space, the method will select the
 *          appropriate block and may leave unused elements at the end.
 *
 * \param   dest    [out] Reference to a pointer that will point to the start
 *                  of the writable block if found; otherwise, nullptr.
 * \param   size    [in/out] Reference to the requested size; updated to the
 *                  maximum available contiguous size if found, else 0.
 *
 * \note    IMPORTANT: The returned size may be LARGER than requested. This
 *          indicates the maximum contiguous block available. You may write
 *          less than this size, but must pass the actual written size to
 *          CommitWrite().
 *
 * \note    An exceptional case occurs when the buffer is empty and the
 *          requested size equals the current read pointer, allowing a reset
 *          of both pointers to enable writing (optimization).
 *
 * \warning You MUST call CommitWrite() after writing data to make it visible
 *          to the consumer. Failing to commit will leak buffer space.
 *
 * \returns True if a contiguous block is found; false if size is invalid
 *          or no block is available. The 'dest' and 'size' parameters are
 *          updated accordingly.
 */
template<typename T>
bool ContiguousRingbuffer<T>::ReserveWrite(T*& dest, size_t& size) noexcept
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
            const size_t available = mCapacity - write - ((read == 0) ? 1 : 0);

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
                mRead.store(0, std::memory_order_release);      // Note: ReserveWrite() modifies mWrite and mRead in this exceptional case!
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
    return false;                                               // No contiguous block available
}

/**
 * \brief   Commits a write operation started with ReserveWrite().
 * \details This is phase 2 of a two-phase write operation. You MUST call
 *          ReserveWrite() first to get a write pointer, write your data,
 *          then call this method to make the data visible to the consumer.
 *
 *          Increments the write pointer if a contiguous block of the given
 *          size is available. If space is not available at the end, but
 *          it is at the start, the wrap pointer is adjusted to prevent
 *          further allocation at the end. The write pointer cannot exceed
 *          the read pointer, preventing race conditions.
 *
 * \param   size    The actual number of elements written (may be less than
 *                  the size returned by ReserveWrite(), but not more).
 *
 * \warning Calling CommitWrite() without ReserveWrite() or with incorrect
 *          size causes buffer corruption and undefined behavior.
 *
 * \returns True if the write pointer was successfully advanced; false if
 *          the size is invalid or no space is available. Returns true if
 *          size is 0, as no update occurs.
 */
template<typename T>
bool ContiguousRingbuffer<T>::CommitWrite(const size_t size) noexcept
{
    // Handle invalid size
    if (0 == size) {
        return true; // No update is done
    }
    if (size >= mCapacity) {
        return false; // Size is not within valid range
    }

    const auto write = mWrite.load(std::memory_order_relaxed);
    // Relaxed is sufficient here: the synchronizing acquire on mRead was done in
    // ReserveWrite() before the data was written. CommitWrite() writes no data, it
    // only publishes, and coherence guarantees this load cannot observe a value
    // older than the one ReserveWrite() saw.
    const auto read  = mRead.load(std::memory_order_relaxed);

    // Case 1: Space at the end
    if (write >= read) {
        if (write < mCapacity) {                                // Robustness, condition should always be true
            // Pre-calculate space at end for efficiency
            const size_t space_at_end = mCapacity - write;
            // Calculate the size available at the end, take into account the extra element when the buffer is empty
            const size_t available = space_at_end - ((read == 0) ? 1 : 0);

            if (size <= available) {
                if (size < space_at_end) {                      // Does the requested block fit?
                    mWrite.store(write + size, std::memory_order_release);
                    return true;
                } else if (size == space_at_end) {              // Exact fit, need to wrap
                    mWrite.store(0, std::memory_order_release);
                    return true;
                }
            }
            // Case 2: Space at the start
            if (size < read) {
                // Relaxed store: the consumer only acts on the shrunk wrap in branches
                // gated on (write < read), i.e. after acquiring the new mWrite below.
                // That release store publishes this wrap store as well.
                mWrap.store(write, std::memory_order_relaxed);  // Shrink wrap to prevent claiming memory at the end
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
 * \brief   Reserves contiguous data in the buffer for reading.
 * \details This is phase 1 of a two-phase read operation:
 *          1. ReserveRead() - Get pointer to readable data
 *          2. [User reads/processes the data]
 *          3. CommitRead() - Release the data, making space available
 *
 *          Returns a pointer to a contiguous block of filled elements,
 *          either at the start or the end of the buffer. If the requested
 *          size exceeds the available data, the method will return the size
 *          of the contiguous block at the end, while additional data may be
 *          available at the start for subsequent reads.
 *
 * \param   dest    [out] Reference to a pointer that will point to the start
 *                  of the readable block if found; otherwise, nullptr.
 * \param   size    [in/out] Reference to the requested size; updated to the
 *                  maximum available contiguous size if found, else 0.
 *
 * \note    The returned size indicates the first contiguous block available,
 *          up to the wrapping point. You may read less than this size, but
 *          must pass the actual read size to CommitRead().
 *
 * \warning You MUST call CommitRead() after reading data to release the space.
 *          Failing to commit will prevent the producer from reusing the space.
 *
 * \returns True if a filled contiguous block is found; false if size is
 *          invalid or no block is available. The 'dest' and 'size'
 *          parameters are updated accordingly.
 */
template<typename T>
bool ContiguousRingbuffer<T>::ReserveRead(T*& dest, size_t& size) noexcept
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
        // Pre-calculate available data for efficiency
        const size_t available_data = write - read;
        if (size <= available_data) {                           // Requested size available?
            size = available_data;
            dest = &mElements[read];
            return true;
        }
    }
    // Case 2: Data available at the end
    else { // write < read
        if (read < mCapacity) {                                 // Robustness, condition should always be true
            const auto wrap = mWrap.load(std::memory_order_acquire);
            // Pre-calculate available data at end for efficiency
            const size_t available_at_end = wrap - read;

            if (size <= available_at_end) {                     // Requested size available?
                size = available_at_end;
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
 * \brief   Commits a read operation started with ReserveRead().
 * \details This is phase 2 of a two-phase read operation. You MUST call
 *          ReserveRead() first to get a read pointer, process your data,
 *          then call this method to release the space for the producer.
 *
 *          Increments the read pointer if the requested size is available
 *          without exceeding the write pointer. If the read pointer wraps
 *          around, it restores the wrap pointer to prevent reading released
 *          data. The read pointer can only equal the write pointer,
 *          preventing race conditions.
 *
 * \param   size    The actual number of elements read/consumed (may be less
 *                  than the size returned by ReserveRead(), but not more).
 *
 * \warning Calling CommitRead() without ReserveRead() or with incorrect
 *          size causes buffer corruption and undefined behavior.
 *
 * \returns True if the read pointer was successfully advanced; false if
 *          the size is invalid or no data is available. Returns true if
 *          size is 0, as no update occurs.
 */
template<typename T>
bool ContiguousRingbuffer<T>::CommitRead(const size_t size) noexcept
{
    // Handle invalid size
    if (0 == size) {
        return true; // No update is done
    }
    if (size >= mCapacity) {
        return false; // Size is not within valid range
    }

    const auto read  = mRead.load(std::memory_order_relaxed);
    // Relaxed is sufficient here: the synchronizing acquire on mWrite was done in
    // ReserveRead() before the data was read. CommitRead() reads no data, it only
    // releases space, and coherence guarantees this load cannot observe a value
    // older than the one ReserveRead() saw.
    const auto write = mWrite.load(std::memory_order_relaxed);
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
                // Relaxed store: the producer never loads mWrap, and Size() reads it
                // relaxed (best-effort). The mRead release store below keeps it
                // ordered for any cross-thread observer anyway.
                mWrap.store(mCapacity, std::memory_order_relaxed);
                mRead.store(0, std::memory_order_release);
                return true;
            }
            // Exception: when read/write were equal at the end of the buffer and a large block was written,
            //            this resulted in wrap being shrunk and becoming equal to read.
            else if (read == wrap) {                            // Data available at the start?
                if (size <= write) {                            // Requested size available?
                    // Relaxed store: same reasoning as the wrap restore above.
                    mWrap.store(mCapacity, std::memory_order_relaxed);
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
 * \details Uses relaxed memory ordering for optimal performance since this
 *          method provides a best-effort snapshot that doesn't require
 *          strict synchronization guarantees.
 * \remark  This value is a snapshot and may be slightly inaccurate due to
 *          concurrent read or write operations. The approximate nature allows
 *          the use of relaxed atomics for better performance.
 * \returns The total number of elements in the buffer, or 0 if the buffer
 *          is empty or Reserve() has not been called yet.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Size() const noexcept
{
    if (0 == mCapacity) {
        return 0; // Buffer not initialized (Reserve() not called yet)
    }

    // Use relaxed ordering: Size() provides a best-effort snapshot and doesn't
    // require synchronization guarantees. Each atomic read is still atomic (no
    // torn reads), but we avoid expensive memory barriers since approximate values
    // are acceptable per the documented behavior.
    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read  = mRead.load(std::memory_order_relaxed);
    const auto wrap  = mWrap.load(std::memory_order_relaxed);

    // Sanity checks: administration out-of-bounds, thus return a 'sane' value
    if (write >= wrap) {
        return mCapacity - 1;                                   // Write out of bounds
    }
    if (read > wrap) {
        return 0;                                               // Read out of bounds
    }
    if ((read == wrap) && (read == mCapacity) && (write > 0)) {
        return 0; // More elements than specified in mCapacity
    }

    // Calculate the number of elements in the buffer
    if (write > read) {
        return write - read;                                    // Case 1: Data available in the middle
    } else if (write < read) {
        return (wrap - read) + write;                           // Case 2: Data wraps around
    }

    // Else: write == read --> buffer empty, return 0
    return 0;
}

/**
 * \brief   Returns the maximum number of elements the buffer can hold.
 * \returns The maximum capacity of the buffer, accounting for the extra
 *          element used to distinguish between 'full' and 'empty' states.
 *          Returns 0 if Reserve() has not been called yet.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Capacity() const noexcept
{
    return (0 == mCapacity) ? 0 : (mCapacity - 1);
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write, read, and wrap pointers to their initial states,
 *          effectively emptying the buffer.
 */
template<typename T>
void ContiguousRingbuffer<T>::Clear() noexcept
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
bool ContiguousRingbuffer<T>::IsLockFree() const noexcept
{
    return (mWrite.is_lock_free() && mRead.is_lock_free() && mWrap.is_lock_free());
}

#ifdef DEBUG
#warning ContiguousRingbuffer DEBUG methods (SetState/CheckState) enabled - here be dragons.

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
    const auto current_write = mWrite.load(std::memory_order_acquire);
    const auto current_read  = mRead.load(std::memory_order_acquire);
    const auto current_wrap  = mWrap.load(std::memory_order_acquire);

    return ( ( write == current_write ) &&
             ( read  == current_read  ) &&
             ( wrap  == current_wrap  ) );
}
#endif // DEBUG

#endif // CONTIGUOUS_RING_BUFFER_HPP_
