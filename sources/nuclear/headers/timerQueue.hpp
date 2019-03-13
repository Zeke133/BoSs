/**
 * @file    timerQueue.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    13-03-2019
 * @brief   TimerQueue class
 * @details   Implementation of queue for thread timers
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <cstdint>

#include "thread.hpp"

/**
 * Class implements queue for thread timers.
 * On each tick just 1 element is changed, not all the stored timers.
 * Queue store time difference between timers instead of absolute time.
 * Such the way it must help to improve scheduling performance.
 * O(1) amortized timer tick execution time.
 * O(1) amortized expired timer return time.
 * O(n) amortized addition of new timer time.
 */
class TimerQueue {

public:

    void push(Thread * newThread, unsigned int needToSleep);

    void tick();

    auto popExpired() -> Thread *;
    
private:

    Thread * queueHead = nullptr;    /**< Pointer to queue head */

};
