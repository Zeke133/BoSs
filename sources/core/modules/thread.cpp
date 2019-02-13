
#include "thread.hpp"

void Thread::create(void(*task)(), uint32_t stackSizeWords, uint32_t * allocatedStack) {

    // need to init stack frame for process

    // go to the last word of stack array
    allocatedStack += stackSizeWords - 1;

    *(allocatedStack--) = 0x21000000;        // xPSR
    *(allocatedStack--) = (uint32_t)task;    // PC (uint32_t) task & TASK_PC_MASK;
    *(allocatedStack--) = 0xF1;    // LR - place thread cleaner here???
    *(allocatedStack--) = 12;        // R12
    *(allocatedStack--) = 3;        // R3
    *(allocatedStack--) = 2;        // R2
    *(allocatedStack--) = 1;        // R1
    *(allocatedStack--) = 0;    // R0   task start 'param' or params

    // *(allocatedStack--) = 0;    // initial EXEC RETURN ???
    *(allocatedStack--) = 0;    // r11
    *(allocatedStack--) = 0;    // r10
    *(allocatedStack--) = 0;    // r9
    *(allocatedStack--) = 0;    // r8
    *(allocatedStack--) = 0;    // r7
    *(allocatedStack--) = 0;    // r6
    *(allocatedStack--) = 0;    // r5
    *(allocatedStack--) = 0;    // r4

    // allocatedStack -= 8;

}