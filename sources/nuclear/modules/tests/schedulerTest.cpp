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
#include "schedulerTestProxy.hpp"

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
    
        // system part
        ASSERT_NO_THROW(SchedulerTestProxy::getActiveThread());
        ASSERT_NO_THROW(Scheduler::tick());
        ASSERT_NO_THROW(SchedulerTestProxy::switchThread());
        // user part
        ASSERT_NO_THROW(Scheduler::kill(threads[0]));
        ASSERT_NO_THROW(Scheduler::sleep(133));
    }

    TEST_F(SchedulerTest, NoThreadValues) {
    
        EXPECT_EQ(SchedulerTestProxy::getActiveThread(), nullptr);
        ASSERT_EQ(SchedulerTestProxy::switchThread(), nullptr);
    }

    TEST_F(SchedulerTest, OneThreadNoThrow) {
        
        ASSERT_NO_THROW(Scheduler::run(threads[1]));
        // system part
        ASSERT_NO_THROW(SchedulerTestProxy::getActiveThread());
        ASSERT_NO_THROW(SchedulerTestProxy::switchThread());
        ASSERT_NO_THROW(Scheduler::tick());
        // user part
        ASSERT_NO_THROW(Scheduler::sleep(45));
        ASSERT_NO_THROW(Scheduler::kill(threads[1]));
        // robustness check
        ASSERT_NO_THROW(SchedulerTestProxy::getActiveThread());
        ASSERT_NO_THROW(Scheduler::tick());
        ASSERT_NO_THROW(Scheduler::sleep(45));
        ASSERT_NO_THROW(Scheduler::kill(threads[1]));
    }

    TEST_F(SchedulerTest, OneThreadNotActivatedValues) {
    
        Scheduler::run(threads[0]);
        ASSERT_EQ(threads[0].getState(), Thread::State::initialized);
        EXPECT_EQ(SchedulerTestProxy::getActiveThread(), nullptr);
        ASSERT_EQ(SchedulerTestProxy::switchThread(), &threads[0]);
        Scheduler::kill(threads[0]);
    }

    TEST_F(SchedulerTest, OneThreadActivatedValues) {
    
        Scheduler::run(threads[0]);
        SchedulerTestProxy::switchThread();
        auto t = SchedulerTestProxy::getActiveThread();
        ASSERT_EQ(t, &threads[0]);
        ASSERT_EQ(t->getState(), Thread::State::active);
        Scheduler::kill(threads[0]);
    }
    
    TEST_F(SchedulerTest, OneThreadSleepingValues) {
    
        Scheduler::run(threads[0]);
        SchedulerTestProxy::switchThread();
        Scheduler::sleep(32);
        ASSERT_EQ(threads[0].getState(), Thread::State::sleep);
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), nullptr);
        Scheduler::kill(threads[0]);
    }
    
    TEST_F(SchedulerTest, OneThreadKilledValue) {
    
        Scheduler::run(threads[0]);
        SchedulerTestProxy::switchThread();
        Scheduler::kill(threads[0]);
        ASSERT_EQ(threads[0].getState(), Thread::State::killed);
        EXPECT_EQ(SchedulerTestProxy::getActiveThread(), nullptr);
    }

    TEST_F(SchedulerTest, FewThreadsNoThrow) {
    
        ASSERT_NO_THROW(Scheduler::run(threads[0]));
        ASSERT_NO_THROW(Scheduler::run(threads[1]));
        ASSERT_NO_THROW(Scheduler::run(threads[2]));
        ASSERT_NO_THROW(SchedulerTestProxy::getActiveThread());
        ASSERT_NO_THROW(SchedulerTestProxy::switchThread());
        ASSERT_NO_THROW(Scheduler::tick());
        ASSERT_NO_THROW(Scheduler::sleep(45));
        ASSERT_NO_THROW(Scheduler::kill(threads[2]));
        ASSERT_NO_THROW(SchedulerTestProxy::switchThread());
        ASSERT_NO_THROW(SchedulerTestProxy::getActiveThread());
        ASSERT_NO_THROW(Scheduler::tick());
        ASSERT_NO_THROW(Scheduler::sleep(24));
        ASSERT_NO_THROW(Scheduler::kill(threads[1]));
        ASSERT_NO_THROW(Scheduler::tick());
        ASSERT_NO_THROW(Scheduler::kill(threads[0]));
        ASSERT_NO_THROW(SchedulerTestProxy::switchThread());
    }

    TEST_F(SchedulerTest, TwoThreadsRoundRobingWithSleep) {

        auto& t1 = threads[0];
        auto& t2 = threads[1];
        Scheduler::run(t1);
        Scheduler::run(t2);

        Scheduler::tick();

        ASSERT_EQ(SchedulerTestProxy::switchThread(), &t1);
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &t1);

        ASSERT_EQ(t2.getState(), Thread::State::initialized);
        ASSERT_EQ(t1.getState(), Thread::State::active);

        Scheduler::sleep(5);

        ASSERT_EQ(SchedulerTestProxy::switchThread(), &t2);
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &t2);

        ASSERT_EQ(t2.getState(), Thread::State::active);
        ASSERT_EQ(t1.getState(), Thread::State::sleep);

        for (int i = 0; i < 4; i++) {
            Scheduler::tick();
            ASSERT_EQ(SchedulerTestProxy::switchThread(), &t2);
            ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &t2);
        }

        Scheduler::kill(t1);
        Scheduler::kill(t2);
    }

    TEST_F(SchedulerTest, FewThreadsRoundRobing) {

        for (auto&& t : threads) Scheduler::run(t);

        auto amount = threads.size();

        for (int i = 0; i < 133; ++i) {

            Scheduler::tick();

            auto&& t = SchedulerTestProxy::switchThread();
            auto idx = i % amount;

            ASSERT_EQ(t, &threads[idx]);
            ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[idx]);

            for (int prevIdx = 0; prevIdx < idx; ++prevIdx) {

                ASSERT_EQ(threads[prevIdx].getState(), Thread::State::waiting);
            }

            ASSERT_EQ(threads[idx].getState(), Thread::State::active);

            for (int nextIdx = idx+1; nextIdx < threads.size(); ++nextIdx) {

                if (i < amount)
                    ASSERT_EQ(threads[nextIdx].getState(), Thread::State::initialized);
                else
                    ASSERT_EQ(threads[nextIdx].getState(), Thread::State::waiting);
            }
        }
        
        for(auto&& t : threads) Scheduler::kill(t);
    }

    TEST_F(SchedulerTest, FewThreadsSleepTimings) {

        for (auto&& t : threads) Scheduler::run(t);

        // run all, 3 and 7 - to sleep
        for (int i = 0; i < 4; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[3]);
        Scheduler::sleep(15);
        SchedulerTestProxy::switchThread();

        for (int i = 0; i < 3; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[7]);
        Scheduler::sleep(3);
        SchedulerTestProxy::switchThread();

        // tick till threads will wake up
        // SLEEP for 3 and 15 ticks - and get to the back of queue

        for (int i = 0; i < 13; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[7]);

        for (int i = 0; i < 10; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[3]);
        
        for(auto&& t : threads) Scheduler::kill(t);
    }

    TEST_F(SchedulerTest, FewThreadsKilling) {

        for (auto&& t : threads) Scheduler::run(t);

        // run all, 3 and 7 - to sleep
        // than kill 9 and 3
        // make sure those are killed and 7 waked up

        for (int i = 0; i < 4; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[3]);
        Scheduler::sleep(15);
        SchedulerTestProxy::switchThread();
        Scheduler::kill(threads[9]);

        for (int i = 0; i < 3; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[7]);
        Scheduler::sleep(28);
        SchedulerTestProxy::switchThread();
        Scheduler::kill(threads[3]);

        for (int i = 0; i < 37; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
        }
        ASSERT_EQ(SchedulerTestProxy::getActiveThread(), &threads[7]);
        
        for (int i = 0; i < threads.size() * 3; ++i) {
            Scheduler::tick();
            SchedulerTestProxy::switchThread();
            ASSERT_NE(SchedulerTestProxy::getActiveThread(), &threads[3]);
            ASSERT_NE(SchedulerTestProxy::getActiveThread(), &threads[9]);
        }
        
        for(auto&& t : threads) Scheduler::kill(t);
    }

} /**< namespace UnitTests */
