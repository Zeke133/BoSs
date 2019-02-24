/**
 * @file    schedulerTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    23-12-2018
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
#include <iterator>
#include <utility>

namespace BossKernelUnitTests {

    class SchedulerTest : public ::testing::Test {

        protected:
        SchedulerTest() {

            if (!stackThreadPairsInitialized) {
                stackThreadPairs.reserve(13);

                for(uint32_t i = 0; i < 13; i++) {
                    
                    std::vector<uint32_t> stack(17, i);
                    Thread thread(&dummyTask, stack.size(), stack.data());

                    stackThreadPairs.push_back(std::make_pair(std::move(stack), std::move(thread)));
                }
                stackThreadPairsInitialized = true;
            }
        }
        // ~SchedulerTest() override {}

        static void dummyTask() {};

        static std::vector<std::pair<std::vector<uint32_t>,Thread>> stackThreadPairs;
        static bool stackThreadPairsInitialized;
    };

    std::vector<std::pair<std::vector<uint32_t>,Thread>> SchedulerTest::stackThreadPairs;
    bool SchedulerTest::stackThreadPairsInitialized = false;

    TEST_F(SchedulerTest, NoThreadExceptions) {
        ASSERT_NO_THROW(Scheduler::getCurrentThread());
        ASSERT_NO_THROW(Scheduler::takeDecision());
    }

    TEST_F(SchedulerTest, NoThreadGetCurrent) {
        EXPECT_EQ(Scheduler::getCurrentThread(), nullptr);
    }

    TEST_F(SchedulerTest, NoThreadDecision) {
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::noAction);
    }

    TEST_F(SchedulerTest, AddFirstThreadException) {
        ASSERT_NO_THROW(Scheduler::addThread(&stackThreadPairs.at(0).second));
    }

    TEST_F(SchedulerTest, OneThreadGetCurrent) {
        ASSERT_EQ(Scheduler::getCurrentThread(), &stackThreadPairs.at(0).second);
        ASSERT_EQ(Scheduler::getCurrentThread()->getState(), Thread::State::initialized);
        ASSERT_EQ(*(Scheduler::getCurrentThread()->getStackTop()), 0);
    }

    TEST_F(SchedulerTest, OneThreadGetNext) {
        ASSERT_EQ(Scheduler::getCurrentThread()->getNext(), &stackThreadPairs.at(0).second);
    }

    TEST_F(SchedulerTest, OneThreadDecision) {
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);
    }

    TEST_F(SchedulerTest, AddSecondThreadException) {
        ASSERT_NO_THROW(Scheduler::addThread(&stackThreadPairs.at(1).second));
    }

    /**< Current Thread should always point to the first added Thread */
    TEST_F(SchedulerTest, TwoThreadsGetCurrent) {
        ASSERT_EQ(Scheduler::getCurrentThread(), &stackThreadPairs.at(0).second);
        ASSERT_EQ(Scheduler::getCurrentThread()->getState(), Thread::State::initialized);
        ASSERT_EQ(*(Scheduler::getCurrentThread()->getStackTop()), 0);
    }

    TEST_F(SchedulerTest, TwoThreadsGetNext) {
        ASSERT_EQ(Scheduler::getCurrentThread()->getNext(), &stackThreadPairs.at(1).second);
        ASSERT_EQ(Scheduler::getCurrentThread()->getNext()->getNext(), &stackThreadPairs.at(0).second);
    }

    TEST_F(SchedulerTest, TwoThreadsDecision) {
        EXPECT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);
    }

    TEST_F(SchedulerTest, ManyThreadsComplexTest) {

        for(uint32_t threadsInList = 2; threadsInList < stackThreadPairs.size(); ++threadsInList) {
            
            // add thread
            ASSERT_NO_THROW(Scheduler::addThread(&stackThreadPairs.at(threadsInList).second));

            // check if current thread is still the 1st added thread
            ASSERT_EQ(Scheduler::getCurrentThread(), &stackThreadPairs.at(0).second);
            ASSERT_EQ(Scheduler::getCurrentThread()->getState(), Thread::State::initialized);
            ASSERT_EQ(*(Scheduler::getCurrentThread()->getStackTop()), 0);
            // check decision
            ASSERT_EQ(Scheduler::takeDecision(), Scheduler::Decision::onlyRestore);

            // check if all links are correctly created
            Thread * threadIterator = Scheduler::getCurrentThread()->getNext();
            for(uint32_t thread = 1; thread < threadsInList; thread++) {
                
                ASSERT_EQ(threadIterator, &stackThreadPairs.at(thread).second);
                threadIterator = threadIterator->getNext();
            }
        }
    }

} /**< namespace UnitTests */
