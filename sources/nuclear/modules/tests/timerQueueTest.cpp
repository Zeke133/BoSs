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
#include <functional>

namespace BossKernelUnitTests {

    class TimerQueueTest : public ::testing::Test {

    protected:

        TimerQueueTest() :
            dummyTask([](){}) {
            
            threads.reserve(stacks.max_size());
            
            for (int i = 0; i < threads.capacity(); ++i) {
                
                auto& stack = stacks.at(i);
                threads.emplace_back(
                        reinterpret_cast<Thread::TaskType>(
                            dummyTask.target<void(*)(void)>()
                            ),
                        stack.max_size(),
                        stack.data());
            }
        };

        std::function<void(void)> dummyTask;

        static constexpr unsigned int threadsNumber = 13;

        std::array<std::array<uint32_t,16>,threadsNumber> stacks;
        std::vector<Thread> threads;
        
        TimerQueue queue;
    };

    TEST_F(TimerQueueTest, EmptyQueueNoThrow) {
        
        ASSERT_NO_THROW(queue.popExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(threads[0], 3));
    }
    
    TEST_F(TimerQueueTest, EmptyQueueValues) {
    
        ASSERT_EQ(queue.popExpired(), nullptr);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueNoThrow) {

        queue.push(threads[0], 6);
        
        ASSERT_NO_THROW(queue.popExpired());
        ASSERT_NO_THROW(queue.tick());
        ASSERT_NO_THROW(queue.push(threads[1], 3));
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueExpired) {
    
        auto& thread = threads[0];

        queue.push(thread, 0);
        ASSERT_EQ(queue.popExpired(), &thread);
        queue.push(thread, 0);
        queue.tick();
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &thread);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueByTimer) {
    
        auto& thread = threads[0];
        
        queue.push(thread, 1);
        ASSERT_EQ(queue.popExpired(), nullptr);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &thread);
        ASSERT_EQ(queue.popExpired(), nullptr);
    }

    TEST_F(TimerQueueTest, ExpiredThreadsInQueue) {
    
        queue.push(threads[0], 0);
        queue.push(threads[1], 0);
        queue.push(threads[2], 0);
        queue.push(threads[3], 2);
        ASSERT_EQ(queue.popExpired(), &threads[0]);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[1]);
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[2]);
        ASSERT_EQ(queue.popExpired(), &threads[3]);
        ASSERT_EQ(queue.popExpired(), nullptr);
    }

    TEST_F(TimerQueueTest, RemoveThreadsFromQueue) {
    
        queue.push(threads[0], 25);
        queue.push(threads[1], 7);
        queue.push(threads[2], 16);
        queue.push(threads[3], 2);
        queue.tick();                               // 1 tick gone
        queue.remove(threads[3]);

        for (int i = 0; i < 5; ++i) {
            queue.tick();
            ASSERT_EQ(queue.popExpired(), nullptr);
        }
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[1]); // 7 ticks gone

        queue.tick();                               // 8 ticks gone
        queue.remove(threads[2]);

        for (int i = 0; i < 16; ++i) {
            queue.tick();
            ASSERT_EQ(queue.popExpired(), nullptr);
        }
        queue.tick();
        ASSERT_EQ(queue.popExpired(), &threads[0]); // 25 ticks gone
        ASSERT_EQ(queue.popExpired(), nullptr);
    }
    
    TEST_F(TimerQueueTest, OneThreadInQueueByTimerX) {
    
        for (int threadId = 0; threadId < threads.size(); ++threadId) {

            const auto& threadTimer = threadId*2;
        
            queue.push(threads[threadId], threadTimer);
            
            for (int i = 0; i < threadTimer; ++i) {
            
                ASSERT_EQ(queue.popExpired(), nullptr);
                queue.tick();
            }
            ASSERT_EQ(queue.popExpired(), &threads[threadId]);
            ASSERT_EQ(queue.popExpired(), nullptr);
        }
    }
    
    TEST_F(TimerQueueTest, ManyThreadsInQueueByTimer) {
        
        queue.push(threads[3], 5);
        queue.push(threads[0], 1);
        queue.push(threads[11], 15);
        queue.push(threads[7], 6);
        queue.push(threads[4], 4);
        
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
