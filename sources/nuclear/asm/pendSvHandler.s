/**
 * @file    pendSvHandler.s
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    21-02-2019
 * @brief   PendSV_Handler.
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
                .equ    takeDecision,       _ZN9Scheduler12takeDecisionEv
                .equ    pauseActiveThread,  _ZN9Scheduler17pauseActiveThreadEv
                .equ    activateNextThread, _ZN9Scheduler18activateNextThreadEv

                BL      takeDecision        /**< call Scheduler::takeDecision() */
                                            /**
                                            * R0 will contain procedure return value
                                            * enum class Decision : uint8_t
                                            *      noAction = 0
                                            *      onlyRestore = 1
                                            *      saveAndRestore = 2
                                            */

                CBZ     R0, .exit           /**< Compare and Branch on Zero */
                                            /**< jump to '.exit' if decision is noAction */

                SUB     R0, #1              /**< if R0 == 1, it will be 0 now */
                CBZ     R0, .restore        /**< jump to '.restore' if decision is onlyRestore */

                BL      pauseActiveThread  /**< call Scheduler::pauseActiveThread() */
                                            /** 
                                            * R0 will contain return value (Thread *)
                                            * which is used as input argument to next procedure
                                            */
                BL      SaveContext         /**< call procedure: takes address of SP in R0! */

.restore:
                BL      activateNextThread  /**< call Scheduler::activateNextThread() return Thread* in R0 */
                B       RestoreContext      /**< call procedure: takes address of SP in R0! */
                                            /**< Return from exception handler to new thread */
.exit:
                MOV     LR, #0xFFFFFFF9     /**< Return from exception handler to Thread MSP */
                BX      LR                  /**< without context switch */

                .fnend
                .end
