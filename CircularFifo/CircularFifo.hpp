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
 * \brief   Single-Producer, Single-Consumer, lock free, wait free, circular buffer.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/CircularFifo
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    09-2018
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
    enum { Capacity = Size + 1 };

    CircularFifo() : _tail(0), _head(0) {}
    virtual ~CircularFifo() {}

    bool push(const Element& item);
    bool pop(Element& item);

    bool peek(Element& item);

    bool empty() const;
    bool full() const;
    bool isLockFree() const;
    void clear();

private:
    size_t increment(size_t idx) const;

    std::atomic<size_t>   _tail;      // tail(input) index
    Element               _array[Capacity];
    std::atomic<size_t>   _head;      // head(output) index
};


/**
 * \brief   Push a new item at the position indexed by the tail. After writing the tail is
 *          incremented one step, or wrapped to the beginning if at end of the queue.
 *          The queue grows with the tail.
 *          When the queue is full, there will be a one slot difference between head and
 *          tail. At this point, any writes will fail.
 * \param   item    Constant reference to the element to add to the queue.
 * \result  True if the element was added, false if not (queue full).
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::push(const Element& item)
{
    const auto current_tail = _tail.load(std::memory_order_relaxed);
    const auto next_tail = increment(current_tail);

    if (next_tail != _head.load(std::memory_order_acquire))
    {
        _array[current_tail] = item;
        _tail.store(next_tail, std::memory_order_release);
        return true;
    }

    return false; // full queue
}

/**
 * \brief   Pop the item indexed by the head. The head is moved toward the tail as
 *          it is incremented one step.
 *          The queue shrinks with the head.
 *          When the queue is empty, head and tail will be equal. At this point,
 *          any reads will fail.
 * \param   item    Reference to put the read value into (read from the queue).
 * \result  True if the element was read, false if not (queue empty).
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::pop(Element& item)
{
    const auto current_head = _head.load(std::memory_order_relaxed);

    if (current_head == _tail.load(std::memory_order_acquire))
    {
        return false; // empty queue
    }

    item = _array[current_head];
    _head.store(increment(current_head), std::memory_order_release);

    return true;
}

/**
 * \brief   Peek the item indexed by the head.
 * \param   item    Reference to put the peeked value into (read from the queue).
 * \result  True if the element could be peeked, false if not (queue empty).
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::peek(Element& item)
{
    const auto current_head = _head.load(std::memory_order_relaxed);

    if (current_head == _tail.load(std::memory_order_acquire))
    {
        return false; // empty queue
    }

    item = _array[current_head];

    return true;
}

/**
 * \brief   Checks if the queue is empty.
 * \remark  This is a snapshot, queue status may change by either producer or
 *          consumer before the other accesses it.
 * \result  Returns true if the queue is empty, else false.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::empty() const
{
    // Snapshot with acceptance of that this comparison operation is not atomic.
    return (_head.load() == _tail.load());
}

/**
 * \brief   Checks if the queue is full.
 * \remark  This is a snapshot, queue status may change by either producer or
 *          consumer before the other accesses it.
 * \result  Returns true if the queue is full, else false.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::full() const
{
    const auto next_tail = increment(_tail.load());

    // Snapshot with acceptance that this comparison is not atomic
    return (next_tail == _head.load());
}

/**
 * \brief   Check if atomic operations on the head and tail are truly lock-free.
 * \remark  This can be used as a sanity check to see if the compiler did the right thing.
 * \result  Returns true if the atomic operations on the head and tail are lock-free, else false.
 */
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::isLockFree() const
{
    return (_tail.is_lock_free() && _head.is_lock_free());
}

/**
 * \brief   Clear the queue (by setting head and tail to 0).
 * \remark  Use sparsely.
 * \note    This is NOT thread safe.
 */
template<typename Element, size_t Size>
void CircularFifo<Element, Size>::clear()
{
    _tail.store(0, std::memory_order_release);
    _head.store(0, std::memory_order_release);
}

/**
 * \brief   Method to calculate when the index should be wrapped back to the
 *          beginning in a circular way.
 * \param   idx     The index to increment (and maybe wrap-around).
 * \result  Returns the incremented index, wrapped-around it it exceeds the Capacity.
 */
template<typename Element, size_t Size>
size_t CircularFifo<Element, Size>::increment(size_t idx) const
{
    return (idx + 1) % Capacity;
}

#endif  // CIRCULARFIFO_HPP_
