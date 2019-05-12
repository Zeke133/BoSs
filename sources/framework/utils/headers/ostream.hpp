/**
 * @file    ostream.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    30-04-2019
 * @brief   Output stream class
 * @details   Simple output stream implementation
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "ITextOutput.hpp"
#include "convertation.hpp"

/**
 * Simple output stream implementation for better usability.
 */
class OStream {

public:

    OStream(ITextOutput& driver) : outputDevice(driver) {};

    enum class OutSet {
        hex,
        dec
    };

    OStream& operator<<(enum OutSet manipulator);       // for output manipulators
    OStream& operator<<(char symbol);                   // for single character
    OStream& operator<<(const char * string);           // for C-string
    OStream& operator<<(uint8_t);
    OStream& operator<<(int16_t);
    OStream& operator<<(uint16_t);
    OStream& operator<<(int32_t num);
    OStream& operator<<(uint32_t num);

private:

    ITextOutput& outputDevice;
    char outputNumbersBase = 10;

};
