#include <msp430.h>

int main(void){
   	WDTCTL  = WDTPW | WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	P1DIR = BIT6;
	P1SEL = BIT6;

	TA0CTL   = TASSEL_2 | MC_1 | ID_3;
	TA0CCR0  = 1042;
	TA0CCR1  = 10;
	TA0CCTL0 = CCIE;
	TA0CCTL1 = OUTMOD_7;

	
	TA1CTL   = TASSEL_2 | MC_1 | ID_3;
	TA1CCR0  = 1042;
	TA1CCR1  = 10;
	TA1CCTL0 = CCIE;
	TA1CCTL1 = OUTMOD_7;
	
	_BIS_SR(LPM0_bits | GIE);
	for(;;);

	return 0;
}



#pragma vector=TIMER_A0_VECTOR
__interrupt void blink_green(void)
{

}
#pragma vector=TIME1_A0_VECTOR
__interrupt void blink_red(void)
{
}
#pragma vector=PORT1_VECTOR
__interrupt void pulse(void)
{

}
