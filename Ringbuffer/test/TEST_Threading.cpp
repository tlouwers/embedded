#include <gtest/gtest.h>
#include "Ringbuffer.hpp"
#include <cstddef>      // size_t
#include <cstdint>      // uint8_t, uint16_t
#include <thread>
#include <vector>

class RingbufferTestThreading : public ::testing::TestWithParam<std::tuple<uint8_t, uint8_t>> {
protected:
    Ringbuffer<int> ringBuff;
    static const int NR_ITEMS_THREAD_TEST = 2000;
    int refArr[NR_ITEMS_THREAD_TEST] = {};
    int measArr[NR_ITEMS_THREAD_TEST] = {};

    void Producer(const size_t nr_items) {
        EXPECT_GT(nr_items, 0);  // Ensure nr_items is greater than 0
        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_items) {
            volatile bool result = false;
            do {
                sched_yield();
                const int* pSrc = &refArr[i];
                result = ringBuff.TryPush(pSrc, nr_items);
            } while (!result);
        }
    }

    void Consumer(const size_t nr_items) {
        EXPECT_GT(nr_items, 0);  // Ensure nr_items is greater than 0
        for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i += nr_items) {
            volatile bool result = false;
            do {
                sched_yield();
                int* pDest = &measArr[i];
                result = ringBuff.TryPop(pDest, nr_items);
            } while (!result);
        }
    }

    void Threaded_Iteration(size_t buffer_size, uint16_t nr_of_runs) {
        auto [prod_nr_items, cons_nr_items] = GetParam();
        EXPECT_GT(nr_of_runs, 0);
        EXPECT_EQ(NR_ITEMS_THREAD_TEST % prod_nr_items, 0);
        EXPECT_EQ(NR_ITEMS_THREAD_TEST % cons_nr_items, 0);

        for (auto run = 0; run < nr_of_runs; run++) {
            EXPECT_TRUE(ringBuff.Resize(buffer_size));  // Ensure resize is successful
            std::fill(std::begin(measArr), std::end(measArr), 0);

            std::thread prod(&RingbufferTestThreading::Producer, this, prod_nr_items);
            std::thread cons(&RingbufferTestThreading::Consumer, this, cons_nr_items);

            prod.join();
            cons.join();

            for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++) {
                EXPECT_EQ(refArr[i], measArr[i]);  // Validate results
            }
        }
    }
};

TEST_P(RingbufferTestThreading, ThreadingOperations) {
    // Fill the reference array with 'known' values
    for (auto i = 0; i < NR_ITEMS_THREAD_TEST; i++) {
        refArr[i] = i;
    }

    const size_t buffer_size = 15;
    const uint16_t nrOfRuns = 200;

    Threaded_Iteration(buffer_size, nrOfRuns);
}

// Define the parameter combinations
INSTANTIATE_TEST_SUITE_P(
    RingbufferThreadingTests,
    RingbufferTestThreading,
    ::testing::Values(
        std::make_tuple(1, 1),
        std::make_tuple(1, 2),
        std::make_tuple(2, 1),
        std::make_tuple(2, 2),
        std::make_tuple(4, 1),
        std::make_tuple(1, 4),
        std::make_tuple(4, 4)
    )
);
