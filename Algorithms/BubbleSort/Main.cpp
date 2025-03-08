/**
 * \file    BubbleSortExample.cpp
 * \brief   Example usage of the BubbleSort function.
 *
 * This example demonstrates how to utilize the BubbleSort function to sort
 * an array of integers. The program fills an array with random numbers,
 * performs multiple sorting tests, and measures the time taken for each
 * sort operation to evaluate performance.
 *
 * Key Features Demonstrated:
 * - Filling an array with random integers within a specified range.
 * - Running multiple tests to measure the performance of the BubbleSort algorithm.
 * - Measuring the time taken to sort the array for each test.
 * - Calculating and displaying the average time taken across all tests.
 * - Computing and displaying the throughput in terms of numbers sorted per second.
 *
 * The example uses standard C++ libraries for random number generation,
 * timing, and output, showcasing how to integrate the BubbleSort function
 * into a performance evaluation application.
 */

#include <iostream>
#include <random>
#include <chrono>
#include "BubbleSort.hpp"

const int SIZE = 1000;
const int NUM_TESTS = 10; // Number of tests to run for performance measurement
const int SEED = 42; // Fixed seed for reproducibility

void FillRandomNumbers(int* array, size_t size) {
    std::mt19937 eng(SEED); // Seed the generator with a fixed value
    std::uniform_int_distribution<> distr(1, 10000); // Define the range

    for (size_t i = 0; i < size; ++i) {
        array[i] = distr(eng); // Generate random numbers
    }
}

int main() {
    // Create an array to hold the random numbers
    int array[SIZE];

    double totalDuration = 0.0;

    for (int test = 0; test < NUM_TESTS; ++test) {
        // Fill the array with random numbers
        FillRandomNumbers(array, SIZE);

        // Measure the sorting time
        auto start = std::chrono::high_resolution_clock::now();
        BubbleSort(array, SIZE);
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate the duration
        std::chrono::duration<double, std::milli> duration = end - start;
        totalDuration += duration.count();

        // Output the time taken to sort for this test
        std::cout << "Test " << (test + 1) << ": Time taken to sort " << SIZE
                  << " numbers: " << duration.count() << " ms" << std::endl;
    }

    // Calculate and output average time taken
    double averageDuration = totalDuration / NUM_TESTS;
    std::cout << "Average time taken over " << NUM_TESTS << " tests: "
              << averageDuration << " ms" << std::endl;

    // Calculate and output throughput
    double throughput = (SIZE * NUM_TESTS) / (totalDuration / 1000.0); // numbers per second
    std::cout << "Throughput: " << throughput << " numbers/second" << std::endl;

    return 0;
}
