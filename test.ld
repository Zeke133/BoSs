MEMORY
{
	
}

SECTIONS
{
	. = 0x0;                    /*ROM (FLASH) address start*/

	.text : {                   /*Text section of executable*/
		startup.*(.text)        /*put here .text sections from object files*/
		test.*(.text)           /*put here .text sections from object files*/
	}

	. = 0x20000000;             /*RAM address start*/

	.data : { *(.data) }        /*put here .data sections from all files*/
	.bss : { *(.bss) }          /*put here .bss sections from all files*/

	. = ALIGN(8);               /*ARM need 8 byte align for SP*/
	. = . + 0x1000;             /*reserve 4kB of place for stack*/
	
    stack_top = .;              /*define stack_top with calculated address*/
}