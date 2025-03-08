/**
 * \file    CircularFifo.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          Kjell Hedström <hedstrom@kjellkod.cc> wrote this file, with
 *          modifications from <terry.louwers@fourtress.nl>. The latter
 *          modified the license for chance on a beer. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 *
 * \note    This is a modification of the code published by
 *          Kjell Hedström, hedstrom@kjellkod.cc, most recent versions
 *          can be found at: http://www.kjellkod.cc/threadsafecircularqueue
 *
 * \class   CircularFifo
 *
 * \brief   Single-Producer, Single-Consumer, lock-free, wait-free, circular buffer.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/CircularFifo
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    03-2025
 */

#ifndef CIRCULARFIFO_HPP_
#define CIRCULARFIFO_HPP_

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include <atomic>
#include <cstddef>


/******************************************************************************
 * Template Class                                                             *
 *****************************************************************************/
template<typename Element, size_t Size>
class CircularFifo
{
public:
    static constexpr size_t Capacity = Size + 1;

    CircularFifo() : _tail(0), _head(0) {}

    bool push(const Element& item);
    bool pop(Element& item);
    bool peek(Element& item) const;

    bool empty() const;
    bool full() const;
    bool isLockFree() const;
    void clear();

private:
    inline size_t increment(size_t idx) const;

    std::atomic<size_t>   _tail;            // Tail (input) index
    Element               _array[Capacity]; // Circular buffer storage
    std::atomic<size_t>   _head;            // Head (output) index
};

/**
 * \brief   Adds an item to the buffer.
 * \param   item    The element to add.
 * \return  True if the item was added successfully; false if the buffer is full.
 * \note    This method is thread-safe and can be called by a single producer.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::push(const Element& item)
{
    const auto current_tail = _tail.load(std::memory_order_relaxed);
    const auto next_tail = increment(current_tail);

    if (next_tail == _head.load(std::memory_order_acquire))
    {
        return false; // Buffer is full
    }

    _array[current_tail] = item;
    _tail.store(next_tail, std::memory_order_release);
    return true;
}

/**
 * \brief   Removes an item from the buffer.
 * \param   item    Reference to store the removed element.
 * \return  True if an item was removed successfully; false if the buffer is empty.
 * \note    This method is thread-safe and can be called by a single consumer.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::pop(Element& item)
{
    const auto current_head = _head.load(std::memory_order_relaxed);

    if (current_head == _tail.load(std::memory_order_acquire))
    {
        return false; // Buffer is empty
    }

    item = _array[current_head];
    _head.store(increment(current_head), std::memory_order_release);
    return true;
}

/**
 * \brief   Peeks at the item at the head of the buffer without removing it.
 * \param   item    Reference to store the peeked element.
 * \return  True if an item was peeked successfully; false if the buffer is empty.
 * \note    This method is thread-safe and can be called by a single consumer.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::peek(Element& item) const
{
    const auto current_head = _head.load(std::memory_order_relaxed);

    if (current_head == _tail.load(std::memory_order_acquire))
    {
        return false; // Buffer is empty
    }

    item = _array[current_head];
    return true;
}

/**
 * \brief   Checks if the buffer is empty.
 * \remark  This is a snapshot; the queue status may change by either producer
 *          or consumer before the other accesses it.
 * \return  True if the buffer is empty; false otherwise.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::empty() const
{
    // Snapshot with acceptance that this comparison operation is not atomic.
    return _head.load() == _tail.load();
}

/**
 * \brief   Checks if the buffer is full.
 * \remark  This is a snapshot; the queue status may change by either producer or
 *          consumer before the other accesses it.
 * \return  True if the buffer is full; false otherwise.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::full() const
{
    const auto next_tail = increment(_tail.load());

    // Snapshot with acceptance that this comparison is not atomic
    return next_tail == _head.load();
}

/**
 * \brief   Checks if atomic operations on the head and tail are lock-free.
 * \return  True if the atomic operations are lock-free; false otherwise.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::isLockFree() const
{
    return _tail.is_lock_free() && _head.is_lock_free();
}

/**
 * \brief   Clears the buffer by resetting head and tail to zero.
 * \note    This operation is not thread-safe and should be used with caution.
 *          It is recommended to ensure that the buffer is empty before calling this method.
 */
template<typename Element, size_t Size>
void CircularFifo<Element, Size>::clear()
{
    _tail.store(0, std::memory_order_release);
    _head.store(0, std::memory_order_release);
}

/**
 * \brief   Increments the index in a circular manner.
 * \param   idx     The index to increment.
 * \return  The incremented index, wrapped around if it exceeds the capacity.
 */
template<typename Element, size_t Size>
inline size_t CircularFifo<Element, Size>::increment(size_t idx) const
{
    return (idx + 1) % Capacity;
}

#endif  // CIRCULARFIFO_HPP_
