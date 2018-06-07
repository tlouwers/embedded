/**
 * \file 	Ringbuffer.hpp
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
 * 			Suited for embedded use, see URLs below.
 *
 * \details <TBD>	Intended use, example, why is it threadsafe
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Inspiration from:
 *          https://en.wikipedia.org/wiki/PEEK_and_POKE
 *          https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
 *          https://www.boost.org/doc/libs/1_54_0/doc/html/boost/lockfree/spsc_queue.html
 *          http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 0.1
 * \date    05-2018
 */

#ifndef RING_BUFFER_HPP_
#define RING_BUFFER_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>      // size_t
#include <algorithm>    // std::copy()


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename T>
class Ringbuffer
{
public:
    Ringbuffer() noexcept;
    ~Ringbuffer();

    bool Resize(const size_t size);

    bool TryPush(const T* src, const size_t size = 1);
    bool TryPop(T* &dest, const size_t size = 1);

    size_t Size(void) const;
    void Clear(void);

private:
    std::atomic<size_t> mWrite;
    std::atomic<size_t> mRead;
    size_t mCapacity;
    T* mElements;
};


template<typename T>
Ringbuffer<T>::Ringbuffer() noexcept :
    mWrite(0), mRead(0), mCapacity(0), mElements(nullptr)
{
    ;
}

template<typename T>
Ringbuffer<T>::~Ringbuffer()
{
    if (mElements != nullptr)
    {
        delete [] mElements;
        mElements = nullptr;
    }
}

template<typename T>
bool Ringbuffer<T>::Resize(const size_t size)
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
        mCapacity   = size + 1;

        mElements = new T[size + 1];

        if (mElements != nullptr)
        {
            return true;
        }
    }

    return false;
}

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

template<typename T>
size_t Ringbuffer<T>::Size(void) const
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
 *          Set
 * \returns Always true.
 */
template<typename T>
void Ringbuffer<T>::Clear(void)
{
    mWrite.store(0, std::memory_order_release);
    mRead.store(0, std::memory_order_release);

//    for (auto&& element : mElements)
//    {
//        element = nullptr;
//    }
}

/**
 * \brief   Check if atomic operations in the buffer are truly lock-free.
 * \result  Returns true if the atomic operations are lock-free, else false.
 */
template<typename T>
bool Ringbuffer<T>::IsLockFree(void) const
{
    return (mWrite.is_lock_free() && mRead.is_lock_free());
}


#endif // RING_BUFFER_HPP_
