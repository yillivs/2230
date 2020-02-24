#include <msp430.h>
int main(void){
 P1DIR = 7;
 P1OUT = BIT1 | BIT0 | BIT2;
}
