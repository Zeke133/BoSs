/**
 * @file    rtc.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   RealTimeClock class
 * @details   Internal real time clock driver
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_pwr.h>
#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_rtc.h>

/**
 * Internal real time clock driver API.
 */
class RealTimeClock {

public:

    RealTimeClock();

    inline auto getTime(void) const -> uint32_t;
    inline void setTime(uint32_t unixTime) const;

    RealTimeClock(const RealTimeClock&) = delete;               /**< Delete copy constructor. */
    RealTimeClock& operator=(const RealTimeClock&) = delete;    /**< Delete copy constructor. */

private:

    inline auto init(void) const -> bool;

};
