/**
 * \file    SoftTimer.cpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   SoftTimer
 *
 * \brief   Flexible software timer helper class.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/SoftTimer
 *
 * \details Intended use is to provide a simple and extendable number of software
 *          timers, based upon a defined hardware timer tick.
 *
 * \author  T. Louwers <terry.louwers@fourtress.nl>
 * \version 1.2
 * \date    03-2025
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "SoftTimer.hpp"


/************************************************************************/
/* Constants                                                            */
/************************************************************************/
static constexpr uint8_t INVALID_TIMER_ID = 0;


/************************************************************************/
/* Public methods                                                       */
/************************************************************************/
/**
 * \brief   Constructor, initializes the timerIndex.
 * \details Initializes the timerIndex to 0.
 */
SoftTimer::SoftTimer() : timerIndex(0)
{
    // No need for ASSERTs; just initialize the member variables.
}

/**
 * \brief   Increments the SoftTimer and processes all registered (and started) timers.
 * \details This method iterates through the timers and calls the appropriate processing
 *          function for each timer that is currently active.
 */
void SoftTimer::IncrementTick()
{
    for (uint8_t i = 0; i < MAX_SOFT_TIMERS; ++i)
    {
        // Map entry for readability, note: use reference!
        TimerEntry& timer = timers[i];

        if (0 != timer.mIndex)
        {
            ProcessTimer(timer); // Process the active timer
        }
    }
}

/**
 * \brief   Registers a Period timer.
 * \details When the time runs out, the callback is called and the timer resets,
 *          creating periodic callbacks.
 * \param   value       The number of TimerPeriods before the callback is called.
 * \param   callback    The callback to call when the time runs out.
 * \returns Index number of the timer inserted (1 or higher), or 0 if insertion failed.
 * \remarks Returns 0 if the value is 0.
 * \note    The timer is not started after registration.
 */
uint8_t SoftTimer::AddPeriodTimer(uint32_t value, const std::function<void()>& callback)
{
    if (0 == value)
    {
        return INVALID_TIMER_ID; // Early return for invalid period value
    }

    TimerEntry entry{ ++timerIndex, callback, Type::Period, State::Stopped, value, value };

    return AddEntryForTimer(entry) ? timerIndex : INVALID_TIMER_ID; // Return the timer index or 0 if failed
}

/**
 * \brief   Registers a Timeout timer.
 * \details When the time runs out, the callback is called and the timer stops.
 * \param   value       The number of TimerPeriods before the callback is called.
 * \param   callback    The callback to call when the time runs out.
 * \returns Index number of the timer inserted (1 or higher), or 0 if insertion failed.
 * \remarks Returns 0 if the value is 0.
 * \note    The timer is not started after registration.
 */
uint8_t SoftTimer::AddTimeoutTimer(uint32_t value, const std::function<void()>& callback)
{
    if (0 == value)
    {
        return INVALID_TIMER_ID; // Early return for invalid timeout value
    }

    TimerEntry entry{ ++timerIndex, callback, Type::TimeOut, State::Stopped, value, value };

    return AddEntryForTimer(entry) ? timerIndex : INVALID_TIMER_ID; // Return the timer index or 0 if failed
}

/**
 * \brief   Registers a Stopwatch timer.
 * \details When started, it counts the number of TimerPeriods. When stopped,
 *          the amount can be retrieved via GetTimerStatus().
 * \returns Index number of the timer inserted (1 or higher), or 0 if insertion failed.
 * \note    The timer is not started after registration.
 */
uint8_t SoftTimer::AddStopwatchTimer()
{
    TimerEntry entry{ ++timerIndex, nullptr, Type::StopWatch, State::Stopped, 0, 0 };

    return AddEntryForTimer(entry) ? timerIndex : INVALID_TIMER_ID; // Return the timer index or 0 if failed
}

/**
 * \brief   Removes a registered timer from the SoftTimer component.
 * \param   id  The id of the timer to remove.
 * \returns True if the timer was removed, else false.
 * \remarks Returns false if the id is 0 or if the timer cannot be found.
 */
bool SoftTimer::RemoveTimer(uint8_t id)
{
    if (INVALID_TIMER_ID == id)
    {
        return false; // Early return for invalid id
    }

    TimerEntry& entry = GetEntryForTimer(id);
    if (0 == entry.mIndex)
    {
        return false; // Early return if timer not found
    }

    // Reset the entry to indicate it is no longer in use
    entry = {};  // Clear the entry
    return true; // Timer successfully removed
}

/**
 * \brief   Start the given timer.
 * \param   id  The id of the timer to start.
 * \returns True if the timer was started, else false.
 */
bool SoftTimer::StartTimer(uint8_t id)
{
    if (INVALID_TIMER_ID == id)
    {
        return false;
    }

    TimerEntry& entry = GetEntryForTimer(id);

    if (0 != entry.mIndex)
    {
        entry.mState = State::Running;
        return true;
    }
    return false;
}

/**
 * \brief   Stops the given timer.
 * \param   id  The id of the timer to stop.
 * \returns True if the timer was stopped, else false.
 * \remarks Returns false if the id is 0 or if the timer cannot be found.
 */
bool SoftTimer::StopTimer(uint8_t id)
{
    if (INVALID_TIMER_ID == id)
    {
        return false;               // Early return for invalid id
    }

    TimerEntry& entry = GetEntryForTimer(id);
    if (0 == entry.mIndex)
    {
        return false;               // Early return if timer not found
    }

    entry.mState = State::Stopped;  // Stop the timer
    return true;                    // Timer successfully stopped
}

/**
 * \brief   Resets a Timeout timer to its original timer value for reuse.
 * \details The timer must be in Expired or Stopped state before calling
 *          the reset.
 * \param   id  The id of the timer to reset.
 * \returns True if the timer was reset successfully, else false.
 * \remarks Returns false if the id is 0 or if the timer cannot be found.
 * \note    The timer is not started after resetting.
 */
bool SoftTimer::ResetTimeoutTimer(uint8_t id)
{
    if (INVALID_TIMER_ID == id)
    {
        return false; // Early return for invalid id
    }

    TimerEntry& entry = GetEntryForTimer(id);
    if (0 == entry.mIndex)
    {
        return false; // Early return if timer not found
    }

    // Check if the timer is in a valid state for resetting
    if (entry.mType == Type::TimeOut)
    {
        if (State::Expired == entry.mState || State::Stopped == entry.mState)
        {
            entry.mCurrentValue = entry.mResetValue; // Reset the current value
            return true; // Timer successfully reset
        }
    }

    return false; // Timer not in a valid state for reset
}

/**
 * \brief   Resets a Timeout timer for reuse with a new timer value.
 * \details The timer must be in Expired or Stopped state before calling
 *          the reset.
 * \param   id      The id of the timer to reset.
 * \param   value   The new number of TimerPeriods before the callback
 *                  of this timer is to be called.
 * \returns True if the timer was reset successfully, else false.
 * \remarks Returns false if the id is 0, if the value is 0, or if the timer cannot be found.
 * \note    The timer is not started after resetting.
 */
bool SoftTimer::ResetTimeoutTimer(uint8_t id, uint32_t value)
{
    if (INVALID_TIMER_ID == id || 0 == value)
    {
        return false; // Early return for invalid id or timeout value
    }

    TimerEntry& entry = GetEntryForTimer(id);
    if (0 == entry.mIndex)
    {
        return false; // Early return if timer not found
    }

    // Check if the timer is in a valid state for resetting
    if (entry.mType == Type::TimeOut)
    {
        if (State::Expired == entry.mState || State::Stopped == entry.mState)
        {
            entry.mResetValue = value; // Set the new reset value
            entry.mCurrentValue = value; // Reset the current value
            return true; // Timer successfully reset
        }
    }

    return false; // Timer not in a valid state for reset
}

/**
 * \brief   Gets the current status of a timer.
 * \param   id  The id of the timer to get the status for.
 * \returns The Status for the requested timer.
 * \remarks The Type and State are set to Invalid and the value to 0 if the
 *          timer could not be found.
 * \remarks Returns a default Status object if the id is 0.
 */
SoftTimer::Status SoftTimer::GetTimerStatus(uint8_t id)
{
    // Return a default Status if the id is 0
    if (INVALID_TIMER_ID == id)
    {
        return Status(Type::Invalid, State::Invalid, 0);
    }

    const TimerEntry& entry = GetEntryForTimer(id);
    if (0 == entry.mIndex)
    {
        return Status(Type::Invalid, State::Invalid, 0); // Timer not found
    }

    // Return the current status of the timer
    return Status(entry.mType, entry.mState, entry.mCurrentValue);
}


/************************************************************************/
/* Private methods                                                      */
/************************************************************************/
/**
 * \brief   Helper function to find an entry in the timers table.
 * \param   id      The id of the timer entry to find.
 * \returns The entry in the timers table if found, else the element at
 *           the last position with mIndex 0.
 */
SoftTimer::TimerEntry& SoftTimer::GetEntryForTimer(uint8_t id)
{
    for (auto i = 0; i < MAX_SOFT_TIMERS; i++)
    {
        if (id == timers[i].mIndex)
        {
            return timers[i];
        }
    }

    // No entry found
    return timers[MAX_SOFT_TIMERS];
}

/**
 * \brief   Helper function to add an timer in the timers table.
 * \param   entryToAdd      Reference to the timer to add.
 * \returns True if the timer could be added, else false.
 * \remarks If an entry already exists will return false.
 */
bool SoftTimer::AddEntryForTimer(const TimerEntry& entryToAdd)
{
    // Make sure the entry does not exist yet
    if (0 != GetEntryForTimer(entryToAdd.mIndex).mIndex)
    {
        return false;
    }

    // If no entry/duplicate found, add the entry
    for (auto i = 0; i < MAX_SOFT_TIMERS; i++)
    {
        // Find an empty entry
        if (0 == timers[i].mIndex)
        {
            timers[i] = entryToAdd;
            return true;
        }
    }

    // Array full, cannot add entry
    return false;
}

/**
 * \brief   Helper method to route the (running) timers to the appropriate handlers.
 * \param   timer   The timer to handle.
 */
void SoftTimer::ProcessTimer(TimerEntry& timer)
{
    // Early return if the timer is not running
    if (State::Running != timer.mState)
    {
        return;
    }

    // Process the timer based on its type
    switch (timer.mType)
    {
        case Type::TimeOut:
            ProcessTimeOutTimer(timer);
            break;
        case Type::Period:
            ProcessPeriodTimer(timer);
            break;
        case Type::StopWatch:
            ProcessStopwatchTimer(timer);
            break;
        default:
            // Handle unexpected timer type if necessary
            break;
    }
}

/**
 * \brief   Timeout timer will stop after time runs out.
 * \param   timer   The timeout timer to handle.
 */
void SoftTimer::ProcessTimeOutTimer(TimerEntry& timer)
{
    // Early return if the timer has not expired
    if (timer.mCurrentValue > 1)
    {
        --timer.mCurrentValue; // Decrement the current value
        return;
    }

    // Timer has expired
    timer.mState = State::Expired;

    // Call the callback if it exists
    if (timer.mCallback)
    {
        timer.mCallback();
    }
}

/**
 * \brief   Period timer will reset itself after the period runs out.
 * \param   timer   The periodic timer to handle.
 */
void SoftTimer::ProcessPeriodTimer(TimerEntry& timer)
{
    // Early return if the timer has not expired
    if (timer.mCurrentValue > 1)
    {
        --timer.mCurrentValue; // Decrement the current value
        return;
    }

    // Timer has expired, reset the current value
    timer.mCurrentValue = timer.mResetValue;

    // Call the callback if it exists
    if (timer.mCallback)
    {
        timer.mCallback();
    }
}

/**
 * \brief   Stopwatch will keep counting until stopped or uint32_t reaches max value.
 * \param   timer   The stopwatch timer to handle.
 */
void SoftTimer::ProcessStopwatchTimer(TimerEntry& timer)
{
    // Early return if the timer has reached its maximum value
    if (UINT32_MAX == timer.mCurrentValue)
    {
        timer.mState = State::Stopped; // Stop the timer if it has reached the maximum
        return;
    }

    // Increment the current value of the stopwatch
    ++timer.mCurrentValue;
}
