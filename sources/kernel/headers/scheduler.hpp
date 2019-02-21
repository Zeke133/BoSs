/**
 * @file    scheduler.hpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    21-02-2018
 * @brief   Scheduler class
 * @details   Implementation of threads scheduling algorithm
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

// using
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
        noAction = 0,   /**< context switch is not needed */
        onlyRestore,    /**< context just need to be restored from thread stack */
        saveAndRestore, /**
                         * need to save current context
                         * switch current thread pointer to next thread scheduled
                         * and restore new context
                         */
    };

    /**
     * Add thread to scheduler list tail.
     * If list is empty - add thread and create self reference in it.
     */
    static void addThread(Thread * thread);

    static Decision takeDecision(void);     /**< Used from ASM context switch procedure */
    static Thread * getCurrentThread(void); /**< Used from ASM context switch procedure */

private:

    static void pauseCurrentThread(void);   /**< Used from ASM context switch procedure */
    static void runCurrentThread(void);     /**< Used from ASM context switch procedure */
   
    static Thread * currentThread;      /**< Pointer to current active thread instance */
    static Decision lastDecision;       /**< Last decision of scheduler */
    
};

#endif
