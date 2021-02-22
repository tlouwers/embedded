/**
 * \file i2c_arbiter.hpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 * \class   I2CArbiter
 *
 * \brief   Arbiter class for I2C (master) implementation.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/Arbiter
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    09-2018
 */

#ifndef I2C_ARBITER_HPP_
#define I2C_ARBITER_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstddef>              // size_t
#include <cstdint>              // uint8_t
#include <atomic>
#include <functional>
#include "../../CircularFifo/CircularFifo.hpp"
#include "i2c_drv_stub.hpp"


/************************************************************************/
/* Defines                                                              */
/************************************************************************/
/**
 * \def     I2C_ARBITER_BUFFER_SIZE
 * \brief   Size of the I2C Arbiter buffer.
 */
#define I2C_ARBITER_BUFFER_SIZE       10        // Tweak to get better results, usually 4


/************************************************************************/
/* Typedefs                                                             */
/************************************************************************/
/**
 * \struct  ArbiterElementI2C
 * \brief   Structure to contain administration items for the arbiter to
 *          delay read/write requests to the I2C bus.
 * \details This element can be queued for later use, it provides a
 *          grouping of useful variables for administration.
 */
struct ArbiterElementI2C {
    bool is_write_request               /** Flag indicating this is a write or read request */  = false;
    HeaderI2C header                    /** Structure with addressing data */                   = {};
    uint8_t* ptrData                    /** Pointer to the data sent/received */                = nullptr;
    size_t length                       /** The length of a message */                          = 0;
    std::function<void()> callbackDone  /** Callback to call when done */                       = nullptr;
};


/************************************************************************/
/* Class declaration                                                    */
/************************************************************************/
class I2CArbiter
{
public:
    I2CArbiter();
    ~I2CArbiter();

    bool Init(const I2C::Config& refConfig) const;
    bool IsInit() const;
    void Sleep();

    bool Write(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length, const std::function<void()>& refCallback);
    bool Read(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length, const std::function<void()>& refCallback);

    bool WriteBlocking(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length);
    bool ReadBlocking(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length);

private:
    CircularFifo<ArbiterElementI2C, I2C_ARBITER_BUFFER_SIZE> mBuffer;

    I2C                mI2C;
    std::atomic<bool>  mBusy;
    std::atomic_flag   mLock = ATOMIC_FLAG_INIT;

    void DataRequestHandler();
};


#endif  // I2C_ARBITER_HPP_
