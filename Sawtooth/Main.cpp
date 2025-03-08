/*
 * \file    Main.cpp
 * \brief   Example usage of the Sawtooth class.
 *
 * \details This program demonstrates how to use the Sawtooth class to
 *          generate sawtooth waveform values. It creates two instances
 *          of the Sawtooth class with specified parameters and generates
 *          a series of values using the Next() method. The program also
 *          shows how to reset the sawtooth generator to start from zero
 *          again.
 *
 *          Example:
 *          - mSawtooth1 generates values in the range 0..79 with 5 steps of 16.
 *          - mSawtooth2 generates values in the range 0..99 with 8 steps of 12.
 *
 *          The output will display the generated values from both sawtooth
 *          generators and demonstrate the reset functionality.
 */

#include <iostream>
#include "Sawtooth.hpp"

int main() {
    // Create sawtooth objects with desired parameters
    Sawtooth mSawtooth1(79, 5);   // 0..79 in 5 steps of 16
    Sawtooth mSawtooth2(99, 8);   // 0..99 in 8 steps of 12

    // Generate values with 'Next()' and print them
    std::cout << "Values from mSawtooth1: ";
    for (auto i = 0; i < 10; i++) {
        uint16_t tmp1 = mSawtooth1.Next();
        std::cout << tmp1 << " ";
    }
    std::cout << std::endl;

    std::cout << "Values from mSawtooth2: ";
    for (auto i = 0; i < 10; i++) {
        uint16_t tmp2 = mSawtooth2.Next();
        std::cout << tmp2 << " ";
    }
    std::cout << std::endl;

    // Use 'Reset()' to start at value '0' again
    mSawtooth1.Reset();
    std::cout << "After resetting mSawtooth1, the first value is: " << mSawtooth1.Next() << std::endl;

    return 0;
}
