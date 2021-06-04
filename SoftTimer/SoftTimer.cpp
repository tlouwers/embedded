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
 * \version 1.0
 * \date    02-2021
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "SoftTimer.hpp"
#include "utility/Assert/Assert.h"


/************************************************************************/
/* Static variable initialization                                       */
/************************************************************************/
/**
 * \struct  TimerEntry
 * \brief   Administration struct for of a registered timer.
 */
struct TimerEntry
{
    uint8_t               mIndex        = 0;                            ///< The timer index;
    std::function<void()> mCallback     = nullptr;                      ///< The callback of a timer.
    SoftTimer::Type       mType         = SoftTimer::Type::Invalid;     ///< The type of timer.
    SoftTimer::State      mState        = SoftTimer::State::Invalid;    ///< The current state of the timer.
    uint32_t              mCurrentValue = 0;                            ///< The current timer value.
    uint32_t              mResetValue   = 0;                            ///< The reset timer value.
};

// The +1 is for an empty unused entry, used for indicating an element is not present.
static TimerEntry timers[MAX_SOFT_TIMERS + 1] = {};

static uint8_t timerIndex = 0;


/************************************************************************/
/* Static functions                                                     */
/************************************************************************/
/**
 * \brief   Helper function to find an entry in the timers table.
 * \param   id      The id of the timer entry to find.
 * \returns The entry in the timers table if found, else the element at
*           the last position with mIndex 0.
 */
static TimerEntry& GetEntryForTimer(uint8_t id)
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
static bool AddEntryForTimer(const TimerEntry& entryToAdd)
{
    // Make sure the entry does not exist yet
    if (GetEntryForTimer(entryToAdd.mIndex).mIndex != 0)
    {
        return false;
    }

    // If no entry/duplicate found, add the entry
    for (auto i = 0; i < MAX_SOFT_TIMERS; i++)
    {
        // Find an empty entry
        if (timers[i].mIndex == 0)
        {
            timers[i] = entryToAdd;
            return true;
        }
    }

    // Array full, cannot add entry
    return false;
}


/************************************************************************/
/* Public methods                                                       */
/************************************************************************/
SoftTimer::SoftTimer()
{
    ASSERT(MAX_SOFT_TIMERS > 0);
    ASSERT(MAX_SOFT_TIMERS <= 256);

    timerIndex = 0;
}

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

        if (timer.mIndex != 0)
        {
            if (timer.mState == State::Running)
            {
                switch (timer.mType)
                {
                    // Timeout timer will stop after time runs out.
                    case SoftTimer::Type::TimeOut:
                        if (timer.mCurrentValue <= 1)
                        {
                            timer.mState = State::Expired;

                            if (timer.mCallback) { timer.mCallback(); }
                        }
                        else
                        {
                            timer.mCurrentValue--;
                        }
                        break;

                    // Period timer will reset itself after period runs out.
                    case SoftTimer::Type::Period:
                        if (timer.mCurrentValue <= 1)
                        {
                            timer.mCurrentValue = timer.mResetValue;

                            if (timer.mCallback) { timer.mCallback(); }
                        }
                        else
                        {
                            timer.mCurrentValue--;
                        }
                        break;

                    // Stopwatch will keep counting until stopped or uint32_t reached max value.
                    case SoftTimer::Type::StopWatch:
                        if (timer.mCurrentValue != UINT32_MAX)
                        {
                            timer.mCurrentValue++;
                        }
                        else
                        {
                            timer.mState = State::Stopped;
                        }
                        break;

                    default: EXPECT(false); break;     // Impossible, invalid value
                }
            }
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

    if (value == 0) { return 0; }

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

    if (value == 0) { return 0; }

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

    if (id == 0) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
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
bool SoftTimer::StartTimer(uint8_t id) const
{
    EXPECT(id > 0);

    if (id == 0) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
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

    if (id == 0) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
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

    if (id == 0) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
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

    if (id == 0)    { return false; }
    if (value == 0) { return false; }

    TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
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
SoftTimer::Status SoftTimer::GetTimerStatus(uint8_t id) const
{
    EXPECT(id > 0);

    SoftTimer::Status status(Type::Invalid, State::Invalid, 0);

    if (id == 0) { return status; }

    const TimerEntry& entry = GetEntryForTimer(id);
    EXPECT(entry.mIndex != 0);

    if (entry.mIndex != 0)
    {
        status.mType  = entry.mType;
        status.mState = entry.mState;
        status.mValue = entry.mCurrentValue;
    }

    return status;
}
