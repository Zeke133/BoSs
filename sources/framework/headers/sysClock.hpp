/**
 * @file    sysClock.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    01-03-2019
 * @brief   System clock initialization API
 * @details   System clock initialization API
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_utils.h>

/**
 * System clock initialization API
 */
namespace SystemClock {
    
void setClockTo72Mhz(void);

}
