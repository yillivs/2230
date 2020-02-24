#include <msp430.h>
#include "dtc.h"

/**  
 *   Make some globalz
 */
unsigned int number = 0;
unsigned int modulation;
int digitSwap = 0;
int colorSwap = 0;

/**
 *	Data structure for 7-segment display representation. Since the segments are active lows must be set
 *	using a BIC operation to P1OUT and P2OUT.
 */

//P1 digit representation 0-F
int hexP1 [16] = {0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 2, 2};

//P2 digit representation, the heavy liftin.
int hexP2 [16] = {0b01000000,0b11101001,0b00100100,0b10100000,0b10001001,0b10000000,0b00000000,0b11101000,0b00000000,0b10000000,0b00001000,0b00000001,0b00100101,0b00100001,0b00000100,0b00001100}; 
//P1 RGB
int rgbP1[3]  = {0b00100010, 0b01000100, 0b10000110};
int rgbP2[3]  = {0b00101101, 0b10000000, 0b00000011};

int main(void){
	//Yep
	WDTCTL   =   WDTPW  |  WDTHOLD;
	BCSCTL1  =   CALBC1_1MHZ;
	DCOCTL   =   CALDCO_1MHZ;

	/*
	 * GPIO ENABLE P1 & P2 to wat I WANT!
	 */
	P1DIR   = 0b11100111; //1 for outputs, 0 for inputs.
	P1OUT   = 0b00001110; //turn off digits, turn on red LED, turn off B segment and set button output high.

	/**There needs to be a button interrupt enabled and a pull up resistor to the button at P1.3 I'd like to set the
	 * output from P1OUT on BIT3 as high before.
	 */
	P1SEL   |= BIT2;
	P1REN   |= BIT3;
	P1IE    |= BIT3;
	P1IES   |= BIT3; //set high
	P1IFG   &= ~BIT3; //clear interrupt flag for button

	//Clear P2.6 & P2.7 for use
	P2SEL   &=  ~(BIT6+BIT7);
	P2SEL   |=  (BIT1+BIT4);

	P2DIR   |= 0b11111111;
	P2OUT   |= 0b11101101; //turn off all segments
	//Segments are active low but lights are active high

	//TimerA0 from da top boi
	TA0CCTL0 = CCIE;
	TA0CTL   = TASSEL_2 | MC_1 | ID_0;
	TA0CCR0  = 1024; // as far as I'm concerned this value can be changed and will change the interrupt.
//	TA0CCR1; //I think it's initialized to zero by default
	TA0CCTL1 = OUTMOD_7;

	//TimerA1
	TA1CTL   = TASSEL_2 | MC_1 | ID_0;
	TA1CCR0  = 1024;
//	TA1CCR1;
//	TA1CCTL0 = CCIE;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_7;

	initialize_dtc(INCH_4 , &number);	

	_BIS_SR(LPM0_bits | GIE); //set Low Power Mode zero and Global interrupt enable.
}

/**
 * Controls the display & 7 seg;
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
	
	
	modulation = number;

	if(colorSwap == 0){ //green
		TA0CCR1 = modulation;	
	}
	else if(colorSwap == 1){ //blue
		TA1CCR1 = modulation;	
	}
	else{ //red
		TA1CCR2 = modulation;
	}

	P1OUT &= ~(BIT5+BIT6+BIT7); //clear my three hex digit displays
	
	modulation >>= 2;
	modulation &= 0xff;	
	int remPlaceHolder[2] = {0};
	int remCount = 0;

	//for(int i = 0; i < 2; i++){
		while(modulation > 0){
		remPlaceHolder[remCount]  = modulation%16;
		modulation                = modulation/16;
		remCount++;
		}
	remCount = 0;
	//}
	if(digitSwap == 0){ //green
		if(colorSwap == 0){
			P1OUT = rgbP1[0] | BIT3;
			P2OUT = rgbP2[0];
		}	
		else if(colorSwap == 1){ //blue
			P1OUT = rgbP1[1] | BIT3;
			P2OUT = rgbP2[1];
		}
		else{
			P1OUT = rgbP1[2] | BIT3;
			P2OUT = rgbP2[2];
		}
	
		P1OUT |= BIT5; //activate 1st digit at the end
	}
	else if(digitSwap == 1){	
		P1OUT = hexP1[remPlaceHolder[1]] | BIT3;
		P2OUT = hexP2[remPlaceHolder[1]];
		P1OUT |= BIT6; //activate 2nd digit
	}
	else{
		P1OUT = hexP1[remPlaceHolder[0]] | BIT3;
		P2OUT = hexP2[remPlaceHolder[0]];
		P1OUT |= BIT7; //activate 3d digit.///////
		digitSwap = -1;
	}
	digitSwap++;
}

#pragma vector=PORT1_VECTOR
__interrupt void buttonToggle (void){
	
	/*
	 * If the button triggers an interrupt I should clear all LED's and display the correct one.
	 */
	
	if(colorSwap == 0){ //Green
	}	
	else if(colorSwap == 1){ //Blue
	}
	else{				//Red
		colorSwap = -1;	
	}
	while(!(BIT3 & P1IN)){} //make sure button is not held down
	__delay_cycles(32000); //delay input to prevent debouncing

	P1IFG &= ~BIT3;
	colorSwap++;
	
}

