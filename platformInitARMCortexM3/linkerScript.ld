/**
 * @file    linkerScript.ld
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    02-09-2019
 * @brief   ARM Cortex-M3 Linker Script
 * @copyright GNU Public License.
 */

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */

/* Configure memory regions. */
MEMORY
{
	ROM (rx)	: ORIGIN = 0x00000000, LENGTH = 128K	/* Flash memory */
	/* abnormal 0x08000000 on STM32, 0x00000000 on ARM Cortex-M3 */
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 8K		/* RAM memory */
	/* 8K on lm3s811evb QEMU and 20k on STM32F103C8T6 */
}

/* ". = 0x0;" or "ENTRY()" are not needed when MEMORY is defined */

/* Configure output sections. */
SECTIONS
{
	/* Executable code to .text section */
	.text :
	{
		/* ARM needs Vector table to be placed at 0x00000000 */
		KEEP(*(.vectors))		/* Don't let GC .vector */
		
		. = ALIGN(4);			/* Align current address pointer to 4 */
		*(.text)				/* put here .text sections from all object files */
		
		/* This section holds executable instructions that contribute to the process initialization code.
		 * When a program starts to run, the system arranges to execute the code in this section
		 * before calling the main program entry point (called main for C programs)
		KEEP(*(.init))
		*/
		/* This section holds executable instructions that contribute to the process termination code.
		 * That is, when a program exits normally, the system arranges to execute the code in this section.
		KEEP(*(.fini))
		*/

		/* .ctors
		 * This section contains a list of global constructor function pointers. */
		/*
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)
		*/

		/* .dtors
		 * This section contains a list of global destructor function pointers. */
 		/*
		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)
		*/

		/* This section contains information necessary for frame unwinding during exception handling. */
		/*
		KEEP(*(.eh_frame*))
		*/
	} > ROM
	
	/* Exceptions unwind info */
	/*
	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > ROM
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > ROM
	__exidx_end = .;
	*/
	
    /* Constant data to .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
    } > ROM

    __etext = .;			/* define a global symbols at end of code */
	
	/* Initialized data section
	 * __data_start__ and __data_end__ will be used to copy data
	 * from LMA (Load Memory Address) in ROM to VMA (Virtual Memory Address) in RAM
	 * in startup code */
	.data : AT (__etext)
	{
		__data_start__ = .;
		*(vtable)			/* check for correct name Vtable */
		*(.data)

		. = ALIGN(4);
		/* This section holds an array of function pointers that contributes to a single
		 * pre-initialization array for the executable or shared object containing the section.
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		*/
		. = ALIGN(4);
		/* This section holds an array of function pointers that contributes to a single
		 * initialization array for the executable or shared object containing the section.
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		*/
		. = ALIGN(4);
		/* This section holds an array of function pointers that contributes to a single
		 * termination array for the executable or shared object containing the section.
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
		*/
		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;

	} > RAM
	
	/* Uninitialized data section */
	.bss (NOLOAD):
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} > RAM

	.heap (NOLOAD):
	{
		__end__ = .;
		PROVIDE(end = .);
		*(.heap*)
		__HeapLimit = .;
	} > RAM

	/* .stack_dummy section doesn't contain any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (NOLOAD):
	{
		*(.stack*)
	} > RAM

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ORIGIN(RAM) + LENGTH(RAM);
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
		
		*(.ARM.exidx)		/* this can force to not include ARM exception unwinder */
		/*
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
		*/
	}
}
