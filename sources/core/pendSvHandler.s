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

                // need to be in pure Asembler

                Scheduler::execute();

                if (Scheduler::lastDecision == Scheduler::Decision::noAction)
                    return;

                if (Scheduler::lastDecision == Scheduler::Decision::saveAndRestore) {

                    saveContext((uint32_t*)currentThread);

                    currentThread->setState(Thread::State::paused);
                    currentThread = nextThread;
                    currentThread->setState(Thread::State::running);
                }

                restoreContext((uint32_t*)currentThread);

                while (1) { /* here should never get */ }



                LDR     R1, [R0]            // dereference Thread * to stackTop *
                LDR     R4, [R1], #4        // Restore software frame
                LDR     R5, [R1], #4
                LDR     R6, [R1], #4
                LDR     R7, [R1], #4
                LDR     R8, [R1], #4
                LDR     R9, [R1], #4
                LDR     R10, [R1], #4
                LDR     R11, [R1], #4
                MSR     PSP, R1             // Restore PSP

                // ------------------

                MOV     LR, #0xFFFFFFFD    // Return from exception
                BX      LR

                .fnend
                .end
