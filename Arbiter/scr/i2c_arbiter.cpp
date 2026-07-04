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
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.2
 * \date    05-2026
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
    (void)irq_state;  // Suppress unused parameter warning
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
{ }

/**
 * \brief   Initializes the I2C bus.
 * \param   refConfig   Configuration of the I2C bus.
 * \returns True if initialized successful, else false.
 */
bool I2CArbiter::Init(const I2C::Config& refConfig)
{
    return mI2C.Init(refConfig);
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
 * \remarks Spins until the bus becomes idle. Callers must not invoke Write/Read
 *          concurrently with Sleep, otherwise the bus may be put to sleep with
 *          a transaction in flight.
 */
void I2CArbiter::Sleep()
{
    while (mBusy.load(std::memory_order_acquire)) { __NOP(); }              // Blocking wait until we can use the bus.

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
    return Enqueue(true, refHeader, const_cast<uint8_t*>(ptrSrc), length, refCallback);
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
    return Enqueue(false, refHeader, ptrDest, length, refCallback);
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
        // Wait for bus to become idle, then atomically acquire it.
        // exchange returns the previous value: spin while we observed 'true'.
        while (mBusy.exchange(true, std::memory_order_acq_rel)) { __NOP(); }

        result = mI2C.WriteBlocking(refHeader, ptrSrc, length);
        assert(result);
        mBusy.store(false, std::memory_order_release);
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
        // Wait for bus to become idle, then atomically acquire it.
        // exchange returns the previous value: spin while we observed 'true'.
        while (mBusy.exchange(true, std::memory_order_acq_rel)) { __NOP(); }

        result = mI2C.ReadBlocking(refHeader, ptrDest, length);
        assert(result);
        mBusy.store(false, std::memory_order_release);
    }

    return result;
}


/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
/**
 * \brief   Queue a request and start the bus if it is idle.
 * \details Common path for Write/Read. Holds the multi-producer lock only
 *          while pushing to the SPSC queue, then attempts to claim the bus
 *          via a single CAS on mBusy. The CAS winner starts the transfer at
 *          the head of the queue; the losers leave their element in the
 *          queue for the in-flight transaction's callback
 *          (DataRequestHandler) to pick up.
 * \param   isWrite     True for a write request, false for a read request.
 * \param   refHeader   Slave/register header.
 * \param   ptrData     Source (write) or destination (read) pointer.
 * \param   length      Number of bytes to transfer.
 * \param   refCallback Callback to invoke when the transaction completes.
 * \returns True if the request was queued (and possibly started), else false.
 */
bool I2CArbiter::Enqueue(bool isWrite, const HeaderI2C& refHeader, uint8_t* ptrData, size_t length, const std::function<void()>& refCallback)
{
    assert(mI2C.IsInit());

    ArbiterElementI2C element;
        element.is_write_request = isWrite;
        element.header           = refHeader;
        element.ptrData          = ptrData;
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

    // Start the transmission, if not busy yet.
    // Use atomic compare-exchange to prevent race condition where multiple
    // threads could both see mBusy as false and both start a transmission.
    if (result && mI2C.IsInit())
    {
        bool expected = false;
        if (mBusy.compare_exchange_strong(expected, true,
                                          std::memory_order_acq_rel,
                                          std::memory_order_acquire))
        {
            // We claimed the idle bus. Start the element at the head of the
            // queue - not necessarily our own: another producer may have
            // pushed just before us. Starting the head keeps transfers and
            // their completion callbacks in queue order.
            StartQueuedTransfer();
        }
    }

    return result;
}

/**
 * \brief   Start the transfer at the head of the queue, or release the bus.
 * \details May only be called while owning the bus (mBusy is true). Because
 *          only the bus owner touches the head of the queue, this keeps the
 *          buffer single-consumer. If a transfer fails to start, that element
 *          is dropped (it gets no completion callback) and the next one is
 *          tried, so a single bad request cannot wedge the bus. When the
 *          queue is empty the bus is released.
 */
void I2CArbiter::StartQueuedTransfer()
{
    ArbiterElementI2C element;

    while (mBuffer.peek(element))
    {
        // Reroute the transfer-done callback to the arbiter.
        const bool started = element.is_write_request
            ? mI2C.Write(element.header, element.ptrData, element.length, [this]() { this->DataRequestHandler(); })
            : mI2C.Read (element.header, element.ptrData, element.length, [this]() { this->DataRequestHandler(); });
        assert(started);
        if (started)
        {
            return;     // Bus stays busy; the callback enters DataRequestHandler.
        }

        // Could not start this element; discard it and try the next.
        ArbiterElementI2C dropped;
        mBuffer.pop(dropped);
    }

    mBusy.store(false, std::memory_order_release);
}

/**
 * \brief   Handler which is called when either TX or RX is done
 *          for I2C, allowing arbitration on the bus.
 * \details Pops the finished element, calls its callback, then starts
 *          the next queued transfer (or releases the bus).
 */
void I2CArbiter::DataRequestHandler()
{
    ArbiterElementI2C element;

    // We own the bus while in this handler, so we are the only consumer of
    // the queue. The finished transfer is always the head of the queue,
    // because transfers are only ever started from the head.
    mBuffer.pop(element);

    // Call the callback, if there was one set.
    if (element.callbackDone)
    {
        element.callbackDone();
    }

    StartQueuedTransfer();
}
