#include <msp430.h>

int counter = 0; //global variable counter

/*
 * This program focuses on turning on an RGB LED in set modes for every button press.
 * I apologize for the comment & code spagetti.
 */
int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;//turn off watchdog
    BCSCTL1 = CALBC1_1MHZ;//calibrate the processor to 1MHz
    DCOCTL  = CALDCO_1MHZ;//calibrate the processor to 1MHz
	

    TA0CTL   = TASSEL_2 | MC_1 | ID_3; //Timer A0 Control is divided
    TA0CCR0  = 31250 - 1; // 31250 - 1; //when the timer reaches 31249 it will return to 0
    // TA0CCR1  = 100 - 1; //implied
    TA0CCTL1 = CCIE; // Capture compare interrupt

    P2DIR = BIT1; // set output direction to BIT1 = 0b0000010
    P2SEL = BIT1; // port 2 selector

    TA1CTL   = TASSEL_2 | MC_1 | ID_3;//Timer A1 Control is divided
    TA1CCR0  = 62500 - 1; //when the timer reaches 62499 it will return to 0
    // TA1CCR1  = 0; //implied
    TA1CCTL1 = OUTMOD_4; //togles instertpins every time the timer value hits


	P1DIR |= BIT0; //Set p1 direction to BIT0 = 0b0000001 for light
    P1OUT |= BIT3; //Set button as high output
	P1REN |= BIT3; //Pull up resistor
	P1IE  |= BIT3; //set interupt to wake up at button
	P1IES |= BIT3; //set high to low
	P1IFG &= ~BIT3; //clear interrupt flag

	__eint(); //enable interrupt
	for(;;){ //infinite loop where red LED blinks
		if((!(BIT3 & P1IN)) && ((counter = 1)||(counter = 0)) ){
			P1OUT ^= BIT0; //bitwise exclusive or to toggle the light
				while(!(BIT3 & P1IN)){} 	
				__delay_cycles(50000); //every half a second

		}
	}
    return 0;
}
/*
 * This should be the interrupt set to blink the green and blue LED's
 * they should be triggered when the counter reaches a certain value.
 * To make sure it listens I've set the interrupt to always listening.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Blink(void)
{
		if((counter == 1) || (counter==2)){
		P1OUT ^= BIT1;
		while(!(BIT3 & P1IN)) {}
		counter++;
		P1IFG &= ~BIT3;
			if(counter == 4){
			counter = 0;//reset mode counter
			}
		}
		else{
		P2OUT ^= BIT2; //use port 2 for blue LED
		while(!(BIT3 & P1IN)) {}
		counter++;
		P1IFG &= ~BIT3;
			if(counter == 4){
			counter = 0;//reset mode counter
			}
		}

		
		P1IFG &= ~BIT3; //reset P1 interrupt flag
}
