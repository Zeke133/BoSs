/**
 * @file    convertation.hpp
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

#pragma once

#include <cstdint>

namespace Convertation {

/**
 * @brief  Convertation of Integer to C string
 * @param  val: input value
 * @param  base: 10 for decimal or 16 for HEX view
 * @param  signs: fixed length of number, 0 - auto
 * @retval const char * string
 */
auto itoa(unsigned int val, char base = 10, char width = 0) -> const char * ;

/**
 * @brief  Convertation of C string to Integer
 * @param  cstring: input string
 * @param  base: 10 for decimal or 16 for HEX view
 * @param  signs: fixed length of number
 * @retval int value
 */
auto atoi(const char * cstring, char base = 10, char signs = 10) -> int;

/**
 * @brief  Date and time representation structure
 */
struct DateTime {

    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t date;
    uint8_t wday;
    uint8_t month;
    uint16_t year;
};

/**
 * @brief  Convertation of DateTime to unix format
 * @param  dateTime: date and time
 * @retval time in unix format
 */
constexpr auto dateTime2Unix(const DateTime& dt) -> unsigned int {

    constexpr int JULIAN_DATE_BASE = 2440588;

    uint8_t a = (14 - dt.month) / 12;
    uint16_t y = dt.year + 4800 - a;
    uint8_t m = dt.month + (12 * a) - 3;

    return (dt.date + ((153 * m + 2) / 5) + (365 * y) + (y / 4) + (-y / 100) + (y / 400) -
                 32045 - JULIAN_DATE_BASE) * 86400 + (dt.hours * 3600) + (dt.minutes * 60) + dt.seconds;

}

/**
 * @brief  Convertation of time in unix format to date and time structure
 * @param  unixTime: time in unix format
 * @retval Structure with date and time
 */
constexpr auto unix2DateTime(unsigned int unixTime) -> DateTime {
    
    // Calc of Julian date JD and Julian day number JDN
    unsigned int jd = ((unixTime + 43200) / (86400 >> 1)) + (2440587 << 1) + 1;
    unsigned int jdn = jd>>1;// Calc of date
    unsigned int a = jdn + 32044;
    unsigned int b = (4 * a + 3) / 146097;
    unsigned int c = a - (146097 * b) / 4;
    unsigned int d = (4 * c + 3) / 1461;
    unsigned int e = c - (1461 * d) / 4;
    unsigned int m = (5 * e + 2) / 153;

    return DateTime {

        .hours = (uint8_t)((unixTime/3600) - (unixTime/86400)*24),
        .minutes = (uint8_t)((unixTime/60) - (unixTime/3600)*60),
        .seconds = (uint8_t)(unixTime - (unixTime/60)*60),
        .date = (uint8_t)(e - (153 * m + 2) / 5 + 1),
        .wday = (uint8_t)(jdn % 7),
        .month = (uint8_t)(m + 3 - 12 * (m / 10)),
        .year = (uint16_t)(100 * b + d - 4800 + (m / 10)),
    };
}

/**
 * @brief  Convertation of Dallas DS18B20 temperature sensor format
 *         to Decimal format in form "125.0000"
 * @param  tempDs18b20: temperature in DS18B20 format
 * @retval Temperature in decimal formal
 */
auto ds18b20Temp2decimal(uint16_t tempDs18b20) -> int;

} // namespace Convertation
