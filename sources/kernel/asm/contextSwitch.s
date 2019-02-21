/**
 * @file    contextSwitch.s
 * @author  Denis Homutovski
 * @version V1.0.1
 * @date    21-02-2019
 * @brief   Procedures of context switch.
 * @details   ARM Cortex-M3.
 *            ------------
 *            Example of hardware stack frame saved/restored on interrupt entering:
 *            <previous>
 *            SP + 0x1C   xPSR    <-  SP points here before interrupt
 *            SP + 0x18   PC
 *            SP + 0x14   LR
 *            SP + 0x10   R12
 *            SP + 0x0C   R3
 *            SP + 0x08   R2
 *            SP + 0x04   R1
 *            SP + 0x00   R0      <-  SP points here after interrup
 *            -----------
 *            Exception return values need to be moved to LR
 *            EXC_RETURN          ->  LR
 *            0xFFFFFFF1 Handler mode MSP
 *            0xFFFFFFF9 Thread mode MSP
 *            0xFFFFFFFD Thread mode PSP
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified         /**< set unified assembler syntax */
                .arch   armv7-m

/**
 * @brief  RestoreContext procedure implementation for ARM Cortex-M3
 * @param  threadStackTop: must be passed as function argument in R0
 * @retval None
 */
                .section .text
                .align  2
                .thumb                  /**< selects the instruction set being generated */
                .thumb_func             /**< specifies that the following symbol is the name of a Thumb encoded function */
                .globl  RestoreContext
                .fnstart
RestoreContext:
                /**
                 * restore registers of next thread
                 * next thread SP passed as function argument in R0
                 */

                // profile methods for clock counts and chose the best one !!!???
                // ------------------
                @ LDR     R1, [R0]            /**< dereference Thread * to stackTop * */
                @ MOV     R2, SP              /**< Save MSP to R2 */
                @ MOV     SP, R1              /**< Restore thread SP from R1 */
                @ POP     {R4-R11}            /**< Restore software frame */
                @ MOV     R1, SP              /**< Restore PSP */
                @ MSR     PSP, R1
                @ MOV     SP, R2              /**< MSP back to SP of exception handler */
                // ------------------   compare two methods for clock counts) and chose the best one
                @ LDR     R1, [R0]            /**< dereference Thread * to stackTop * */
                @ LDMIA   R1!, {R4-R11}       /**< Increment address After each access. */
                @ MSR     PSP, R1
                // ------------------
                LDR     R1, [R0]            /**< dereference Thread * to stackTop * */
                LDR     R4, [R1], #4        /**< Restore software frame */
                LDR     R5, [R1], #4
                LDR     R6, [R1], #4
                LDR     R7, [R1], #4
                LDR     R8, [R1], #4
                LDR     R9, [R1], #4
                LDR     R10, [R1], #4
                LDR     R11, [R1], #4
                MSR     PSP, R1             /**< Restore PSP */
                // ------------------

                MOV     LR, #0xFFFFFFFD     /**< Return from exception handler to new thread */
                BX      LR

                .fnend  /**< RestoreContext end */

/**
 * @brief  SaveContext procedure implementation for ARM Cortex-M3
 * @param  threadStackTop: passed as function argument in R0
 * @retval None
 */
                .section .text
                .align  2
                .thumb
                .thumb_func
                .globl  SaveContext
                .fnstart
SaveContext:
                /**
                 * save registers of current thread
                 * current thread SP * passed as function argument in R0
                 */
                MRS     R1, PSP             /**< copy PSP to R1 */
                STR     R11, [R1, #-4]!     /**< save software frame */
                STR     R10, [R1, #-4]!
                STR     R9, [R1, #-4]!
                STR     R8, [R1, #-4]!
                STR     R7, [R1, #-4]!
                STR     R6, [R1, #-4]!
                STR     R5, [R1, #-4]!
                STR     R4, [R1, #-4]!
                STR     R1, [R0]            /**< Save PSP, dereference Thread * to stackTop * */
                
                // profile methods for clock counts and chose the best one !!!???
                // ------------------
                @ MOV R1, SP                  /**< Save MSP */
                @ MOV SP, R0                  /**< Copy registers */
                @ PUSH {R4-R11}
                @ MOV R0, SP                  /**< Save new PSP */
                @ MOV SP, R1                  /**< Restore MSP */
                // ------------------
                @ STMDB                     /**< Decrement address Before each access */
                @ MRS      R0, PSP          /**< get PSP to R0 */
                @ SUBS	 R0, #16            /**< move 4 x 4 bytes down on memory, because STMIA makes R0++ only, not R0-- */
                @ STMIA	 R0!,{R4-R7}        /**< store R4-R7 to address in R0 and increment R0 */
                @ MOV	     R4, R8         /**< copy highest registers to lowest */
                @ MOV	     R5, R9
                @ MOV	     R6, R10
                @ MOV	     R7, R11
                @ SUBS	 R0, #32            /**< move down to free space */
                @ STMIA	 R0!,{R4-R7}        /**< store highest registers values */
                @ SUBS     R0, #16          /**< move to our frame top */
                // ------------------

                BX      LR

                .fnend  /**< SaveContext end */

                .end    /**< end of file */
