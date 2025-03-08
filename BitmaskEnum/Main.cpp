/**
 * \file    BitmaskEnumExample.cpp
 * \brief   Example usage of bitmask operations with an enum class for LED colors.
 *
 * This example demonstrates how to define an enum class for LED colors and
 * perform bitmask operations to manage combinations of colors. The program
 * initializes LED colors, combines and filters them using bitwise operations,
 * and outputs the current state of the colors.
 *
 * Key Features Demonstrated:
 * - Defining an enum class for LED colors with associated bit values.
 * - Enabling bitmask operations for the enum class.
 * - Converting enum values to string representations for display.
 * - Combining colors using bitwise OR, filtering colors using bitwise AND,
 *   toggling colors using bitwise XOR, and negating colors using bitwise NOT.
 * - Displaying the current state of LED colors after each operation.
 *
 * The example showcases how to effectively use bitmasking with enums in C++
 * to manage multiple states in a compact and efficient manner.
 */

#include <iostream>
#include <string>
#include "BitmaskEnum.hpp"

// Define the enum class for LED colors
enum class LEDColors : uint8_t {
    NONE  = 0x00,   // No color
    BLUE  = 0x01,   // Bit 1
    RED   = 0x02,   // Bit 2
    GREEN = 0x04,   // Bit 3
    WHITE = 0x08    // Bit 4
};

// Enable bitmask operations for the LEDColors enum
ENABLE_BITMASK_OPERATORS(LEDColors)

// Function to convert LEDColors to string
std::string toString(LEDColors color) {
    switch (color) {
        case LEDColors::NONE:  return "NONE";
        case LEDColors::BLUE:  return "BLUE";
        case LEDColors::RED:   return "RED";
        case LEDColors::GREEN: return "GREEN";
        case LEDColors::WHITE: return "WHITE";
        default:               return "UNKNOWN";
    }
}

// Function to get the combined color as a string
std::string getCurrentColorsString(LEDColors colors) {
    std::string result;
    if (colors == LEDColors::NONE) {
        return "NONE";
    }
    if ((colors & LEDColors::BLUE) == LEDColors::BLUE)  result += "BLUE ";
    if ((colors & LEDColors::RED) == LEDColors::RED)    result += "RED ";
    if ((colors & LEDColors::GREEN) == LEDColors::GREEN) result += "GREEN ";
    if ((colors & LEDColors::WHITE) == LEDColors::WHITE) result += "WHITE ";
    return result.empty() ? "NONE" : result;
}


int main() {
    // Initialize LED colors
    LEDColors currentColors = LEDColors::NONE;

    // Combine colors using bitmask
    currentColors = LEDColors::RED | LEDColors::GREEN; // currentColors: Bit 2 and 3
    std::cout << "Current Colors: " << getCurrentColorsString(currentColors) << std::endl;

    // Add BLUE to the current colors
    currentColors |= LEDColors::BLUE; // currentColors: Bit 1, 2, and 3
    std::cout << "After adding BLUE: " << getCurrentColorsString(currentColors) << std::endl;

    // Filter, only if RED is set use it
    currentColors &= LEDColors::RED; // currentColors: Bit 2
    std::cout << "After filtering for RED: " << getCurrentColorsString(currentColors) << std::endl;

    // Combine BLUE and GREEN
    currentColors = LEDColors::BLUE | LEDColors::GREEN; // currentColors: Bit 1 and 3
    std::cout << "Current Colors: " << getCurrentColorsString(currentColors) << std::endl;

    // Filter, only if RED is set use it
    currentColors &= LEDColors::RED; // currentColors: 0x00
    std::cout << "After filtering for RED: " << getCurrentColorsString(currentColors) << std::endl;

    // Toggle WHITE
    currentColors ^= LEDColors::WHITE; // currentColors: Bit 4
    std::cout << "After toggling WHITE: " << getCurrentColorsString(currentColors) << std::endl;

    // Toggle WHITE again
    currentColors ^= LEDColors::WHITE; // currentColors: 0x00
    std::cout << "After toggling WHITE again: " << getCurrentColorsString(currentColors) << std::endl;

    // Set all but GREEN and RED
    currentColors = ~(LEDColors::GREEN | LEDColors::RED); // currentColors: Bit 1 and 4
    std::cout << "After setting all but GREEN and RED: " << getCurrentColorsString(currentColors) << std::endl;

    // Combine BLUE and GREEN
    currentColors = LEDColors::BLUE | LEDColors::GREEN; // currentColors: Bit 1 and 3
    std::cout << "Current Colors: " << getCurrentColorsString(currentColors) << std::endl;

    // Remove GREEN
    currentColors &= ~LEDColors::GREEN; // currentColors: Bit 1
    std::cout << "After removing GREEN: " << getCurrentColorsString(currentColors) << std::endl;

    // Add GREEN back
    currentColors |= LEDColors::GREEN; // currentColors: Bit 1 and 3
    std::cout << "After adding GREEN back: " << getCurrentColorsString(currentColors) << std::endl;

    return 0;
}
