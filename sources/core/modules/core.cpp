#include <cstdint>

<<<<<<< HEAD
#include "core_cm3.h"
=======
>>>>>>> f76522a622cf114e3941f203dd3f4b2c0f7273df
#include "thread.hpp"

// SysTick timer
unsigned int * const SYST_CSR = (unsigned int *)0xE000E010;     // Privileged a SysTick Control and Status Register
unsigned int * const SYST_RVR = (unsigned int *)0xE000E014;     // Privileged UNKNOWN SysTick Reload Value Register
unsigned int * const SYST_CVR = (unsigned int *)0xE000E018;     // Privileged UNKNOWN SysTick Current Value Register
unsigned int * const SYST_CALIB = (unsigned int *)0xE000E01;    // Privileged a SysTick Calibration Value Register
// UART0 data register on this MCU
unsigned int * const UART0DR = (unsigned int *)0x4000c000;

unsigned int * const REG_CPUID = (unsigned int *)0xE000ED00; // 412FC231
unsigned int * const REG_STCSR = (unsigned int *)0xE000E010;
unsigned int * const REG_CCR = (unsigned int *)0xE000ED14;

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

    // timer_stop();
    // asm volatile("b task_switch");
    ContextSwitch();
}

/* System initialization
 */
void SystemInit(void) {

    printStr("ARM Cortex-M3 has started up!\n");
}

/* Pre-main initialisation
 */
void __START(void) {

    sysTickInit();

    const char * hexTable = "0123456789ABCDEF";

    printStr("REG_CCR = ");

    unsigned char * regVal = (unsigned char *)REG_CCR;
    // unsigned int value = *regVal;
    unsigned char tempMSB, tempLSB;

    for (int i = 3; i >= 0; --i) {

        tempLSB = regVal[i] & 0x0f;
        tempMSB = regVal[i] >> 4;
        printChar(hexTable[tempMSB]);
        printChar(hexTable[tempLSB]);
    }

    printStr("\n");
    printStr("__StackTop = ");
    extern unsigned char * __StackTop;

    for (int i = 3; i >= 0; --i) {

        tempLSB = __StackTop[i] & 0x0f;
        tempMSB = __StackTop[i] >> 4;
        printChar(hexTable[tempMSB]);
        printChar(hexTable[tempLSB]);
    }

    printStr("\n");
    printStr("REG_SYST_CVRRW = ");

    regVal = (unsigned char *)REG_SYST_CVRRW;
    // value = *regVal;
    for (int i = 3; i >= 0; --i) {

        tempLSB = regVal[i] & 0x0f;
        tempMSB = regVal[i] >> 4;
        printChar(hexTable[tempMSB]);
        printChar(hexTable[tempLSB]);
    }

    uint32_t threadStack[100];  // stack for thread 400 bytes
    Thread task1;

    // sleep forever
    while(1) ;
}

}
