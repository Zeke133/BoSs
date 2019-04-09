/**
 * @file    schedulerTestProxy.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    24-03-2019
 * @brief   Scheduler class Unit-tests proxy
 * @details   Used for testing of Scheduler private API
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "scheduler.hpp"

/**
 * Proxy class used for Scheduler private API Unit-tests
 */
class SchedulerTestProxy {

public:

    // system API part used mostly from ASM procedures
    
    static auto getActiveThread() {
        return Scheduler::getActiveThread();
    };

    static auto switchThread() {                /**< Used with PendSV */
        return Scheduler::switchThread();
    };
    static auto sleepActiveThread() {           /**< Used with SVCall */
        return Scheduler::sleepActiveThread();
    };

};

/**
 * BoSs Unit test mocks for Scheduler
 */
void SaveContext(Thread * t) {};

void svCallSleep(void) {

    auto t = SchedulerTestProxy::sleepActiveThread();
    SaveContext(t);
    // trigger PendSV
};
