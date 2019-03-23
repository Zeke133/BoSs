/**
 * @file    schedulerTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    17-03-2019
 * @brief   Scheduler class tests
 * @details   Unit tests
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

/* tested source */
#include "scheduler.hpp"

/* testing framework */
#include <gtest/gtest.h>

/* std */
#include <vector>
#include <functional>

namespace BossKernelUnitTests {

    class SchedulerTest : public ::testing::Test {

    protected:
    
        SchedulerTest() :
            dummyTask([](){}) {

            threads.reserve(stacks.max_size());
            for(int i = 0; i < threads.capacity(); ++i) {
                
                auto& stack = stacks.at(i);
                threads.emplace_back(
                        reinterpret_cast<Thread::TaskType>(
                            dummyTask.target<void(*)(void)>()),
                        stack.max_size(),
                        stack.data());
            }
        }

        std::function<void(void)> dummyTask;

        static constexpr unsigned int threadsNumber = 13;

        std::array <std::array <uint32_t, 17>, threadsNumber> stacks;
        std::vector <Thread> threads;
    };

    TEST_F(SchedulerTest, NoThreadNoThrow) {
    
        ASSERT_NO_THROW(Scheduler::getActiveThread());
        ASSERT_NO_THROW(Scheduler::takeDecision());
        ASSERT_NO_THROW(Scheduler::kill(threads[0]));
    }

    TEST_F(SchedulerTest, NoThreadGetActive) {
    
        EXPECT_EQ(Scheduler::getActiveThread(), nullptr);
    }

    TEST_F(SchedulerTest, NoThreadTakeDecision) {
    
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::noAction);
    }

    TEST_F(SchedulerTest, AddThreadNoThrow) {
        
        ASSERT_NO_THROW(Scheduler::addThread(threads[0]));
        Scheduler::kill(threads[0]);
    }

    TEST_F(SchedulerTest, OneThreadNotActivated) {
    
        Scheduler::addThread(threads[0]);
        EXPECT_EQ(Scheduler::getActiveThread(), nullptr);
        Scheduler::kill(threads[0]);
    }
    
    TEST_F(SchedulerTest, OneThreadKilledGetActive) {
    
        Scheduler::addThread(threads[0]);
        ASSERT_NO_THROW(Scheduler::kill(threads[0]));
        EXPECT_EQ(Scheduler::getActiveThread(), nullptr);
    }

    // TEST_F(SchedulerTest, OneThreadActivatedGetActive) {
        
    //     Scheduler::addThread(threads[0]);
    //     ASSERT_EQ(Scheduler::getActiveThread(), &(threads.at(0)));
    //     ASSERT_EQ(Scheduler::getActiveThread()->getState(), Thread::State::initialized);
    //     Scheduler::kill(threads[0]);
    // }

    TEST_F(SchedulerTest, OneThreadTakeDecision) {
    
        Scheduler::addThread(threads[0]);
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);
        Scheduler::kill(threads[0]);
    }

    TEST_F(SchedulerTest, AddSecondThreadNoThrow) {
    
        Scheduler::addThread(threads[0]);
        ASSERT_NO_THROW(Scheduler::addThread(threads[1]));
        Scheduler::kill(threads[0]);
        Scheduler::kill(threads[1]);
    }

    // TEST_F(SchedulerTest, TwoThreadsGetActive) {
    
    //     Scheduler::addThread(threads[0]);
    //     Scheduler::addThread(threads[1]);
    //     ASSERT_EQ(Scheduler::getActiveThread(), &(threads.at(0)));
    //     ASSERT_EQ(Scheduler::getActiveThread()->getState(), Thread::State::initialized);
    //     Scheduler::kill(threads[0]);
    //     Scheduler::kill(threads[1]);
    // }

    // TEST_F(SchedulerTest, TwoThreadsKill) {
    
    //     Scheduler::addThread(threads[0]);
    //     Scheduler::addThread(threads[1]);
    //     ASSERT_EQ(Scheduler::getActiveThread(), &(threads.at(0)));
    //     Scheduler::kill(threads[0]);
    //     ASSERT_EQ(Scheduler::getActiveThread(), &(threads.at(1)));
    //     Scheduler::kill(threads[1]);
    //     ASSERT_EQ(Scheduler::getActiveThread(), nullptr);
    // }

    TEST_F(SchedulerTest, TwoThreadsTakeDecision) {
    
        Scheduler::addThread(threads[0]);
        Scheduler::addThread(threads[1]);
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);
        Scheduler::kill(threads[0]);
        Scheduler::kill(threads[1]);
    }

    // TEST_F(SchedulerTest, ManyThreadsComplexTest) {

    //     for(uint32_t threadIdx = 0; threadIdx < threads.size(); ++threadIdx) {
            
    //         ASSERT_NO_THROW(Scheduler::addThread(threads[threadIdx]));
    //         // Check if current thread is the 1st added thread
    //         ASSERT_EQ(Scheduler::getActiveThread(), &(threads.at(0)));
    //         ASSERT_EQ(Scheduler::getActiveThread()->getState(), Thread::State::initialized);
    //         // Check decision
    //         ASSERT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);
    //     }
    //     for(auto& thread : threads) {
    //         ASSERT_NO_THROW(Scheduler::kill(thread));
    //     }
    // }

} /**< namespace UnitTests */
