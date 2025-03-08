#include "../../Catch/catch.hpp"

#include "Ringbuffer.hpp"
#include <cstddef>      // size_t
#include <cstdint>      // uint8_t, uint16_t
#include <thread>


static Ringbuffer<int> ringBuff;            // Our buffer used for testing

static const int   NR_ITEMS_THREAD_TEST  = 2000;
static int refArr [NR_ITEMS_THREAD_TEST] = {};
static int measArr[NR_ITEMS_THREAD_TEST] = {};


void Producer(const size_t nr_items)
{
    // Use a 'const' to prevent changes to 'nr_items'
    REQUIRE(nr_items > 0);

    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_items)
    {
        // Try to put an item in the queue, but queue may be full.
        // Assume Consumer will empty it so we can continue.

        volatile bool result = false;
        do
        {
            sched_yield();

            const int* pSrc = &refArr[i];
            result = ringBuff.TryPush(pSrc, nr_items);
        } while (result == false);
    }
}

void Consumer(const size_t nr_items)
{
    // Use a 'const' to prevent changes to 'nr_items'
    REQUIRE(nr_items > 0);

    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_items)
    {
        // Try to get an item from the queue, but queue may be empty.
        // Assume the Producer will fill the queue.

        volatile bool result = false;
        do
        {
            sched_yield();

            int* pDest = &measArr[i];
            result = ringBuff.TryPop(pDest, nr_items);

        } while (result == false);
    }
}


void Threaded_Iteration(size_t buffer_size, uint16_t nr_of_runs, uint8_t prod_nr_items, uint8_t cons_nr_items)
{
    REQUIRE(nr_of_runs > 0);
    REQUIRE(NR_ITEMS_THREAD_TEST % prod_nr_items == 0);         // As NR_ITEMS_THREAD_TEST is used, be sure we can completely finish our run
    REQUIRE(NR_ITEMS_THREAD_TEST % cons_nr_items == 0);


    for (auto run = 0; run < nr_of_runs; run++)
    {
        REQUIRE(ringBuff.Resize(buffer_size) == true);          // Clears previous state

        // Clear the measurement array for each iteration
        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
        {
            measArr[i] = 0;
        }

        // Create and immediately start threads
        std::thread prod(Producer, prod_nr_items);
        std::thread cons(Consumer, cons_nr_items);

        // Threads running...

        // Cleanup when done
        prod.join();
        cons.join();

        // Validate results
        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
        {
            REQUIRE(refArr[i] == measArr[i]);
        }
    }
}


TEST_CASE( "Ringbuffer threading operations", "[Ringbuffer]" )
{
    // Fill the reference array with 'known' values
    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
    {
        refArr[i] = i;
    }

    const size_t buffer_size = 15;
    const uint16_t nrOfRuns  = 200;

    SECTION( "threading with Producer(1), Consumer(1)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 1, 1);
    }
    SECTION( "threading with Producer(1), Consumer(2)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 1, 2);
    }
    SECTION( "threading with Producer(2), Consumer(1)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 2, 1);
    }
    SECTION( "threading with Producer(2), Consumer(2)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 2, 2);
    }
    SECTION( "threading with Producer(4), Consumer(1)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 4, 1);
    }
    SECTION( "threading with Producer(1), Consumer(4)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 1, 4);
    }
    SECTION( "threading with Producer(4), Consumer(4)" )
    {
        Threaded_Iteration(buffer_size, nrOfRuns, 4, 4);
    }
}
