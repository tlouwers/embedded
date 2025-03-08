/**
 * \file    MovingAverageExample.cpp
 * \brief   Example usage of the MovingAverage class.
 *
 * This example demonstrates how to utilize the MovingAverage class to compute
 * the moving average of a series of floating-point values. The program creates
 * an instance of MovingAverage, resizes its buffer, and adds a large number
 * of randomly generated values to calculate the average.
 *
 * Key Features Demonstrated:
 * - Resizing the moving average buffer to a user-defined size.
 * - Generating random floating-point values within a specified range.
 * - Adding values to the moving average and calculating the resulting average.
 * - Measuring the time taken to process a large number of values.
 * - Displaying the calculated moving average, elapsed time, and throughput.
 *
 * The example uses standard C++ libraries for random number generation,
 * timing, and formatted output, showcasing how to integrate the MovingAverage
 * class into a performance-oriented application.
 */

#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include "MovingAverage.hpp"

int main() {
    // User-defined parameters
    const uint16_t bufferSize = 1000; // Size of the moving average buffer
    const int numValues = 1000000;     // Number of values to add (increased for longer duration)

    // Create a MovingAverage instance for float type
    MovingAverage<float> movingAverage;

    // Resize the moving average buffer
    if (!movingAverage.Resize(bufferSize)) {
        std::cerr << "Failed to resize the moving average buffer." << std::endl;
        return 1;
    }

    // Random number generator for generating test data with a fixed seed
    std::default_random_engine generator(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> distribution(0.0, 100.0);

    // Clear the moving average before the run
    movingAverage.Fill(0.0f); // Fill with zeros to reset

    // Measure performance
    auto start = std::chrono::high_resolution_clock::now();

    // Add random values to the moving average
    for (int i = 0; i < numValues; ++i) {
        float value = distribution(generator);
        movingAverage.Add(value);
    }

    // Calculate the average
    float average = movingAverage.GetAverage();

    // Measure end time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Output results for this run
    std::cout << "Calculated Moving Average: " << std::fixed << std::setprecision(4) << average
              << ", Time taken: " << elapsed.count() << " seconds"
              << ", Throughput: " << (numValues / elapsed.count()) << " values per second" << std::endl;

    return 0;
}
