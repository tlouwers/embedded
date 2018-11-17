/**
 * \file i2c_arbiter.cpp
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
 * \details This code is intended to be used as Arbiter on an I2C (master) of a
 *          Cortex-M4 microcontroller. The concept is practically identical for
 *          SPI (and probably UART).
 *
 *          As example:
 *          In 'main.cpp' we call app.Test() 10x in quick succession, then await
 *          the handling of the callbacks. The Arbiter will make sure there are
 *          no collisions on the I2C bus by making sure each transactions is
 *          handled one after the other.
 *          The 'i2c_driver_stub.cpp' spawns a thread to mimic the hardware DMA
 *          handling of an I2C transaction by sleeping a little while, then
 *          calling the callback.
 *          Note: do check for the result values, the example omits them for
 *                clarity.
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
#include "i2c_arbiter.hpp"
#include <cassert>


/************************************************************************/
/* Stubs for Atmel Cortex-M4 defines                                    */
/************************************************************************/
using irqflags_t = uint32_t;

irqflags_t cpu_irq_save(void)
{
    return 1;
}

void cpu_irq_restore(irqflags_t irq_state)
{
    ;
}

#define __NOP()     { asm volatile (""); }


/************************************************************************/
/* Public Methods                                                       */
/************************************************************************/
/**
 * \brief   Constructor.
 */
I2CArbiter::I2CArbiter() :
    mBusy(false)
{
    mBuffer.clear();
}

/**
 * \brief   Destructor.
 */
I2CArbiter::~I2CArbiter()
{
    mBusy = false;

    mLock.clear(std::memory_order_release);

    mBuffer.clear();
}

/**
 * \brief   Initializes the I2C bus.
 * \param   refConfig   Configuration of the I2C bus.
 * \returns True if initialized successful, else false.
 */
bool I2CArbiter::Init(const I2C::Config& refConfig)
{
    return (mI2C.Init(refConfig)) ? true : false;
}

/**
 * \brief   Check if I2C is initialized or not.
 * \returns True if initialized, else false.
 */
bool I2CArbiter::IsInit() const
{
    return mI2C.IsInit();
}

/**
 * \brief   Put I2C Arbiter module to sleep, first wait until all messages are sent,
 *          then clear the buffer and put I2C bus to sleep.
 * \remarks When timeout is reached the I2C bus is forced to sleep regardless.
 */
void I2CArbiter::Sleep()
{
    while (mBusy) { __NOP() }                                               // Blocking wait until we can use the bus. Use __ASM instruction to prevent loop from being optimized away.

    irqflags_t irq_state = cpu_irq_save();                                  // Disable global interrupts to prevent race condition
    while (mLock.test_and_set(std::memory_order_acquire)) { __NOP(); }      // Acquire lock - start of critical section

    mBuffer.clear();

    mLock.clear(std::memory_order_release);                                 // Release lock - end of critical section
    cpu_irq_restore(irq_state);                                             // Restore global interrupts

    mI2C.Sleep();
}

/**
 * \brief   Pass thru method to I2C Write method.
 * \details If the bus is busy the pointers to the data are queued and send when
 *          the bus becomes available.
 * \param   refHeader       The header containing the intended slave and write register.
 * \param   ptrSrc          The message to write.
 * \param   length          The length of the message.
 * \param   refCallback     Callback to call when data is sent.
 * \returns True if the request could be handled, else false.
 * \note    Asserts when I2C is not yet initialized.
 */
bool I2CArbiter::Write(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length, const std::function<void()>& refCallback)
{
    assert(mI2C.IsInit());

    ArbiterElementI2C element;
        element.is_write_request = true;
        element.header           = refHeader;
        element.ptrData          = const_cast<uint8_t *>(ptrSrc);
        element.length           = length;
        element.callbackDone     = refCallback;

    // The lock is needed to make a multiple producer of the CircularBuffer
    //  (which is single producer thread safe only).
    // The DataRequestHandler is the single consumer, there no lock is
    //  needed (or allowed! as it is inside an ISR).

    irqflags_t irq_state = cpu_irq_save();                                  // Disable global interrupts to prevent race condition
    while (mLock.test_and_set(std::memory_order_acquire)) { __NOP(); }      // Acquire lock - start of critical section

    bool result = mBuffer.push(element);
    assert(result);

    mLock.clear(std::memory_order_release);                                 // Release lock - end of critical section
    cpu_irq_restore(irq_state);                                             // Restore global interrupts

    // Start the transmission, if not busy yet
    if (mI2C.IsInit())
    {
        if (!mBusy)
        {
            mBusy = true;

            // Reroute the data received callback to the arbiter
            result = mI2C.Write(refHeader, ptrSrc, length, [this]() { this->DataRequestHandler(); });
            assert(result);
        }
    }

    return result;
}

/**
 * \brief   Pass thru method to I2C Read method.
 * \details If the bus is busy the pointers to the data are queued and send when
 *          the bus becomes available.
 * \param   refHeader       The header containing the intended slave and read register.
 * \param   ptrDest         The buffer to store the read data.
 * \param   length          The length of the message.
 * \param   refCallback     Callback to call when data is received.
 * \returns True if the request could be handled, else false.
 * \note    Asserts when I2C is not yet initialized.
 */
bool I2CArbiter::Read(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length, const std::function<void()>& refCallback)
{
    assert(mI2C.IsInit());

    ArbiterElementI2C element;
        element.is_write_request = false;
        element.header           = refHeader;
        element.ptrData          = ptrDest;
        element.length           = length;
        element.callbackDone     = refCallback;

    // The lock is needed to make a multiple producer of the CircularBuffer
    //  (which is single producer thread safe only).
    // The DataRequestHandler is the single consumer, there no lock is
    //  needed (or allowed! as it is inside an ISR).

    irqflags_t irq_state = cpu_irq_save();                                  // Disable global interrupts to prevent race condition
    while (mLock.test_and_set(std::memory_order_acquire)) { __NOP(); }      // Acquire lock - start of critical section

    bool result = mBuffer.push(element);
    assert(result);

    mLock.clear(std::memory_order_release);                                 // Release lock - end of critical section
    cpu_irq_restore(irq_state);                                             // Restore global interrupts

    // Start the transmission, if not busy yet
    if (mI2C.IsInit())
    {
        if (!mBusy)
        {
            mBusy = true;

            // Reroute the data received callback to the arbiter
            result = mI2C.Read(refHeader, ptrDest, length, [this]() { this->DataRequestHandler(); });
            assert(result);
        }
    }

    return result;
}

/**
 * \brief   Pass thru method to I2C WriteBlocking method.
 * \details If the bus is busy a blocking wait is done until the bus becomes
 *          available, then the data is send. If the bus does not become
 *          available the sending of data is skipped.
 * \param   refHeader   The header containing the intended slave and write register.
 * \param   ptrSrc      The message to write.
 * \param   length      The length of the message.
 * \returns True if the message was sent, else false.
 */
bool I2CArbiter::WriteBlocking(const HeaderI2C& refHeader, const uint8_t* ptrSrc, size_t length)
{
    assert(mI2C.IsInit());

    bool result = false;

    if (mI2C.IsInit())
    {
        while (mBusy) { __NOP(); }

        mBusy = true;
        result = mI2C.WriteBlocking(refHeader, ptrSrc, length);
        assert(result);
        mBusy = false;
    }

    return result;
}

/**
 * \brief   Pass thru method to I2C ReadBlocking method.
 * \details If the bus is busy a blocking wait is done until the bus becomes
 *          available, then the data is send. If the bus does not become
 *          available the sending of data is skipped.
 * \param   refHeader   The header containing the intended slave and read register.
 * \param   ptrDest     The buffer to store the read data.
 * \param   length      The length of the message.
 * \returns True if the message was sent, else false.
 */
bool I2CArbiter::ReadBlocking(const HeaderI2C& refHeader, uint8_t* ptrDest, size_t length)
{
    assert(mI2C.IsInit());

    bool result = false;

    if (mI2C.IsInit())
    {
        while (mBusy) { __NOP(); }

        mBusy = true;
        result = mI2C.ReadBlocking(refHeader, ptrDest, length);
        assert(result);
        mBusy = false;
    }

    return result;
}


/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
/**
 * \brief   Handler which is called when either TX or RX is done
 *          for I2C, allowing arbitration on the bus.
 * \details Checks if there is queued data, if so send it, else
 *          release the bus.
 */
void I2CArbiter::DataRequestHandler()
{
    ArbiterElementI2C element;

    // Since we are the only consumer, using the CircularBuffer class
    // provides thread safety.

    // Remove the element from the queue, handled.
    mBuffer.pop(element);

    // Call the callback, if there was one set.
    if (element.callbackDone)
    {
        element.callbackDone();
    }

    bool result = false;

    // Check if we need to handle the next item.
    if (mBuffer.peek(element))
    {
        if (element.is_write_request)
        {
            // Reroute the data to send callback to the arbiter
            result = mI2C.Write(element.header, element.ptrData, element.length, [this]() { this->DataRequestHandler(); });
            assert(result);
        }
        else
        {
            // Reroute the data received callback to the arbiter
            result = mI2C.Read(element.header, element.ptrData, element.length, [this]() { this->DataRequestHandler(); });
            assert(result);
        }
    }
    else
    {
        mBusy = false;
    }

    (void)(result);     // Hide compiler warning: unused variable
}
