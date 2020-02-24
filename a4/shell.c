#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>

/******
 *
 *    CONSTANTS
 *
 ******/
//Integer array can be invoked to display digits on the 7 segment
int digitRep [16] = {0b11000000, 0b11001111, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000, 0b10001000, 0b10000011, 0b10100111, 0b10100001, 0b10000110, 0b10001110};
int hexBro [4];
// any constants or data structures go here

/******
 *
 *    GLOBALS
 *
 ******/
int digitCounter = 1;

// any variables needed by interrupts go here

/******
 *
 *    PROTOTYPES
 *
 ******/
int shell_cmd_help(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);
int shell_cmd_hexd(shell_cmd_args *args);

/******
 *
 *    SHELL COMMANDS STRUCT
 *
 ******/
shell_cmds my_shell_cmds = {
  .count = 3,
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
  // you should implement your function to
  // parse the first two characters in the
  // first argument here 
  char *argVal = args->args[0].val;
  int arguments [2] = { (int)*argVal, (int)*(argVal+1)};

  int remPlaceHolder [2];
  int remCount = 0;
  int broCount = 0;
  
  for(int i = 0; i < 2;){
	while(arguments[i] > 0){
		remPlaceHolder[remCount]  = arguments[i]%16;
		arguments[i]              = arguments[i]/16;
		remCount = 1;
	}
	
	 //reset counter statement.
	remCount = 0;
    
	if((i == 0) && (broCount == 0)){
		hexBro[0] = remPlaceHolder[1];
		broCount = broCount + 1;
	}
	if((i == 0) && (broCount == 1)){
		hexBro[1] = remPlaceHolder[0];
		broCount = broCount + 1;
	}
	if((i == 1) && (broCount == 2)){
		hexBro[2] = remPlaceHolder[1];
		broCount = broCount + 1;
	}
	if((i == 1) && (broCount == 3)){
		hexBro[3] = remPlaceHolder[0];
		broCount = 0;
	}
	
	i++;
  }
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
  
  // probably want to set up some P1 and P2
  // pin functionality here

  //P1 Intialization.
  P1DIR = -1;
  P1OUT = -1;//since -1 is to display all the digits setting to zero displays none.
  P1OUT &= ~(BIT4+BIT5+BIT6+BIT7);//turn off all digits

  P2SEL &= ~(BIT6 | BIT7);

  //P2 Initialization
  //P2OUT controls digits displayed.
  P2DIR = -1;
  P2OUT = -1;


  /* TIMER A0 *******************************/
  TA0CTL   = TASSEL_2 | MC_1 | ID_0;
  TA0CCR0  = 2500;
  TA0CCR1  = 11;
  TA0CCTL0 = CCIE;
  TA0CCTL1 = OUTMOD_7;

  // don't forget to initialize your timer!

  serial_init(9600);                        // Initialize Serial Comms
  __eint();                                 // Enable Global Interrupts

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
/** In this interrupt I set P1OUT to display the proper digit ever pass through
 */
 
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
	    
	    P1OUT &= ~(BIT4+BIT5+BIT6+BIT7);	   
	
	if (digitCounter == 1){
		P1OUT |= BIT4;
		P2OUT = digitRep[hexBro[0]];
	}
	else if (digitCounter == 2){
		P1OUT |= BIT5;
		P2OUT = digitRep[hexBro[1]];
	}
	else if (digitCounter == 3){
		P1OUT |= BIT6;
		P2OUT = digitRep[hexBro[2]];
	}
	else {
		P1OUT |= BIT7;
		P2OUT = digitRep[hexBro[3]];
		digitCounter = 0;
	}
	digitCounter++;
}
