/**
 * @file    svCallHandler.s
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    24-03-2019
 * @brief   SVC_Handler.
 * @details   ARM Cortex-M3.
 *            void SVC_Handler(void)
 *            This procedure handles Supervisor Call interrupt.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified         /**< set unified assembler syntax */
                .arch   armv7-m

/**
 * @brief  SVC_Handler implementation for ARM Cortex-M3
 * @param  None
 * @retval None
 */

                .section .text
                .align  2
                .thumb                  /**< selects the instruction set being generated */
                .thumb_func             /**< specifies that the following symbol is the name of a Thumb encoded function */
                .globl  SVC_Handler
                .fnstart
SVC_Handler:

                /**
                 * It is forbidden to modify SW frame registers (R4-R11) and to use stack in this procedure.
                 * Otherwise saved thread stack frames will get corrupted.
                 */

                /* Here the procedure names definitions linked from C++ code class Scheduler */
                .equ    sleepActiveThread,  _ZN9Scheduler17sleepActiveThreadEv

                // Get SVCall code...

                BL      sleepActiveThread   /**< call Scheduler::sleepActiveThread() */
                                            /** 
                                             * R0 will contain return value (Thread *)
                                             * which is used as input argument to next procedure
                                             */
                BL      SaveContext         /**< call ASM procedure: takes address of SP in R0! */

                MOV     R0, #0xED04         /**< ICSR address 0xE000ED04 */
                MOVT    R0, #0xE000
                MOV     R2, #0x00000001
                LDR     R1, [R0]            /**< Load ICSR value from RAM to R1 */
                BFI     R1, R2, #28, #1     /**< [28]PENDSVSET - PendSV set-pending bit */
                STR     R1, [R0]            /**< Store modified ICSR value to RAM */

                MOV     LR, #0xFFFFFFF9     /**< Return from exception handler to Thread MSP */
                BX      LR                  /**
                                             * Tail-chaining should not let return to Thread
                                             * PendSV must appear next
                                             */

                .fnend
                .end
