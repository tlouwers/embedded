/**
 * \file Main.cpp
 *
 * \brief   This example application is used to check throughput of the
 *           Ringbuffer, as well as show its use.
 *
 * \details  1. The ring buffer is configured using the Resize() method
 *               (with total usable capacity as ringBufferSize).
 *           2. Both producer and consumer threads work on single integers
 *               per iteration.
 *           3. Global atomic counters record the number of items produced
 *               and consumed.
 *           4. The test’s start time is recorded with high-resolution timers.
 *               After 5 seconds the threads are requested to stop.
 *           5. The elapsed time is computed and throughput (items per second)
 *               is printed along with a summary of produced/consumed counts and
 *               the consumer’s sum.
 *
 * \author  Your Name
 * \version 1.0
 * \date    02-2025
 */

 #include <iostream>
 #include <thread>
 #include <chrono>
 #include <atomic>
 #include "Ringbuffer.hpp"

 // Global atomic flag to request threads to stop.
 std::atomic<bool> running{true};

 // Performance counters
 std::atomic<size_t> itemsProduced{0};
 std::atomic<size_t> itemsConsumed{0};
 long long consumerSum{0};

 // Producer thread: write sequential integers.
 void producer(Ringbuffer<int>& buffer) {
     int counter = 0;
     while (running.load(std::memory_order_acquire)) {
         if (buffer.TryPush(&counter, 1)) { // Push one item at a time
             itemsProduced++;
             counter++; // Increment the counter for the next item
         }
         std::this_thread::yield();
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
     }
 }

 // Consumer thread: reads integers from the buffer.
 void consumer(Ringbuffer<int>& buffer) {
     int dest[2] = { 0 };
     int* pDest = &dest[0];
     while (running.load(std::memory_order_acquire)) {
         if (buffer.TryPop(pDest, 1)) { // Pop one item at a time
             consumerSum += dest[0];
             itemsConsumed++;
         }
         std::this_thread::yield();
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
     }
 }

 int main() {
     constexpr size_t ringBufferSize = 1024;
     Ringbuffer<int> ringBuffer;
     if (!ringBuffer.Resize(ringBufferSize)) {
         std::cerr << "Failed to allocate ring buffer.\n";
         return 1;
     }
     std::cout << "Ring buffer capacity = " << ringBuffer.Capacity() << std::endl;

     // Record start time.
     auto startTime = std::chrono::high_resolution_clock::now();

     // Start producer and consumer threads.
     std::thread prodThread(producer, std::ref(ringBuffer));
     std::thread consThread(consumer, std::ref(ringBuffer));

     // Let the test run for 5 seconds.
     std::this_thread::sleep_for(std::chrono::seconds(5));
     running.store(false, std::memory_order_release);

     // Join the threads.
     prodThread.join();
     consThread.join();

     // Calculate elapsed time.
     auto endTime = std::chrono::high_resolution_clock::now();
     auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

     std::cout << "Test completed." << std::endl;
     std::cout << "Elapsed time: " << elapsedUs / 1e6 << " seconds" << std::endl;
     std::cout << "Items produced: " << itemsProduced.load(std::memory_order_relaxed) << std::endl;
     std::cout << "Items consumed: " << itemsConsumed.load(std::memory_order_relaxed) << std::endl;
     std::cout << "Consumer sum: " << consumerSum << std::endl;

     // Optionally, display throughput (items per second).
     double seconds = elapsedUs / 1e6;
     std::cout << "Producer throughput: "
               << itemsProduced.load(std::memory_order_relaxed) / seconds
               << " items/sec" << std::endl;
     std::cout << "Consumer throughput: "
               << itemsConsumed.load(std::memory_order_relaxed) / seconds
               << " items/sec" << std::endl;

     return 0;
 }
