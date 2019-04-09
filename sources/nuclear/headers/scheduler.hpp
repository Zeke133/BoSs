/**
 * @file    scheduler.hpp
 * @author  Denis Homutovski
 * @version V1.0.2
 * @date    18-03-2019
 * @brief   Scheduler class
 * @details   Implementation of threads scheduling algorithm
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include "thread.hpp"
#include "timerQueue.hpp"
#include "svCallApi.hpp"

class SchedulerTestProxy;   /**< Used for private API Unit-tests */

/**
 * Class implement kernel threads scheduling algorithm.
 * Some parts of code are used from Asembler, so change carefully.
 */
class Scheduler {

friend SchedulerTestProxy;

public:

    // user API part
    static void run(Thread& newThread);
    static void kill(Thread& thread);
    static void sleep(unsigned int ms);

    // system API part
    static void tick() { sleepingQueue.tick(); };           /**< Used with SysTick */

private:

    // system API part used mostly from ASM procedures
    // and made private to hide from user
    static auto switchThread() -> Thread *;                 /**< Used with ASM PendSV */
    static auto sleepActiveThread() -> Thread *;            /**< Used with ASM SVCall */
    static auto getActiveThread() { return activeThreadPtr; };
    // internal usage
    static void pauseActiveThread();                        /**< Used with switchThread() */
    static void setActiveThread(Thread * nextThreadPtr);    /**< Used with switchThread() */

    // prioritized ?
    static ThreadList normalQueue;              /**< boost::intrusive slist */
    static TimerQueue sleepingQueue;            /**< Sleeping threads queue */
    // TimersQueue - Make HIGHEST priority and use just for really short procedures
    // BlockedThreadsQueue - mutex/semafore and s.o.

    static Thread * activeThreadPtr;            /**< Current active thread pointer */

};
