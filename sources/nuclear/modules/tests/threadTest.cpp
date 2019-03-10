/**
 * @file    threadTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    22-12-2019
 * @brief   Thread class tests
 * @details   Unit tests
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

/* tested source */
#include "thread.hpp"

/* testing framework */
#include <gtest/gtest.h>

/* std */
#include <vector>
#include <iterator>

namespace BossKernelUnitTests {

    /**
     * Initialization of stack frame for new thread on ARM Cortex-M3.
     * ------------
     * Example of hardware stack frame saved/restored on interrupt entering:
     * <previous>
     * SP + 0x1C   xPSR    <-  SP points here before interrupt
     * SP + 0x18   PC
     * SP + 0x14   LR
     * SP + 0x10   R12
     * SP + 0x0C   R3
     * SP + 0x08   R2
     * SP + 0x04   R1
     * SP + 0x00   R0      <-  SP points here after interrup
     * -----------
     */

    class ThreadTest : public ::testing::Test {

        protected:

        ThreadTest() :
            task([](){ std::cout << "Dummy task"; }),
            stack(16, 666),
            thread(task, stack.size(), stack.data()) {
        };

        std::vector<uint32_t> stack;
        std::function<void(void)> task;
        Thread thread;
    };

    TEST_F(ThreadTest, StateInitialization) {
        
        EXPECT_EQ(thread.getState(), Thread::State::initialized);
    }
    
    TEST_F(ThreadTest, LinkToNextInitialization) {

        EXPECT_EQ(thread.getNext(), nullptr);
    }
    
    TEST_F(ThreadTest, TopOfStackAccess) {

        auto stackTopPtr = thread.getStackTop();

        auto stackTop = std::end(stack);
        stackTop -= 16;
        *stackTop = 2212;

        ASSERT_EQ(*stackTopPtr, *stackTop);
    }

    TEST_F(ThreadTest, StackFrameInitialization) {

        auto taskRawPointer = task.target<void(*)(void)>();
        auto stackTop = std::end(stack);

        uint32_t xpsr = *--stackTop;
        uint32_t pc = *--stackTop;
        uint32_t lr = *--stackTop;

        EXPECT_EQ(xpsr, 0x21000000);
        ASSERT_EQ(pc, reinterpret_cast<uintptr_t>(taskRawPointer));
        EXPECT_EQ(lr, 333);
    }

    TEST_F(ThreadTest, SleepTicksInitialization) {

        EXPECT_EQ(thread.getSleepTicks(), 0);
    }
    
    TEST_F(ThreadTest, SetSleepTicks) {
        
        thread.setSleepTicks(159357);
        EXPECT_EQ(thread.getSleepTicks(), 159357);
        thread.setSleepTicks(0);
        EXPECT_EQ(thread.getSleepTicks(), 0);
    }

    TEST_F(ThreadTest, SetNext) {

        Thread * nextThread = (Thread *)0x987654321;
        
        thread.setNext(nextThread);

        EXPECT_EQ(thread.getNext(), (Thread *)0x987654321);
    }

    TEST_F(ThreadTest, SetState) {
        
        thread.setState(Thread::State::paused);
        EXPECT_EQ(thread.getState(), Thread::State::paused);
        thread.setState(Thread::State::running);
        EXPECT_EQ(thread.getState(), Thread::State::running);
        thread.setState(Thread::State::initialized);
        EXPECT_EQ(thread.getState(), Thread::State::initialized);
    }

} /**< namespace UnitTests */
