/**
 * @file    linkerMemoryStm32f103c8t6.ld
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    21-09-2019
 * @brief   ARM Cortex-M3 Linker Script
 * @details   Specify STM32F103C8T6 target hardware memory regions.
 * @copyright GNU Public License.
 */

/* Linker script defines target hardware memory regions FLASH and RAM. */

/* Configure memory regions. */
MEMORY
{
	ROM (rx)	: ORIGIN = 0x08000000, LENGTH = 128K	/* Flash memory */
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 20k		/* RAM memory */
}
