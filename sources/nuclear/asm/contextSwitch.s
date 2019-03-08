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

                LDR     R1, [R0]            /**< Dereference Thread * to stackTop * */
                LDMIA   R1!, {R4-R11}       /**< Increment address in R1 after each load to register */
                MSR     PSP, R1             /**< Restore PSP */

                MOV     LR, #0xFFFFFFFD     /**< Return from exception handler to new thread */
                BX      LR

                /* takes 13 clock cycles */

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
                
                MRS     R1, PSP             /**< Move PSP to R1 */
                STMDB   R1!,{R4-R11}        /**< Store R4-R11 to address in R1 and decrement R1 */
                STR     R1, [R0]            /**< Save PSP, dereference Thread * to stackTop * */

                BX      LR

                /* takes 11 clock cycles */

                .fnend  /**< SaveContext end */

                .end    /**< end of file */
