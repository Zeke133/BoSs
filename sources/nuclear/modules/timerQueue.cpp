/**
 * @file    timerQueue.cpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    17-03-2019
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
void TimerQueue::push(Thread& newThread, unsigned int needToSleep) {

    auto it = queue.begin();
    auto prevIt = it;
    unsigned int sumSleepTicks = 0;

    const auto& endIt = queue.cend();
    while (it != endIt) {

        sumSleepTicks += (*it).getSleepTicks();

        if (needToSleep < sumSleepTicks) {
            break;
        }

        prevIt = it++;
    }

    if (it == endIt) {

        newThread.setSleepTicks(needToSleep - sumSleepTicks);
        queue.push_back(newThread);
    }
    else {

        if (it == prevIt) { // when inserting element on 1-st position
            
            newThread.setSleepTicks(needToSleep);
            (*it).setSleepTicks(sumSleepTicks - needToSleep);
            queue.push_front(newThread);
        }
        else {

            newThread.setSleepTicks(needToSleep - (sumSleepTicks - (*it).getSleepTicks()));
            queue.insert_after(prevIt, newThread);
            (*it).setSleepTicks(sumSleepTicks - needToSleep);
        }
    }
}

/**
 * @brief  Decrement timers in queue. Need to be called each SysTick.
 * @retval None
 */
void TimerQueue::tick() {

    auto it = queue.begin();
    while (it != queue.cend()) {

        auto threadSleepTicks = (*it).getSleepTicks();
        if (threadSleepTicks > 0) {

            (*it).setSleepTicks(--threadSleepTicks);
            return;
        }
        it++;
    }
}

/**
 * @brief  Pop expired thread from queue. It will be removed from queue.
 *         Can return only one thread, so must be called untill no any expired threads
 *         will remain in queue.
 * @retval 'Thread *' pointer to expired thread or 'nullptr' if no any expired thread in queue.
 */
auto TimerQueue::popExpired() -> Thread * {

    if (!queue.empty() && queue.front().getSleepTicks() == 0) {

        auto frontPtr = &(queue.front());
        queue.pop_front();
        return frontPtr;
    }
    return nullptr;
}

