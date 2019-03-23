/**
 * @file    scheduler.cpp
 * @author  Denis Homutovski
 * @version V1.0.2
 * @date    18-02-2019
 * @brief   Scheduler class
 * @details   Implementation of threads scheduling algorithm
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "scheduler.hpp"

ThreadList Scheduler::normalQueue;
Thread * Scheduler::activeThreadPtr = nullptr;
Thread * Scheduler::nextThreadPtr = nullptr;

/**
 * @brief  Thread scheduling algorithm implementation
 * @param  None
 * @retval Scheduler::Decision - decision about need for thread switch
 */
Scheduler::Decision Scheduler::takeDecision() {

    // Check timers/sleeping/blocked

    if (normalQueue.empty()) {
        
        return Decision::noAction;
    }
    else {

        nextThreadPtr = &normalQueue.front();
        normalQueue.pop_front();

        if (activeThreadPtr == nullptr && nextThreadPtr->getState() == Thread::State::initialized) {
            /**
             * Has to happen just once for 1st thread on system start.
             */
            return Decision::onlyRestore;
        }

        return Decision::saveAndRestore;
    }
}

/**
 * @brief  Set current active thread to Thread::State::waiting.
 *         Used directly from asembler context switcher.
 * @param  None
 * @retval Thread * - current thread pointer
 */
Thread * Scheduler::pauseActiveThread() {

    activeThreadPtr->setState(Thread::State::waiting);
    normalQueue.push_back(*activeThreadPtr);
    return activeThreadPtr;
}

/**
 * @brief  Step to next scheduled thread and
 *         set state to Thread::State::active.
 *         Used directly from asembler context switcher.
 * @param  None
 * @retval Thread * - current thread pointer
 */
Thread * Scheduler::activateNextThread() {

    activeThreadPtr = nextThreadPtr;
    activeThreadPtr->setState(Thread::State::active);
    return activeThreadPtr;
}

/**
 * @brief  Get pointer to current active thread instance
 * @param  None
 * @retval Thread * - current active thread pointer or nullptr
 */
auto Scheduler::getActiveThread(void) -> Thread * {

    return activeThreadPtr;
}

/**
 * 
 * 
 * @brief  Add new thread.
 * @param  Thread& - thread to add
 * @retval None
 */
void Scheduler::addThread(Thread& newThread) {

    normalQueue.push_back(newThread);
}

/**
 * 
 * 
 * @brief  Kill thread.
 * @param  Thread& - thread to kill
 * @retval None
 */
void Scheduler::kill(Thread& thread) {

    if (activeThreadPtr == &thread) {

        // self kill!
        thread.setState(Thread::State::killed);
        // call some systemCall to schedule to next thread
    }
    else {

        normalQueue.remove(thread);
    }
}
