/**
 * @file    application.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    10-03-2019
 * @brief   Application class
 * @details   Implementation of application
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "application.hpp"

#include "thread.hpp"
#include "scheduler.hpp"

#include "led.hpp"
#include "sysClock.hpp"

// SysTick timer
uint32_t * const SYST_CSR = (uint32_t *)0xE000E010;     // Privileged a SysTick Control and Status Register
uint32_t * const SYST_RVR = (uint32_t *)0xE000E014;     // Privileged UNKNOWN SysTick Reload Value Register
uint32_t * const SYST_CVR = (uint32_t *)0xE000E018;     // Privileged UNKNOWN SysTick Current Value Register
uint32_t * const SYST_CALIB = (uint32_t *)0xE000E01;    // Privileged a SysTick Calibration Value Register
// Interrupt Control and State Register
uint32_t * const ICSR = (uint32_t *)0xE000ED04;
// UART0 data register on this MCU
uint32_t * const UART0DR = (uint32_t *)0x4000c000;


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
    *SYST_RVR = 0x0ffffff;   // 0x00000001-0x00FFFFFF. If the SysTick interrupt is required every 100 clock pulses, set RELOAD to 99.

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
    
    // timer stop
    // *SYST_CSR = 0;

    // Trigger the pendSV
    *ICSR = 0x10000000; // [28]PENDSVSETRWPendSV set-pending bit.

}

/* Pre-main initialisation
 */
#ifdef QEMU_DUMMY
void SystemInit(void) {
    printStr("ARM Cortex-M3 has started up!\n");
}
#endif
void __START(void) {

    // nucleus

    // priority of interrupts SysTick and PendSV

    volatile uint32_t thread1Stack[40];     // stack for thread 100x4=400 bytes
    volatile uint32_t thread2Stack[30];
    volatile uint32_t thread3Stack[33];

    const auto& task1 = []() {
        while(1) { 
        #ifdef QEMU_DUMMY
            printStr("TASK1\n");
        #else
            LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
        #endif
        }
    };
    const auto& task2 = []() {
        while(1) { 
        #ifdef QEMU_DUMMY
            printStr("task2\n");
        #else
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
        #endif
        }
    };

    Thread thread1(task1, (sizeof thread1Stack)/4, (uint32_t *)&thread1Stack);
    Thread thread2(task2, (sizeof thread2Stack)/4, (uint32_t *)&thread2Stack);
    Thread thread3(task2, (sizeof thread3Stack)/4, (uint32_t *)&thread3Stack);

    Scheduler::addThread(&thread1);
    Scheduler::addThread(&thread2);
    // Scheduler::addThread(&thread3);

    #ifndef QEMU_DUMMY
    SystemClock::setClockTo72Mhz();
    #endif

    LED led(GPIOC, LL_GPIO_PIN_13);
    led.on();

    sysTickInit();
    
    // switch to unprivileged and sleep? - or trigger ContextSwitch imideatly
    while(1) ;
}

}
