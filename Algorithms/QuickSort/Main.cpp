/**
 * \file    QuickSortExample.cpp
 * \brief   Example usage of the QuickSort function.
 *
 * This example demonstrates how to utilize the QuickSort function to sort
 * a large array of integers. The program generates an array filled with
 * random numbers, measures the time taken to sort the array, and outputs
 * relevant performance statistics.
 *
 * Key Features Demonstrated:
 * - Generating a large array of random integers within a specified range.
 * - Measuring the time taken to sort the array using the QuickSort algorithm.
 * - Calculating and displaying the duration of the sorting operation.
 * - Printing throughput in terms of elements sorted per second.
 *
 * The example uses standard C++ libraries for random number generation,
 * timing, and formatted output, showcasing how to integrate the QuickSort
 * function into a performance evaluation application.
 */

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip> // For std::setprecision
#include "QuickSort.hpp"

void PrintStatistics(size_t size, double duration) {
    std::cout << "Array Size: " << size << " elements\n";
    std::cout << "Time taken: " << duration << " seconds\n";
    std::cout << std::fixed << std::setprecision(0); // Set fixed format and no decimal places
    std::cout << "Throughput: " << (size / duration) << " elements/second\n";
}

int main() {
    // Set the size of the array
    const size_t ARRAY_SIZE = 1'000'000; // 1 million elements
    std::vector<int> array(ARRAY_SIZE);

    // Generate random numbers with a fixed seed for reproducibility
    std::mt19937 generator(42); // Fixed seed
    std::uniform_int_distribution<int> distribution(0, 1000000); // Random numbers between 0 and 1,000,000

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = distribution(generator);
    }

    // Measure the time taken to sort the array
    auto start = std::chrono::high_resolution_clock::now();
    QuickSort(array.data(), 0, static_cast<int32_t>(array.size() - 1));
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration in seconds
    std::chrono::duration<double> duration = end - start;

    // Print statistics
    PrintStatistics(ARRAY_SIZE, duration.count());

    return 0;
}
