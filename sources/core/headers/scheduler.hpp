/**
 * @file    scheduler.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    18-02-2018
 * @brief   Scheduler class
 * @details   Implementation of threads planning
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
 * Class header code-style template.
 * Here put detailed description ...
 * ...
 */
class Scheduler {

public:

    /**
     * Specifies decisions of scheduler to be fulfilled in interrupt handler
     */
    enum class Decision : uint8_t {
        noAction = 0,   /// context switch is not needed
        onlyRestore,    /// context just need to be restored from thread stack
        saveAndRestore, /// context need to be saved, thread switched to next
                        /// and new context restored

        LAST_ELEMENT
    };

    /**
     * Add thread to scheduler list tail.
     * If list is empty - add thread and create self reference in it.
     */
    static void addThread(Thread * thread);

private:

    static void takeDecision(void);     /**< Used from ASM context switch procedure */
    static void pauseCurrentThread(void);   /**< Used from ASM context switch procedure */
    static void runCurrentThread(void); /**< Used from ASM context switch procedure */
   
    static Thread * currentThread;      /**< Pointer to current active thread instance */
    static Decision lastDecision;       /**< Last decision of scheduler */
    
};

#endif
