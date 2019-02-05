/* ARM Cortex-M3 Linker Script */

/* Configure memory regions. */
MEMORY
{
	ROM (rx)	: ORIGIN = 0x00000000, LENGTH = 128K	/* Flash memory */
	RAM (rwx)	: ORIGIN = 0x20000000, LENGTH = 20K		/* RAM memory */
}

/* Configure output sections. */
SECTIONS
{
	/* ". = 0x0;" or "ENTRY()" are not needed when MEMORY is defined */

	/* ARM needs Vector table to be placed at 0x00000000 */
	.vector :
	{
		. = ALIGN(4);			/* Align current address pointer to 4 */
		KEEP(*(.vector))		/* Don't let GC .vector */
	} > ROM

	/* Executable code to .text section */
	.text :
	{
		. = ALIGN(4);

		/* Executable code */
		*(.text)				/* put here .text sections from all object files */
		
		/* Global constructors and destructors sections */
		/*
		KEEP(*(.init))
		KEEP(*(.fini))
		*/

		/* .ctors */
		/*
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)
		*/

		/* .dtors */
 		/*
		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)
		*/

		/* Exception frames information */
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
	
    _etext = .;             /* define a global symbols at end of code */
    
    /* Constant data to .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
    } > ROM

	. = 0x20000000;             /*RAM address start*/

	.data : { *(.data) }        /*put here .data sections from all files*/
	.bss : { *(.bss) }          /*put here .bss sections from all files*/

	. = ALIGN(8);               /*ARM need 8 byte align for SP*/
	. = . + 0x1000;             /*reserve 4kB of place for stack*/
	
    stack_top = .;              /*define stack_top with calculated address*/
}
