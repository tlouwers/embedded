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
 * \version 1.1
 * \date    05-2024
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "SoftTimer.hpp"
#include "utility/Assert/Assert.h"


/************************************************************************/
/* Public methods                                                       */
/************************************************************************/
/**
 * \brief   Constructor, resets the timerIndex.
 */
SoftTimer::SoftTimer()
{
    ASSERT(MAX_SOFT_TIMERS > 0);
    ASSERT(MAX_SOFT_TIMERS <= UINT8_MAX);

    timerIndex = 0;
}

/**
 * \brief   Destructor, resets the timerIndex.
 */
SoftTimer::~SoftTimer()
{
    timerIndex = 0;
}

/**
 * \brief   Increment the SoftTimer and all registered (and started) timers.
 */
void SoftTimer::IncrementTick()
{
    for (auto i = 0; i < MAX_SOFT_TIMERS; i++)
    {
        // Map entry for readability, note: use reference!
        TimerEntry& timer = timers[i];

        if (0 != timer.mIndex)
        {
            ProcessTimer(timer);
        }
    }
}

/**
 * \brief   Registers a Period timer.
 * \details When the time runs out of the Period timer, the callback is called and
 *          the time is reset, creating periodic callbacks to happen.
 * \param   value       The number of SoftTimer TimerPeriods before the callback
 *                      of this timer is to be called.
 * \param   callback    The callback to call when the time runs out.
 * \returns Index number of the timer inserted, 1 or higher. Returns 0 if insertion failed.
 * \remarks Asserts when value is 0.
 * \note    The timer is not started after registering.
 */
uint8_t SoftTimer::AddPeriodTimer(uint32_t value, const std::function<void()>& callback)
{
    EXPECT(value > 0);

    if (0 == value) { return 0; }

    TimerEntry entry;
    entry.mIndex        = timerIndex + 1;
    entry.mState        = State::Stopped;
    entry.mType         = Type::Period;
    entry.mResetValue   = value;
    entry.mCurrentValue = value;
    entry.mCallback     = callback;

    return (AddEntryForTimer(entry)) ? (++timerIndex) : 0;
}

/**
 * \brief   Registers a Timeout timer.
 * \details When the time runs out of the Timeout timer, the callback is called and
 *          the timer is stopped.
 * \param   value       The number of SoftTimer TimerPeriods before the callback
 *                      of this timer is to be called.
 * \param   callback    The callback to call when the time runs out.
 * \returns Index number of the timer inserted, 1 or higher. Returns 0 if insertion failed.
 * \remarks Asserts when value is 0.
 * \note    The timer is not started after registering.
 */
uint8_t SoftTimer::AddTimeoutTimer(uint32_t value, const std::function<void()>& callback)
{
    EXPECT(value > 0);

    if (0 == value) { return 0; }

    TimerEntry entry;
    entry.mIndex        = timerIndex + 1;
    entry.mState        = State::Stopped;
    entry.mType         = Type::TimeOut;
    entry.mResetValue   = value;
    entry.mCurrentValue = value;
    entry.mCallback     = callback;

    return (AddEntryForTimer(entry)) ? (++timerIndex) : 0;
}

/**
 * \brief   Registers a Stopwatch timer.
 * \details When started, it will count the number of SoftTimer TimerPeriods. When
 *          the timer is stopped, the amount of TimerPeriods can be retrieved via
 *          GetTimerStatus().
 * \returns Index number of the timer inserted, 1 or higher. Returns 0 if insertion failed.
 * \note    The timer is not started after registering.
 */
uint8_t SoftTimer::AddStopwatchTimer()
{
    TimerEntry entry;
    entry.mIndex        = timerIndex + 1;
    entry.mState        = State::Stopped;
    entry.mType         = Type::StopWatch;
    entry.mResetValue   = 0;
    entry.mCurrentValue = 0;
    entry.mCallback     = nullptr;

    return (AddEntryForTimer(entry)) ? (++timerIndex) : 0;
}

/**
 * \brief   Removes a registered timer from the SoftTimer component.
 * \param   id  The id of the timer to remove.
 * \result  Returns true if the timer was removed, else false.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 */
bool SoftTimer::RemoveTimer(uint8_t id)
{
    EXPECT(id > 0);

    if (0 == id) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        entry.mIndex        = 0;
        entry.mCallback     = nullptr;
        entry.mType         = Type::Invalid;
        entry.mState        = State::Invalid;
        entry.mCurrentValue = 0;
        entry.mResetValue   = 0;

        return true;
    }
    return false;
}

/**
 * \brief   Start the given timer.
 * \param   id  The id of the timer to start.
 * \returns True if the timer was started, else false.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 */
bool SoftTimer::StartTimer(uint8_t id)
{
    EXPECT(id > 0);

    if (0 == id) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        entry.mState = State::Running;
        return true;
    }
    return false;
}

/**
 * \brief   Stop the given timer.
 * \param   id  The id of the timer to stop.
 * \returns True if the timer was stopped, else false.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 */
bool SoftTimer::StopTimer(uint8_t id) const
{
    EXPECT(id > 0);

    if (0 == id) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        entry.mState = State::Stopped;
        return true;
    }
    return false;
}

/**
 * \brief   Reset a Timeout timer to its original timer value for reuse.
 * \details The timer must be in Expired or Stopped state before calling
 *          the reset.
 * \param   id  The id of the timer to reset.
 * \returns True if timer reset successfully, else false.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 * \note    The timer is not started after resetting.
 */
bool SoftTimer::ResetTimeoutTimer(uint8_t id)
{
    EXPECT(id > 0);

    if (0 == id) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        if (entry.mType == Type::TimeOut)
        {
            if (entry.mState == State::Expired ||
                entry.mState == State::Stopped)
            {
                entry.mCurrentValue = entry.mResetValue;
                return true;
            }
        }
    }
    return false;
}

/**
 * \brief   Reset a Timeout timer for reuse.
 * \details The timer must be in Expired or Stopped state before calling
 *          the reset.
 * \param   id      The id of the timer to reset.
 * \param   value   The number of SoftTimer TimerPeriods before the callback
 *                  of this timer is to be called.
 * \returns True if timer reset successfully, else false.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 * \note    The timer is not started after resetting.
 */
bool SoftTimer::ResetTimeoutTimer(uint8_t id, uint32_t value)
{
    EXPECT(id > 0);
    EXPECT(value > 0);

    if (0 == id)    { return false; }
    if (0 == value) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        if (entry.mType == Type::TimeOut)
        {
            if (entry.mState == State::Expired ||
                entry.mState == State::Stopped)
            {
                entry.mResetValue   = value;
                entry.mCurrentValue = value;
                return true;
            }
        }
    }
    return false;
}

/**
 * \brief   Get the current status of a timer.
 * \param   id  The id of the timer to get the status for.
 * \returns The Status for the requested timer.
 * \remarks The Type and State are set to Invalid and the value to 0 if the
 *          timer could not be found.
 * \remarks Asserts when id is 0.
 * \remarks Asserts when timer cannot be found in array.
 */
SoftTimer::Status SoftTimer::GetTimerStatus(uint8_t id)
{
    EXPECT(id > 0);

    SoftTimer::Status status(Type::Invalid, State::Invalid, 0);

    if (0 == id) { return status; }

    const TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (0 != entry.mIndex)
    {
        status.mType  = entry.mType;
        status.mState = entry.mState;
        status.mValue = entry.mCurrentValue;
    }

    return status;
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
        if (timers[i].mIndex == id)
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
 * \brief Helper method to route the (running) timers to the appropriate handlers.
 * \param   timer   The timer to handle.
 */
void SoftTimer::ProcessTimer(TimerEntry& timer)
{
    if (State::Running == timer.mState) {
        switch (timer.mType) {
            case SoftTimer::Type::TimeOut:
                ProcessTimeOutTimer(timer);
                break;
            case SoftTimer::Type::Period:
                ProcessPeriodTimer(timer);
                break;
            case SoftTimer::Type::StopWatch:
                ProcessStopwatchTimer(timer);
                break;
            default:
                EXPECT(false);  // Impossible, invalid value
                break;
        }
    }
}

/**
 * \brief   Timeout timer will stop after time runs out.
 * \param   timer   The timeout timer to handle.
 */
void SoftTimer::ProcessTimeOutTimer(TimerEntry& timer) {
    if (1 >= timer.mCurrentValue)
    {
        timer.mState = State::Expired;

        if (timer.mCallback) { timer.mCallback(); }
    }
    else
    {
        timer.mCurrentValue--;
    }
}

/**
 * \brief   Period timer will reset itself after period runs out.
 * \param   timer   The periodic timer to handle.
 */
void SoftTimer::ProcessPeriodTimer(TimerEntry& timer) {
    if (1 >= timer.mCurrentValue)
    {
        timer.mCurrentValue = timer.mResetValue;

        if (timer.mCallback) { timer.mCallback(); }
    }
    else
    {
        timer.mCurrentValue--;
    }
}

/**
 * \brief   Stopwatch will keep counting until stopped or uint32_t reached max value.
 * \param   timer   The stopwatch timer to handle.
 */
void SoftTimer::ProcessStopwatchTimer(TimerEntry& timer) {
    if (UINT32_MAX != timer.mCurrentValue)
    {
        timer.mCurrentValue++;
    }
    else
    {
        timer.mState = State::Stopped;
    }
}
