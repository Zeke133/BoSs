/**
 * @file    sleepingTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    12-03-2019
 * @brief   Sleeping class tests
 * @details   Unit tests
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

/* tested source */
#include "sleeping.hpp"

/* testing framework */
#include <gtest/gtest.h>

/* std */
#include <vector>
#include <iterator>

namespace BossKernelUnitTests {

    class SleepingQueueTest : public ::testing::Test {

        protected:

        SleepingQueueTest() : dummyTask([](){ std::cout << "Dummy task"; }) {
            
            auto threadsCount = 13;
            stacks.reserve(threadsCount);
            threads.reserve(threadsCount);
            
            for (int i = 0; i < threadsCount; ++i) {
                
                threads.emplace_back(dummyTask, stacks[i].size(), stacks[i].data());
            }
        };

        std::function<void(void)> dummyTask;
        std::vector<std::array<uint32_t,16>> stacks;
        std::vector<Thread> threads;
        
        SleepingThreads queue;
    };

    TEST_F(SleepingQueueTest, EmptyQueueNoExceptions) {
    
        const auto& threadPtr = &threads[0];
        
        ASSERT_NO_THROW(queue.getExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(threadPtr, 3));
    }
    
    TEST_F(SleepingQueueTest, EmptyQueueValues) {
    
        ASSERT_EQ(queue.getExpired(), nullptr);
    }
    
    TEST_F(SleepingQueueTest, OneThreadInQueueNoExceptions) {
    
        const auto& threadPtr = &threads[0];
        
        queue.push(threadPtr, 6);
        
        ASSERT_NO_THROW(queue.getExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(threadPtr, 3));
    }
    
    TEST_F(SleepingQueueTest, OneThreadInQueueExpired) {
    
        const auto& threadPtr = &threads[0];
        
        queue.push(threadPtr, 0);
        ASSERT_EQ(queue.getExpired(), threadPtr);
    }
    
    TEST_F(SleepingQueueTest, OneThreadInQueueByTimer1) {
    
        const auto& threadPtr = &threads[0];
        
        queue.push(threadPtr, 1);
        ASSERT_EQ(queue.getExpired(), nullptr);
        queue.tick();
        ASSERT_EQ(queue.getExpired(), threadPtr);
        ASSERT_EQ(queue.getExpired(), nullptr);
    }
    
    TEST_F(SleepingQueueTest, OneThreadInQueueByTimerX) {
    
        for (int timeOut = 0; timeOut < 13; ++timeOut) {
        
            queue.push(&threads[timeOut], timeOut);
            
            for (int i = 0; i < timeOut; ++i) {
            
                ASSERT_EQ(queue.getExpired(), nullptr);
                queue.tick();
            }
            ASSERT_EQ(queue.getExpired(), &threads[timeOut]);
            ASSERT_EQ(queue.getExpired(), nullptr);
        }
    }
    
    TEST_F(SleepingQueueTest, ManyThreadsInQueueByTimer) {
        
        queue.push(&threads[3], 5);
        queue.push(&threads[0], 1);
        queue.push(&threads[11], 15);
        queue.push(&threads[7], 6);
        queue.push(&threads[4], 4);
        
        int i = 0;
        ASSERT_EQ(queue.getExpired(), nullptr);
        queue.tick();
        ++i;
        
        ASSERT_EQ(queue.getExpired(), &threads[0]);
        ASSERT_EQ(queue.getExpired(), nullptr);
        
        for ( ; i <= 4; ++i)  queue.tick();
        ASSERT_EQ(queue.getExpired(), &threads[4]);
        ASSERT_EQ(queue.getExpired(), nullptr);
        queue.tick();
        ++i;
        ASSERT_EQ(queue.getExpired(), &threads[3]);
        ASSERT_EQ(queue.getExpired(), nullptr);
        queue.tick();
        ++i;
        ASSERT_EQ(queue.getExpired(), &threads[7]);
        ASSERT_EQ(queue.getExpired(), nullptr);
        
        for ( ; i <= 15; ++i)  queue.tick();
        ASSERT_EQ(queue.getExpired(), &threads[11]);
        ASSERT_EQ(queue.getExpired(), nullptr);
    }

} /**< namespace UnitTests */
