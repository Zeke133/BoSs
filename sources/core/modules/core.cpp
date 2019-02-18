#include <cstdint>

// #include "core_cm3.h"
#include "thread.hpp"
#include "scheduler.hpp"

// SysTick timer
unsigned int * const SYST_CSR = (unsigned int *)0xE000E010;     // Privileged a SysTick Control and Status Register
unsigned int * const SYST_RVR = (unsigned int *)0xE000E014;     // Privileged UNKNOWN SysTick Reload Value Register
unsigned int * const SYST_CVR = (unsigned int *)0xE000E018;     // Privileged UNKNOWN SysTick Current Value Register
unsigned int * const SYST_CALIB = (unsigned int *)0xE000E01;    // Privileged a SysTick Calibration Value Register
// UART0 data register on this MCU
unsigned int * const UART0DR = (unsigned int *)0x4000c000;

unsigned int * const CPUID = (unsigned int *)0xE000ED00; // 412FC231
unsigned int * const STCSR = (unsigned int *)0xE000E010;
unsigned int * const CCR = (unsigned int *)0xE000ED14;

Thread * currentThread; // not pointer to SP just to TCB - so need dereference

void printChar(unsigned char sym) {

    *UART0DR = sym; /* Transmit char */
}
void printStr(const char *s) {

        while(*s != '\0') {
            
            printChar(*s);
            s++; /* Next char */
    }
}

void sysTickInit() {

    // 1.Program reload value.
    *SYST_RVR = 0x000ffff;   // 0x00000001-0x00FFFFFF. If the SysTick interrupt is required every 100 clock pulses, set RELOAD to 99.

    // 2.Clear current value.
    *SYST_CVR = 0x0;          // A write of any value clears the field to 0, and also clears the SYST_CSR COUNTFLAG bit to 0.

    // 3.Program Control and Status register.
    *SYST_CSR = 0x7;          // processor clock 4, enable SysTick exception 2, enable counter 1
    // Bits:
    // [16] COUNTFLAG Returns 1 if timer counted to 0 since last time this was read.
    // [2] CLKSOURCE Indicates the clock source:0 = external clock 1 - processor clock.
    // [1] TICKINT Enables SysTick exception request:0 = counting down to zero does not assert the SysTick exception request
    // 1 = counting down to zero asserts the SysTick exception request.
    // Software can use COUNTFLAG to determine if SysTick has ever counted to zero.
    // [0] ENABLE Enables the counter:0 = counter disabled 1 = counter enabled.
}

extern "C" {

void SysTick_Handler(void) {

    // *SYST_CSR = 0;  // timer stop

    /* Trigger the pendSV */
    *((uint32_t volatile *)0xE000ED04) = 0x10000000;
    // 0xE000ED04 represents the address of the System Control Block Register
    // Interrupt Control and State (INTCTRL) and the value 0x10000000 is
    // the PENDSVSET bit
}

/* System initialization
 */
void SystemInit(void) {

    printStr("ARM Cortex-M3 has started up!\n");
}

/* Pre-main initialisation
 */
void __START(void) {

    // nucleus

    // priority of interrupts SysTick and PendSV

    volatile uint32_t thread1Stack[40];     // stack for thread 100x4=400 bytes
    volatile uint32_t thread2Stack[30];
    // volatile uint32_t thread3Stack[20];
    // thread1Stack[0] = 0xf00;                 // here can place MagicInt of stack end
    // thread1Stack[39] = 0xf39;                // here can place MagicInt of stack top

    Thread task1([]() { while(1) { printStr("TASK1!\n"); } }, (sizeof thread1Stack)/4, (uint32_t *)&thread1Stack);
    Thread task2([]() { while(1) { printStr("TASK2!\n"); } }, (sizeof thread2Stack)/4, (uint32_t *)&thread2Stack);
    // Thread task3([]() { while(1) { printStr("TASK3!\n"); } }, (sizeof thread3Stack)/4, (uint32_t *)&thread3Stack);

    task1.setNext(&task2);
    task2.setNext(&task1);
    // task3.setNext(&task1);  // list should be closed

    currentThread = &task1;
    //task1.start();

    sysTickInit();
    
    while(1) ;
}

}
