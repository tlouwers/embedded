/**
 * \file    SoftTimer.hpp
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

#ifndef SOFT_TIMER_HPP_
#define SOFT_TIMER_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdint>
#include <functional>
#include "interfaces/ISoftTimer.hpp"


/************************************************************************/
/* Defines                                                              */
/************************************************************************/
/**
 * \brief   The maximum number of SoftTimers.
 * \details To save RAM this can be set to a lower value to save 32 bytes
 *          per timer. The default is 3, the maximum 255.
 */
constexpr uint8_t MAX_SOFT_TIMERS = 3;


/************************************************************************/
/* Class declaration                                                    */
/************************************************************************/
class SoftTimer final : public ISoftTimer
{
public:
    SoftTimer();
    virtual ~SoftTimer();

    void IncrementTick();

    uint8_t AddPeriodTimer(uint32_t value, const std::function<void()>& callback) override;
    uint8_t AddTimeoutTimer(uint32_t value, const std::function<void()>& callback) override;
    uint8_t AddStopwatchTimer() override;

    bool RemoveTimer(uint8_t id) override;

    bool StartTimer(uint8_t id) override;
    bool StopTimer(uint8_t id) override;

    bool ResetTimeoutTimer(uint8_t id) override;
    bool ResetTimeoutTimer(uint8_t id, uint32_t value) override;

    Status GetTimerStatus(uint8_t id) override;

private:
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
    TimerEntry timers[MAX_SOFT_TIMERS + 1] = {};
    uint8_t timerIndex = 0;

    TimerEntry& GetEntryForTimer(uint8_t id);
    bool AddEntryForTimer(const TimerEntry& entryToAdd);

    void ProcessTimer(TimerEntry& timer);
    void ProcessTimeOutTimer(TimerEntry& timer);
    void ProcessPeriodTimer(TimerEntry& timer);
    void ProcessStopwatchTimer(TimerEntry& timer);
};


#endif  // SOFT_TIMER_HPP_
