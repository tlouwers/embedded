#include "gtest/gtest.h"
#include <functional>


// Test subject
#include "../SoftTimer.hpp"


namespace {


// Test fixture for SoftTimer
class SoftTimer_Test : public ::testing::Test
{
protected:
    const uint32_t PERIOD  = 3;
    const uint32_t TIMEOUT = 3;

    SoftTimer_Test() :
        mSubject()
    {
        // Initialize test matter
    }

    void IncrementTick(uint32_t nr_times)
    {
        for (uint32_t i = 0; i < nr_times; i++)
        {
            mSubject.IncrementTick();
        }
    }

    void CallbackTimer()
    {
        mCallbackTimerCount++;
    }

    void SetupStartRunAndRemovePeriodTimer(uint32_t period, uint32_t nr_ticks)
    {
        uint8_t timerID = mSubject.AddPeriodTimer(period, [this]() { this->CallbackTimer(); } );
        EXPECT_NE(timerID, 0);      // Must not be 0, any other value is accepted

        EXPECT_TRUE(mSubject.StartTimer(timerID));

        mCallbackTimerCount = 0;
        IncrementTick(nr_ticks);

        EXPECT_TRUE(mSubject.RemoveTimer(timerID));
    }


    uint8_t SetupStartAndRunTimeoutTimer(uint32_t timeout, uint32_t nr_ticks)
    {
        uint8_t timerID = mSubject.AddTimeoutTimer(timeout, [this]() { this->CallbackTimer(); } );
        EXPECT_NE(timerID, 0);      // Must not be 0, any other value is accepted

        EXPECT_TRUE(mSubject.StartTimer(timerID));

        mCallbackTimerCount = 0;
        IncrementTick(nr_ticks);

        return timerID;
    }

    SoftTimer mSubject;
    uint32_t mCallbackTimerCount = 0;
};


TEST_F(SoftTimer_Test, IncrementTickDoesNothingWithoutSubscribedTimers)
{
    for (auto i = 0; i < 1000; i++)
    {
        mSubject.IncrementTick();
    }
}

TEST_F(SoftTimer_Test, GetTimerStatus_InvalidId)
{
    ISoftTimer::Status status = mSubject.GetTimerStatus(-1);

    EXPECT_EQ(status.mType,  SoftTimer::Type::Invalid);
    EXPECT_EQ(status.mState, SoftTimer::State::Invalid);

    status = mSubject.GetTimerStatus(0);

    EXPECT_EQ(status.mType,  SoftTimer::Type::Invalid);
    EXPECT_EQ(status.mState, SoftTimer::State::Invalid);

    // When no timer subscribed also a positive number is invalid
    status = mSubject.GetTimerStatus(1);

    EXPECT_EQ(status.mType,  SoftTimer::Type::Invalid);
    EXPECT_EQ(status.mState, SoftTimer::State::Invalid);

    status = mSubject.GetTimerStatus(UINT8_MAX);

    EXPECT_EQ(status.mType,  SoftTimer::Type::Invalid);
    EXPECT_EQ(status.mState, SoftTimer::State::Invalid);
}

TEST_F(SoftTimer_Test, RemoveTimer_InvalidValues)
{
    // Invalid ID
    EXPECT_FALSE(mSubject.RemoveTimer(0));

    // Note: no timers subscribed
    EXPECT_FALSE(mSubject.RemoveTimer(1));

    EXPECT_FALSE(mSubject.RemoveTimer(UINT8_MAX));
}

TEST_F(SoftTimer_Test, AddRemovePeriodTimer)
{
    uint8_t timers[4] = {};

    // Value 0 is invalid, timer not added, hence returns 0
    timers[0] = mSubject.AddPeriodTimer(0, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timers[0], 0);

    timers[0] = mSubject.AddPeriodTimer(1, [this]() { this->CallbackTimer(); } );
    timers[1] = mSubject.AddPeriodTimer(2, [this]() { this->CallbackTimer(); } );
    timers[2] = mSubject.AddPeriodTimer(3, [this]() { this->CallbackTimer(); } );
    timers[3] = mSubject.AddPeriodTimer(4, [this]() { this->CallbackTimer(); } );

    EXPECT_EQ(timers[0], 1);
    EXPECT_EQ(timers[1], 2);
    EXPECT_EQ(timers[2], 3);
    EXPECT_EQ(timers[3], 0);    // Not added - no space

    EXPECT_TRUE(mSubject.RemoveTimer(timers[0]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[1]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[2]));

    EXPECT_FALSE(mSubject.RemoveTimer(timers[3]));
}

TEST_F(SoftTimer_Test, AddRemoveTimeoutTimer)
{
    uint8_t timers[4] = {};

    // Value 0 is invalid, timer not added, hence returns 0
    timers[0] = mSubject.AddTimeoutTimer(0, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timers[0], 0);

    timers[0] = mSubject.AddTimeoutTimer(1, [this]() { this->CallbackTimer(); } );
    timers[1] = mSubject.AddTimeoutTimer(2, [this]() { this->CallbackTimer(); } );
    timers[2] = mSubject.AddTimeoutTimer(3, [this]() { this->CallbackTimer(); } );
    timers[3] = mSubject.AddTimeoutTimer(4, [this]() { this->CallbackTimer(); } );

    EXPECT_EQ(timers[0], 1);
    EXPECT_EQ(timers[1], 2);
    EXPECT_EQ(timers[2], 3);
    EXPECT_EQ(timers[3], 0);    // Not added - no space

    EXPECT_TRUE(mSubject.RemoveTimer(timers[0]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[1]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[2]));

    EXPECT_FALSE(mSubject.RemoveTimer(timers[3]));
}

TEST_F(SoftTimer_Test, AddRemoveStopwatchTimer)
{
    uint8_t timers[4] = {};

    timers[0] = mSubject.AddStopwatchTimer();
    timers[1] = mSubject.AddStopwatchTimer();
    timers[2] = mSubject.AddStopwatchTimer();
    timers[3] = mSubject.AddStopwatchTimer();

    EXPECT_EQ(timers[0], 1);
    EXPECT_EQ(timers[1], 2);
    EXPECT_EQ(timers[2], 3);
    EXPECT_EQ(timers[3], 0);    // Not added - no space

    EXPECT_TRUE(mSubject.RemoveTimer(timers[0]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[1]));
    EXPECT_TRUE(mSubject.RemoveTimer(timers[2]));

    EXPECT_FALSE(mSubject.RemoveTimer(timers[3]));
}

TEST_F(SoftTimer_Test, PeriodTimer_IfTimerNotStartedItDoesNotTrigger)
{
    uint8_t timerID = mSubject.AddPeriodTimer(PERIOD, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timerID, 1);

    mCallbackTimerCount = 0;
    IncrementTick(10);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_IfTimerNotStartedItDoesNotTrigger)
{
    uint8_t timerID = mSubject.AddTimeoutTimer(TIMEOUT, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timerID, 1);

    mCallbackTimerCount = 0;
    IncrementTick(10);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, PeriodTimer_CallbackTriggers)
{
    SetupStartRunAndRemovePeriodTimer(PERIOD, 1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    SetupStartRunAndRemovePeriodTimer(PERIOD, 2);
    EXPECT_EQ(mCallbackTimerCount, 0);

    SetupStartRunAndRemovePeriodTimer(PERIOD, 3);
    EXPECT_EQ(mCallbackTimerCount, 1);

    SetupStartRunAndRemovePeriodTimer(PERIOD, 4);
    EXPECT_EQ(mCallbackTimerCount, 1);

    SetupStartRunAndRemovePeriodTimer(PERIOD, 5);
    EXPECT_EQ(mCallbackTimerCount, 1);

    SetupStartRunAndRemovePeriodTimer(PERIOD, 6);
    EXPECT_EQ(mCallbackTimerCount, 2);
}

TEST_F(SoftTimer_Test, TimeoutTimer_CallbackTriggers)
{
    uint8_t timerID = mSubject.AddTimeoutTimer(TIMEOUT, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timerID, 1);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    mCallbackTimerCount = 0;
    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    mCallbackTimerCount = 0;
    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    mCallbackTimerCount = 0;
    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 1);

    // Once expired, timer will not trigger callback again
    mCallbackTimerCount = 0;
    IncrementTick(10);
    EXPECT_EQ(mCallbackTimerCount, 0);

    const ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, StopwatchTimer_CountsTicksProperly)
{
    uint8_t timerID = mSubject.AddStopwatchTimer();
    EXPECT_EQ(timerID, 1);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    IncrementTick(10);

    const ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mValue, 10);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, PeriodTimer_StartStopTimer)
{
    uint8_t timerID = mSubject.AddPeriodTimer(PERIOD, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timerID, 1);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    mCallbackTimerCount = 0;
    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.StopTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    IncrementTick(10);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(2);
    EXPECT_EQ(mCallbackTimerCount, 1);

    EXPECT_FALSE(mSubject.StopTimer(0));
    EXPECT_FALSE(mSubject.StopTimer(UINT8_MAX));
    EXPECT_TRUE(mSubject.StopTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    // Stopping a stopped timer yields true
    EXPECT_TRUE(mSubject.StopTimer(timerID));

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_StartStopTimer)
{
    uint8_t timerID = mSubject.AddTimeoutTimer(TIMEOUT, [this]() { this->CallbackTimer(); } );
    EXPECT_EQ(timerID, 1);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    mCallbackTimerCount = 0;
    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.StopTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    IncrementTick(10);
    EXPECT_EQ(mCallbackTimerCount, 0);

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(2);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_FALSE(mSubject.StopTimer(0));
    EXPECT_FALSE(mSubject.StopTimer(UINT8_MAX));
    EXPECT_TRUE(mSubject.StopTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Stopped);

    // Stopping a stopped timer yields true
    EXPECT_TRUE(mSubject.StopTimer(timerID));

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_ResetDefault)
{
    uint8_t timerID = SetupStartAndRunTimeoutTimer(TIMEOUT, 2);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    // 1 tick to go: but we reset the timer --> 3 ticks to go

    EXPECT_FALSE(mSubject.ResetTimeoutTimer(timerID));

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.StopTimer(timerID));
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID));
    EXPECT_TRUE(mSubject.StartTimer(timerID));

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID));

    EXPECT_TRUE(mSubject.StartTimer(timerID));
    mCallbackTimerCount = 0;
    IncrementTick(3);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_ResetInvalidValueFails)
{
    uint8_t timerID = SetupStartAndRunTimeoutTimer(TIMEOUT, 2);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    // 1 tick to go: but we try to reset the timer with invalid value: fails

    EXPECT_FALSE(mSubject.ResetTimeoutTimer(timerID, 0));

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.StopTimer(timerID));

    EXPECT_FALSE(mSubject.ResetTimeoutTimer(timerID, 0));

    EXPECT_TRUE(mSubject.StartTimer(timerID));

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_ResetValue_Smaller)
{
    uint8_t timerID = SetupStartAndRunTimeoutTimer(TIMEOUT, 2);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    // 1 tick to go: but we reset the timer --> 2 ticks to go

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.StopTimer(timerID));
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID, 2));
    EXPECT_TRUE(mSubject.StartTimer(timerID));

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID, 2));

    EXPECT_TRUE(mSubject.StartTimer(timerID));
    mCallbackTimerCount = 0;
    IncrementTick(2);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}

TEST_F(SoftTimer_Test, TimeoutTimer_ResetValue_Larger)
{
    uint8_t timerID = SetupStartAndRunTimeoutTimer(TIMEOUT, 2);

    ISoftTimer::Status status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    // 1 tick to go: but we reset the timer --> 4 ticks to go

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.StopTimer(timerID));
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID, 4));
    EXPECT_TRUE(mSubject.StartTimer(timerID));

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 0);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Running);

    IncrementTick(1);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    // Reset only works when Stopped or Expired
    EXPECT_TRUE(mSubject.ResetTimeoutTimer(timerID, 4));

    EXPECT_TRUE(mSubject.StartTimer(timerID));
    mCallbackTimerCount = 0;
    IncrementTick(4);
    EXPECT_EQ(mCallbackTimerCount, 1);

    status = mSubject.GetTimerStatus(timerID);
    EXPECT_EQ(status.mState, SoftTimer::State::Expired);

    EXPECT_TRUE(mSubject.RemoveTimer(timerID));
}


} // namespace
