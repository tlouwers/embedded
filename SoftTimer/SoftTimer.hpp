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
 * \version 1.0
 * \date    02-2021
 */

#ifndef SOFT_TIMER_HPP_
#define SOFT_TIMER_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdint>
#include <functional>


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
class SoftTimer final
{
public:
/**
     * \enum    Type
     * \brief   Available timer types to register with the SoftTimer component.
     */
    enum class Type : uint8_t
    {
        TimeOut,
        StopWatch,
        Period,
        Invalid,
    };

    /**
     * \enum    State
     * \brief   The states a registered timer can have.
     */
    enum class State : uint8_t
    {
        Running,
        Stopped,
        Expired,
        Invalid,
    };

    /**
     * \struct  Status
     * \brief   Configuration struct for the status of a registered timer.
     */
    struct Status
    {
        /**
         * \brief   Configuration struct for the status of a registered timer.
         * \param   type    The type of the timer.
         * \param   state   The state of the timer.
         * \param   value   The amount of SoftTimer TimerPeriods of the timer.
         */
        Status(Type type, State state, uint32_t value) :
            mType(type),
            mState(state),
            mValue(value)
        { }

        Type     mType;     ///< The type of the timer.
        State    mState;    ///< The state of the timer.
        uint32_t mValue;    ///< The amount of SoftTimer TimerPeriods of the timer.
    };


    SoftTimer();
    virtual ~SoftTimer();

    void IncrementTick();

    uint8_t AddPeriodTimer(uint32_t value, const std::function<void()>& callback);
    uint8_t AddTimeoutTimer(uint32_t value, const std::function<void()>& callback);
    uint8_t AddStopwatchTimer();

    bool RemoveTimer(uint8_t id);

    bool StartTimer(uint8_t id) const;
    bool StopTimer(uint8_t id) const;

    bool ResetTimeoutTimer(uint8_t id);
    bool ResetTimeoutTimer(uint8_t id, uint32_t value);

    Status GetTimerStatus(uint8_t id) const;
};


#endif  // SOFT_TIMER_HPP_
