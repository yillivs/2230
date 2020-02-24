#include <msp430.h>

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    P1DIR = BIT1 | BIT0;
    P1OUT = 0;

    for(;;) {
        __delay_cycles(250000);
        P1OUT ^= BIT0;
        __delay_cycles(250000);
        P1OUT ^= BIT1 | BIT0;
    }

    return 0;
}

