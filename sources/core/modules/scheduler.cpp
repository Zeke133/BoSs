/**
 * @file    scheduler.cpp
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

#include <scheduler.hpp>

// /**@{*/
// /** Grouped description. */
// uint8_t ClassTempate::staticAtribute1 = 0;
// uint8_t ClassTempate::staticAtribute2 = 0;
// /**@}*/

/**
 * Some text.
 * About public method.
 * 
 * Implicitly change Decision private atribute
 */
static void Scheduler::execute() const {

    if (currentThread != nullptr) {

        if (currentThread->getState() == Thread::State::initialized) {

            lastDecision = Decision::onlyRestore;
            // current thread was just initialized and never run yet
            // so has no context to save

        } else {

            Thread * nextThread;
            while ((nextThread = currentThread->getNext()) != nullptr) {

                auto nextThreadState = nextThread->getState();
                
                if (nextThreadState == Thread::State::paused) {

                    // check some timers or mutex
                    // continue or break as result
                    break;
                }
                else if (nextThreadState == Thread::State::running) {

                    // nothing to do. we have only 1 thread in list
                    lastDecision = Decision::noAction;
                    return;
                }
                else if (nextThreadState == Thread::State::initialized) {

                    break;
                }
            }

            lastDecision = Decision::saveAndRestore;
        }
    } else {
        
        lastDecision = Decision::noAction;
        // print in core init - no any task was created
    }
}
