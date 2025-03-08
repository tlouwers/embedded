/**
 * \file    SoftTimerExample.cpp
 * \brief   Example usage of the SoftTimer class.
 *
 * This example demonstrates how to utilize the SoftTimer class to manage
 * different types of timers, including periodic timers, timeout timers,
 * and stopwatch timers. The program creates an instance of SoftTimer,
 * adds various timers, starts them, and simulates timer ticks in a loop.
 *
 * Key Features Demonstrated:
 * - Adding a periodic timer that triggers a callback every few ticks.
 * - Adding a timeout timer that triggers a callback after a specified number of ticks.
 * - Adding a stopwatch timer to count elapsed ticks.
 * - Incrementing the timers in a loop to simulate the passage of time.
 * - Displaying the status of each timer after the simulation.
 *
 * The example uses standard C++ libraries for output and timing, and
 * showcases how to integrate the SoftTimer class into a simple application.
 */

#include <iostream>
#include <chrono>
#include <thread>
#include "SoftTimer.hpp"

// Function to simulate a periodic callback
void PeriodicCallback() {
    std::cout << "Periodic Timer Callback Triggered!" << std::endl;
}

// Function to simulate a timeout callback
void TimeoutCallback() {
    std::cout << "Timeout Timer Callback Triggered!" << std::endl;
}

int main() {
    // Create an instance of SoftTimer
    SoftTimer timerManager;

    // Add a periodic timer that triggers every 3 ticks
    uint8_t periodicTimerId = timerManager.AddPeriodTimer(3, PeriodicCallback);
    if (periodicTimerId) {
        std::cout << "Added Periodic Timer with ID: " << (int)periodicTimerId << std::endl;
    }

    // Add a timeout timer that triggers after 5 ticks
    uint8_t timeoutTimerId = timerManager.AddTimeoutTimer(5, TimeoutCallback);
    if (timeoutTimerId) {
        std::cout << "Added Timeout Timer with ID: " << (int)timeoutTimerId << std::endl;
    }

    // Add a stopwatch timer
    uint8_t stopwatchTimerId = timerManager.AddStopwatchTimer();
    if (stopwatchTimerId) {
        std::cout << "Added Stopwatch Timer with ID: " << (int)stopwatchTimerId << std::endl;
    }

    // Start the timers
    timerManager.StartTimer(periodicTimerId);
    timerManager.StartTimer(timeoutTimerId);
    timerManager.StartTimer(stopwatchTimerId);

    // Simulate timer ticks
    for (int tick = 0; tick < 10; ++tick) {
        std::cout << "Tick: " << tick + 1 << std::endl;

        // Increment the tick for all active timers
        timerManager.IncrementTick();

        // Simulate a delay for the next tick
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Stop the timers
    timerManager.StopTimer(periodicTimerId);
    timerManager.StopTimer(timeoutTimerId);
    timerManager.StopTimer(stopwatchTimerId);

    // Display the status of the timers
    auto periodicStatus = timerManager.GetTimerStatus(periodicTimerId);
    auto timeoutStatus = timerManager.GetTimerStatus(timeoutTimerId);
    auto stopwatchStatus = timerManager.GetTimerStatus(stopwatchTimerId);

    std::cout << "Periodic Timer Status: Type: " << (int)periodicStatus.mType
              << ", State: " << (int)periodicStatus.mState
              << ", Current Value: " << periodicStatus.mValue << std::endl;

    std::cout << "Timeout Timer Status: Type: " << (int)timeoutStatus.mType
              << ", State: " << (int)timeoutStatus.mState
              << ", Current Value: " << timeoutStatus.mValue << std::endl;

    std::cout << "Stopwatch Timer Status: Type: " << (int)stopwatchStatus.mType
              << ", State: " << (int)stopwatchStatus.mState
              << ", Current Value: " << stopwatchStatus.mValue << std::endl;

    return 0;
}
