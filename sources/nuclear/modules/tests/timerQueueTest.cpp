/**
 * @file    timerQueueTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    13-03-2019
 * @brief   TimerQueue class tests
 * @details   Unit tests
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

/* tested source */
#include "timerQueue.hpp"

/* testing framework */
#include <gtest/gtest.h>

/* std */
#include <vector>
#include <iterator>

namespace BossKernelUnitTests {

    class TimerQueueTest : public ::testing::Test {

        protected:

        TimerQueueTest() : dummyTask([](){ std::cout << "Dummy task"; }) {
            
            auto threadsCount = stacks.max_size();
            threads.reserve(threadsCount);
            
            for (int i = 0; i < threadsCount; ++i) {
                
                const auto& stackForThreadPtr = stacks.at(i).data();
                const auto& stackSize = stacks.at(i).max_size();

                threads.emplace_back(dummyTask, stackSize, stackForThreadPtr);
            }
        };

        std::function<void(void)> dummyTask;

        std::array<std::array<uint32_t,16>,13> stacks;
        std::vector<Thread> threads;
        
        TimerQueue queue;
    };

    TEST_F(TimerQueueTest, EmptyQueueNoExceptions) {
        
        ASSERT_NO_THROW(queue.popExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(&threads[0], 3));
    }
    
    TEST_F(TimerQueueTest, EmptyQueueValues) {
    
        ASSERT_EQ(queue.popExpired(), nullptr);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueNoExceptions) {

        queue.push(&threads[0], 6);
        
        ASSERT_NO_THROW(queue.popExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(&threads[1], 3));
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueExpired) {
    
        const auto& threadPtr = &threads[0];

        queue.push(threadPtr, 0);
        ASSERT_EQ(queue.popExpired(), threadPtr);
        queue.push(threadPtr, 0);
        queue.tick();
        queue.tick();
        ASSERT_EQ(queue.popExpired(), threadPtr);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueByTimer) {
    
        const auto& threadPtr = &threads[0];
        
        queue.push(threadPtr, 1);
        ASSERT_EQ(queue.popExpired(), nullptr);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), threadPtr);
        ASSERT_EQ(queue.popExpired(), nullptr);
    }

    TEST_F(TimerQueueTest, ExpiredThreadsInQueue) {
    
        queue.push(&threads[0], 0);
        queue.push(&threads[1], 0);
        queue.push(&threads[2], 0);
        queue.push(&threads[3], 2);
        ASSERT_EQ(queue.popExpired(), &threads[0]);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[1]);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[2]);
        ASSERT_EQ(queue.popExpired(), &threads[3]);
        ASSERT_EQ(queue.popExpired(), nullptr);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueByTimerX) {
    
        for (int threadId = 0; threadId < threads.size(); ++threadId) {

            const auto& threadTimer = threadId*2;
        
            queue.push(&threads[threadId], threadTimer);
            
            for (int i = 0; i < threadTimer; ++i) {
            
                ASSERT_EQ(queue.popExpired(), nullptr);
                queue.tick();
            }
            ASSERT_EQ(queue.popExpired(), &threads[threadId]);
            ASSERT_EQ(queue.popExpired(), nullptr);
        }
    }
    
    TEST_F(TimerQueueTest, ManyThreadsInQueueByTimer) {
        
        queue.push(&threads[3], 5);
        queue.push(&threads[0], 1);
        queue.push(&threads[11], 15);
        queue.push(&threads[7], 6);
        queue.push(&threads[4], 4);
        
        int i = 0;
        ASSERT_EQ(queue.popExpired(), nullptr);
        queue.tick();
        ++i;
        
        ASSERT_EQ(queue.popExpired(), &threads[0]);
        ASSERT_EQ(queue.popExpired(), nullptr);
        
        for ( ; i < 4; ++i)  queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[4]);
        ASSERT_EQ(queue.popExpired(), nullptr);
        queue.tick();
        ++i;
        ASSERT_EQ(queue.popExpired(), &threads[3]);
        ASSERT_EQ(queue.popExpired(), nullptr);
        queue.tick();
        ++i;
        ASSERT_EQ(queue.popExpired(), &threads[7]);
        ASSERT_EQ(queue.popExpired(), nullptr);
        
        for ( ; i < 15; ++i)  queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[11]);
        ASSERT_EQ(queue.popExpired(), nullptr);
    }

} /**< namespace UnitTests */
