/**
 * @file    convertation.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   Convertation methods
 * @details   Convertation methods
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "convertation.hpp"

#include <array>

auto Convertation::itoa(unsigned int val, char base, char width) -> const char * {

    const char * symbols = "0123456789ABCDEF";
    static std::array<char, 11> buf;
    int i;

    for(i = 0; i < 10; i++) {
        buf[i] = '0';
    }

    buf[i--] = 0;

    while(1) {

        if(i < 0) return &buf[0];

        buf[i] = symbols[val % base];
        val /= base;

        if(val == 0) return width ? &buf[10-width] : &buf[i];
        i--;
    }
    return "ERROR!";
}

auto Convertation::atoi(const char * cstring, char base, char signs) -> int {

    int32_t res = 0;
    int8_t sign = 1;
    const uint8_t * symbols = (const uint8_t *)"0123456789ABCDEF";

    if (base > 16) return 0;

    uint32_t i = 0;

    if (cstring[0] == '-') {
        sign = -1;
        i = 1;
    }

    for( ; i < signs; i++) {
        // check for end of string
        if (cstring[i] == 0) break;
        // search of symbol
        uint8_t s = 0;
        for ( ; s < base; s++) {
            if (cstring[i] == symbols[s]) break;
        }
        // no such symbol
        if (s == base) break;
        // add to number
        res *= base;
        res += s;
    }

    return res * sign;
}

auto Convertation::ds18b20Temp2decimal(uint16_t tempDs18b20) -> int {

    int result = 0;

    if (tempDs18b20 & 0xF800) {     // sign is "-"
        result = (~tempDs18b20 >> 4) & 0x7F;
        result *= 10000;
        result += 10000;
        if (tempDs18b20 & 0x08) result -= 5000;
        if (tempDs18b20 & 0x04) result -= 2500;
        if (tempDs18b20 & 0x02) result -= 1250;
        if (tempDs18b20 & 0x01) result -= 625;
        result *= -1;
    } else {                        // sign is "+"
        result = (tempDs18b20 >> 4) & 0x7F;
        result *= 10000;        
        if (tempDs18b20 & 0x08) result += 5000;
        if (tempDs18b20 & 0x04) result += 2500;
        if (tempDs18b20 & 0x02) result += 1250;
        if (tempDs18b20 & 0x01) result += 625;
    }
    return result;
}
