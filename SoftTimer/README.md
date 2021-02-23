
# SoftTimer
SoftTimer utility class.

## Description
Intended use is to provide a simple and extendable number of software timers, based upon a defined (hardware) timer tick.
This flexibility allows for a hardware timer to be the source of the timer increment (tick), but this can be an RTOS task
or irregular clock as well. Based upon this main tick increment (expected to be faster than the derived software timers) 
The intervals of the software timers are calculated. This allows for timers tick often (millisecond range) upto those
that days to increment once.

Three types of timers are provided:
- Period: when the time runs out of the Period timer, the callback is called and the time is reset, creating periodic callbacks to happen. This behaves like regular timers.
- Timeout: when the time runs out of the Timeout timer, the callback is called and the timer is stopped. This acts as a single-shot timer.
- Stopwatch: when started, it will count the number of SoftTimer TimerPeriods. When the timer is stopped, the amount of TimerPeriods can be retrieved via GetTimerStatus().

# Requirements
- ST Microelectronics STM32F407G-DISC1 (can be ported easily to other ST microcontrollers)
- C++11
- A timer or mechanism providing a steady 'tick' to increment the SoftTimer.

# Notes
This utility class provides a way to create up to 255 timers, using a single increment, or 'tick' method to calculate time passed. The more timer are used, the more overhead in administration is present, up to a point it cannot meet the requested timing constraints anymore. Behavior is not defined in such case - be conservative and measure.
The provided 'tick' determines the accuracy of the software clocks. For instance: if a Timer ISR is used to 'tick' every 100 ms and all software clocks can execute and perform their tasks withing this period, they are guaranteed to run at exactly this 100 ms and will not drift.
To minimize the execution time of the software clock callbacks, be sure to only use it to increment a flag, which is processed by the main loop 'later'. If you add code/processing in the callbacks this will exponentially increase the time needed to handle the clocks inside the 'tick' and quite quickly this will reach its limits.

If you happen to find an issue, and are able to provide a reproducible scenario I am happy to have a look. If you have a fix, or a refactoring that would improve the code please let me know so I can update it.

# Examples
```cpp
// Declare the class (in Application.hpp for example):
SoftTimer mTimers;

// Variables to keep track of the timers (in Application.hpp for example)
uint8_t mIdTimer1 = 0;
uint8_t mIdTimer2 = 0;

// Flags to indicate timer tick passed (in Application.hpp for example)
bool mTimer1Incremented = false;
bool mTimer2Incremented = false;


// Register the timers before use
Application::Initialize() :
{
    // Period timer, 3 * 'tick' duration
    mIdTimer1 = mTimers.AddPeriodTimer(3, [this]() { this->Timer1Tick(); });

    // Period timer, 7 * 'tick' duration
    mIdTimer2 = mTimers.AddPeriodTimer(7, [this]() { this->Timer2Tick(); });

    // Do not forget to start the timers
    if (! mTimers.StartTimer(mIdTimer1))
    {
        // Handle timer1 not started, log?
    }
    if (! mTimers.StartTimer(mIdTimer2))
    {
        // Handle timer2 not started, log?
    }
}

// Provide a 'tick' to increment the timers. Preferred is to use a proper timer ISR or even an RTOS task, but for the example: any repeating method can be used. Here a blunt wait is used to emulate a 100 ms 'tick' using the main loop.
Application::Process() :
{
    // Increment the SoftTimers by incrementing the 'tick'
    mTimers.IncrementTick();

    // Check if processing is needed
    if (mTimer1Incremented)
    {
        mTimer1Incremented = false;

        // Process timer1 work
    }
    if (mTimer2Incremented)
    {
        mTimer2Incremented = false;

        // Process timer2 work
    }

    // Wait for 100 ms - blocking
    delay_ms(100);
}


// The Timer1 callback (as example):
void Application::Timer1Tick()
{
    // Set flag, keep as short as possible
    mTimer1Incremented = true;
}

// The Timer2 callback (as example):
void Application::Timer2Tick()
{
    // Set flag, keep as short as possible
    mTimer2Incremented = true;
}
```