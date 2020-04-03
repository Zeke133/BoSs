/**
 * @file    application.cpp
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    17-03-2019
 * @brief   Application class
 * @details   Implementation of application
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

#include "application.hpp"

extern void main(void);

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
    *SYST_RVR = 0x11940;   // 0x00000001-0x00FFFFFF. If the SysTick interrupt is required every 100 clock pulses, set RELOAD to 99.
    // 72000 - 0x11940
    // 72000000 - 0x44AA200 - out of range

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

#ifdef NO_USE_LIBG
// used with boost
void __assert_func(const char* file, int line, const char* func, const char* expression) {
    while(1);
};
// need for correct global classes initialization
void callStaticConstructors() {

    // Start and end points of the constructor list,
    // defined by the linker script.
    extern void (*__init_array_start)();
    extern void (*__init_array_end)();

    // Call each function in the list.
    // We have to take the address of the symbols, as __init_array_start *is*
    // the first function pointer, not the address of it.
    for (void (**p)() = &__init_array_start; p < &__init_array_end; ++p) {
        (*p)();
    }
}
// kernel initialization
void _start() {

    callStaticConstructors();
    // priority of interrupts SysTick and PendSV
    main();

    // switch to unprivileged and sleep??? - or trigger ContextSwitch imideatly
}
#endif

void SysTick_Handler(void) {
    
    // *SYST_CSR = 0;   // timer stop
    Scheduler::tick();
    *ICSR = 0x10000000; // [28]PENDSVSET - RW - PendSV set-pending bit.
}

/* Pre-main initialisation
 */
#ifdef QEMU_DUMMY
// dummy clock for QEMU
uint32_t SystemCoreClock = 72000000U;

void SystemInit(void) {
    printStr("ARM Cortex-M3 has started up!\n");
}
#endif
void main(void) {

    #ifndef QEMU_DUMMY
    SystemClock::setClockTo72Mhz();
    #endif

    Delay delay;
    LED led(GPIOC, LL_GPIO_PIN_13);
    led.on();
    delay.us(100000);
    led.off();
    delay.us(100000);
    led.on();

    // auto dma = DMA::make_dma<DMA::Device::USART1_TX>();
    // auto uart = Uart::make_uart<1>(dma);
    
    // uart.puts("start");

    uint32_t thread1Stack[1000];
    // uint32_t thread2Stack[30];

    const auto& task1 = []() {
        while(1) { 
        #ifdef QEMU_DUMMY
            printStr("TASK1\n");
            // Scheduler::sleep(5);
        #else
            
            LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
            Scheduler::sleep(1);
        #endif
        }
    };
    // const auto& task2 = []() {
    //     while(1) { 
    //     #ifdef QEMU_DUMMY
    //         printStr("task2\n");
    //     #else
    //         LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    //         Scheduler::sleep(1);
    //     #endif
    //     }
    // };

    Thread thread1(task1, (sizeof thread1Stack)/4, (uint32_t *)&thread1Stack);
    // Thread thread2(task2, (sizeof thread2Stack)/4, (uint32_t *)&thread2Stack);

    Scheduler::run(thread1);
    // Scheduler::run(thread2);

    // where to place???
    #ifndef QEMU_DUMMY
    // Configure the SysTick timer to overflow every 1 ms
    SysTick_Config(SystemCoreClock / 1000);
    #endif

    while(1);
    
}

}
