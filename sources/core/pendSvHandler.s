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

                //BKPT    #0x23
                BL      _ZN9Scheduler12takeDecisionEv       // call Scheduler::takeDecision()

                LDR     R0, =_ZN9Scheduler12lastDecisionE   // load PC-relative address of label to R0
                LDR     R1, [R0]                            // get value of Scheduler::lastDecision attribute to R1

                MOV     R2, 0               // Scheduler::Decision::noAction
                CMP     R1, R2
                IT      EQ                  // if (lastDecision == noAction)
                BEQ     .exit

                LDR     R2, =_ZN9Scheduler13currentThreadE  // load PC-relative address of label to R2
                LDR     R0, [R2]                            // get value of Scheduler::currentThread attribute to R0

                MOV     R2, 1               // Scheduler::Decision::onlyRestore
                CMP     R1, R2
                IT      EQ
                BEQ     .restore

                BL      saveContext         // call procedure, takes address of SP in R0
                BL      _ZN9Scheduler18pauseCurrentThreadEv // call Scheduler::pauseCurrentThread()

.restore:
                BL      _ZN9Scheduler16runCurrentThreadEv   // call Scheduler::runCurrentThread()
                BL      restoreContext      // call procedure, takes address of SP in R0

.exit:
                BX      LR                  // Return from exception

                .fnend
                .end
