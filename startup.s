.text

//.type start, %function      // tells the assembler what type a symbol is. %function and %object.

.word stack_top             // put SP at 0x0
.word start                 // put PC at 0x4, start specified as function for reason address to be like - (address | 0x1)

.global start
start:
	ldr r1, =startupProcedure	// load c_entry address from .c file to R1 register
	bx r1						// run code execution from R1
