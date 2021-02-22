/**
 * \file Application_Stub.cpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   Application_Stub
 *
 * \brief   Class stubbing the bare essentials of an embedded application.
 *          Used to showcase the Arbiter class functionality in this project.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Arbiter
 *
 * \details See 'i2c_arbiter.cpp' as main project file.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    09-2018
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "Application_Stub.hpp"
#include <cassert>
#include <iostream>         // std::cout, std::endl


/************************************************************************/
/* Public Methods                                                       */
/************************************************************************/
/**
 * \brief   Constructor, prepares callback.
 */
Application_Stub::Application_Stub() :
    mI2CArbiter()
{
   ;
}

/**
 * \brief   Destructor.
 */
Application_Stub::~Application_Stub()
{
    //dtor
}

/**
 * \brief   Initializes the I2C example arbiter.
 * \returns True if the arbiter (and I2C stub) could be initialized, else
 *          false.
 */
bool Application_Stub::Init()
{
    bool result = mI2CArbiter.Init(I2C::Config(5, I2C::BusSpeed::Full));
    assert(result);

    result = mI2CArbiter.IsInit();
    assert(result);

    return result;
}

/**
 * \brief   Main test method, this mimics a call to read a message over I2C,
 *          for instance to read a FIFO from a motion controller. The call
 *          is asynchronous.
 */
void Application_Stub::Test()
{
    mI2CArbiter.Write(mHeader, mSrc, sizeof(mSrc), [this]() { this->Callback(); });
}


/************************************************************************/
/* Private Methods                                                      */
/************************************************************************/
/**
 * \brief   Example callback, called when the test method (asynchronous) is
 *          done with the mimicked I2C transaction.
 */
void Application_Stub::Callback()
{
    std::cout << "Callback called" << std::endl;
}
