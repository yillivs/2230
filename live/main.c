#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>

int fac(int);

int main(void)
{
  /* CLOCKS *********************************/
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;

  /* GPIO ***********************************/
  P1DIR = -1;
  P1OUT = 0;

  P2DIR = -1;
  P2OUT = 0;

  serial_init(9600);                        // Initialize Serial Comms

  cio_printf("4! = %i\n\r", fac(4));
  cio_printf("1! = %i\n\r", fac(1));
  cio_printf("0! = %i\n\r", fac(0));
  cio_printf("6! = %i\n\r", fac(6));
  cio_printf("7! = %i\n\r", fac(7));

  _BIS_SR(LPM4_bits);

  return 0;
}

