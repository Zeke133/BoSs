/**
 * @file    timerQueue.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    13-03-2019
 * @brief   TimerQueue class
 * @details   Implementation of queue for thread timers
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "timerQueue.hpp"

/**
 * @brief  Push timer to queue.
 * @param  newThread: pointer to thread which need to be timed-out
 * @param  needToSleep: amount of tick() after which timer must shout.
 * @retval None
 */
void TimerQueue::push(Thread * newThread, unsigned int needToSleep) {

    auto threadIterator = queueHead;
    decltype(threadIterator) prevThreadIterator = nullptr;
    
    unsigned int sumSleepTicks = 0;

    while (threadIterator != nullptr) {

        sumSleepTicks += threadIterator->getSleepTicks();

        if (needToSleep < sumSleepTicks) {
            break;
        }

        prevThreadIterator = threadIterator;
        threadIterator = threadIterator->getNext();
    }

    // update link in previous element
    if (prevThreadIterator == nullptr) queueHead = newThread;
    else prevThreadIterator->setNext(newThread);

    // create link to next element
    if (threadIterator == nullptr) {

        newThread->setNext(nullptr);
        newThread->setSleepTicks(needToSleep - sumSleepTicks);
    }
    else {

        newThread->setNext(threadIterator);
        newThread->setSleepTicks(needToSleep - (sumSleepTicks - threadIterator->getSleepTicks()));

        // update next element timer
        threadIterator->setSleepTicks(sumSleepTicks - needToSleep);
    }
}

/**
 * @brief  Decrement timers in queue.
 * @retval None
 */
void TimerQueue::tick() {

    auto threadIterator = queueHead;

    while (threadIterator != nullptr) {

        auto threadSleepTicks = threadIterator->getSleepTicks();

        if (threadSleepTicks > 0) {

            threadIterator->setSleepTicks(--threadSleepTicks);
            break;
        }
        else {

            threadIterator = threadIterator->getNext();
        }
    }
}

/**
 * @brief  Pop expired thread from queue. It will be removed from queue.
 *         Can return only one thread, so must be called untill no any expired threads
 *         will remain in queue.
 * @retval 'Thread *' pointer to expired thread or 'nullptr' if no any expired thread in queue.
 */
auto TimerQueue::popExpired() -> Thread * {

    if (queueHead != nullptr && queueHead->getSleepTicks() == 0) {

        auto popHead = queueHead;
        queueHead = queueHead->getNext();
        return popHead;
    }
    return nullptr;
}

