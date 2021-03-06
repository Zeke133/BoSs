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

#include "ostream.hpp"

using Convertation::itoa;

/**
 * @brief  Output stream configuration.
 * @param  manipulator: dec - decimal output; hex - hexadecimal.
 * @retval OStream&
 */
OStream& OStream::operator<<(enum OutSet manipulator) {

    switch (manipulator)
    {
    case OutSet::dec:
        outputNumbersBase = 10;
        break;

    case OutSet::hex:
        outputNumbersBase = 16;
        break;
    }
    
    return *this;
};

OStream& OStream::operator<<(char symbol) {
    outputDevice.putc(symbol);
    return *this;
};

OStream& OStream::operator<<(const char * cString) {
    outputDevice.puts(cString);
    return *this;
};

OStream& OStream::operator<<(uint8_t num) {
    outputDevice.puts(itoa(num, outputNumbersBase));
    return *this;
};
OStream& OStream::operator<<(uint16_t num) {
    outputDevice.puts(itoa(num, outputNumbersBase));
    return *this;
};
OStream& OStream::operator<<(int16_t num) {
    if (num < 0) outputDevice.putc('-');
    outputDevice.puts(itoa(num, outputNumbersBase));
    return *this;
};
OStream& OStream::operator<<(uint32_t num) {
    outputDevice.puts(itoa(num, outputNumbersBase));
    return *this;
};
OStream& OStream::operator<<(int32_t num) {
    if (num < 0) outputDevice.putc('-');
    outputDevice.puts(itoa(num, outputNumbersBase));
    return *this;
};
