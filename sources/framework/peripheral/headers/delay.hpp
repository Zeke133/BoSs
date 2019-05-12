/**
 * @file    delay.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    11-04-2019
 * @brief   Delay class.
 * @details   Implementation of delays.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <IDelayer.hpp>
#include <IProfiler.hpp>

#include <stm32f1xx.h>

/**
 * Delay implementation based on DWT.
 * Supports delays in Microseconds.
 */
class Delay : public IProfiler, public IDelayer {

public:

    Delay();

    // delete copy constructor and assignment operator
    Delay(const Delay&) = delete;
    Delay& operator=(const Delay&) = delete;

    // IDelayer implementation
    void us(unsigned int us);                   /**< error may acure because of overflow every 59.65232354166667 sec */

    // IProfiler implementation
    virtual void startProfiling(void);          /**< start counting of cpu ticks */
    virtual auto getExecutionTicks(void) -> unsigned int;   /**< get amount of CPU ticks from measuring start */

    auto sysTicks2us(unsigned int ticks) -> unsigned int;

private:

    auto getDWTTicksValue(void) const -> unsigned int;

    const unsigned int sysTicksPerUs;
    unsigned int profilingStart;       /**< Start value of DWT */

};
