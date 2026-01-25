/**
 * \file Main.cpp
 *
 * \brief   CPU-bound throughput benchmark for ContiguousRingbuffer.
 *
 * \details  1. The ring buffer is configured using the Reserve() method
 *               (with total usable capacity as ringBufferSize).
 *           2. Both producer and consumer threads work on blocks of up
 *               to 4 integers per iteration at full CPU speed.
 *           3. Global atomic counters record the number of items produced
 *               and consumed.
 *           4. The test runs for exactly 5 seconds, then stops.
 *           5. Final results show throughput (items per second), total
 *               produced/consumed counts, and the consumer's checksum.
 *
 * \note    This benchmark runs at full CPU speed (no sleeps) to measure
 *          the maximum throughput of the ring buffer implementation.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    01-2026
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "ContiguousRingbuffer.hpp"

// Global atomic flag to request threads to stop.
std::atomic<bool> running{true};

// Performance counters
std::atomic<size_t> itemsProduced{0};
std::atomic<size_t> itemsConsumed{0};

// Producer thread: write sequential integers at full speed.
void producer(ContiguousRingbuffer<int>& buffer) {
    int counter = 1;
    while (running.load(std::memory_order_acquire)) {
        // Try to reserve a block for up to 4 integers.
        size_t reqSize = 4;
        int* dest = nullptr;
        if (buffer.ReserveWrite(dest, reqSize) && dest) {
            // Write produced data into the buffer.
            size_t actualSize = reqSize; // actual available block size is returned via reqSize
            for (size_t i = 0; i < actualSize; ++i) {
                dest[i] = counter++;
            }
            // Publish the block.
            if (!buffer.CommitWrite(actualSize)) {
                std::cerr << "Producer: CommitWrite failed\n";
            } else {
                itemsProduced += actualSize;
            }
        }
        // No sleep - run at full CPU speed
    }
}

// Consumer thread: reads integers from the buffer at full speed.
void consumer(ContiguousRingbuffer<int>& buffer, long long& sum) {
    while (running.load(std::memory_order_acquire)) {
        size_t reqSize = 4;
        int* dest = nullptr;
        if (buffer.ReserveRead(dest, reqSize) && dest) {
            size_t actualSize = reqSize;
            for (size_t i = 0; i < actualSize; ++i) {
                sum += dest[i];
                ++itemsConsumed;
            }
            if (!buffer.CommitRead(actualSize)) {
                std::cerr << "Consumer: CommitRead failed\n";
            }
        }
        // No sleep - run at full CPU speed
    }
}

int main() {
    constexpr size_t ringBufferSize = 1024;
    ContiguousRingbuffer<int> ringBuffer;
    if (!ringBuffer.Reserve(ringBufferSize)) {
        std::cerr << "Failed to allocate ring buffer.\n";
        return 1;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "ContiguousRingbuffer CPU Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Ring buffer capacity: " << ringBuffer.Capacity() << std::endl;
    std::cout << "Running for 5 seconds at full CPU speed..." << std::endl;
    std::cout << std::endl;

    long long consumerSum = 0;

    // Record start time.
    auto startTime = std::chrono::high_resolution_clock::now();

    // Start producer and consumer threads.
    std::thread prodThread(producer, std::ref(ringBuffer));
    std::thread consThread(consumer, std::ref(ringBuffer), std::ref(consumerSum));

    // Let the test run for exactly 5 seconds.
    std::this_thread::sleep_for(std::chrono::seconds(5));
    running.store(false, std::memory_order_release);

    // Join the threads.
    prodThread.join();
    consThread.join();

    // Calculate elapsed time.
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double seconds = static_cast<double>(elapsedUs) / 1e6;

    // Display results
    std::cout << "========================================" << std::endl;
    std::cout << "Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Elapsed time:        " << seconds << " seconds" << std::endl;
    std::cout << "Items produced:      " << itemsProduced.load(std::memory_order_relaxed) << std::endl;
    std::cout << "Items consumed:      " << itemsConsumed.load(std::memory_order_relaxed) << std::endl;
    std::cout << "Consumer checksum:   " << consumerSum << std::endl;
    std::cout << std::endl;
    std::cout << "Producer throughput: "
              << static_cast<size_t>(static_cast<double>(itemsProduced.load(std::memory_order_relaxed)) / seconds)
              << " items/sec" << std::endl;
    std::cout << "Consumer throughput: "
              << static_cast<size_t>(static_cast<double>(itemsConsumed.load(std::memory_order_relaxed)) / seconds)
              << " items/sec" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
