/**
 * @file    libc.s
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    27-02-2019
 * @brief   Standart C library functions.
 * @details   ARM Cortex-M3.
 *            Some libc functions needed in project.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified         /**< set unified assembler syntax */
                .arch   armv7-m

/**
 * @brief  int __clzsi2 (unsigned int a); implementation for ARM Cortex-M3
 * @details
 *         These functions return the number of leading 0-bits in a,
 *         starting at the most significant bit position.
 *         If a is zero, the result is undefined. 
 * @param  unsigned int a
 * @retval int
 */
                .section .text
                .align  2
                .thumb                  /**< selects the instruction set being generated */
                .thumb_func             /**< specifies that the following symbol is the name of a Thumb encoded function */
                .globl  __clzsi2
                .fnstart
__clzsi2:

                CLZ     R1, R0
                MOV     R0, R1

                BX      LR  /**< Return from function */

                .fnend
                .end
