#include <msp430.h>
#include <stdlib.h>
#include <string.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>

#include "shell.h"

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

  cio_print("args given:\n\r");

  for(k = 0; k < args->count; k++) {
    cio_printf(" - %s\n\r", args->args[k].val);
  }

  return 0;
}

int shell_cmd_push(shell_cmd_args *args)
{
	//if Stack has room, green light
  if(stack_top <  4){
	stack[stack_top] = malloc(sizeof(box_t));
	stack[stack_top]->name     = malloc(strlen(args->args[0].val)+1);
	strncpy(stack[stack_top]->name, args->args[0].val, strlen(args->args[0].val)+1);
	stack[stack_top]->quantity = atoi(args->args[1].val);
	stack[stack_top]->price    = atoi(args->args[2].val);
	P1OUT |= BIT6;
	stack_top++;
  }	  
  //if stack is full, red light
  else if(stack_top == 4){
	stack[stack_top]           = malloc(sizeof(box_t));
	stack[stack_top]->name     = malloc(strlen(args->args[0].val)+1);
	strncpy(stack[stack_top]->name, args->args[0].val, strlen(args->args[0].val)+1);
	stack[stack_top]->quantity = atoi(args->args[1].val);
	stack[stack_top]->price    = atoi(args->args[2].val);
	P1OUT &= ~BIT6; // clear green light
	P1OUT |= BIT0;
	stack_top++;
  }
  //if there is no room to add to stack. stack_top = 5;
  else{
	cio_printf("Full stack cannot add more");	
  }

  return 0;
}

int shell_cmd_pop(shell_cmd_args *args)
{
 if(stack_top == 0){ 
	cio_printf("Empty stack, cannot remove");
 }
 else if(stack_top == 5){
	free(stack[stack_top-1]->name);
	free(stack[stack_top-1]);
	stack_top--;
	P1OUT &= ~BIT0;
	P1OUT |= BIT6;
 }
 else if(stack_top == 1){
	free(stack[stack_top-1]->name);
	free(stack[stack_top-1]);
	stack_top--;
	P1OUT &= ~BIT6;
 }
 else{
	free(stack[stack_top-1]->name);
	free(stack[stack_top-1]);
	stack_top--;
 }

  return 0;
}

int shell_cmd_sell(shell_cmd_args *args)
{
  if(stack_top == 0){ 
	cio_printf("Empty stack, cannot remove");
 }
 else if(stack_top == 5){
	cio_printf("%s : $%i.%i",stack[stack_top-1]->name, (stack[stack_top-1]->quantity*stack[stack_top-1]->price)/100,
			cost(stack[stack_top-1])); 
	free(stack[stack_top-1]->name);	 
	free(stack[stack_top-1]);
	stack_top--;
	P1OUT &= ~BIT0;
	P1OUT |= BIT6;
 }
 else if(stack_top == 1){
	cio_printf("%s : $%i.%i",stack[stack_top-1]->name, (stack[stack_top-1]->quantity*stack[stack_top-1]->price)/100,
			cost(stack[stack_top-1])); 
	free(stack[stack_top-1]->name);
	free(stack[stack_top-1]);
	stack_top--;
	P1OUT &= ~BIT6;
 }
 else{
	cio_printf("%s : $%i.%i",stack[stack_top-1]->name, (stack[stack_top-1]->quantity*stack[stack_top-1]->price)/100,
			cost(stack[stack_top-1])); 
	free(stack[stack_top-1]->name);
	free(stack[stack_top-1]);
	stack_top--;
 }


  return 0;
}

int shell_cmd_show(shell_cmd_args *args)
{
  if(stack_top > 0){
	for(int i = stack_top-1; i >= 0 ; i--){
		cio_printf("- %s %i %i \n\r", stack[i]->name, stack[i]->quantity, stack[i]->price);
	}
  } 
  else{
		cio_printf("Stack is empty");
  }

  return 0;
}

int shell_process(char *cmd_line)
{
  return shell_process_cmds(&my_shell_cmds, cmd_line);
}

/******
 *
 *    FUNCTIONS
 *
 ******/

int cost(box_t* box) {
	int pennies = box->quantity * box->price;
	int cents = pennies % 100;
    return cents;
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
  P1DIR = ~BIT3;
  P1OUT = BIT3;

  P1REN = BIT3;
  P1IE  = BIT3;
  P1IES = BIT3;
  P1IFG = 0;

  P2DIR = -1;
  P2OUT = 0;

  serial_init(9600);                        // Initialize Serial Comms
  __eint();                                 // Enable Global Interrupts

/******
 *
 *    PROGRAM LOOP
 *
 ******/
  for (;;) {
    int j = 0;                              // Char array counter
    memset(cmd_line, 0, 90);                // Init empty array

    cio_print("$ ");               // Display prompt
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

    cio_print("\n\n\r");           // Delimit command result

    switch(shell_process(cmd_line))         // Execute specified shell command
    {                                       // and handle any errors
      case SHELL_PROCESS_ERR_CMD_UNKN:
        cio_print("ERROR, unknown command given\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_LEN:
        cio_print("ERROR, an arguement is too lengthy\n\r");
        break;
      case SHELL_PROCESS_ERR_ARGS_MAX:
        cio_print("ERROR, too many arguements given\n\r");
        break;
      default:
        break;
    }

    cio_print("\n");               // Delimit Result
  }

  return 0;
}

/******
 *
 *    INTERRUPTS
 *
 ******/
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  // swap the top two boxes on the stack when the button is pressed
  box_t *temp = stack[stack_top - 1];
  stack[stack_top - 1] = stack[stack_top - 2];
  stack[stack_top - 2] = temp;

  while (!(BIT3 & P1IN)) {} // is finger off of button yet?
  __delay_cycles(32000); // wait 32ms
  P1IFG &= ~BIT3; // clear interrupt flag
}

