/**
 * \file    ISoftTimer.hpp
 *
 * \brief   SoftTimer interface class.
 *
 * \details This class is intended to act as interface for the SoftTimer class,
 *          to ease unit testing.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/SoftTimer
 *
 * \author  T. Louwers <terry.louwers@fourtress.nl>
 * \version 1.0
 * \date    06-2021
 */

#ifndef ISOFT_TIMER_HPP_
#define ISOFT_TIMER_HPP_


/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdint>
#include <functional>


/************************************************************************/
/* Interface declaration                                                */
/************************************************************************/
/**
 * \brief   SoftTimer interface class.
 */
class ISoftTimer
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

    virtual uint8_t AddPeriodTimer(uint32_t value, const std::function<void()>& callback) = 0;
    virtual uint8_t AddTimeoutTimer(uint32_t value, const std::function<void()>& callback) = 0;
    virtual uint8_t AddStopwatchTimer() = 0;

    virtual bool RemoveTimer(uint8_t id) = 0;

    virtual bool StartTimer(uint8_t id) const = 0;
    virtual bool StopTimer(uint8_t id) const = 0;

    virtual bool ResetTimeoutTimer(uint8_t id) = 0;
    virtual bool ResetTimeoutTimer(uint8_t id, uint32_t value) = 0;

    virtual Status GetTimerStatus(uint8_t id) const = 0;
};


#endif  // ISOFT_TIMER_HPP_
