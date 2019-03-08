/**
 * @file    sleeping.hpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    07-03-2019
 * @brief   Sleeping class
 * @details   Implementation of sleeping threads queue
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#pragma once

#include <cstdint>

#include "thread.hpp"

/**
 * Class implements sleeping threads queue.
 */
class SleepingThreads {

public:

    void addThread(Thread * thread, uint32_t counter) {
        listHead = thread;
        // addIfNotEmpty

        // sum all counters
        // thread->setCounter(counter - sum);
    };

    // this will call from SysTick
    void tickCounters() {
        // while(thread = listHead->next == 0) {
        //     ... getNext
        // }
        // thread->counter--
    }

    // this will call from PendSV
    // return null or Thread ready to be runned
    Thread * getReadyThread() {
        // listHead->moveToNext
        // return thread;
        return nullptr;
    }

private:

    void pushQueue(Thread * thread, uint32_t sleepTicks) {

        if (queueHead == nullptr) {

            queueHead = thread;
            thread->setSleepTicks(sleepTicks);
            thread->setNext(nullptr);
        }
        else {

            auto curr = queueHead;
            uint32_t sumSleepTicks = 0;

            while (curr != nullptr) {

                auto currSleepTicks = curr->getSleepTicks();
                if (sleepTicks < sumSleepTicks + currSleepTicks) {

                    // correct to handle insert in the middle of queue
                    // insert only if next is more than inserted otherwise getNext
                    thread->setSleepTicks(sleepTicks);
                    thread->setNext(curr);
                    curr->setSleepTicks(currSleepTicks - sleepTicks);
                    queueHead = thread;
                }
                else {

                    sumSleepTicks += currSleepTicks;
                    curr = curr->getNext();
                }
            }
            // next == nullptr                    
            curr->setNext(thread);
            thread->setNext(nullptr);
            thread->setSleepTicks(sleepTicks - currSleepTicks);
        }            
    }

    Thread * pullQueue() {

    }

    Thread * queueHead = nullptr;    /**< Pointer to queue head */
    Thread * tail = nullptr;

};
