#include <msp430.h>

void rgb(char RED, char GREEN, char BLUE) {
	  P1OUT = RED << 1 | GREEN << 2 | BLUE << 3;
}

int main(void) {
 WDTCTL = WDTPW | WDTHOLD;
 BCSCTL1 = CALBC1_1MHZ;
 DCOCTL = CALDCO_1MHZ;
  

 P1DIR = 0b00001110;

 for (;;) {
	rgb(0, 0, 0);
    __delay_cycles(1000000);	
	 rgb(1, 0, 0);
    __delay_cycles(1000000);
	rgb(1, 1, 0);
    __delay_cycles(1000000);
	rgb(0, 1, 0);
	__delay_cycles(1000000);
	rgb(0, 1, 1);
	__delay_cycles(1000000);
	rgb(0, 0, 1);
	__delay_cycles(1000000);


 }
 return 0;
}
