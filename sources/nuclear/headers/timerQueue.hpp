/**
 * @file    timerQueue.hpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    17-03-2019
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
 * On each tick just one element in queue needs to be changed, not all the stored timers.
 * It's achieved because each element store time addition between timers instead of absolute time.
 * Such the way it must help to improve scheduling performance.
 * tick - O(1) constant amortized timer tick execution time.
 * pop - O(1) constant expired timer access time.
 * push - O(n) linear amortized time of new timer addition.
 */
class TimerQueue {

public:

    void push(Thread& newThread, unsigned int needToSleep);

    void tick();

    auto popExpired() -> Thread *;
    
private:

    ThreadList queue;   /**< boost::intrusive slist */

};
