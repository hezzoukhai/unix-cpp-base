/*****************************************************************************/
/* (c) Hightech Payment Systems 2013                                         */
/*     Description : Get key used to encrypt/decrypt database password       */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <define.h>


int GetCryptogram(char *Cryptogram)
{
  
	char sLoc[]="\x23\x15\x20\x8C\x91\x10\xAD\x40\x04\xDA\x01\x19\xC8\x02\x51\x32";
  HexToAscii(sLoc,Cryptogram,32);
  
  memset(sLoc,'\0',sizeof(sLoc));
  
  return 0;
}

