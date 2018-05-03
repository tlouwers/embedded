
#include "ContiguousRingbuffer.hpp"

#include <iostream>
#include <cassert>
#include <cstddef>      // size_t
#include <thread>
#include <chrono>


static const size_t CAPACITY = 5;               // Default size of buffer under test
static ContiguousRingbuffer<int> ringBuff;      // Our buffer used for testing

static const int NR_ITEMS_THREAD_TEST    = 2000;
static int refArr[NR_ITEMS_THREAD_TEST]  = {};
static int measArr[NR_ITEMS_THREAD_TEST] = {};


void Producer(void)
{
    std::cout << "Producer start" << std::endl;

    int* data_prod = nullptr;
    size_t size_prod;

    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
    {
        // Try to put an item in the queue, but queue may be full.
        // Assume Consumer will empty it so we can continue.

        volatile bool result = false;
        do
        {
            sched_yield();

            size_prod = 1;
            if (ringBuff.Poke(data_prod, size_prod))
            {
                *data_prod = refArr[i];
                result = ringBuff.Write(1);
            }
        } while (result == false);
    }

    std::cout << "Producer end" << std::endl;
}

void Consumer(void)
{
    std::cout << "Consumer start" << std::endl;

    int* data_cons = nullptr;
    size_t size_cons;

    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
    {
        // Try to get an item from the queue, but queue may be empty.
        // Assume the Producer will fill the queue.

        volatile bool result = false;
        do
        {
            sched_yield();

            size_cons = 1;
            if (ringBuff.Peek(data_cons, size_cons))
            {
                measArr[i] = *data_cons;
                result = ringBuff.Read(1);
            }
        } while (result == false);
    }

    std::cout << "Consumer end" << std::endl;
}

// Helper method to add an element to buffer
bool AddOne(int val)
{
    int* data = nullptr;
    size_t size = 1;

    bool result = ringBuff.Poke(data, size);
    assert(result);

    if (result)
    {
        data[0] = val;

        result = ringBuff.Write(1);
        assert(result);
    }

    return result;
}

// Helper method to remove an element from buffer
bool RemoveOne(int& val)
{
    int* data = nullptr;
    size_t size = 1;

    bool result = ringBuff.Peek(data, size);
    assert(result);

    if (result)
    {
        val = data[0];

        result = ringBuff.Read(1);
        assert(result);
    }

    return result;
}

// Helper method to add a block of elements to buffer
bool AddBlock(int index_start, size_t block_size)
{
    int* data = nullptr;
    size_t size = block_size;

    bool result = ringBuff.Poke(data, size);
    assert(result);

    if (result)
    {
        for (size_t i = 0; i < block_size; i++)
        {
            data[i] = index_start++;
        }

        size = block_size;
        result = ringBuff.Write(size);
        assert(result);
    }

    return result;
}

// Helper method to remove a block of elements
bool RemoveBlock(size_t block_size)
{
    int* data = nullptr;
    size_t size = block_size;

    bool result = ringBuff.Peek(data, size);
    assert(result);

    if (result)
    {
        // Copy the block values?

        size = block_size;
        result = ringBuff.Read(size);
        assert(result);
    }

    return result;
}


bool TEST_Poke(void)
{
    bool result = ringBuff.Clear();
    assert(result);

    int* data;
    size_t size = 0;

    // -----

    ringBuff.SetState(0, 0, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 3 elements free at start
    assert(result);
    assert(size == 3);

    ringBuff.SetState(1, 0, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 2 elements free
    assert(result);
    assert(size == 2);

    ringBuff.SetState(2, 0, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free
    assert(result);
    assert(size == 1);

    ringBuff.SetState(3, 0, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // Buffer full
    assert(result == false);
    assert(size == 0);

    // -----

    ringBuff.SetState(1, 1, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 3 elements free at end
    assert(result);
    assert(size == 3);

    ringBuff.SetState(0, 1, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // Buffer full
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(2, 1, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 2 elements free at end
    assert(result);
    assert(size == 2);

    ringBuff.SetState(3, 1, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free at end
    assert(result);
    assert(size == 1);

    // -----

    ringBuff.SetState(2, 2, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 2 elements free at end, 1 element free at start not contiguous
    assert(result);
    assert(size == 2);

    ringBuff.SetState(0, 2, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free at start
    assert(result);
    assert(size == 1);


    ringBuff.SetState(1, 2, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // Buffer full
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(3, 2, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free at end, 1 element free at start not contiguous
    assert(result);
    assert(size == 1);

    // -----

    ringBuff.SetState(3, 3, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free at end, 2 elements free at start not contiguous
    assert(result);
    assert(size == 1);

    ringBuff.SetState(0, 3, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 2 elements free at start
    assert(result);
    assert(size == 2);

    ringBuff.SetState(1, 3, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free
    assert(result);
    assert(size == 1);

    ringBuff.SetState(2, 3, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // Buffer full
    assert(result == false);
    assert(size == 0);

    // -----

    return true;
}


bool TEST_Peek(void)
{
    bool result = ringBuff.Clear();
    assert(result);

    int* data;
    size_t size = 0;

    // -----

    ringBuff.SetState(0, 0, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // Buffer empty
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(1, 0, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element at start
    assert(result);
    assert(size == 1);

    ringBuff.SetState(2, 0, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 2 elements at start
    assert(result);
    assert(size == 2);

    ringBuff.SetState(3, 0, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 3 elements at start
    assert(result);
    assert(size == 3);

    // -----

    ringBuff.SetState(1, 1, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // Buffer empty
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(0, 1, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 3 elements at end
    assert(result);
    assert(size == 3);

    ringBuff.SetState(2, 1, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element
    assert(result);
    assert(size == 1);

    ringBuff.SetState(3, 1, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 2 elements
    assert(result);
    assert(size == 2);

    // -----

    ringBuff.SetState(2, 2, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // Buffer empty
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(0, 2, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 2 elements at end
    assert(result);
    assert(size == 2);


    ringBuff.SetState(1, 2, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 2 elements at end, 1 element at start not contiguous
    assert(result);
    assert(size == 2);

    ringBuff.SetState(3, 2, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element
    assert(result);
    assert(size == 1);

    // -----

    ringBuff.SetState(3, 3, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // Buffer empty
    assert(result == false);
    assert(size == 0);

    ringBuff.SetState(0, 3, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element at end
    assert(result);
    assert(size == 1);

    ringBuff.SetState(1, 3, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element at end, 1 element at start not contiguous
    assert(result);
    assert(size == 1);

    ringBuff.SetState(2, 3, 4);

    size = 1;
    result = ringBuff.Peek(data, size);     // 1 element at end, 2 elements at start not contiguous
    assert(result);
    assert(size == 1);

    // -----

    return true;
}


bool TEST_Write(void)
{
    bool result = ringBuff.Clear();
    assert(result);

    size_t size = 0;

    // -----

    ringBuff.SetState(0, 0, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 1;
    result = ringBuff.Write(size);          // 3 elements free at start, can write 1
    assert(result);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 2;
    result = ringBuff.Write(size);          // 3 elements free at start, can write 2
    assert(result);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 3;
    result = ringBuff.Write(size);          // 3 elements free at start, can write 3
    assert(result);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 4;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 0, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    ringBuff.SetState(1, 0, 4);

    size = 1;
    result = ringBuff.Write(size);          // 2 elements free, can write 1
    assert(result);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    ringBuff.SetState(1, 0, 4);

    size = 2;
    result = ringBuff.Write(size);          // 2 elements free, can write 2
    assert(result);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    ringBuff.SetState(1, 0, 4);

    size = 3;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 0, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    ringBuff.SetState(2, 0, 4);

    size = 1;
    result = ringBuff.Write(size);          // 1 element free, can write 1
    assert(result);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    ringBuff.SetState(2, 0, 4);

    size = 2;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 0, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    ringBuff.SetState(3, 0, 4);

    size = 1;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    // -----

    ringBuff.SetState(1, 1, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);

    ringBuff.SetState(1, 1, 4);

    size = 1;
    result = ringBuff.Write(size);          // 3 elements free at end, can write 1
    assert(result);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    ringBuff.SetState(1, 1, 4);

    size = 2;
    result = ringBuff.Write(size);          // 3 elements free at end, can write 2
    assert(result);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    ringBuff.SetState(1, 1, 4);

    size = 3;
    result = ringBuff.Write(size);          // 3 elements free at end, can write 3, wraps
    assert(result);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    ringBuff.SetState(1, 1, 4);

    size = 4;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 1, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    ringBuff.SetState(0, 1, 4);

    size = 1;
    result = ringBuff.Write(size);         // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 1, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    ringBuff.SetState(2, 1, 4);

    size = 1;
    result = ringBuff.Write(size);          // 2 elements free at end, can write 1
    assert(result);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    ringBuff.SetState(2, 1, 4);

    size = 2;
    result = ringBuff.Write(size);          // 2 elements free at end, can write 2, wraps
    assert(result);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    ringBuff.SetState(2, 1, 4);

    size = 3;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 1, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    ringBuff.SetState(3, 1, 4);

    size = 1;
    result = ringBuff.Write(size);          // 1 element free at end, can write 1, wraps
    assert(result);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    ringBuff.SetState(3, 1, 4);

    size = 2;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    // -----

    ringBuff.SetState(2, 2, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    ringBuff.SetState(2, 2, 4);

    size = 1;
    result = ringBuff.Write(size);          // 3 elements free, 1 element free at start not contiguous, can write 1
    assert(result);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(2, 2, 4);

    size = 2;
    result = ringBuff.Write(size);          // 3 elements free, 1 element free at start not contiguous, can write 2
    assert(result);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    ringBuff.SetState(2, 2, 4);

    size = 3;
    result = ringBuff.Write(size);          // Fails, cannot write 2 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    ringBuff.SetState(2, 2, 4);

    size = 4;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 2, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    ringBuff.SetState(0, 2, 4);

    size = 1;
    result = ringBuff.Write(size);          // 1 element free, can write 1
    assert(result);
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    ringBuff.SetState(0, 2, 4);

    size = 2;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 2, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    ringBuff.SetState(1, 2, 4);

    size = 1;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 2, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(3, 2, 4);

    size = 1;
    result = ringBuff.Write(size);          // 2 elements free, 1 element free at start not contiguous, can write 1, wraps
    assert(result);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    ringBuff.SetState(3, 2, 4);

    size = 2;
    result = ringBuff.Write(size);          // Fails, cannot write 2 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(3, 2, 4);

    size = 3;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    // -----

    ringBuff.SetState(3, 3, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 1;
    result = ringBuff.Write(size);          // 3 elements free, 2 elements free at start not contiguous, can write 1, wraps
    assert(result);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 2;
    result = ringBuff.Write(size);          // 3 elements free, 2 elements free at start not contiguous, can write 2 at start, wrap is shrunk by 1
    assert(result);
    result = ringBuff.CheckState(2, 3, 3);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 3;
    result = ringBuff.Write(size);          // Fails, cannot write 3 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 4;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 3, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(0, 3, 4);

    size = 1;
    result = ringBuff.Write(size);          // 2 elements free at start, can write 1
    assert(result);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    ringBuff.SetState(0, 3, 4);

    size = 2;
    result = ringBuff.Write(size);          // 2 elements free at start, can write 2
    assert(result);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    ringBuff.SetState(0, 3, 4);

    size = 3;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 3, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    ringBuff.SetState(1, 3, 4);

    size = 1;
    result = ringBuff.Write(size);          // 1 element free, can write 1
    assert(result);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    ringBuff.SetState(1, 3, 4);

    size = 2;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 3, 4);

    size = 0;
    result = ringBuff.Write(size);          // Fails: cannot write 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    ringBuff.SetState(2, 3, 4);

    size = 1;
    result = ringBuff.Write(size);          // Buffer full
    assert(result == false);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    // -----

    return true;
}


bool TEST_Read(void)
{
    bool result = ringBuff.Clear();
    assert(result);

    size_t size = 0;

    // -----

    ringBuff.SetState(0, 0, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 1;
    result = ringBuff.Read(size);           // Fails: buffer empty
    assert(result == false);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 0, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    ringBuff.SetState(1, 0, 4);

    size = 1;
    result = ringBuff.Read(size);           // 1 element available at start, can read 1
    assert(result);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);

    ringBuff.SetState(1, 0, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: only 1 element available at start
    assert(result == false);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 0, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    ringBuff.SetState(2, 0, 4);

    size = 1;
    result = ringBuff.Read(size);           // 2 elements available at start, can read 1
    assert(result);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    ringBuff.SetState(2, 0, 4);

    size = 2;
    result = ringBuff.Read(size);           // 2 elements available at start, can read 2
    assert(result);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    ringBuff.SetState(2, 0, 4);

    size = 3;
    result = ringBuff.Read(size);           // Fails: only 2 elements available at start
    assert(result == false);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 0, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    ringBuff.SetState(3, 0, 4);

    size = 1;
    result = ringBuff.Read(size);           // 3 elements available at start, can read 1
    assert(result);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    ringBuff.SetState(3, 0, 4);

    size = 2;
    result = ringBuff.Read(size);           // 3 elements available at start, can read 2
    assert(result);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(3, 0, 4);

    size = 3;
    result = ringBuff.Read(size);            // 3 elements available at start, can read 3
    assert(result);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 0, 4);

    size = 4;
    result = ringBuff.Read(size);           // Fails: only 3 elements available at start
    assert(result == false);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    // -----

    ringBuff.SetState(1, 1, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);

    ringBuff.SetState(1, 1, 4);

    size = 1;
    result = ringBuff.Read(size);           // Fails: buffer empty
    assert(result == false);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 1, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    ringBuff.SetState(0, 1, 4);

    size = 1;
    result = ringBuff.Read(size);           // 3 elements available at end, can read 1
    assert(result);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    ringBuff.SetState(0, 1, 4);

    size = 2;
    result = ringBuff.Read(size);           // 3 elements available at end, can read 2
    assert(result);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(0, 1, 4);

    size = 3;
    result = ringBuff.Read(size);           // 3 elements available at end, can read 3, wraps
    assert(result);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    ringBuff.SetState(0, 1, 4);

    size = 4;
    result = ringBuff.Read(size);           // Fails: only 3 elements available at end
    assert(result == false);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 1, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    ringBuff.SetState(2, 1, 4);

    size = 1;
    result = ringBuff.Read(size);           // 1 element available, can read 1
    assert(result);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    ringBuff.SetState(2, 1, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: only 1 element available
    assert(result == false);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 1, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    ringBuff.SetState(3, 1, 4);

    size = 1;
    result = ringBuff.Read(size);           // 2 elements available, can read 1
    assert(result);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(3, 1, 4);

    size = 2;
    result = ringBuff.Read(size);           // 2 elements available, can read 2
    assert(result);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 1, 4);

    size = 3;
    result = ringBuff.Read(size);           // Fails: only 2 elements available
    assert(result == false);
    result = ringBuff.CheckState(3, 1, 4);
    assert(result);

    // -----

    ringBuff.SetState(2, 2, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    ringBuff.SetState(2, 2, 4);

    size = 1;
    result = ringBuff.Read(size);           // Fails: buffer empty
    assert(result == false);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 2, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    ringBuff.SetState(0, 2, 4);

    size = 1;
    result = ringBuff.Read(size);           // 2 elements available at end, can read 1
    assert(result);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(0, 2, 4);

    size = 2;
    result = ringBuff.Read(size);           // 2 elements available at end, can read 2, wraps
    assert(result);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    ringBuff.SetState(0, 2, 4);

    size = 3;
    result = ringBuff.Read(size);           // Fails: only 2 elements available
    assert(result == false);
    result = ringBuff.CheckState(0, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 2, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    ringBuff.SetState(1, 2, 4);

    size = 1;
    result = ringBuff.Read(size);           // 2 elements available at end, 1 not contiguous, can read 1
    assert(result);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    ringBuff.SetState(1, 2, 4);

    size = 2;
    result = ringBuff.Read(size);           // 2 elements available at end, 1 not contiguous, can read 2
    assert(result);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    ringBuff.SetState(1, 2, 4);

    size = 3;
    result = ringBuff.Read(size);           // Fails: 2 elements available at end, 1 not contiguous
    assert(result == false);
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    // --

    ringBuff.SetState(3, 2, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    ringBuff.SetState(3, 2, 4);

    size = 1;
    result = ringBuff.Read(size);           // 1 element available, can read 1
    assert(result);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 2, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: 1 element available
    assert(result == false);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    // -----

    ringBuff.SetState(3, 3, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 1;
    result = ringBuff.Read(size);           // Fails: buffer empty
    assert(result == false);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(0, 3, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(0, 3, 4);

    size = 1;
    result = ringBuff.Read(size);           // 1 element available at the end, can read 1, wraps
    assert(result);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    ringBuff.SetState(0, 3, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: 1 element available
    assert(result == false);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(1, 3, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    ringBuff.SetState(1, 3, 4);

    size = 1;
    result = ringBuff.Read(size);           // 2 elements available, 1 not contiguous, can read 1, wraps
    assert(result);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    ringBuff.SetState(1, 3, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: 2 elements available, 1 not contiguous
    assert(result == false);
    result = ringBuff.CheckState(1, 3, 4);
    assert(result);

    // --

    ringBuff.SetState(2, 3, 4);

    size = 0;
    result = ringBuff.Read(size);           // Fails: cannot read 0 elements
    assert(result == false);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    ringBuff.SetState(2, 3, 4);

    size = 1;
    result = ringBuff.Read(size);           // 3 elements available, 2 not contiguous, can read 1, wraps
    assert(result);
    result = ringBuff.CheckState(2, 0, 4);
    assert(result);

    ringBuff.SetState(2, 3, 4);

    size = 2;
    result = ringBuff.Read(size);           // Fails: 3 elements available, 2 not contiguous
    assert(result == false);
    result = ringBuff.CheckState(2, 3, 4);
    assert(result);

    // -----

    return true;
}


bool TEST_Resize(void)
{
    const size_t SMALL_CAPACITY = 3;
    const size_t LARGE_CAPACITY = 30;

    bool result = ringBuff.Resize(CAPACITY);
    assert(result);

    int* data = nullptr;
    size_t size = 0;

    size = 1;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == CAPACITY);

    size = ringBuff.Size();
    assert(size == 0);

    AddOne(1);
    AddOne(2);

    size = ringBuff.Size();
    assert(size == 2);

    // Small capacity: resize to 3
    result = ringBuff.Resize(SMALL_CAPACITY);
    assert(result);

    size = 1;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == SMALL_CAPACITY);

    size = ringBuff.Size();
    assert(size == 0);

    AddOne(1);
    AddOne(2);

    size = ringBuff.Size();
    assert(size == 2);

     // Large capacity: resize to 30
    result = ringBuff.Resize(LARGE_CAPACITY);
    assert(result);

    size = 1;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == LARGE_CAPACITY);

    size = ringBuff.Size();
    assert(size == 0);

    AddOne(1);
    AddOne(2);

    size = ringBuff.Size();
    assert(size == 2);

    result = ringBuff.Clear();
    assert(result);

    size = ringBuff.Size();
    assert(size == 0);

    return true;
}


bool TEST_RunAround(void)
{
    bool result = ringBuff.Resize(CAPACITY);
    assert(result);

    int index = 1;

    std::cout << std::endl << std::endl;

    for (auto i = 0; i < 42; i++)
    {
        result = AddOne(index++);
        assert(result);
        ringBuff.Print();

        int val = -1;
        result = RemoveOne(val);
        assert(result);
        std::cout << "Read: " << val << std::endl;
    }

    return true;
}


bool TEST_WriteAndRead(void)
{
    bool result = ringBuff.Resize(3);
    assert(result);

    int index = 1;
    int val = -1;

    result = ringBuff.CheckState(0, 0, 4);
    assert(result);

    result = AddOne(index++);
    assert(result);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);
    assert(val == 1);

    result = AddOne(index++);
    assert(result);
    result = ringBuff.CheckState(2, 1, 4);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    result = ringBuff.CheckState(2, 2, 4);
    assert(result);
    assert(val == 2);

    result = AddOne(index++);
    assert(result);
    result = ringBuff.CheckState(3, 2, 4);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    result = ringBuff.CheckState(3, 3, 4);
    assert(result);
    assert(val == 3);

    result = AddOne(index++);
    assert(result);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    result = ringBuff.CheckState(0, 0, 4);
    assert(result);
    assert(val == 4);

    result = AddOne(index++);
    assert(result);
    result = ringBuff.CheckState(1, 0, 4);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    result = ringBuff.CheckState(1, 1, 4);
    assert(result);
    assert(val == 5);

    // -----

    int* data = nullptr;

    ringBuff.SetState(3, 3, 4);

    size_t size = 1;
    result = ringBuff.Poke(data, size);     // 3 elements, 1 at end, other 2 at start not contiguous
    assert(result);
    assert(size == 1);
    result = ringBuff.Write(1);
    assert(result);
    result = ringBuff.CheckState(0, 3, 4);
    assert(result);

    ringBuff.SetState(0, 0, 4);

    size = 1;
    result = ringBuff.Poke(data, size);     // 3 elements free at end
    assert(result);
    assert(size == 3);
    result = ringBuff.Write(3);
    assert(result);
    result = ringBuff.CheckState(3, 0, 4);
    assert(result);

    size = 1;
    result = ringBuff.Poke(data, size);     // Buffer full: 3 elements placed
    assert(result == false);
    assert(size == 0);

    result = RemoveOne(val);
    assert(result);

    size = 1;
    result = ringBuff.Poke(data, size);     // 1 element free at end
    assert(result);
    assert(size == 1);
    result = ringBuff.Write(1);
    assert(result);
    result = ringBuff.CheckState(0, 1, 4);
    assert(result);

    // -----

    ringBuff.Resize(CAPACITY);

    result = AddOne(1);
    assert(result);
    size = ringBuff.Size();
    assert(size == 1);
    result = AddOne(2);
    assert(result);
    size = ringBuff.Size();
    assert(size == 2);
    result = AddOne(3);
    assert(result);
    size = ringBuff.Size();
    assert(size == 3);
    result = AddOne(4);
    assert(result);
    size = ringBuff.Size();
    assert(size == 4);
    result = AddOne(5);
    assert(result);
    size = ringBuff.Size();
    assert(size == 5);

    result = RemoveOne(val);
    assert(result);
    assert(val == 1);
    size = ringBuff.Size();
    assert(size == 4);
    result = RemoveOne(val);
    assert(result);
    assert(val == 2);
    size = ringBuff.Size();
    assert(size == 3);
    result = RemoveOne(val);
    assert(result);
    assert(val == 3);
    size = ringBuff.Size();
    assert(size == 2);

    result = AddOne(6);     // Write wraps, value at end of buffer
    assert(result);
    size = ringBuff.Size();
    assert(size == 3);
    result = ringBuff.CheckState(0, 3, 6);
    assert(result);

    result = RemoveOne(val);
    assert(result);
    assert(val == 4);
    size = ringBuff.Size();
    assert(size == 2);
    result = RemoveOne(val);
    assert(result);
    assert(val == 5);
    size = ringBuff.Size();
    assert(size == 1);
    result = RemoveOne(val);
    assert(result);
    assert(val == 6);
    size = ringBuff.Size();
    assert(size == 0);

    // -----

    ringBuff.Resize(3);

    result = AddOne(0);
    assert(result);
    result = AddOne(1);
    assert(result);
    result = AddOne(2);
    assert(result);
    result = RemoveOne(val);
    assert(result);
    assert(val == 0);
    result = RemoveOne(val);
    assert(result);
    assert(val == 1);
    result = AddOne(3);
    assert(result);

    // Had threading issue here: depending on the value of read the available space is calculated incorrectly.

    data = nullptr;
    size = 1;

    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == 1);
    data[0] = 4;
    result = ringBuff.Write(1);
    assert(result);
    ringBuff.Print();
    result = ringBuff.CheckState(1, 2, 4);
    assert(result);

    return true;
}


bool TEST_Wrap(void)
{
    bool result = true;

    int* data;
    size_t size = 0;

    // -----

    result &= ringBuff.Resize(3);
    assert(result);

    ringBuff.SetState(3, 3, 4);

    size = 2;
    result &= ringBuff.Poke(data, size);
    assert(result);
    assert(size == 2);
    result &= ringBuff.Write(2);
    assert(result);
    result &= ringBuff.CheckState(2, 3, 3);
    assert(result);

    int val = ringBuff.Size();
    assert(val == 2);

    result &= ringBuff.Read(1);
    assert(result);
    result &= ringBuff.CheckState(2, 1, 4);
    assert(result);

    val = ringBuff.Size();
    assert(val == 1);

    // --

    ringBuff.SetState(3, 3, 4);

    size = 2;
    result &= ringBuff.Poke(data, size);
    assert(result);
    assert(size == 2);
    result &= ringBuff.Write(2);
    assert(result);
    result &= ringBuff.CheckState(2, 3, 3);
    assert(result);

    result &= ringBuff.Read(2);
    assert(result);
    result &= ringBuff.CheckState(2, 2, 4);
    assert(result);

    val = ringBuff.Size();
    assert(val == 0);

    // --

    ringBuff.SetState(3, 3, 4);

    size = 3;
    result &= ringBuff.Poke(data, size);
    assert(result == false);
    assert(size == 0);

    // -----

    result = true;
    result &= ringBuff.Resize(4);
    assert(result);

    ringBuff.SetState(3, 3, 5);

    result &= AddOne(4);
    assert(result);

    size = 2;
    result &= ringBuff.Poke(data, size);
    assert(result);
    assert(size == 2);
    data[0] = 1;
    data[1] = 2;
    result &= ringBuff.Write(2);
    assert(result);
    result &= ringBuff.CheckState(2, 3, 4);
    assert(result);

    val = ringBuff.Size();
    assert(val == 3);

    size = 2;
    result &= ringBuff.Peek(data, size);
    assert(result == false);
    assert(size == 0);

    result = true;
    size = 1;
    result &= ringBuff.Peek(data, size);
    assert(result);
    assert(data[0] == 4);
    result &= ringBuff.Read(1);
    assert(result);
    result &= ringBuff.CheckState(2, 0, 5);
    assert(result);

    val = ringBuff.Size();
    assert(val == 2);

    size = 1;
    result &= ringBuff.Peek(data, size);
    assert(result);
    assert(size == 2);

    // --

    ringBuff.SetState(3, 3, 5);

    result &= AddOne(4);
    assert(result);

    size = 2;
    result &= ringBuff.Poke(data, size);
    assert(result);
    assert(size == 2);
    data[0] = 1;
    data[1] = 2;
    result &= ringBuff.Write(2);
    assert(result);
    result &= ringBuff.CheckState(2, 3, 4);
    assert(result);

    result &= RemoveOne(val);
    assert(result);
    assert(val == 4);
    result &= ringBuff.CheckState(2, 0, 5);
	assert(result);

    size = 1;
    result &= ringBuff.Peek(data, size);
    assert(result);
    assert(size == 2);
    assert(data[0] == 1);
    assert(data[1] == 2);

    result &= ringBuff.Read(2);
    assert(result);
    result &= ringBuff.CheckState(2, 2, 5);
    assert(result);

    val = ringBuff.Size();
    assert(val == 0);

    // -----

    result = ringBuff.Resize(4);
    assert(result);

    ringBuff.SetState(4, 4, 5);                 // Filled 4 elements, removed 4 elements

    size = 2;
    result &= ringBuff.Poke(data, size);
    assert(result);
    data[0] = 7;
    data[1] = 8;
    size = 2;
    result &= ringBuff.Write(size);
    assert(result);

    result &= ringBuff.CheckState(2, 4, 4);
    assert(result);

    size = 1;
    result &= ringBuff.Peek(data, size);        // Elements available from the start, 2 elements
    assert(result);
    size = ringBuff.Size();
    assert(size == 2);
    assert(data[0] == 7);
    assert(data[1] == 8);
    size = 2;
    result &= ringBuff.Read(size);
    assert(result);

    result &= ringBuff.CheckState(2, 2, 5);
    assert(result);

    return true;
}


bool TEST_Blocks(void)
{
    bool result = true;

    const size_t block_size = 256;

    result &= ringBuff.Resize(block_size * 4);
    assert(result);

    int index = 0;

    ringBuff.SetState(1024, 1024, 1025);        // Filled 4 blocks, removed 4 blocks

    result &= AddBlock(index, block_size);      // Block written at the start, wrap reduced from 1025 to 1024
    assert(result);                             // 1 block = 256 elements in buffer
    index += block_size;
    result &= AddBlock(index, block_size);
    assert(result);                             // 2 blocks = 512 elements in buffer
    index += block_size;
    result &= AddBlock(index, block_size);
    assert(result);                             // 3 blocks = 768 elements in buffer

    int* data = nullptr;
    size_t size = block_size;
    result &= ringBuff.Poke(data, size);        // Cannot add another block, 1 element short
    assert(result == false);

    result = true;
    size = block_size;
    result &= ringBuff.Peek(data, size);        // Elements available from the start, 3 blocks
    assert(result);
    size = ringBuff.Size();
    assert(size == 768);

    size = block_size;
    result &= ringBuff.Read(size);
    assert(result);

    result &= ringBuff.CheckState(768, 256, 1025);
    assert(result);
    size = ringBuff.Size();
    assert(size == 512);                        // After read still 2 blocks available

    result &= RemoveBlock(block_size);
    assert(result);                             // 1 block = 256 elements in buffer
    result &= RemoveBlock(block_size);
    assert(result);                             // Buffer empty
    size = ringBuff.Size();
    assert(size == 0);

    return result;
}


bool TEST_FoundIssues(void)
{
    int* data   = nullptr;
    size_t size = 0;
    int val     = -1;

    // -----

    bool result = ringBuff.Resize(8);
    assert(result);

    ringBuff.SetState(5, 5, 9);

    result = AddOne(6);
    assert(result);
    result = AddOne(7);
    assert(result);
    result = ringBuff.CheckState(7, 5, 9);
    assert(result);
    size = ringBuff.Size();
    assert(size == 2);

    size = 2;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == 2);
    size = 3;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == 4);
    size = 5;
    result = ringBuff.Poke(data, size);
    assert(result == false);
    assert(size == 0);
    size = 4;
    result = ringBuff.Poke(data, size);
    assert(result);
    assert(size == 4);
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    result = ringBuff.Write(3);
    assert(result);
    result = ringBuff.CheckState(3, 5, 7);
    assert(result);
    size = ringBuff.Size();
    assert(size == 5);

    size = 2;
    result = ringBuff.Peek(data, size);
    assert(result);
    assert(size == 2);
    size = 3;
    result = ringBuff.Peek(data, size);
    assert(result == false);
    assert(size == 0);

    result = RemoveOne(val);
    assert(result);
    assert(val == 6);
    result = RemoveOne(val);
    assert(result);
    assert(val == 7);
    result = ringBuff.CheckState(3, 0, 9);
    assert(result);
    size = ringBuff.Size();
    assert(size == 3);

    size = 3;
    result = ringBuff.Peek(data, size);
    assert(result);
    assert(size == 3);
    assert(data[0] == 1);
    assert(data[1] == 2);
    assert(data[2] == 3);
    result = ringBuff.Read(3);
    assert(result);
    result = ringBuff.CheckState(3, 3, 9);
    assert(result);
    size = ringBuff.Size();
    assert(size == 0);

    // -----

    result = ringBuff.Resize(4);
    assert(result);

    ringBuff.SetState(4, 0, 5);     // Buffer full

    size = 1;
    result = ringBuff.Poke(data, size);
    assert(result == false);

    ringBuff.SetState(3, 0, 5);     // Space for 1 element available

    size = 1;
    result = ringBuff.Poke(data, size);
    assert(result);

    // -----

    result = ringBuff.Resize(4);
    assert(result);

    ringBuff.SetState(5, 0, 5);     // Impossible

    size = 1;
    result = ringBuff.Write(size);
    assert(result == false);

    ringBuff.SetState(4, 0, 5);     // Buffer full

    size = 1;
    result = ringBuff.Write(size);
    assert(result == false);

    ringBuff.SetState(3, 0, 5);     // Space for 1 element available

    size = 1;
    result = ringBuff.Write(size);
    assert(result);

    // -----

    return result;
}


int main(void)
{
    bool result = true;
    (void)result;           // Remove compiler warning

    // Set default for the buffer under test
    ringBuff.Resize(3);

    // Add linker option: link to atomic: -latomic
    result = ringBuff.IsLockFree();
    assert(result);

    result = TEST_Poke();
    assert(result);

    result = TEST_Peek();
    assert(result);

    result = TEST_Write();
    assert(result);

    result = TEST_Read();
    assert(result);

    result = TEST_WriteAndRead();
    assert(result);

    result = TEST_Resize();
    assert(result);

    result = TEST_RunAround();
    assert(result);

    result = TEST_Wrap();
    assert(result);

    result = TEST_Blocks();
    assert(result);

    result = TEST_FoundIssues();
    assert(result);


    // Threading tests
    const int nrOfRuns = 100;
    std::cout << std::endl << std::endl << std::endl;

    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
    {
        refArr[i]  = i;                 // Fill the reference array
    }

    for (auto run = 0; run < nrOfRuns; run++)
    {
        //ringBuff.Resize(CAPACITY);    // Clears previous state
        ringBuff.Resize(15);          // Clears previous state

        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
        {
            measArr[i] = 0;             // Clear the measurement array
        }

        std::cout << "Starting run: " << run << std::endl;

        auto start = std::chrono::steady_clock::now();

        std::thread prod(Producer);     // prod starts running
        std::thread cons(Consumer);     // cons starts running

        // Threads running

        prod.join();
        cons.join();

        auto end = std::chrono::steady_clock::now();

        // Validate results
        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
        {
            if (refArr[i] != measArr[i])
            {
                std::cout << "Error found during run: " << run << ", at location: " << i << ", items: [" << refArr[i] << " -- " << measArr[i] << "]" << std::endl;
            }
        }

        auto diff = end - start;
        std::cout << "Completed run: " << run << ", duration: " << std::chrono::duration<double, std::micro>(diff).count() << " microseconds" << std::endl << std::endl << std::endl;
    }



    int dummy;
	std::cin >> dummy;

    return 0;
}
