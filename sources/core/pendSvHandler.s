/**
 * @file    pendSvHandler.s
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    18-02-2019
 * @brief   PendSV_Handler.
 * @details   ARM Cortex-M3.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified                             // set unified assembler syntax
                .arch   armv7-m

/**
 * @brief  PendSV_Handler implementation for ARM Cortex-M3
 *         void PendSV_Handler(void)
 * @retval 0xFFFFFFFD Thread mode PSP
 */

                .section .text
                .align  2
                .thumb                                      // selects the instruction set being generated
                .thumb_func                                 // specifies that the following symbol is the name of a Thumb encoded function
                .globl  PendSV_Handler
                .fnstart
PendSV_Handler:

                // Can not touch SW frame registers in this procedure
                // and change stack.
                // Because saved/restored thread stack frames will be corrupted
                // or MSP will grow on each procedure run.

                BL      Scheduler::execute

                LDR     R0, =Scheduler::lastDecision
                MOV     R1, 0               // Scheduler::Decision::noAction
                CMP     R0, R1
                IT      EQ                  // if equal
                BEQ     .exit

                MOV     R1, 1               // Scheduler::Decision::saveAndRestore
                CMP     R0, R1
                ITT     EQ                  // if equal
                BLEQ    Scheduler::saveContext
                BLEQ    Scheduler::stepThreadList

                BL      Scheduler::restoreContext

.exit:
                BX      LR                  // Return from exception

                .fnend
                .end
