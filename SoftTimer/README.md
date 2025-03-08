# SoftTimer

## Overview
The `SoftTimer` class provides a flexible and extendable solution for managing software timers based on a defined hardware timer tick. This allows for precise timing intervals, accommodating a wide range of applications from millisecond-level timing to timers that can run for days.

## Features
The `SoftTimer` class supports three types of timers:

- **Period Timer**: Triggers a callback function at regular intervals, resetting itself after each callback to create periodic events.
- **Timeout Timer**: Executes a callback function once when the timer expires, effectively acting as a single-shot timer.
- **Stopwatch Timer**: Counts the number of timer periods until stopped, allowing retrieval of the elapsed time via `GetTimerStatus()`.

## Requirements
- Compatible with ST Microelectronics STM32F407G-DISC1 (easily portable to other ST microcontrollers).
- Requires C++11 or later.
- A mechanism to provide a steady 'tick' for incrementing the `SoftTimer`.

## Usage Notes
- The `SoftTimer` can manage up to 254 timers using a single increment method to track elapsed time. However, increased timer usage may lead to overhead that could affect timing accuracy. It is advisable to measure performance under expected conditions.
- The accuracy of the software timers is directly influenced by the frequency of the tick. For example, if a timer interrupt service routine (ISR) ticks every 100 ms, all timers will execute their callbacks within this period without drift.
- To minimize execution time during timer callbacks, keep the callback functions lightweight. Use them primarily to set flags that are processed later in the main loop. Heavy processing in callbacks can quickly exceed timing limits.

If you encounter any issues or have suggestions for improvements, please reach out with a reproducible scenario or code enhancements.

## Example Usage
```cpp
// Declare the SoftTimer instance
SoftTimer mTimers;

// Timer IDs
uint8_t mIdTimer1 = 0;
uint8_t mIdTimer2 = 0;

// Flags to indicate timer ticks
bool mTimer1Incremented = false;
bool mTimer2Incremented = false;

// Initialize timers
void Application::Initialize()
{
    // Register a Period timer for 3 ticks
    mIdTimer1 = mTimers.AddPeriodTimer(3, [this]() { this->Timer1Tick(); });

    // Register a Period timer for 7 ticks
    mIdTimer2 = mTimers.AddPeriodTimer(7, [this]() { this->Timer2Tick(); });

    // Start the timers
    if (!mTimers.StartTimer(mIdTimer1)) {
        // Handle timer1 not started
    }
    if (!mTimers.StartTimer(mIdTimer2)) {
        // Handle timer2 not started
    }
}

// Main processing loop
void Application::Process()
{
    // Increment the SoftTimers
    mTimers.IncrementTick();

    // Check and process timer flags
    if (mTimer1Incremented) {
        mTimer1Incremented = false;
        // Process timer1 work
    }
    if (mTimer2Incremented) {
        mTimer2Incremented = false;
        // Process timer2 work
    }

    // Simulate a 100 ms delay
    delay_ms(100);
}

// Timer1 callback
void Application::Timer1Tick()
{
    mTimer1Incremented = true; // Set flag
}

// Timer2 callback
void Application::Timer2Tick()
{
    mTimer2Incremented = true; // Set flag
}
```