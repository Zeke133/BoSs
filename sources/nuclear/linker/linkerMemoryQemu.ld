/**
 * @file    linkerMemoryQemu.ld
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    21-09-2019
 * @brief   ARM Cortex-M3 Linker Script
 * @details   Specify  QEMU lm3s811evb target hardware memory regions.
 * @copyright GNU Public License.
 */

/* Linker script defines target hardware memory regions FLASH and RAM. */

/* Configure memory regions. */
MEMORY
{
	ROM (rx)	: ORIGIN = 0x00000000, LENGTH = 128K	/* Flash memory */
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 8K		/* RAM memory */
}
