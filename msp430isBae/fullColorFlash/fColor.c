#include <msp430.h>

int main(void){

	WDTCTL  = WDTPW | WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	//Clear out bit registers. This should turn off all my lights
	//despite any residual set bits.
	P1OUT &= ~(BIT0+BIT1+BIT2);

	//I'm going to set my outputs to be the lights and my input to be
	//my button at P1.3
	P1IN = BIT3;	
	




}
