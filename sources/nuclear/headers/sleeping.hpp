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

    void push(Thread * addThread, unsigned int needToSleep) {

        auto threadIterator = queueHead;
        decltype(threadIterator) prevThreadIterator = nullptr;
        
        unsigned int sumSleepTicks = 0;

        while (threadIterator != nullptr) {

            sumSleepTicks += threadIterator->getSleepTicks();
            if (needToSleep < sumSleepTicks)
                break;

            prevThreadIterator = threadIterator;
            threadIterator = threadIterator->getNext();
        }

        if (threadIterator == nullptr) {    // adding last element

            addThread->setNext(nullptr);
            addThread->setSleepTicks(needToSleep - sumSleepTicks);

            if (prevThreadIterator == nullptr) {    // adding to empty list
                
                queueHead = addThread;
            }
            else {

                prevThreadIterator->setNext(addThread);
            }
        }
        else {

            prevThreadIterator->setNext(addThread);
            addThread->setNext(threadIterator);
            addThread->setSleepTicks(needToSleep - (sumSleepTicks - threadIterator->getSleepTicks()));
            threadIterator->setSleepTicks(sumSleepTicks - needToSleep);
        }

        // extract common part
    }

    void tick() {

        auto threadIterator = queueHead;

        while (threadIterator != nullptr) {

            auto headSleepTicks = threadIterator->getSleepTicks();

            if (headSleepTicks != 0) {

                queueHead->setSleepTicks(headSleepTicks - 1);
                break;
            }
            else {

                threadIterator = threadIterator->getNext();
            }
        }
    }

    auto getExpired() -> Thread * {

        auto thread = queueHead;

        if (thread == nullptr) return nullptr;

        else {

            if (thread->getSleepTicks() == 0) {
                
                queueHead = thread->getNext();
                return thread;
            }
            else {
                
                return nullptr;
            }
        }
    }
    
private:

    Thread * queueHead = nullptr;    /**< Pointer to queue head */

};
