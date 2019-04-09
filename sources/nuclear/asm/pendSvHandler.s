/**
 * @file    pendSvHandler.s
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    21-02-2019
 * @brief   BoSs PendSV_Handler.
 * @details   ARM Cortex-M3.
 *            void PendSV_Handler(void)
 *            This interrupt process context switch.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified         /**< set unified assembler syntax */
                .arch   armv7-m

/**
 * @brief  PendSV_Handler implementation for ARM Cortex-M3
 * @param  None
 * @retval None
 */

                .section .text
                .align  2
                .thumb                  /**< selects the instruction set being generated */
                .thumb_func             /**< specifies that the following symbol is the name of a Thumb encoded function */
                .globl  PendSV_Handler
                .fnstart
PendSV_Handler:

                /**
                 * It is forbidden to modify SW frame registers (R4-R11) and to use stack in this procedure.
                 * Otherwise saved/restored thread stack frames will get corrupted
                 * or MSP will be garbaged on each procedure run
                 * because procedure can exit in any place.
                 */

                /* Here the procedure names definitions linked from C++ code class Scheduler */
                .equ    switchThread,       _ZN9Scheduler12switchThreadEv

                BL      switchThread        /**< call Scheduler::switchThread() */
                                            /**
                                             * R0 will contain return value (Thread *)
                                             * which is used as input argument to next procedure
                                             */

                CBZ     R0, .noThread       /**< Compare and Branch on Zero */
                                            /**< jump to '.noThread' */

                B       RestoreContext      /**< call ASM procedure: takes address of SP in R0! */
                                            /**< Return from exception handler to new thread */
.noThread:
                MOV     LR, #0xFFFFFFF9     /**< Return from exception handler to Thread MSP */
                BX      LR                  /**< without context switch */

                .fnend
                .end
