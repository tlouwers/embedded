/**
 * \file i2c_drv_stub.cpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   I2C
 *
 * \brief   Class stubbing the bare essentials of an I2C driver (master) of an
 *          Atmel Cortex-M4 microcontroller.
 *          Used to showcase the Arbiter class functionality in this project.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Arbiter
 *
 * \details See 'i2c_arbiter.cpp' as main project file.
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the code please let me know
 *          so I can update it.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    09-2018
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "i2c_drv_stub.hpp"
#include <cassert>
#include <iostream>         // std::cout, std::endl
#include <string>           // std::to_string
#include <thread>           // std::this_thread::sleep_for
#include <chrono>           // std::chrono::seconds


/************************************************************************/
/* Static variable initialization                                       */
/************************************************************************/
static I2CVariables i2c_variables {};


/************************************************************************/
/* Static functions                                                     */
/************************************************************************/
/**
 * \brief   Check if a callback is available to call, if so call the callback.
 *          This method is used to flag/indicate using classes a transmit is done.
 * \param   refI2CVariables The I2CVariables struct containing information
 *                          about the callback to call when transmission is done.
 */
static void CallCallbackDone(const I2CVariables& refI2CVariables)
{
    if (refI2CVariables.callbackDone)
    {
        refI2CVariables.callbackDone();
    }
}


/************************************************************************/
/* Public Methods                                                       */
/************************************************************************/
/**
 * \brief   Construct the I2C module.
 * \note    Must call Init() to be able to use the bus.
 * \note    Asserts when trying to initialize it twice for the same I2C instance.
 */
I2C::I2C() :
    mRefI2CVariables(i2c_variables)
{
    assert(!mRefI2CVariables.ptrToI2CInstance);
    mRefI2CVariables.ptrToI2CInstance = this;
}

/**
 * \brief   Destruct the I2C module.
 */
I2C::~I2C()
{
    mRefI2CVariables.initialized = false;
}

/**
 * \brief   Initializes the I2C bus.
 * \param   refConfig   Configuration of the I2C bus.
 * \returns True if successful, else false.
 */
bool I2C::Init(const Config& refConfig) const
{
    bool result = true;

    switch (refConfig.mBusSpeed)
    {
        case BusSpeed::Standard: std::cout << "BusSpeed: [" << 100000 << "]" << std::endl; break;
        case BusSpeed::Full:     std::cout << "BusSpeed: [" << 400000 << "]" << std::endl; break;
        default: result = false; assert(false);     break;
    }

    std::cout << "InterruptPriority: [" << std::to_string(refConfig.mInterruptPriority) << "]" << std::endl;

    if (result) { mRefI2CVariables.initialized = true; }

    return result;
}

/**
 * \brief   Check if I2C is initialized or not.
 * \returns True if initialized, else false.
 */
bool I2C::IsInit() const
{
    return mRefI2CVariables.initialized;
}

/**
 * \brief   Puts the I2C module in sleep mode.
 */
void I2C::Sleep() const
{
    mRefI2CVariables.initialized = false;
}

/**
 * \brief   Asynchronous write. Calls refCallback when done.
 * \param   refHeader       The header containing the intended slave and write register.
 * \param   ptrSrc          The message to write.
 * \param   length          The length of the message.
 * \param   refCallback     Callback to call when data is sent.
 * \returns True if transaction can be setup, else false.
 * \note    Asserts when ptrSrc is nullptr.
 * \note    Asserts when the length < 1.
 */
bool I2C::Write(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length, const std::function<void()>& refCallback)
{
    assert(ptrSrc);
    assert(length > 0);

    if ((ptrSrc != nullptr) && (length > 0))
    {
        mRefI2CVariables.ptrSrc       = ptrSrc;
        mRefI2CVariables.length       = length;
        mRefI2CVariables.callbackDone = refCallback;

        // Do not care about the header, this is a stub.

        // Small delay to mimic DMA setup
        std::this_thread::sleep_for(std::chrono::milliseconds(3));

        // Setup and start DMA stub
        std::async(std::launch::async, &I2C::AsyncCallbackStub, this);

        return true;
    }
    return false;
}

/**
 * \brief   Asynchronous read. Calls refCallback when done.
 * \param   refHeader       The header containing the intended slave and read register.
 * \param   ptrDest         The buffer to store the read data.
 * \param   length          The length of the message.
 * \param   refCallback     Callback to call when data is received.
 * \returns True if transaction can be setup, else false.
 * \note    Asserts when ptrDest is nullptr.
 * \note    Asserts when the length < 2.
 */
bool I2C::Read(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length, const std::function<void()>& refCallback)
{
    assert(ptrDest);
    assert(length > 1);

    if ((ptrDest != nullptr) && (length > 1))
    {
        mRefI2CVariables.ptrDest      = ptrDest;
        mRefI2CVariables.length       = length;
        mRefI2CVariables.callbackDone = refCallback;

        // Do not care about the header, this is a stub.

        // Small delay to mimic DMA setup
        std::this_thread::sleep_for(std::chrono::milliseconds(3));

        // Setup and start DMA stub
        std::async(std::launch::async, &I2C::AsyncCallbackStub, this);

        return true;
    }
    return false;
}

/**
 * \brief   Blocking write.
 * \param   refHeader   The header containing the intended slave and write register.
 * \param   ptrSrc      The message to write.
 * \param   length      The length of the message.
 * \returns True if the message was sent, else false.
 * \note    Asserts when pointer to ptrSrc is null.
 */
bool I2C::WriteBlocking(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length)
{
    assert(ptrSrc);

    if (length == 0) { return false; }

    // Do not care about the header, this is a stub.

    // Small delay to mimic I2C transmission
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return true;
}

/**
 * \brief   Blocking read.
 * \param   refHeader  The header containing the intended slave and read register.
 * \param   ptrDest    The buffer to store the read data.
 * \param   length     The length of the message.
 * \returns True if the message was sent, else false.
 * \note    Asserts when pointer to ptrDest is null.
 */
bool I2C::ReadBlocking(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length)
{
    assert(ptrDest);

    if (length == 0) { return false; }

    // Do not care about the header, this is a stub.

    // Small delay to mimic I2C transmission
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return true;
}


/************************************************************************/
/* Private Methods                                                      */
/************************************************************************/
// Mimic ISR: DMA callback to call thus stub
void I2C::AsyncCallbackStub()
{
    // Small delay to mimic DMA transaction
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    CallCallbackDone(i2c_variables);
}
