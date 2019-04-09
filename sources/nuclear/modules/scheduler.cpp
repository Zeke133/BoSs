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
TimerQueue Scheduler::sleepingQueue;
Thread * Scheduler::activeThreadPtr = nullptr;

/**
 * @brief  Thread scheduling algorithm implementation
 * @param  None
 * @retval Thread * - pointer to thread SP to be restored or nullptr
 */
auto Scheduler::switchThread() -> Thread * {

    // Check timers/sleeping/blocked
    {
        auto awakeThread = sleepingQueue.popExpired();
        
        while (awakeThread != nullptr) {

            normalQueue.push_back(*awakeThread);
            awakeThread = sleepingQueue.popExpired();
        }
    }

    if (!normalQueue.empty()) {

        auto nextThreadPtr = &normalQueue.front();
        normalQueue.pop_front();

        if (activeThreadPtr != nullptr) {

            pauseActiveThread();
            normalQueue.push_back(*activeThreadPtr);
            SaveContext(activeThreadPtr);
        }

        setActiveThread(nextThreadPtr);
        return activeThreadPtr;

    } else {
        
        if (activeThreadPtr != nullptr) {
            // return to Thread PSP
            // this code can be optimized
            // here is no need to save and restore same context
            SaveContext(activeThreadPtr);
            return activeThreadPtr;
        }
        // return to Thread MSP
        return nullptr;
    }
}

/**
 * @brief  Set active thread to Thread::State::waiting
 *         and push it to the back of queue.
 * @param  None
 * @retval None
 */
void Scheduler::pauseActiveThread() {

    activeThreadPtr->setState(Thread::State::waiting);
}

/**
 * @brief  Set state to Thread::State::active.
 * @param  Thread * - next thread pointer
 * @retval None
 */
void Scheduler::setActiveThread(Thread * nextThreadPtr) {

    activeThreadPtr = nextThreadPtr;
    activeThreadPtr->setState(Thread::State::active);
}

/**
 * @brief  Set active thread to Thread::State::sleep
 *         and push it to the sleeping threads queue.
 *         Used directly from asembler SVCall.
 * @param  None
 * @retval Thread * - current thread pointer
 */
auto Scheduler::sleepActiveThread() -> Thread * {

    auto t = activeThreadPtr;
    activeThreadPtr = nullptr;
    sleepingQueue.push(*t, t->getSleepTicks());
    t->setState(Thread::State::sleep);
    return t;
}

/**
 * @brief  Add thread to execution queue.
 * @param  Thread& - thread to add
 * @retval None
 */
void Scheduler::run(Thread& newThread) {

    normalQueue.push_back(newThread);
}

/**
 * @brief  Kill thread.
 * @param  Thread& - thread to kill
 * @retval None
 */
void Scheduler::kill(Thread& thread) {

    thread.setState(Thread::State::killed);

    if (activeThreadPtr == &thread) {   // self kill!

        activeThreadPtr = nullptr;
        // call some systemCall to schedule to next thread
        // without currentThread context save ???
    }
    else {

        // check where delete from ???
        normalQueue.remove(thread);
        sleepingQueue.remove(thread);
    }
}

/**
 * @brief  Sleep current thread for amount of miliseconds
 * @param  unsigned int ms - miliseconds to sleep
 * @retval Thread * - current active thread pointer or nullptr
 */
void Scheduler::sleep(unsigned int ms) {

    auto t = Scheduler::getActiveThread();
    if (t != nullptr) {

        t->setSleepTicks(ms);
        svCallSleep();
    }
};
