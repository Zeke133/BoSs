/**
 * @file    thread.cpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    10-03-2019
 * @brief   Thread class
 * @details   Implementation of thread instance
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "thread.hpp"

/**
 * @brief  Thread constructor
 * @param  task: pointer to thread task function
 * @param  stackSizeWords: size of thread stack in words (word = 4 byte)
 * @param  allocatedStack: pointer to memory allocated for thread stack 
 * @retval None
 */
#ifdef NO_USE_LIBG
Thread::Thread(taskType task, uint32_t stackSizeWords, uint32_t * allocatedStack) {
#else
Thread::Thread(std::function<void(void)> task, uint32_t stackSizeWords, uint32_t * allocatedStack) {
#endif

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

    allocatedStack += stackSizeWords - 1;   /**< move to the last word of allocated stack array */

    *(allocatedStack--) = 0x21000000;       /**< default xPSR ??? 0x01000000 */

    #ifdef NO_USE_LIBG
    //  -fno-rtti will disable usage of std::function
    *(allocatedStack--) = (unsigned long)task;  /**< PC - address of procedure to execute */
    #else
    *(allocatedStack--) = (uintptr_t)task.target<void(*)()>();  /**< PC - address of procedure to execute */
    #endif

    *(allocatedStack--) = onReturnProc;     /**< LR - address of procedure to execute on task() return */
    *(allocatedStack--) = 12;               /**< R12 */
    *(allocatedStack--) = 3;                /**< R3 */
    *(allocatedStack--) = 2;                /**< R2 */
    *(allocatedStack--) = 1;                /**< R1 */
    *(allocatedStack) = 0;                  /**< R0 - task argument can be passed */

    allocatedStack -= 8;                    /**< to save time: no need to set all registers except debuging */

    stackTop = allocatedStack;              /**< save top of the stack */
    state = State::initialized;
    sleepTicks = 0;
}
