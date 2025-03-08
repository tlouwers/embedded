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
 * \brief   Single-Producer, Single-Consumer, lock free, wait free, ringbuffer.
 *          Suited for embedded use, see URLs below.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Ringbuffer
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    05-2024
 */

#ifndef RING_BUFFER_HPP_
#define RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>
#include <algorithm>


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

private:
    std::atomic<size_t> mWrite{0};
    std::atomic<size_t> mRead{0};
    size_t mCapacity{0};
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
{ }

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

        return (nullptr != mElements);
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
        if (nullptr != src)
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
        if (nullptr != dest)
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
    const auto write = mWrite.load(std::memory_order_relaxed);
    const auto read  = mRead.load(std::memory_order_relaxed);

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
    if (nullptr != mElements)
    {
        delete [] mElements;
        mElements = nullptr;
    }
}


#endif // RING_BUFFER_HPP_
