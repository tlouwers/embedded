/**
 * \file main.cpp
 *
 * \licence "THE BEER-WARE LICENSE" (Revision 42):
 *          <terry.louwers@fourtress.nl> wrote this file. As long as you retain
 *          this notice you can do whatever you want with this stuff. If we
 *          meet some day, and you think this stuff is worth it, you can buy me
 *          a beer in return.
 *                                                                Terry Louwers
 *
 * \brief   Main entry point for the application.
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
#include <cassert>
#include <iostream>         // std::cout, std::endl
#include "Application_Stub.hpp"


/************************************************************************/
/* Main entry point of application                                      */
/************************************************************************/
/**
 * \brief   Main entry point of the application.
 *          Initializes stuff, then calls 10x the Test() method of 'app' to
 *          mimic a class trying to access the I2C bus in very quick
 *          succession. Then awaits the user to enter "q"to quit. In the mean
 *          time, the handling of I2C is managed by the Arbiter, meaning there
 *          are no collisions in the (stubbed) transactions on I2C.
 * \returns Always 0.
 */
int main(void)
{
    // Init pins, set clock crystals, etc

    Application_Stub app;

    bool result = app.Init();
    assert(result);


    // Queuing 10 requests
    for (auto i = 0; i < 10; i++)
    {
        app.Test();
    }


    // Wait until the callbacks are called: ~200 ms
    std::string user_input;
    std::cout << "Please enter 'q' to quit" << std::endl;
    do
    {
        std::cin >> user_input;
    } while (user_input != "q");


	return 0;
}
