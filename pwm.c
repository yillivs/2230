#include <msp430.h>

int direction;

int main(void){
   	WDTCTL  = WDTPW | WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	P1DIR = BIT6;
	P1SEL &= ~BIT6;

	TA0CTL   = TASSEL_2 | MC_1 | ID_3;
	TA0CCR0  = 1042;
	TA0CCR1  = 10;
	TA0CCTL0 = CCIE;
	TA0CCTL1 = OUTMOD_7;

	P2DIR  = BIT1;
	P2OUT &= ~BIT1
	P2SEL  = BIT1;
	
	TA1CTL   = TASSEL_2 | MC_1 | ID_3;
	TA1CCR0  = 1042;
	TA1CCR1  = 11;
	TA1CCTL0 = CCIE;
	TA1CCTL1 = OUTMOD_7;

	P1REN = BIT3;
	P1OUT = BIT3;
	P1IE  = BIT3;
	P1IES = BIT3;
	P1IFG &= ~BIT3;
	
	_BIS_SR(LPM0_bits | GIE);
	for(;;);

	return 0;
}



#pragma vector=TIMER_A0_VECTOR
__interrupt void blink_green(void)
{
	if(TA0CCR1 == 1000 || TA0CCR1 == 10)}
		direction ^= -2;
	}
	
	TA0CCR1 += direction;
	TA1CCR1 += direction;
}
#pragma vector=TIME1_A0_VECTOR
__interrupt void blink_red(void)
{

}
#pragma vector=PORT1_VECTOR
__interrupt void button(void)
{
	P1SEL ^= BIT6;
	P2SEL ^= BIT1;


}
