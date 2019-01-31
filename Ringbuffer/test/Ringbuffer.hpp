/**
 * \file    Ringbuffer.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   Ringbuffer
 *
 *
 * \brief   Single-Producer, Single-Consumer, lock free, wait free, ringbuffer.
 *          Suited for embedded use, see URLs below.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Ringbuffer
 *
 * \details This code is intended to be used on a Cortex-M4 microcontroller.
 *          Be sure to read up on the documentation, as it has some
 *          other-than-usual behavior.
 *
 *          As example:
 *          Producer will fill the buffer, Consumer will empty the buffer.
 *          Note: do check for the result values, the example omits them for
 *                clarity.
 *
 *          // Declare the buffer
 *          Ringbuffer<int> ringBuff;
 *
 *          // Reserve the size to hold the elements, 'int' in this example
 *          ringBuff.Resize(5);
 *
 *          // If needed, check the number of elements in the buffer
 *          size_t nr_elements = ringBuff.Size();
 *
 *          // Assuming the Producer has 'source', data to put in buffer
 *          int src_arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
 *
 *          // Writing data to the buffer
 *          size_t size = 4;                        // The amount of elements to add to buffer
 *          bool result = TryPush(src_arr, size);
 *
 *          // Assuming the Consumer has a 'destination' to place retrieved data
 *          int dest_arr[10] = { };
 *
 *          // Reading data from the buffer
 *          size_t size = 4;                        // The amount of elements to retrieve from buffer
 *          bool result = TryPop(dest_arr, size);
 *
 *          The intended use in a nutshell:
 *          The Ringbuffer can be used a regular ringbuffer, with the addition
 *          that for a single consumer, single producer it is thread safe.
 *          Assuming an Interrupt Service Routine (ISR) as thread 'Producer'
 *          and the main application loop as thread 'Consumer'.
 *          The Producer will push items to the buffer, where the consumer will
 *          read them. Putting items in the buffer is a copy action, so is
 *          retrieving them. If data cannot be put into buffer or retrieved
 *          from buffer the TryPush() and TryPop() will return with false.
 *          This can happen when the buffer is filling up and the Consumer does
 *          not read data from buffer fast enough.
 *          Although a Size() method is provided it is a snapshot or indication,
 *          due to the threaded nature the buffer contents can have changed
 *          before the method returns.
 *
 *          Thread safety is guaranteed by preventing the write pointer to
 *          overtake or become equal to the read pointer, preventing the read
 *          to overtake the write pointer (but they can become equal).
 *          If TryPush() uses an old value of the read pointer this would
 *          mean the buffer is 'more full' (or entirely full) at the time,
 *          allowing less data to be inserted.
 *          If TryPop() uses an old value of the write pointer this would
 *          mean the buffer is 'more empty' (or completely empty) at the time,
 *          allowing less data to be removed.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
 *          https://www.boost.org/doc/libs/1_54_0/doc/html/boost/lockfree/spsc_queue.html
 *          http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    05-2018
 */

#ifndef RING_BUFFER_HPP_
#define RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>      // size_t
#include <algorithm>    // std::min(), std::copy()

// Debugging only!
#include <iostream>


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
    void Clear();

    bool IsLockFree() const;

    // Debug
    void Print() const;
    void SetState(size_t write, size_t read);
    bool CheckState(size_t write, size_t read);

private:
    std::atomic<size_t> mWrite;
    std::atomic<size_t> mRead;
    size_t mCapacity;
    T* mElements;

    void DeleteBuffer();
};


/**
 * \brief   Constructor.
 * \details Buffer needs to be set to a size with 'Resize()' before it can be
 *          used.
 */
template<typename T>
Ringbuffer<T>::Ringbuffer() noexcept :
    mWrite(0), mRead(0), mCapacity(0), mElements(nullptr)
{
    ;
}

/**
 * \brief   Destructor.
 * \details Frees memory allocated to the buffer if needed.
 */
template<typename T>
Ringbuffer<T>::~Ringbuffer()
{
    DeleteBuffer();
}

/**
 * \brief   Resizes the buffer to the given size by freeing and (re)allocating
 *          memory.
 * \details Frees memory allocated to the buffer if needed. Then tries to
 *          allocate the requested memory size.
 *          The buffer allocates 1 element more than requested, this to be able
 *          to differentiate in a thread safe way the buffer full/buffer empty
 *          status.
 *          Resizing to previous size is allowed, this frees and allocates
 *          memory like other Resize().
 * \param   size    Size of the memory to allocate.
 * \returns True if the requested size could be allocated, else false. False if
 *          the requested size equals 0.
 */
template<typename T>
bool Ringbuffer<T>::Resize(const size_t size) noexcept
{
    DeleteBuffer();

    if (size > 0)
    {
        mWrite    = 0;
        mRead     = 0;
        mCapacity = size + 1;

        mElements = new(std::nothrow) T[size + 1];

        if (mElements != nullptr)
        {
            return true;
        }
    }

    return false;
}

/**
 * \brief   Try to copy 'size' elements from location 'src' into the buffer.
 * \details Underlying copy mechanism is std::copy(). If there is enough space
 *          available this will copy the given elements into the buffer.
 *          Will only succeed if all elements could be copied in buffer.
 * \returns True if all elements could be copied into the buffer. False if not.
 *          False if 'size' is 0 of larger than buffer capacity.
 *          False if 'size' is larger  than the space remaining.
 *          False if 'src' is nullptr.
 */
template<typename T>
bool Ringbuffer<T>::TryPush(const T* src, const size_t size)
{
    if ((size > 0) && (size < mCapacity))               // Size within valid range?
    {
        if (src != nullptr)
        {
            const auto write = mWrite.load(std::memory_order_relaxed);
            const auto read  = mRead.load(std::memory_order_relaxed);

            if (write < mCapacity)                          // Robustness, condition should always be true
            {
                if (write >= read)                          // Space at the end?
                {
                    size_t available = 0;

                    if (write == read)
                    {
                        available = (mCapacity - 1);        // Buffer empty
                    }
                    else if (write == (mCapacity - 1))
                    {
                        available = read;                   // mWrite at the end, read < write
                    }
                    else
                    {
                        available = mCapacity - write - ( (read > 0) ? 0 : 1 );
                    }

                    if (size <= available)
                    {
                        const auto available_upto_end = std::min(size, (mCapacity - write));

                        std::copy(src, (src + available_upto_end), (mElements + write));

                        if ((size - available_upto_end) > 0)
                        {
                            std::copy((src + available_upto_end), (src + size), mElements);
                        }

                        mWrite.store(((write + size) % mCapacity), std::memory_order_release);
                        return true;
                    }
                }
                else     // write < read                    // Space at the start?
                {
                    const auto available = (mCapacity - 1) - ((mCapacity - read) + write);

                    if (size <= available)
                    {
                        std::copy(src, (src + size), (mElements + write));
                        mWrite.store((write + size), std::memory_order_release);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/**
 * \brief   Try to retrieve (copy) 'size' elements to location 'dest' from the
 *          buffer.
 * \details Underlying copy mechanism is std::copy(). If there are enough
 *          elements available this will copy the given amount of elements into
 *          'dest'. Will only succeed if all requested elements could be copied.
 * \returns True if all elements could be copied into 'dest'. False if not.
 *          False if 'size' is 0 of larger than buffer capacity.
 *          False if 'size' is larger  than the number of elements available.
 *          False if 'dest' is nullptr.
 */
template<typename T>
bool Ringbuffer<T>::TryPop(T* &dest, const size_t size)
{
    if ((size > 0) && (size < mCapacity))               // Size within valid range?
    {
        if (dest != nullptr)
        {
            const auto write = mWrite.load(std::memory_order_relaxed);
            const auto read  = mRead.load(std::memory_order_relaxed);

            if (read < mCapacity)                          // Robustness, condition should always be true
            {
                size_t available = 0;

                if (write > read)                          // Data at the start?
                {
                    available = write - read;
                }
                else if (write < read)                     // Data at the end?
                {
                    if (read == (mCapacity - 1))
                    {
                        available = write + 1;             // mRead at the end, write < read
                    }
                    else
                    {
                        available = (mCapacity - read) + write;
                    }
                }
                // Else: buffer empty

                if (size <= available)
                {
                    const auto available_upto_end = std::min(size, (mCapacity - read));

                    std::copy((mElements + read), (mElements + (read + available_upto_end)), dest);

                    if ((size - available_upto_end) > 0)
                    {
                        std::copy(mElements, (mElements + (available - available_upto_end)), (dest + available_upto_end));
                    }

                    mRead.store(((read + size) % mCapacity), std::memory_order_release);
                    return true;
                }
            }
        }
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
size_t Ringbuffer<T>::Size() const
{
    const auto write = mWrite.load(std::memory_order_acquire);
    const auto read  = mRead.load(std::memory_order_acquire);

    if (write > read)
    {
        return write - read;
    }
    else if (write < read)
    {
        return (mCapacity - read) + write;
    }
    // Else: write == read --> buffer empty, return 0

    return 0;
}

/**
 * \brief   Clears the buffer.
 * \details Resets the write and read pointers to the initial state.
 *          Does not clear the elements, they remain in memory until
 *          overwritten or the buffer is destructed.
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

/**
 * \brief   Debug method to force a state to be set to the mWrite, mRead pointers.
 * \param   write   Value to set mWrite to.
 * \param   read    Value to set mRead to.
 * \remarks There are no check, so know what you are doing!
 */
template<typename T>
void Ringbuffer<T>::SetState(size_t write, size_t read)
{
    #warning DEBUG method SetState() enabled - carefull, there be dragons here.

    mWrite.store(write, std::memory_order_release);
    mRead.store(read, std::memory_order_release);
}

/**
 * \brief   Debug method to check the state of the mWrite, mRead pointers.
 * \param   write   Value to check mWrite against.
 * \param   read    Value to check mRead against.
 * \returns True if the state matches, else false.
 */
template<typename T>
bool Ringbuffer<T>::CheckState(size_t write, size_t read)
{
    #warning DEBUG method CheckState() enabled.

    const auto current_write = mWrite.load(std::memory_order_acquire);
    const auto current_read  = mRead.load(std::memory_order_acquire);

    return ( ( write == current_write ) &&
             ( read  == current_read  ) );
}

/**
 * \brief   Debug method to print the contents of the buffer.
 * \details Prints the contents of the buffer along with the write and read
 *          pointers and the size for reference.
 */
template<typename T>
void Ringbuffer<T>::Print() const
{
    std::cout << "Read(" << mRead << "), Write(" << mWrite << "), Elements[";
    for (size_t i = 0; i < (mCapacity - 1); i++)
    {
        std::cout << mElements[i] << "|";
    }
    std::cout << mElements[(mCapacity - 1)] << "]" << ", Size(" << Size() << ")" << std::endl;
}


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
    if (mElements != nullptr)
    {
        delete [] mElements;
        mElements = nullptr;
    }
}


#endif // RING_BUFFER_HPP_
