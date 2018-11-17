/**
 * \file i2c_drv_stub.hpp
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

#ifndef I2C_DRV_STUB_HPP_
#define I2C_DRV_STUB_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstddef>          // size_t
#include <cstdint>          // uint8_t, uint16_t, uint32_t
#include <functional>
#include <future>           // std::async, mimic DMA duration

// Forward declaration as the struct I2CVariables uses a pointer to I2C.
class I2C;

/************************************************************************/
/* Structures                                                           */
/************************************************************************/
/**
 * \struct  I2CVariables
 * \brief   Data structure to contain data for a I2C instance.
 */
struct I2CVariables {
    bool initialized                    /** Flag if instance is initialized */ = false;
    uint8_t* ptrDest                    /** Pointer to the dest memory */      = nullptr;
    const uint8_t* ptrSrc               /** Pointer to the scr memory */       = nullptr;
    size_t length                       /** The length of a message */         = 0;
    I2C * ptrToI2CInstance              /** Pointer to I2C instance */         = nullptr;
    std::function<void()> callbackDone  /** Callback to call when done */      = nullptr;
};

/**
 * \struct  HeaderI2C
 * \brief   Data structure to contain information about the slave and the register to read/write.
 */
struct HeaderI2C {
    bool ten_bit_address  /** Flag indicating 10 bit address is used */                                        = false;
    uint16_t slave        /** The slave address */                                                             = 0;
    uint8_t  reg[3]       /** The register to read/write, note for 10 bit address only 2 bytes are allowed */  = {};
    uint8_t  reg_length   /** The length of the register segment (1..3 bytes) */                               = 0;
};


/************************************************************************/
/* Class declaration                                                    */
/************************************************************************/
class I2C
{
public:
    /**
     * \enum    BusSpeed
     * \brief   Available I2C bus speeds.
     */
    enum class BusSpeed
    {
        Standard,   // 100 kHz
        Full        // 400 kHz
    };

    /**
     * \struct  Config
     * \brief   Configuration struct for I2C.
     */
    struct Config
    {
        /**
         * \brief   Constructor of the I2C configuration struct.
         * \param   interruptPriority   Priority of the interrupt.
         * \param   busSpeed            Speed of the I2C bus.
         */
        Config(uint8_t interruptPriority,
               BusSpeed busSpeed) :
            mInterruptPriority(interruptPriority),
            mBusSpeed(busSpeed)
        { }

        uint8_t mInterruptPriority  /** Interrupt priority */;
        BusSpeed mBusSpeed          /** Speed of the I2C bus */;
    };


    I2C();
    ~I2C();

    bool Init(const Config& refConfig) const;
    bool IsInit() const;
    void Sleep() const;

    bool Write(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length, const std::function<void()>& refCallback);
    bool Read(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length, const std::function<void()>& refCallbackData);

    bool WriteBlocking(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length);
    bool ReadBlocking(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length);

private:
    I2CVariables& mRefI2CVariables;

    void AsyncCallbackStub();
};


#endif  // I2C_DRV_STUB_HPP_
