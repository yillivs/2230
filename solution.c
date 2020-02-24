#include <msp430g2553.h>
#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>

/******
 *
 *    CONSTANTS
 *
 ******/
// typedef enum { _A = 10, _B = 11, _C = 12, _D = 13, _E = 14, _F = 15 } hex_digits;
// typedef enum { FALSE, TRUE } boolean;

const char digit[] = { BIT4, BIT5, BIT6, BIT7 };  // Digit Select

const char num[] = { /* _ B G A  C F E D
    0 */ 0x20, /* Numeric Display
    1 */ 0x37, /*
    2 */ 0x0C, /*
    3 */ 0x06, /*
    4 */ 0x13, /*
    5 */ 0x42, /*
    6 */ 0x40, /*
    7 */ 0x27, /*
    8 */ 0x00, /*
    9 */ 0x03, /*
    A */ 0x01, /*
    b */ 0x50, /*
    c */ 0x5C, /*
    d */ 0x14, /*
    E */ 0x48, /*
    F */ 0x49
};

/******
 *
 *    GLOBALS
 *
 ******/

unsigned number = 0;
     int cursor = 0;
     int adc10  = 1;

/******
 *
 *    PROTOTYPES
 *
 ******/
int shell_cmd_help(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);
int shell_cmd_hexd(shell_cmd_args *args);
int shell_cmd_dadc(shell_cmd_args *args);
int shell_cmd_eadc(shell_cmd_args *args);

/******
 *
 *    SHELL COMMANDS STRUCT
 *
 ******/
shell_cmds my_shell_cmds = {
  .count = 5,
  .cmds  = {
    {
      .cmd  = "help",
      .desc = "list available commands",
      .func = shell_cmd_help
    },
    {
      .cmd  = "args",
      .desc = "print back given arguments",
      .func = shell_cmd_argt
    },
    {
      .cmd  = "hexd",
      .desc = "display two characters as hex bytes on the display",
      .func = shell_cmd_hexd
    },
    {
      .cmd  = "dadc",
      .desc = "disable adc10 conversion",
      .func = shell_cmd_dadc
    },
    {
      .cmd  = "eadc",
      .desc = "enable adc10 conversion",
      .func = shell_cmd_eadc
    },
  }
};

/******
 *
 *    CALLBACK HANDLERS
 *
 ******/
int shell_cmd_help(shell_cmd_args *args)
{
  int k;

  for(k = 0; k < my_shell_cmds.count; k++) {
    cio_printf("%s: %s\n\r", my_shell_cmds.cmds[k].cmd, my_shell_cmds.cmds[k].desc);
  }

  return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
  int k;

  cio_print((char *)"args given:\n\r");

  for(k = 0; k < args->count; k++) {
    cio_printf(" - %s\n\r", args->args[k].val);
  }

  return 0;
}

int shell_cmd_hexd(shell_cmd_args *args)
{
  char* b = args->args[0].val;

  number = ((b[0] & 0xFF) << 8) + (b[1] & 0xFF);

  cio_printf("DISPLAYING: 0x%x\n\r", number);

  return 0;
}

int shell_cmd_dadc(shell_cmd_args *args)
{
  ADC10CTL0 &= ~ENC;
  adc10 = 0;

  cio_printf((char *)"DISABLED ADC10 CONVERSION\n\r");

  return 0;
}

int shell_cmd_eadc(shell_cmd_args *args)
{
  ADC10CTL0 |= ENC;
  ADC10CTL0 |= ADC10SC;
  adc10 = 1;

  cio_printf((char *)"ENABLED ADC10 CONVERSION\n\r");

  return 0;
}

int shell_process(char *cmd_line)
{
  return shell_process_cmds(&my_shell_cmds, cmd_line);
}

/******
 *
 *    INITIALIZATION
 *
 ******/
int main(void)
{
  /* CLOCKS *********************************/
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;

  /* GPIO ***********************************/
  P1DIR    = 0xF0;                          // P1.4-P1.7
  P1OUT    = 0;                             // All Digits Off

  P2SEL   &= ~0xC0;                         // Use P2.6 & P2.7 as GPIO

  P2DIR    = 0xFF;                          // P2.0-P2.7
  P2OUT    = 0xFF;                          // All Segments Off

  /* TIMER A0 *******************************/
  TA0CCTL0 = CCIE;                          // CC Interrupt Enable
  TA0CCR0  = 4000;                          // 1 Mhz / 4k = 250 Interrupts / Sec
  TA0CTL   = TASSEL_2                       // Source SMCLK
           | MC_1                           // Up Mode
           | ID_0;                          // No Division

  /* ADC10CTL *******************************/
  ADC10CTL0 &= ~ENC;                        // Disable ADC before configuration.
  ADC10CTL0 = ADC10ON;                      // Turn ADC on in single line before configuration.
  while(ADC10CTL1 & ADC10BUSY);             // Make sure the ADC is not running per 22.2.7
  ADC10DTC0 = ADC10CT;                      // Repeat conversion.
  ADC10DTC1 = 1;                            // Only one conversion at a time.
  ADC10SA = (unsigned int) &number;         // Put results at specified place in memory.
  ADC10CTL0 |= ADC10SHT_3 | SREF_0 | REFON | MSC; // 64 clock ticks, Use Reference, Reference on
                                            // ADC On, Multi-Sample Conversion, Interrupts enabled.
  ADC10CTL1 = INCH_0 | ADC10SSEL_3 | ADC10DIV_7 | CONSEQ_2; // Set channel, Use SMCLK,
                                            // 1/8 Divider, Repeat single channel.
  ADC10AE0  = 1;                            // Analog Enable P1.0
  ADC10CTL0 |= ENC;                         // Enable conversion.
  ADC10CTL0 |= ADC10SC;                     // Start conversion

  serial_init(9600);                        // Initialize Serial Comms
  __eint();

/******
 *
 *    PROGRAM LOOP
 *
 ******/
  for (;;) {
    int j = 0;                              // Char array counter
    char cmd_line[90] = {0};                // Init empty array

    cio_print((char *) "$ ");               // Display prompt
    char c = cio_getc();                    // Wait for a character
    while(c != '\r') {                      // until return sent then ...
      if(c == 0x08) {                       //  was it the delete key?
        if(j != 0) {                        //  cursor NOT at start?
          cmd_line[--j] = 0;                //  delete key logic
          cio_printc(0x08); cio_printc(' '); cio_printc(0x08);
        }
      } else {                              // otherwise ...
        cmd_line[j++] = c; cio_printc(c);   //  echo received char
      }
      c = cio_getc();                       // Wait for another
    }

    cio_print((char *) "\n\n\r");           // Delimit command result

    switch(shell_process(cmd_line))         // Execute specified shell command
    {                                       // and handle any errors
      case SHELL_PROCESS_ERR_CMD_UNKN:
        cio_print((char *) "ERROR, unknown command given\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_LEN:
        cio_print((char *) "ERROR, an arguement is too lengthy\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_MAX:
        cio_print((char *) "ERROR, too many arguements given\n\r");
        break;
      default:
        break;
    }

    cio_print((char *) "\n");               // Delimit Result
  }

  return 0;
}

/******
 *
 *    INTERRUPTS
 *
 ******/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    int n;
    if (adc10) {
        n = number; // & 0x3FC;
    } else {
        n = number;
    }

    P1OUT &= ~0xF0;                         // Turn Off All Digits
    if (++cursor > 3) cursor = 0;
    switch (cursor) {                       // Only Display Necessary Digits
      case 0:                               // Split Counter Variable to
        P2OUT = num[n / 0x1000 % 0x10] | BIT7;
        break;                              // Determine Number to Display
      case 1:
        P2OUT = num[n / 0x100 % 0x10]  | BIT7;
        break;
      case 2:
        P2OUT = num[n / 0x10 % 0x10]   | BIT7;
        break;
      case 3:
        P2OUT = num[n % 0x10]          | BIT7;
        break;
    }

    P1OUT = digit[cursor];                  // Next Digit Back On
}

