/**
 * @file    svCallApi.s
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    24-03-2019
 * @brief   BoSs Supervisor Call API.
 * @details   ARM Cortex-M3.
 *            This API implements Supervisor Calls.
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

                .syntax unified         /**< set unified assembler syntax */
                .arch   armv7-m

/**
 * @brief  svCallSleep implementation for ARM Cortex-M3
 * @param  None
 * @retval None
 */

                .section .text
                .align  2
                .thumb                  /**< selects the instruction set being generated */
                .thumb_func             /**< specifies that the following symbol is the name of a Thumb encoded function */
                .globl  svCallSleep
                .fnstart
svCallSleep:
                
                SVC     #0x13           /**< 0x13 sleep code */
                BX      LR              /**< Return from function */

                .fnend
                .end
