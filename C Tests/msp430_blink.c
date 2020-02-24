#include <msp430.h>

int main(void){
	WDTCTL = WDTHOLD | WDTPW;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = 0b00000001;
	P1OUT = 0b00000001;

	return 0;
}
