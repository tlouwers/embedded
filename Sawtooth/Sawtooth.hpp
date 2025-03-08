/*
 * \file    Sawtooth.hpp

 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   Sawtooth
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Sawtooth
 *
 * \details This code is intended to generate a simple sawtooth within given
 *          parameters. The range and steepness can be set.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    03-2025
 */

#ifndef SAWTOOTH_HPP_
#define SAWTOOTH_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdint>


/************************************************************************/
/* Class declaration                                                    */
/************************************************************************/
/**
 * \brief   Simple sawtooth generator class.
 */
class Sawtooth
{
public:
    /**
     * \brief   Constructor.
     * \param   maxValue    The maximum value of the sawtooth waveform. Example: 24 means [0..23].
     * \param   nrSteps     The number of steps before wrapping at maxValue.
     */
    Sawtooth(uint16_t maxValue, uint16_t nrSteps) :
        mMaxValue(maxValue),
        mValue(0)
    {
        // Prevent divide by 0 by ensuring nrSteps is at least 1
        if (nrSteps == 0) {
            mStepSize = maxValue; // If no steps, set step size to maxValue
        } else {
            mStepSize = maxValue / nrSteps;
        }
    }

    /**
     * \brief   Destructor.
     */
    virtual ~Sawtooth() = default;

    /**
     * \brief   Add the next step for the sawtooth. Wraps if needed.
     * \returns Next value of the sawtooth.
     * \note    If the step size equals the maximum value this always returns 0.
     */
    uint16_t Next()
    {
        if (mStepSize == 0) {
            return 0; // Early return if step size is zero
        }

        mValue += mStepSize;
        if (mValue >= mMaxValue) {
            mValue -= mMaxValue; // Wrap around
        }
        return mValue;
    }

    /**
     * \brief   Resets the sawtooth value to 0.
     */
    void Reset() { mValue = 0; }

private:
    uint16_t mMaxValue;
    uint16_t mStepSize;
    uint16_t mValue;
};


#endif  // SAWTOOTH_HPP_
