#include <msp430.h>

int main(void){
	WDTCTL = WDTPW | WDTHOLD; 
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;


	//Set output direction in P1DIR, and set the output as high in P1OUT.
	
	P1DIR = BIT0;
	P1OUT = BIT0;
	
	while(1){

	P1OUT ^= BIT0; //bitwise xor toggles bits on and off
	__delay_cycles(100000);
	}


	return 0;
}
