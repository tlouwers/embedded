/**
 * \file Application_Stub.hpp
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

#ifndef APPLICATION_STUB_HPP_
#define APPLICATION_STUB_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdint>
#include "i2c_arbiter.hpp"
#include "i2c_drv_stub.hpp"


/************************************************************************/
/* Class declaration                                                    */
/************************************************************************/
class Application_Stub
{
public:
    Application_Stub();
    virtual ~Application_Stub();

    bool Init();
    void Test();

private:
    I2CArbiter mI2CArbiter;

    HeaderI2C mHeader;
    const uint8_t mSrc[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    void Callback();
};


#endif // APPLICATION_STUB_HPP_
