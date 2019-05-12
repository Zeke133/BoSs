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

#include "delay.hpp"

/**
 * Configuring DWT (Data Watchpoint and Trace) for implementation of 1us delays
 */
Delay::Delay() :
    sysTicksPerUs(SystemCoreClock / 1000000) {

    // DWT_Init
    // if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // }

    // For measuring of execution time
    profilingStart = 0;
}

void Delay::us(unsigned int us) {

    unsigned int ticksBegin = getDWTTicksValue();
    unsigned int ticksEnd = ticksBegin + us * sysTicksPerUs;
    // wait
    if (ticksEnd < ticksBegin)
        while (getDWTTicksValue() > ticksBegin || getDWTTicksValue() < ticksEnd) ;
    else
        while (getDWTTicksValue() < ticksEnd) ;
}

inline auto Delay::getDWTTicksValue(void) const -> unsigned int {

    return DWT->CYCCNT;
}

inline void Delay::startProfiling(void) {

    profilingStart = getDWTTicksValue();
}

inline auto Delay::getExecutionTicks(void) -> unsigned int {

    unsigned int ticksNow = getDWTTicksValue();

    // overflow happened
    // no overflow or execution time is more than 59 seconds what is unlikely

    return (profilingStart > ticksNow) ? (0xffffffff - profilingStart + ticksNow) : (ticksNow - profilingStart);
}

auto Delay::sysTicks2us(unsigned int ticks) -> unsigned int {

    return ticks / sysTicksPerUs;
}
