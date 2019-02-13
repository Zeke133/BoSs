/**
 * @file    contextSwitch.s
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    10-02-2019
 * @brief   Procedure of context switch.
 * @details   ARM Cortex-M3.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax  unified                            // set unified assembler syntax
                .arch    armv7-m


/**
 * ContextSwitch implementation
 * processor is in Thread mode
 * processor is in Handler mode
 * <previous>
 * SP + 0x1C   xPSR    <-  SP points here before interrupt
 * SP + 0x18   PC
 * SP + 0x14   LR
 * SP + 0x10   R12
 * SP + 0x0C   R3
 * SP + 0x08   R2
 * SP + 0x04   R1
 * SP + 0x00   R0      <-  SP points here after interrup
 */
                .section .text
                .align   2
                .thumb                                      // selects the instruction set being generated
                .thumb_func                                 // specifies that the following symbol is the name of a Thumb encoded function
                @ .type    ContextSwitch, %function         // does it needed in combination with .thumb_func ???
                .globl   ContextSwitch
                .fnstart
ContextSwitch:
                @ p.23
                @ MSR instruction to set the Active stack pointer bit to 1
                @ or
                @ perform an exception return to Thread mode with the appropriate EXC_RETURN value
                @ When changing the stack pointer, software must use an ISB instruction immediately after the MSRinstruction. This ensures that instructions after the ISB instruction execute using the new stack pointer.

                @ Exception return occurs when the processor is in Handler mode
                @ and executes one of the following instructions attempts to
                @ set the PC to an EXC_RETURN value:
                @ •an LDM or POP instruction that loads the PC
                @ •an LDR instruction with PC as the destination
                @ •a BX instruction using any register.

                @ The processor saves an EXC_RETURN value to the LR on exception entry.
                @ The exception mechanism relies on this value to detect when the processor has completed an exception handler.
                @ 0xFFFFFFF1 Return to Handler mode. Exception return gets state from the MSP
                @ 0xFFFFFFF9 Return to Thread mode. Exception Return get state from the MSP
                @ 0xFFFFFFFD Return to Thread mode. Exception return gets state from the PSP

                // save all thread registers to it's stack
                // what to do if we run from MSP or first thread
                // or this exception is the last one

                // where from take thread top of the stack value ?
                // need global pointer for thread stack

                ldr      r0, threadStackTop
                msr      psp, r0

                // thread contex must be on stack already
                // so thread init is needed



                .fnend
                .size    Reset_Handler, . - Reset_Handler

                .end
