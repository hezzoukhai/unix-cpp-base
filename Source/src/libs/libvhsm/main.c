# include <stdio.h>
# include <string.h>
# include <ctype.h>
#include "des.h"

int  msisdnToCode(char* sKey, char* sMsisdn, char* szCode);

void main (void) 
{
	char sPin[256];
  char sPhone[17];

 printf("\n --> Enter The Phone Number: "); scanf("%s", sPhone);

	/** msisdnToCode("1234567890abcdef", "162123456", sPin); **/
	/** msisdnToCode("1234567890abcdef", sPhone, sPin); **/
	 msisdnToCode("0101010101010101", sPhone, sPin); 
	/** printf ("\n %s", sPin); **/
	printf ("\n The Pin For the phone %s is : %s\n", sPhone, sPin);

}


