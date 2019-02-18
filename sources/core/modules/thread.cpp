
#include "thread.hpp"

Thread::Thread(task_t task, uint32_t stackSizeWords, uint32_t * allocatedStack) {

    // Initialization of stack frame for new thread
    allocatedStack += stackSizeWords - 1;   // move to the last word of allocated stack array

    *(allocatedStack--) = 0x21000000;       // xPSR ??? 0x01000000
    *(allocatedStack--) = (uint32_t)task;   // PC - address of procedure to execute
    *(allocatedStack--) = onReturnProcedurePtr; // LR - address of procedure to execute on task() return
    *(allocatedStack--) = 12;               // R12
    *(allocatedStack--) = 3;                // R3
    *(allocatedStack--) = 2;                // R2
    *(allocatedStack--) = 1;                // R1
    *(allocatedStack--) = 0;                // R0 - task argument can be passed

    *(allocatedStack--) = 11;               // r11
    *(allocatedStack--) = 10;               // r10
    *(allocatedStack--) = 9;                // r9
    *(allocatedStack--) = 8;                // r8
    *(allocatedStack--) = 7;                // r7
    *(allocatedStack--) = 6;                // r6
    *(allocatedStack--) = 5;                // r5
    *(allocatedStack) = 4;                  // r4

    // allocatedStack -= 8;                 // to save time, no need to set all registers except debuging

    stackTop = allocatedStack;              // save top of the stack
    state = State::initialized;
}
