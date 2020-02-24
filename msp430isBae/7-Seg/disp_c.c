#include <msp430.h>

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    P1DIR = -1;
	P1OUT = 0b11110011;
	

    P2SEL &= ~(BIT6 | BIT7);

    P2DIR = -1;
	P2OUT = 0;

    return 0;
}
