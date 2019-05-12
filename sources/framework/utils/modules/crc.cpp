/**
 * @file    crc.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    13-04-2019
 * @brief   CRC calculation methods
 * @details   CRC calculation methods
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "crc.hpp"

auto CRC::calcCRC_1wire(unsigned char crc, unsigned char byte) -> unsigned char {

    for (unsigned char i = 0; i < 8; i++) {
        
        // exclusive or least sig bit of current shift reg with the data bit
        unsigned char firstBit = (crc & 0x01) ^ (byte & 0x01);
        // shift one place to the right
        crc >>= 1;
        // CRC ^ binary 1000 1100
        if (firstBit) crc ^= 0x8C;
        byte >>= 1;
    }
    return crc;
}

auto CRC::calcCRC_1wireQuick(unsigned char crc, unsigned char byte) -> unsigned char {
    
    return crc8tableDallas[crc ^ byte];
}
