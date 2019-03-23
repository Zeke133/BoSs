/**
 * @file    threadTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.2
 * @date    17-03-2019
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
#include <functional>

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
            task([](){ /* Dummy task */ }),
            thread(
                    reinterpret_cast<Thread::TaskType>(
                        task.target<void(*)(void)>()
                        ),
                    stack.max_size(),
                    stack.data())
        {};

        std::array<uint32_t,16> stack;
        std::function<void(void)> task;
        Thread thread;
    };

    TEST_F(ThreadTest, StateInitialization) {
        
        EXPECT_EQ(thread.getState(), Thread::State::initialized);
    }
    
    TEST_F(ThreadTest, StackPointerAccess) {

        // 16 words is initialized stack frame length
        *(std::end(stack)-16) = 2212;

        ASSERT_EQ(*thread.getStackPointer(), 2212);
    }

    TEST_F(ThreadTest, StackFrameInitialization) {

        auto taskRawPointer = task.target<void(*)(void)>();
        auto sp = std::end(stack);

        uint32_t xpsr = *--sp;
        uint32_t pc = *--sp;
        uint32_t lr = *--sp;

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

    TEST_F(ThreadTest, SetState) {
        
        thread.setState(Thread::State::initialized);
        EXPECT_EQ(thread.getState(), Thread::State::initialized);
        thread.setState(Thread::State::waiting);
        EXPECT_EQ(thread.getState(), Thread::State::waiting);
        thread.setState(Thread::State::active);
        EXPECT_EQ(thread.getState(), Thread::State::active);
        thread.setState(Thread::State::sleep);
        EXPECT_EQ(thread.getState(), Thread::State::sleep);
        thread.setState(Thread::State::blocked);
        EXPECT_EQ(thread.getState(), Thread::State::blocked);
        thread.setState(Thread::State::killed);
        EXPECT_EQ(thread.getState(), Thread::State::killed);
    }

} /**< namespace UnitTests */
