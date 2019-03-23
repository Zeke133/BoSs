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

#include <cstdint>

#include "thread.hpp"

/**
 * Class implement kernel threads scheduling algorithm.
 * Some parts of code are used from Asembler, so change carefully.
 */
class Scheduler {

public:

    /**
     * Specifies decisions of scheduler to be fulfilled in interrupt handler
     */
    enum class Decision : uint8_t {
        noAction = 0,       /**< context switch is not needed */
        onlyRestore = 1,    /**< context just need to be restored from thread stack */
        saveAndRestore = 2  /**
                             * need to save current context
                             * switch current thread pointer to next thread scheduled
                             * and restore new context
                             */
    };

    static void addThread(Thread& newThread);
    static void kill(Thread& thread);

    static auto takeDecision(void) -> Decision; /**< Used from ASM context switch procedure */
    static auto getActiveThread(void) -> Thread * ;

    // join()
    // wait()

private:

    static Thread * pauseActiveThread(void);    /**< Used from ASM context switch procedure */
    static Thread * activateNextThread(void);   /**< Used from ASM context switch procedure */

    static ThreadList normalQueue;              /**< boost::intrusive slist */

    static Thread * activeThreadPtr;            /**< Current active thread pointer */
    static Thread * nextThreadPtr;              /**< Next thread to be executed pointer */

    // ActiveThreadsQueue - prioritized?
    // SleepingThreadsQueue
    // TimersQueue - Make HIGHEST priority and use just for really short procedures
    // BlockedThreadsQueue - mutex/semafore and s.o.
};
