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
 * \details This code is intended to be used to feed DMA handling of a
 *          Cortex-M4 microcontroller, but can see its share of use in other
 *          scenarios as well. Be sure to read up on the documentation, as it
 *          has some other-than-usual behavior.
 *
 *          As example:
 *          Producer will fill the buffer, Consumer will empty the buffer.
 *          Note: do check for the result values, the example omits them for
 *                clarity.
 *
 *          // Declare the buffer
 *          ContiguousRingbuffer<int> ringBuff;
 *
 *          // Reserve the size to hold the elements, 'int' in this example
 *          ringBuff.Resize(5);
 *
 *          // Check if there is room, then write 1 element
 *          int* data = nullptr;
 *          size_t size = 1;
 *          int val = 42;
 *          if (ringBuff.Poke(data, size))  // 'size' changes to the space available
 *          {
 *              data[0] = val;
 *              ringBuff.Write(1);          // Administer the data is written, 1 element
 *          }
 *
 *          // Check if there is at least 1 element in buffer, then read it
 *          size_t size = 1;
 *          if (ringBuff.Peek(data, size))  // 'size' changes to the elements available
 *          {
 *              val = data[0];
 *              ringBuff.Read(1);           // Administer the data is read, 1 element
 *          }
 *
 *          The intended use in a nutshell:
 *          Assuming an Interrupt Service Routine (ISR) as thread 'Producer'
 *          and  the main application loop as thread 'Consumer'.
 *          Producer will use Poke() to request a contiguous block of elements,
 *          this can be passed on to the DMA (to fill the data). The CPU can
 *          perform another task. When DMA finishes it flags the data as
 *          written by calling Write() with the correct size. Consumer is
 *          expected to check if data is available in the buffer by calling
 *          Peek() - either with the size it can manage at the time or 1 to
 *          get an indication of the largest contiguous block available. When
 *          the is read/processed the memory is released to the buffer (for
 *          reuse) with a call to Read() with the correct size.
 *
 *          Thread safety is guaranteed by preventing the write pointer to
 *          overtake or become equal to the read pointer, preventing the read
 *          to overtake the write pointer (but they can become equal).
 *          If Poke() / Write() use an old value of the read pointer this would
 *          mean the buffer is 'more full' (or entirely full) at the time,
 *          allowing less data to be inserted.
 *          If Peek() / Read() use an old value of the write pointer this would
 *          mean the buffer is 'more empty' (or completely empty) at the time,
 *          allowing less data to be removed.
 *          The race condition on the wrap pointer is prevented partly by not
 *          allowing Write() and Read() to overtake each other. In addition,
 *          Write() will be the first to pass the wrapping point, Read() at
 *          this point will not use the wrap pointer. When Read() passes the
 *          wrapping point Write() will not use the wrap pointer. In this case
 *          it is irrelevant whether Write() or Read() use an 'old' or 'new'
 *          state, at the time they need the wrap pointer the other is
 *          guaranteed not to alter it.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://en.wikipedia.org/wiki/PEEK_and_POKE
 *          https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
 *          https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    04-2018
 */

#ifndef CONTIGUOUS_RING_BUFFER_HPP_
#define CONTIGUOUS_RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>      // size_t

// Debugging only!
#include <iostream>


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename T>
class ContiguousRingbuffer
{
public:
    ContiguousRingbuffer() noexcept;
    ~ContiguousRingbuffer();

    bool Resize(const size_t size);

    bool Poke(T* &dest, size_t& size);
    bool Write(const size_t size);

    bool Peek(T* &dest, size_t& size);
    bool Read(const size_t size);

    size_t Size(void) const;
    bool Clear(void);

    // Debug
    bool IsLockFree(void) const;
    void Print(void) const;
    void SetState(size_t write, size_t read, size_t wrap);
    bool CheckState(size_t write, size_t read, size_t wrap );

private:
    std::atomic<size_t> mWrite;
    std::atomic<size_t> mRead;
    std::atomic<size_t> mWrap;
    size_t mCapacity;
    T* mElements;
};

/**
 * \brief   Constructor.
 * \details Buffer needs to be set to a size with 'Resize()' before it can be
 *          used.
 */
template<typename T>
ContiguousRingbuffer<T>::ContiguousRingbuffer() noexcept :
    mWrite(0), mRead(0), mWrap(0), mCapacity(0), mElements(nullptr)
{
    ;
}

/**
 * \brief   Destructor.
 * \details Frees memory allocated to the buffer if needed.
 */
template<typename T>
ContiguousRingbuffer<T>::~ContiguousRingbuffer()
{
    if (mElements != nullptr)
    {
        delete [] mElements;
        mElements = nullptr;
    }
}

/**
 * \brief   Resizes the buffer to the given size by freeing and (re)allocating
 *          memory.
 * \details Frees memory allocated to the buffer if needed. Then tries to
 *          allocate the requested memory size.
 *          The buffer allocates 1 element more than requested, this to be able
 *          to differentiate in a thread safe way the buffer full/buffer empty
 *          status.
 * \param   size    Size of the memory to allocate.
 * \returns True if the requested size could be allocated, else false. False if
 *          the requested size equals 0.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Resize(const size_t size)
{
    if (mElements != nullptr)
    {
        delete [] mElements;
        mElements = nullptr;
    }

    if (size > 0)
    {
        mWrite      = 0;
        mRead       = 0;
        mWrap       = size + 1;
        mCapacity   = size + 1;

        mElements = new T[size + 1];

        if (mElements != nullptr)
        {
            return true;
        }
    }

    return false;
}

/**
 * \brief   Check (and return) if a contiguous block of elements is available.
 * \details This can be at the end of the buffer or at the start.
 *          The user is expected to fill the 'dest' with data and 'commit' this
 *          data to the buffer with a call to 'Write()'.
 * \param   dest    Reference to pointer to T. If the method returns true it
 *                  points to the start of the contiguous block, else nullptr.
 *                  The contiguous block can be either at the end, else at the
 *                  start of the buffer.
 * \param   size    Reference to size. If the method returns true it is set to
 *                  largest free contiguous block available, else to 0.
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
    if ((size > 0) && (size < mCapacity))               // Size within valid range?
    {
        const auto write = mWrite.load(std::memory_order_relaxed);
        const auto read  = mRead.load(std::memory_order_acquire);

        if (write >= read)                              // Space at the end?
        {
            if (write < mCapacity)                      // Robustness, condition should always be true
            {
                // Calculate the size available at the end, take into account the extra element when the buffer is empty.
                const size_t available = mCapacity - write - ( (read > 0) ? 0 : 1 );

                if (size <= available)                  // Does the requested block fit?
                {
                    size = available;
                    dest = mElements + write;
                    return true;
                }
                else                                    // Space at the start?
                {
                    if (size < read)                    // Does the requested block fit?
                    {
                        size = read;
                        dest = mElements;
                        return true;
                    }
                }
            }
        }
        else    // write < read                         // Space at the start?
        {
            if ((write + size) < read)                  // Does the requested block fit?
            {
                size = read - write - 1;
                dest = mElements + write;
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
 *          over a contiguous block. If not it it will return false. It handles
 *          the wrap as well, with a twist: if a contiguous block is not
 *          available at the end of the buffer, but it is at the start, it will
 *          shrink the wrap pointer to prevent more elements to be allocated at
 *          the end. A 'Read()' will correct for this when crossing over the
 *          wrap pointer. Note that 'Write()' is not able to pass beyond the
 *          read pointer, meaning a race condition with the wrap is prevented.
 * \param   size    The size to advance the write pointer with.
 * \returns True if the write pointer could be advanced, else false. False if
 *          size is not within valid range.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Write(const size_t size)
{
    if ((size > 0) && (size < mCapacity))
    {
        const auto write = mWrite.load(std::memory_order_relaxed);
        const auto read  = mRead.load(std::memory_order_acquire);

        if (write >= read)                              // Space at the end?
        {
            // Calculate the size available at the end, take into account the extra element when the buffer is empty.
            const size_t available = (write < mCapacity) ? mCapacity -   write - ( (read > 0) ? 0 : 1 ) :
                                                           mCapacity - ( write - ( (read > 0) ? 0 : 1 ) );

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
        else    // write < read                         // Space at the start?
        {
            if ((write + size) < read)                  // Does the requested block fit?
            {
                mWrite.store((write + size), std::memory_order_release);
                return true;
            }
        }
    }

    return false;
}

/**
 * \brief   Check (and return) if a filled contiguous block of elements is
 *          available.
 * \details This can be at the end of the buffer or at the start.
 *          The user is expected to read the 'dest' data and 'release' this
 *          data from the buffer with a 'Read()'.
 * \param   dest   Reference to pointer to T. If the method returns true it
 *                 points to the start of the contiguous block, else nullptr.
 * \param   size   Reference to size. If the method returns true it is set to
 *                 the filled contiguous block of elements, else to
 *                 0. There could be more elements filled then indicated by
 *                 size, this means that the filled contiguous block at the
 *                 end is returned. Once 'Read()' another call to 'Peek() will
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
    if ((size > 0) && (size < mCapacity))               // Size is in valid range?
    {
        const auto read  = mRead.load(std::memory_order_relaxed);
        const auto write = mWrite.load(std::memory_order_acquire);

        if (write >= read)                              // Data at the start?
        {
            if ((read + size) <= write)                 // Requested size available?
            {
                size = write - read;
                dest = mElements + read;
                return true;
            }
        }
        else    // write < read                         // Data at the end?
        {
            if (read < mCapacity)                       // Robustness, condition should always be true
            {
                const auto wrap = mWrap.load(std::memory_order_acquire);

                if ((read + size) <= wrap)              // Requested size available?
                {
                    size = wrap - read;
                    dest = mElements + read;
                    return true;
                }
                // Exception: when read/write were equal at the end of the buffer and a large block was written,
                //            this resulted in wrap being shrunk and becoming equal to read.
                else if (read == wrap)                  // Data available at the start?
                {
                    if (size <= write)                  // Requested size available?
                    {
                        size = write;
                        dest = mElements;
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
 *          Note that 'Read()' is not able to pass beyond the
 *          write pointer, meaning a race condition with the wrap is prevented.
 * \param   size    The size to advance the read pointer with.
 * \returns True if the read pointer could be advanced, else false. False if
 *          size is not within valid range.
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
            const auto wrap = mWrap.load(std::memory_order_acquire);

            if (read_and_size < wrap)                   // Requested size available? And we do not wrap?
            {
                mRead.store(read_and_size, std::memory_order_release);
                return true;
            }
            else if (read_and_size == wrap)             // Requested size available? And we do wrap?
            {
                mWrap.store(mCapacity, std::memory_order_release);
                mRead.store(0, std::memory_order_release);
                return true;
            }
            // Exception: when read/write were equal at the end of the buffer and a large block was written,
            //            this resulted in wrap being shrunk and becoming equal to read.
            else if (read == wrap)                      // Data available at the start?
            {
                if (size <= write)                      // Requested size available?
                {
                    mWrap.store(mCapacity, std::memory_order_release);
                    mRead.store(size, std::memory_order_release);
                    return true;
                }
            }
        }
        // Else: buffer empty
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
size_t ContiguousRingbuffer<T>::Size(void) const
{
    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read  = mRead.load(std::memory_order_acquire);
    const auto wrap  = mWrap.load(std::memory_order_acquire);

    if (write > read)
    {
        return write - read;
    }
    else if (write < read)
    {
        return (wrap - read) + write;
    }
    // Else: write == read --> buffer empty, return 0

	return 0;
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write/read/wrap pointers to the initial state.
 * \returns Always true.
 */
template<typename T>
bool ContiguousRingbuffer<T>::Clear(void)
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);
    mWrap.store(mCapacity, std::memory_order_release);

    return true;
}

/**
 * \brief   Check if atomic operations in the buffer are truly lock-free.
 * \result  Returns true if the atomic operations are lock-free, else false.
 */
template<typename T>
bool ContiguousRingbuffer<T>::IsLockFree(void) const
{
    return (mWrite.is_lock_free() && mRead.is_lock_free() && mWrap.is_lock_free());
}

/**
 * \brief   Debug method to force a state to be set to the mWrite, mRead and
 *          mWrap and  pointers.
 * \param   write   Value to set mWrite to.
 * \param   read    Value to set mRead to.
 * \param   wrap    Value to set mWrap to.
 * \remarks There are no check, so know what you are doing!
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
 * \brief   Debug method to check the state of the mWrite, mRead and mWrap
 *          pointers.
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

/**
 * \brief   Debug method to print the contents of the buffer.
 * \details Prints the contents of the buffer along with the write/read/wrap
 *          pointers and the size for reference.
 */
template<typename T>
void ContiguousRingbuffer<T>::Print(void) const
{
    std::cout << "Read(" << mRead << "), Write(" << mWrite << "), Wrap(" << mWrap << "), Elements[";
    for (size_t i = 0; i < (mCapacity - 1); i++)
    {
        std::cout << mElements[i] << "|";
    }
    std::cout << mElements[(mCapacity - 1)] << "]" << ", Size(" << Size() << ")" << std::endl;
}


#endif // CONTIGUOUS_RING_BUFFER_HPP_
