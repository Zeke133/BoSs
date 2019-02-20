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
Thread * Scheduler::currentThread = nullptr;
Scheduler::Decision Scheduler::lastDecision = Scheduler::Decision::noAction;
// /**@}*/

/**
 * Some text.
 * About public method.
 * 
 * Implicitly change Decision private atribute
 */
void Scheduler::takeDecision() {

    if (currentThread != nullptr) {

        if (currentThread->getState() == Thread::State::initialized) {

            lastDecision = Decision::onlyRestore;
            // current thread was just initialized and never run yet
            // so has no context to save
            // HAPPENS just on 1st thread!!!???

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

/**
 * Some text.
 * About public method.
 * 
 * Implicitly change Decision private atribute
 */
void Scheduler::pauseCurrentThread() {

    currentThread->setState(Thread::State::paused);
    currentThread = currentThread->getNext();
}

/**
 * Some text.
 * About public method.
 * 
 * Implicitly change Decision private atribute
 */
void Scheduler::runCurrentThread() {

    currentThread->setState(Thread::State::running);
}

/**
 * Some text.
 * About public method.
 * 
 * Implicitly change Decision private atribute
 */
void Scheduler::addThread(Thread * thread) {

    // last element must be linked to first one
    if (currentThread == nullptr) {

        currentThread = thread;

        if (currentThread->getNext() == nullptr) {
            
            currentThread->setNext(currentThread);
        }

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
