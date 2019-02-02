
extern "C" {

// address of UART0 data register on this MCU
volatile unsigned int * const UART0DR = (unsigned int *)0x4000c000;
 
void print_str(const char *s) {

        while(*s != '\0') {
            
            *UART0DR = (unsigned int)(*s); /* Transmit char */
            s++; /* Next char */
    }
}
 
void c_entry() {

    print_str("Hello ARM!\n");
}

}
