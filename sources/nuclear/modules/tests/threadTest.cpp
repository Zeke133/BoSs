/**
 * @file    threadTest.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    22-12-2018
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

        ThreadTest() : stack(16, 666) {};

        std::vector<uint32_t> stack;
    };

    TEST_F(ThreadTest, StateInitialization) {
        
        Thread thread((Thread::taskType)0x1234, stack.size(), stack.data());

        EXPECT_EQ(thread.getState(), Thread::State::initialized);
    }
    
    TEST_F(ThreadTest, LinkToNextInitialization) {
        
        Thread thread((Thread::taskType)0x1234, stack.size(), stack.data());

        EXPECT_EQ(thread.getNext(), nullptr);
    }
    
    TEST_F(ThreadTest, TopOfStackAccess) {

        Thread thread((Thread::taskType)0x12345678, stack.size(), stack.data());
        auto stackTopPtr = thread.getStackTop();

        auto stackTop = std::end(stack);
        stackTop -= 16;
        *stackTop = 2212;

        ASSERT_EQ(*stackTopPtr, *stackTop);
    }

    TEST_F(ThreadTest, StackFrameInitialization) {

        Thread thread((Thread::taskType)0x12345678, stack.size(), stack.data());

        auto stackTop = std::end(stack);
        uint32_t xpsr = *--stackTop;
        uint32_t pc = *--stackTop;
        uint32_t lr = *--stackTop;

        EXPECT_EQ(xpsr, 0x21000000);
        ASSERT_EQ(pc, 0x12345678);
        EXPECT_EQ(lr, 333);
    }

} /**< namespace UnitTests */
