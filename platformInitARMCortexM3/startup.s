/**
 * @file    startup_ARMCM3.S
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    02-09-2019
 * @brief   ARM Cortex-M3 startup code
 * @copyright GNU Public License.
 */

                .syntax  unified                            // set unified assembler syntax
                .arch    armv7-m


/**
 * Stack Configuration
 * Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
 */
                .equ     Stack_Size, 0x00000400             // assign value to variable

                .section .stack                             // assemble the following code into end of subsection
                .align   3                                  // advances the location counter until it is a multiple of 3
	            .globl   __StackTop                         // set variable visible globally
	            .globl   __StackLimit
__StackLimit:
                .space   Stack_Size                         // '.space size , fill' emits size bytes, each of value fill
                .size    __StackLimit, . - __StackLimit     // include auxiliary debugging information in the symbol table
__StackTop:
                .size    __StackTop, . - __StackTop


/**
 * Heap Configuration
 * Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
 */
                .equ     Heap_Size, 0x00000000

                .if      Heap_Size != 0                     /* Heap is provided */
                .section .heap
                .align   3
	            .globl   __HeapBase
	            .globl   __HeapLimit
__HeapBase:
                .space   Heap_Size
                .size    __HeapBase, . - __HeapBase
__HeapLimit:
                .size    __HeapLimit, . - __HeapLimit
                .endif

/**
 * Vector table
 * Here will set initial StackPointer and Reset_Handler
 */
                .section .vectors
                .align   2
                .globl   __Vectors
                .globl   __Vectors_End
                .globl   __Vectors_Size
__Vectors:
                .long   __StackTop                          /*     Top of Stack */
                .long    Reset_Handler                      /*     Reset Handler */
                .long    NMI_Handler                        /* -14 NMI Handler */
                .long    HardFault_Handler                  /* -13 Hard Fault Handler */
                .long    MemManage_Handler                  /* -12 MPU Fault Handler */
                .long    BusFault_Handler                   /* -11 Bus Fault Handler */
                .long    UsageFault_Handler                 /* -10 Usage Fault Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    SVC_Handler                        /*  -5 SVCall Handler */
                .long    DebugMon_Handler                   /*  -4 Debug Monitor Handler */
                .long    0                                  /*     Reserved */
                .long    PendSV_Handler                     /*  -2 PendSV Handler */
                .long    SysTick_Handler                    /*  -1 SysTick Handler */

                /* External interrupts */
                .long    WWDG_IRQHandler                    // Window Watchdog
                .long    PVD_IRQHandler                     // PVD through EXTI Line detect
                .long    TAMPER_IRQHandler                  // Tamper
                .long    RTC_IRQHandler                     // RTC
                .long    FLASH_IRQHandler                   // Flash
                .long    RCC_IRQHandler                     // RCC
                .long    EXTI0_IRQHandler                   // EXTI Line 0
                .long    EXTI1_IRQHandler                   // EXTI Line 1
                .long    EXTI2_IRQHandler                   // EXTI Line 2
                .long    EXTI3_IRQHandler                   // EXTI Line 3
                .long    EXTI4_IRQHandler                   // EXTI Line 4
                .long    DMA1_Channel1_IRQHandler           // DMA1 Channel 1
                .long    DMA1_Channel2_IRQHandler           // DMA1 Channel 2
                .long    DMA1_Channel3_IRQHandler           // DMA1 Channel 3
                .long    DMA1_Channel4_IRQHandler           // DMA1 Channel 4
                .long    DMA1_Channel5_IRQHandler           // DMA1 Channel 5
                .long    DMA1_Channel6_IRQHandler           // DMA1 Channel 6
                .long    DMA1_Channel7_IRQHandler           // DMA1 Channel 7
                .long    ADC1_2_IRQHandler                  // ADC1_2
                .long    USB_HP_CAN1_TX_IRQHandler          // USB High Priority or CAN1 TX
                .long    USB_LP_CAN1_RX0_IRQHandler         // USB Low  Priority or CAN1 RX0
                .long    CAN1_RX1_IRQHandler                // CAN1 RX1
                .long    CAN1_SCE_IRQHandler                // CAN1 SCE
                .long    EXTI9_5_IRQHandler                 // EXTI Line 9..5
                .long    TIM1_BRK_IRQHandler                // TIM1 Break
                .long    TIM1_UP_IRQHandler                 // TIM1 Update
                .long    TIM1_TRG_COM_IRQHandler            // TIM1 Trigger and Commutation
                .long    TIM1_CC_IRQHandler                 // TIM1 Capture Compare
                .long    TIM2_IRQHandler                    // TIM2
                .long    TIM3_IRQHandler                    // TIM3
                .long    TIM4_IRQHandler                    // TIM4
                .long    I2C1_EV_IRQHandler                 // I2C1 Event
                .long    I2C1_ER_IRQHandler                 // I2C1 Error
                .long    I2C2_EV_IRQHandler                 // I2C2 Event
                .long    I2C2_ER_IRQHandler                 // I2C2 Error
                .long    SPI1_IRQHandler                    // SPI1
                .long    SPI2_IRQHandler                    // SPI2
                .long    USART1_IRQHandler                  // USART1
                .long    USART2_IRQHandler                  // USART2
                .long    USART3_IRQHandler                  // USART3
                .long    EXTI15_10_IRQHandler               // EXTI Line 15..10
                .long    RTCAlarm_IRQHandler                // RTC Alarm through EXTI Line
                .long    USBWakeUp_IRQHandler               @ USB Wakeup from suspend
__Vectors_End:
                .equ     __Vectors_Size, __Vectors_End - __Vectors
                .size    __Vectors, . - __Vectors

/**
 * Reset_Handler implementation
 */
                .section .text
                .align   2
                .thumb                                      // selects the instruction set being generated
                .thumb_func                                 // specifies that the following symbol is the name of a Thumb encoded function
                .globl   Reset_Handler
                .fnstart
Reset_Handler:
/* Firstly it copies data from read only memory to RAM.
 *
 * The ranges of copy from/to are specified by following symbols
 *   __etext: LMA of start of the section to copy from. Usually end of text
 *   __data_start__: VMA of start of the section to copy to
 *   __data_end__: VMA of end of the section to copy to
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
                ldr      r1, =__etext                       // Load register from memory, PC-relative (address = ProgrammCounter +/- X)
                ldr      r2, =__data_start__
                
                ldr      r3, =__data_end__
.L_loop1:
                cmp      r2, r3                             // subtracts the value of r3 from the value in r2
                ittt     lt                                 // apply cond LT (N != V Less than, signed) for first, second and third instr in block
                ldrlt    r0, [r1], #4                       // Loads (conditionally) to r0 a word from the address in r1, and than increments r1 by 4
                strlt    r0, [r2], #4                       // Stores (conditionally) value of r0 to the address in r2, and than increments r2 by 4
                blt      .L_loop1                           // Branch (conditionally) without updating of LR(Link Register) to .L_loop1

/* This part of work usually is done in C library startup code.
 * Otherwise, enable it in this startup.
 *
 * Single BSS section scheme.
 *
 * The BSS section is specified by following symbols
 *   __bss_start__: start of the BSS section.
 *   __bss_end__: end of the BSS section.
 *
 * Both addresses must be aligned to 4 bytes boundary.
 */
                ldr      r1, =__bss_start__
                ldr      r2, =__bss_end__

                movs     r0, 0                              // Write value of 0x00 to r0, status flags get updated
.L_loop3:
                cmp      r1, r2
                itt      lt
                strlt    r0, [r1], #4
                blt      .L_loop3

                bl       SystemInit                         // Branch with updating of LR(Link Register) to function
                bl       __START

                .fnend
                .size    Reset_Handler, . - Reset_Handler

/**
 * Default_Handler implementation
 */
                .thumb_func
                .weak    Default_Handler
                .fnstart
Default_Handler:
                b        .
                .fnend
                .size    Default_Handler, . - Default_Handler

/**
 * Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm

/* Default exception/interrupt handler */

                Set_Default_Handler    NMI_Handler
                Set_Default_Handler    HardFault_Handler
                Set_Default_Handler    MemManage_Handler
                Set_Default_Handler    BusFault_Handler
                Set_Default_Handler    UsageFault_Handler
                Set_Default_Handler    SVC_Handler
                Set_Default_Handler    DebugMon_Handler
                Set_Default_Handler    PendSV_Handler
                Set_Default_Handler    SysTick_Handler

                // External Interrupts
                Set_Default_Handler    WWDG_IRQHandler            // Window Watchdog
                Set_Default_Handler    PVD_IRQHandler             // PVD through EXTI Line detect
                Set_Default_Handler    TAMPER_IRQHandler          // Tamper
                Set_Default_Handler    RTC_IRQHandler             // RTC
                Set_Default_Handler    FLASH_IRQHandler           // Flash
                Set_Default_Handler    RCC_IRQHandler             // RCC
                Set_Default_Handler    EXTI0_IRQHandler           // EXTI Line 0
                Set_Default_Handler    EXTI1_IRQHandler           // EXTI Line 1
                Set_Default_Handler    EXTI2_IRQHandler           // EXTI Line 2
                Set_Default_Handler    EXTI3_IRQHandler           // EXTI Line 3
                Set_Default_Handler    EXTI4_IRQHandler           // EXTI Line 4
                Set_Default_Handler    DMA1_Channel1_IRQHandler   // DMA1 Channel 1
                Set_Default_Handler    DMA1_Channel2_IRQHandler   // DMA1 Channel 2
                Set_Default_Handler    DMA1_Channel3_IRQHandler   // DMA1 Channel 3
                Set_Default_Handler    DMA1_Channel4_IRQHandler   // DMA1 Channel 4
                Set_Default_Handler    DMA1_Channel5_IRQHandler   // DMA1 Channel 5
                Set_Default_Handler    DMA1_Channel6_IRQHandler   // DMA1 Channel 6
                Set_Default_Handler    DMA1_Channel7_IRQHandler   // DMA1 Channel 7
                Set_Default_Handler    ADC1_2_IRQHandler          // ADC1_2
                Set_Default_Handler    USB_HP_CAN1_TX_IRQHandler  // USB High Priority or CAN1 TX
                Set_Default_Handler    USB_LP_CAN1_RX0_IRQHandler // USB Low  Priority or CAN1 RX0
                Set_Default_Handler    CAN1_RX1_IRQHandler        // CAN1 RX1
                Set_Default_Handler    CAN1_SCE_IRQHandler        // CAN1 SCE
                Set_Default_Handler    EXTI9_5_IRQHandler         // EXTI Line 9..5
                Set_Default_Handler    TIM1_BRK_IRQHandler        // TIM1 Break
                Set_Default_Handler    TIM1_UP_IRQHandler         // TIM1 Update
                Set_Default_Handler    TIM1_TRG_COM_IRQHandler    // TIM1 Trigger and Commutation
                Set_Default_Handler    TIM1_CC_IRQHandler         // TIM1 Capture Compare
                Set_Default_Handler    TIM2_IRQHandler            // TIM2
                Set_Default_Handler    TIM3_IRQHandler            // TIM3
                Set_Default_Handler    TIM4_IRQHandler            // TIM4
                Set_Default_Handler    I2C1_EV_IRQHandler         // I2C1 Event
                Set_Default_Handler    I2C1_ER_IRQHandler         // I2C1 Error
                Set_Default_Handler    I2C2_EV_IRQHandler         // I2C2 Event
                Set_Default_Handler    I2C2_ER_IRQHandler         // I2C2 Error
                Set_Default_Handler    SPI1_IRQHandler            // SPI1
                Set_Default_Handler    SPI2_IRQHandler            // SPI2
                Set_Default_Handler    USART1_IRQHandler          // USART1
                Set_Default_Handler    USART2_IRQHandler          // USART2
                Set_Default_Handler    USART3_IRQHandler          // USART3
                Set_Default_Handler    EXTI15_10_IRQHandler       // EXTI Line 15..10
                Set_Default_Handler    RTCAlarm_IRQHandler        // RTC Alarm through EXTI Line
                Set_Default_Handler    USBWakeUp_IRQHandler       // USB Wakeup from suspend

                .end
