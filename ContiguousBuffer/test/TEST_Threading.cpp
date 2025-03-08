#include <gtest/gtest.h>
#include "ContiguousRingbuffer.hpp"
#include <cstddef>      // size_t
#include <cstdint>      // uint8_t, uint16_t
#include <thread>


static const int   NR_ITEMS_THREAD_TEST  = 2000;
static int refArr [NR_ITEMS_THREAD_TEST] = {};
static int measArr[NR_ITEMS_THREAD_TEST] = {};


class TEST_Threading : public ::testing::Test {
protected:
    ContiguousRingbuffer<int> mRingBuffer;

    void TearDown() override
    {
        mRingBuffer.Clear();
    };

    void Producer(size_t nr_items)
    {
        const size_t nr_of_items = nr_items;
        EXPECT_GT(nr_of_items, 0);

        int* data_prod = nullptr;
        size_t size_prod;

        for (size_t i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_of_items)
        {
            volatile bool result = false;
            do
            {
                sched_yield();

                size_prod = nr_of_items;
                if (mRingBuffer.Poke(data_prod, size_prod))
                {
                    size_t k = i;
                    for (size_t j = 0; j < nr_of_items; j++)
                    {
                        *data_prod++ = refArr[k++];
                    }

                    result = mRingBuffer.Write(nr_of_items);
                }
            } while (result == false);
        }
    };

    void Consumer(size_t nr_items)
    {
        const size_t nr_of_items = nr_items;
        EXPECT_GT(nr_of_items, 0);

        int* data_cons = nullptr;
        size_t size_cons;

        for (size_t i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_of_items)
        {
            volatile bool result = false;
            do
            {
                sched_yield();

                size_cons = nr_of_items;
                if (mRingBuffer.Peek(data_cons, size_cons))
                {
                    size_t k = i;
                    for (size_t j = 0; j < nr_of_items; j++)
                    {
                        measArr[k++] = *data_cons++;
                    }
                    result = mRingBuffer.Read(nr_of_items);
                }
            } while (result == false);
        }
    };

    void Threaded_Iteration(size_t buffer_size, uint16_t nr_of_runs, uint8_t prod_nr_items, uint8_t cons_nr_items)
    {
        EXPECT_GT(nr_of_runs, 0);
        EXPECT_EQ(NR_ITEMS_THREAD_TEST % prod_nr_items, 0);
        EXPECT_EQ(NR_ITEMS_THREAD_TEST % cons_nr_items, 0);

        for (auto run = 0; run < nr_of_runs; run++)
        {
            EXPECT_TRUE(mRingBuffer.Resize(buffer_size)); // Clears previous state

            // Clear the measurement array for each iteration
            std::fill_n(measArr, NR_ITEMS_THREAD_TEST, 0);

            // Create and immediately start threads
            std::thread prod([this, prod_nr_items]() { Producer(prod_nr_items); });
            std::thread cons([this, cons_nr_items]() { Consumer(cons_nr_items); });

            // Threads running...

            // Cleanup when done
            prod.join();
            cons.join();

            // Validate results
            for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
            {
                EXPECT_EQ(refArr[i], measArr[i]);
            }
        }
    };
};

TEST_F(TEST_Threading, ThreadingOperations)
{
    // Fill the reference array with 'known' values
    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++)
    {
        refArr[i] = i;
    }

    const size_t buffer_size = 15;
    const uint16_t nrOfRuns  = 200;

    // Test cases
    Threaded_Iteration(buffer_size, nrOfRuns, 1, 1);
    Threaded_Iteration(buffer_size, nrOfRuns, 1, 2);
    Threaded_Iteration(buffer_size, nrOfRuns, 2, 1);
    Threaded_Iteration(buffer_size, nrOfRuns, 2, 2);
    Threaded_Iteration(buffer_size, nrOfRuns, 4, 1);
    Threaded_Iteration(buffer_size, nrOfRuns, 1, 4);
    Threaded_Iteration(buffer_size, nrOfRuns, 4, 4);
}
