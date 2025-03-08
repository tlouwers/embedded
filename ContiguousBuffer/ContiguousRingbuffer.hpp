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
 * \note    https://github.com/tlouwers/embedded/tree/master/ContiguousBuffer
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.4
 * \date    05-2024
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

private:
    std::atomic<size_t> mWrite{0};
    std::atomic<size_t> mRead{0};
    std::atomic<size_t> mWrap{0};
    size_t mCapacity{0};
    std::unique_ptr<T[]> mElements;
};

/**
 * \brief   Constructor.
 * \details The buffer needs to be set to a size with 'Resize()' before it can
 *          be used.
 */
template<typename T>
ContiguousRingbuffer<T>::ContiguousRingbuffer() noexcept :
    mWrite(0), mRead(0), mWrap(0), mCapacity(0)
{ }

/**
 * \brief   Resizes the buffer to the given size by freeing and (re)allocating
 *          memory.
 * \details Frees memory allocated to the buffer if needed. Then tries to
 *          allocate the requested memory size.
 *          The buffer allocates 1 element more than requested, this to be able
 *          to differentiate in a thread safe way 'buffer full'/'buffer empty'
 *          status.
 *          Resizing to a previous size is allowed, this frees and allocates
 *          memory like other calls to Resize().
 * \param   size    The number of elements to allocate memory for.
 * \returns True if the requested size could be allocated, else false. False if
 *          the requested size equals 0.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Resize(const size_t size) noexcept
{
    mElements.reset();

    if (size > 0)
    {
        mWrite.store(0, std::memory_order_release);
        mRead.store(0, std::memory_order_release);
        mWrap.store(size + 1, std::memory_order_release);
        mCapacity = size + 1;

        mElements = std::unique_ptr<T[]>(new(std::nothrow) T[size + 1]);

        return (nullptr != mElements);
    }

    return false;
}

/**
 * \brief   Check (and return) if a contiguous block of elements is available.
 * \details This can be at the end of the buffer or at the start.
 *          The user is expected to fill the 'dest' with data and 'commit' this
 *          data to the buffer with a call to 'Write()'.
 *          Be aware that the returned 'size' has special behavior: it is
 *          possible there are 2 contiguous blocks of elements available,
 *          depending on the requested 'size' and the size of these 'free'
 *          blocks one of these blocks is selected: 'dest' and 'size' then
 *          relate to one of these blocks. This means if 'size' is larger than
 *          the first contiguous 'free' block the second (at the start) is
 *          selected, causing some unused elements to be left at the end of the
 *          buffer.
 * \param   dest    Reference to pointer to T. If the method returns true it
 *                  points to the start of the contiguous block, else nullptr.
 *                  The contiguous block can be either at the end (if enough
 *                  space is available), else at the start of the buffer.
 * \param   size    Reference to size. If the method returns true it is set to
 *                  largest free contiguous block available, else to 0.
 * \note    This has 1 exceptional case: if the buffer is empty and a block is
 *          requested equal to the size of the read pointer at that moment,
 *          Poke() will reset the write and read pointer to allow writing that
 *          block.
 * \returns True if a contiguous block of elements of 'size' could be found,
 *          else false. False if size is not within valid range.
 *          The 'size' will be set to the maximum size of elements in a
 *          contiguous block still free, else 0 (when false). Note that first
 *          the block at the end of the buffer is checked, then the start.
 *          The 'dest' will be set to the start of the contiguous block
 *          still free, else nullptr (when false).
 */
template<typename T>
bool ContiguousRingbuffer<T>::Poke(T* &dest, size_t& size)
{
    if ((size > 0) && (size < mCapacity))                   // Size within valid range?
    {
        const auto write = mWrite.load(std::memory_order_relaxed);
        const auto read  = mRead.load(std::memory_order_acquire);

        if (write >= read)                                  // Space at the end?
        {
            if (write < mCapacity)                          // Robustness, condition should always be true
            {
                // Calculate the size available at the end, take into account the extra element when the buffer is empty.
                const size_t available = mCapacity - write - ( (read > 0) ? 0 : 1 );

                if (size <= available)                      // Does the requested block fit?
                {
                    size = available;
                    dest = &mElements[write];
                    return true;
                }
                else                                        // Space at the start?
                {
                    if (size < read)                        // Does the requested block fit?
                    {
                        size = read - 1;
                        dest = &mElements[0];
                        return true;
                    }
                    // If the buffer is empty and the next block would fill it completely, allow this exception
                    else if ((write == read) && (size == read))
                    {
                        // Allowed as buffer is empty, we are Producer and will not call Write() before this.
                        // The Consumer will use Peek() which at the moment mWrite is used will also have an update mRead.
                        dest = &mElements[0];
                        mRead.store(0, std::memory_order_release);      // Note: Poke() modifies mWrite and mRead!
                        mWrite.store(0, std::memory_order_release);
                        return true;
                    }
                }
            }
        }
        else    // write < read                             // Space at the start?
        {
            if ((write + size) < read)                      // Does the requested block fit?
            {
                size = read - write - 1;
                dest = &mElements[write];
                return true;
            }
        }
    }

    dest = nullptr;
    size = 0;
    return false;
}

/**
 * \brief   Advances the write pointer with 'size'.
 * \details Advances the write pointer with the given size as long as it moves
 *          over a contiguous block. If not, it it will return false. It handles
 *          the wrap as well, with a twist: if a contiguous block is not
 *          available at the end of the buffer, but it is at the start, it will
 *          shrink the wrap pointer to prevent more elements to be allocated at
 *          the end. A 'Read()' will correct for this when crossing over the
 *          wrap pointer. Note that 'Write()' is not able to pass beyond the
 *          read pointer, meaning a race condition with the wrap is prevented.
 * \param   size    The size to advance the write pointer with.
 * \returns True if the write pointer could be advanced, else false. False if
 *          size is not within valid range. True if size is 0, as no update is
 *          done.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Write(const size_t size)
{
    if ((size > 0) && (size < mCapacity))
    {
        const auto write = mWrite.load(std::memory_order_relaxed);
        const auto read  = mRead.load(std::memory_order_acquire);

        if (write >= read)                                  // Space at the end?
        {
            if (write < mCapacity)                          // Robustness, condition should always be true
            {
                // Calculate the size available at the end, take into account the extra element when the buffer is empty.
                const size_t available = mCapacity - write - ( (read > 0) ? 0 : 1 );

                if (size <= available)
                {
                    if (size < (mCapacity - write))         // Does the requested block fit?
                    {
                        mWrite.store((write + size), std::memory_order_release);
                        return true;
                    }
                    else if (size == (mCapacity - write))   // Does the requested block fit exactly and we need to wrap?
                    {
                        mWrite.store(0, std::memory_order_release);
                        return true;
                    }
                }
                else                                        // Space at the start?
                {
                    if (size < read)
                    {
                        mWrap.store(write, std::memory_order_release);  // Shrink wrap to prevent claiming memory at the end.
                        mWrite.store(size, std::memory_order_release);
                        return true;
                    }
                }
            }
        }
        else    // write < read                             // Space at the start?
        {
            if ((write + size) < read)                      // Does the requested block fit?
            {
                mWrite.store((write + size), std::memory_order_release);
                return true;
            }
        }
    }
    else
    {
        return (size == 0);
    }

    return false;
}

/**
 * \brief   Check (and return) if a filled contiguous block of elements is
 *          available.
 * \details This can be at the end of the buffer or at the start.
 *          The user is expected to read the 'dest' data and 'release' this
 *          data from the buffer with a 'Read()'. This may not be all data
 *          available, there can be another contiguous block filled at the
 *          start of the buffer, which can be retrieved with another 'Peek()'
 *          and 'Read()'.
 * \param   dest   Reference to pointer to T. If the method returns true it
 *                 points to the start of the first contiguous block available,
 *                 else nullptr.
 * \param   size   Reference to size. If the method returns true it is set to
 *                 the filled contiguous block of elements, else to
 *                 0. There could be more elements filled then indicated by
 *                 size, this means that the filled contiguous block at the
 *                 end is returned. Once 'Read()', another call to 'Peek() will
 *                 return the remaining filled size at the start.
 * \returns True if available data of 'size' could be found, else false. False
 *          if size is not within valid range.
 *          The 'size' will be set to the maximum size of elements in a
 *          contiguous block available, else 0 (when false).
 *          The 'dest' will be set to the start of the contiguous block
 *          available, else nullptr (when false).
 */
template<typename T>
bool ContiguousRingbuffer<T>::Peek(T* &dest, size_t& size)
{
    if ((size > 0) && (size < mCapacity))                   // Size is in valid range?
    {
        const auto read  = mRead.load(std::memory_order_relaxed);
        const auto write = mWrite.load(std::memory_order_acquire);

        if (write >= read)                                  // Data at the start?
        {
            if ((read + size) <= write)                     // Requested size available?
            {
                size = write - read;
                dest = &mElements[read];
                return true;
            }
        }
        else    // write < read                             // Data at the end?
        {
            if (read < mCapacity)                           // Robustness, condition should always be true
            {
                const auto wrap = mWrap.load(std::memory_order_acquire);

                if ((read + size) <= wrap)                  // Requested size available?
                {
                    size = wrap - read;
                    dest = &mElements[read];
                    return true;
                }
                // Exception: when read/write were equal at the end of the buffer and a large block was written,
                //            this resulted in wrap being shrunk and becoming equal to read.
                else if (read == wrap)                      // Data available at the start?
                {
                    if (size <= write)                      // Requested size available?
                    {
                        size = write;
                        dest = &mElements[0];
                        return true;
                    }
                }
            }
        }
    }

    dest = nullptr;
    size = 0;
    return false;
}

/**
 * \brief   Advances the read pointer with 'size'.
 * \details Advances the read pointer with the given size as long as it does
 *          not move over the write pointer (but can become equal). It handles
 *          the wrap as well, with a twist: if moving over the wrap pointer and
 *          this is not the original size, it will restore the wrap pointer.
 *          The read pointer is set after the wrap (start of the buffer, before
 *          the write) to prevent reading released data.
 *          Note that 'Read()' is not able to pass beyond the write pointer,
 *          meaning a race condition with the wrap is prevented.
 * \param   size    The size to advance the read pointer with.
 * \returns True if the read pointer could be advanced, else false. False if
 *          size is not within valid range. True if size is 0, as no update is
 *          done.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Read(const size_t size)
{
    if ((size > 0) && (size < mCapacity))               // Size is in valid range?
    {
        const auto read  = mRead.load(std::memory_order_relaxed);
        const auto write = mWrite.load(std::memory_order_acquire);

        const auto read_and_size = read + size;

        if (read < write)                               // Data available at the start?
        {
            if (read_and_size <= write)                 // Requested size available?
            {
                mRead.store(read_and_size, std::memory_order_release);
                return true;
            }
        }
        else if (read > write)                          // Data available at the end?
        {
            if (read < mCapacity)                       // Robustness, condition should always be true
            {
                const auto wrap = mWrap.load(std::memory_order_acquire);

                if (read_and_size < wrap)               // Requested size available? And we do not wrap?
                {
                    mRead.store(read_and_size, std::memory_order_release);
                    return true;
                }
                else if (read_and_size == wrap)         // Requested size available? And we do wrap?
                {
                    mWrap.store(mCapacity, std::memory_order_release);
                    mRead.store(0, std::memory_order_release);
                    return true;
                }
                // Exception: when read/write were equal at the end of the buffer and a large block was written,
                //            this resulted in wrap being shrunk and becoming equal to read.
                else if (read == wrap)                  // Data available at the start?
                {
                    if (size <= write)                  // Requested size available?
                    {
                        mWrap.store(mCapacity, std::memory_order_release);
                        mRead.store(size, std::memory_order_release);
                        return true;
                    }
                }
            }
        }
        // Else: buffer empty
    }
    else
    {
        return (size == 0);
    }

    return false;
}

/**
 * \brief   Returns the number of elements in the buffer.
 * \remark  This is a snapshot, either read or write can change the status
 *          causing the size to be (slightly) incorrect.
 * \returns The total number of elements in the buffer.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Size() const
{
    if (mCapacity == 0) { return 0; }                       // Buffer not 'Resize()' yet

    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read  = mRead.load(std::memory_order_acquire);
    const auto wrap  = mWrap.load(std::memory_order_acquire);

    // Sanity checks: administration out-of-bounds, thus return a 'sane' value
    if  (write >= wrap) { return mCapacity - 1; }           // Write out of bounds
    if  (read  >  wrap) { return 0;             }           // Read out of bounds
    if ((read  == wrap) && (read == mCapacity) && (write > 0)) { return 0; }    // More elements than specified in mCapacity

    size_t result = 0;

    if (write > read)
    {
        result = write - read;
    }
    else if (write < read)
    {
        result = (wrap - read) + write;
    }
    // Else: write == read --> buffer empty, return 0

    return result;
}

/**
 * \brief   Returns the maximum number of elements the buffer may contain.
 * \returns The max number of elements the buffer may contain.
 */
template<typename T>
size_t ContiguousRingbuffer<T>::Capacity() const
{
    return mCapacity - 1;
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write/read/wrap pointers to the initial state.
 */
template<typename T>
void ContiguousRingbuffer<T>::Clear()
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
    mWrap.store(mCapacity, std::memory_order_release);
}

/**
 * \brief   Check if atomic operations in the buffer are truly lock-free.
 * \result  Returns true if the atomic operations are lock-free, else false.
 */
template<typename T>
bool ContiguousRingbuffer<T>::IsLockFree() const
{
    return (mWrite.is_lock_free() && mRead.is_lock_free() && mWrap.is_lock_free());
}


#endif // CONTIGUOUS_RING_BUFFER_HPP_
