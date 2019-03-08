/**
 * @file    scheduler.cpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    21-02-2019
 * @brief   Scheduler class
 * @details   Implementation of threads scheduling algorithm
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "scheduler.hpp"

Thread * Scheduler::currentThread = nullptr;
Thread * Scheduler::nextThread = nullptr;
Scheduler::Decision Scheduler::lastDecision = Scheduler::Decision::noAction;

/**
 * @brief  Thread scheduling algorithm implementation
 * @param  None
 * @retval Scheduler::Decision - decision about need for thread switch
 */
Scheduler::Decision Scheduler::takeDecision() {

    if (currentThread != nullptr) {

        if (currentThread->getState() == Thread::State::initialized) {

            /**
             * Current thread was just initialized and never executed yet
             * so there is no need in context saving.
             * Must happen just once for 1st thread on system start. ???
             */
            nextThread = currentThread;
            return Decision::onlyRestore;

        } else {

            Thread * next;
            while ( (next = currentThread->getNext()) ) {

                if (next == nullptr) {

                    /**
                     * Need to crash or do nothing ???
                     */
                    return Decision::noAction;
                }

                auto nextState = next->getState();

                switch (nextState)
                {
                    case Thread::State::initialized:
                        /* Next thread run 1st time */
                        nextThread = next;
                        return Decision::saveAndRestore;

                    case Thread::State::paused:
                        /**
                         * check some timers or mutex
                         * break - if thread not ready to run
                         * than next thread will be checked
                         * need to set pointer to nextThread to execute !!!???
                         */
                        nextThread = next;
                        return Decision::saveAndRestore;
                    
                    case Thread::State::running:
                        /* nothing to do. we have only 1 thread in list */
                        return Decision::noAction;
                
                    default:
                        return Decision::noAction;
                }
            }
        }
    }
    
    return Decision::noAction;  /**< can print in core init that no any task was created! */
}

/**
 * @brief  Get pointer to current active thread instance
 * @param  None
 * @retval Thread * - current thread pointer
 */
Thread * Scheduler::getCurrentThread() {

    return currentThread;
}

/**
 * @brief  Set current active thread to Thread::State::paused
 * @param  None
 * @retval Thread * - current thread pointer
 */
Thread * Scheduler::pauseCurrentThread() {

    currentThread->setState(Thread::State::paused);
    return currentThread;
}

/**
 * @brief  Step to next thread in list and
 *         set thread state to Thread::State::running
 * @param  None
 * @retval Thread * - current thread pointer
 */
Thread * Scheduler::runNextThread() {

    currentThread = nextThread;
    currentThread->setState(Thread::State::running);
    return currentThread;
}

/**
 * 
 * 
 * @brief  Add thread to scheduler list tail.
 *         If list is empty - add thread and create self reference in it.
 * @param  Thread * - thread to add
 * @retval None
 */
void Scheduler::addThread(Thread * thread) {

    /**
     * last element must be linked to the first one
     * [0]->[1]->[3]->[4]
     *  ^              |
     *  |______________|
     */
    if (currentThread == nullptr) {

        currentThread = thread;
        currentThread->setNext(currentThread);

    } else {

        auto threadIterator = currentThread;
        // iterate throw the enclosed list of threads
        // untill find the last element (has link to current element)
        while (threadIterator->getNext() != currentThread) {

            threadIterator = threadIterator->getNext();
        }
        // insert new thread to the last posititon
        thread->setNext(threadIterator->getNext());
        threadIterator->setNext(thread);
    }
}
