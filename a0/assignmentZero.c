#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/* @Author: Oliver Sanchez
 * This program attempts to convert decimal format numbers into different numerical bases.
 */
int main(int argc, char const **argv) //second value of array must be **argv[] error?
{
	char *ascii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
	char buffer[32] = {0}; //character array as buffer initialized to 0.

//argc must equal 3. Our argument count should contain program name, decimal number and desired base.
	if(argc == 3){
		
		int decimal = atoi(argv[1]);
		int base = atoi(argv[2]);
		int bufferCounter = 31;

		if(base == 2||base == 4||base == 8||base == 16|| base == 64)
		{
		 	int mask = base-1;		
			int shift = log(base)/log(2); //log2(base); 
			printf("mask : %i shift : %i ascii char: %c\n",mask, shift, ascii[10]);		
			printf("masking result : %i\n",(mask&decimal));
			while(mask>0)
			{
				if((mask & decimal) == 0)
				{
					buffer[bufferCounter] = 0;
					bufferCounter--;
					printf("0 %i %i\n",mask,decimal);		
				}
				else
				{
					decimal = (mask & decimal);
					printf("%c\n", ascii[decimal]);
					mask = mask >> shift; //I thought this would shift the mask so that the loop is divided
					printf("%i", mask);
				}	
			}

		}
		/*else
		{//regular modulus algorithm
			while(decimal > 0)
			{
				int remainder = decimal % base;
				buffer[bufferCounter] = remainder + '0';
				int decimal = decimal / base;
				printf("%c" ,ascii[decimal]);
				decimal--;
			}		
		}
	}*/
	else{
		printf("Expecting two command line inputs. Decimal number and base desired.");
	}


}
}
