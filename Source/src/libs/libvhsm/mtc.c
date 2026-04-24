#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "des.h"

int  msisdnToCode(char* sKey, char* sMsisdn, char* szCode);
void Compress (unsigned char sSource[], unsigned char sTarget[], int nLen);
void Uncompress ( unsigned char sSource[], unsigned char sTarget[], int nLen);
void toDecimal(char sString[], int nLen);
static unsigned char oddParity(unsigned char s);
void adjustParity(char* sKey);
void initKey(char* skey);


int msisdnToCode(char* sKey, char* szMsisdn, char* szCode)
{
	
	char sMsisdn_C[17];
	unsigned char sWork[17];
 char phone_num_hex[16];
 char v_out[16];
 int v_long = 1;
	int nLen, i;

	Compress((unsigned char*)sKey, sWork, 16);
	adjustParity((char*)sWork);
	initKey((char*)sWork);
	
	nLen = strlen(szMsisdn);
	if (nLen > 16) 
		nLen = 16;
	memset(sWork, 0x0f, 16);                
	for (i=0; i<=nLen; i++)	
		sWork[16-i] = szMsisdn[nLen-i];
		
 printf("\n 1 --> %s <-- -->%d<--\n", sWork, strlen(sWork));	 
	Compress(sWork, (unsigned char*)sMsisdn_C, 16);
printf("\n - --> %s <--\n", sMsisdn_C);

printf("\n 2 --> ", sMsisdn_C);	
for(i=0; i<8; ++i)
	printf("0x%.02X, ", sMsisdn_C[i]);	
printf(" <-- \n", sMsisdn_C);	

memset(phone_num_hex,  0x0f, 16);
memset(v_out,  0, 16);
memcpy(phone_num_hex+8, sMsisdn_C, 8);

 DES_Hex(1, phone_num_hex, sKey,  v_out);
 printf("\n sizeof(v_long) = %d\n", sizeof(v_long));

	encrypt(sMsisdn_C);

printf("\n * --> ", sMsisdn_C);	
for(i=0; i<8; ++i)
	printf("0x%.02X, ", sMsisdn_C[i]);	
printf(" <-- \n", sMsisdn_C);	

/** printf("\n 3 --> %s <-- \n", sMsisdn_C);	 **/
	Uncompress((unsigned char*)sMsisdn_C, sWork, 8);

printf("\n 4 --> %s <-- \n", sWork);	
	toDecimal((char*)sWork, 16);	

printf("\n --> %s <-- \n", sWork);	
	szCode[0] = sWork[2-1];
	szCode[1] = sWork[7-1];
	szCode[2] = sWork[9-1];
	szCode[3] = sWork[14-1];
	szCode[4] = '\0';

	return (0);
}


void Compress (unsigned char sSource[], unsigned char sTarget[], int nLen){

	int  i, j;
	char cGauche, cDroit;
	char sWork[1000];

	for (i=0; i<(nLen*2); i++)
		sWork[i] =  toupper(sSource[i]);

	for (i=0,j=0; i<nLen; i++){    /*  0x41(A) - 0x37(7) = 0x0A */
		if (sWork[j] >= 'A') cGauche = (sWork[j++] - '7') << 4;
		else                 cGauche = (sWork[j++] - '0') << 4;
		if (sWork[j] >= 'A') cDroit  =  sWork[j++] - '7';
		else                 cDroit  =  sWork[j++] - '0';
		sTarget[i] = cGauche | cDroit;
	}
}


void Uncompress ( unsigned char sSource[], unsigned char sTarget[], int nLen){

	int i = 0, j = 0;
	for (; i<nLen; i++){
  printf("\n --> sSource[%d]= 0x%.02X", i, (unsigned char)sSource[i]);
	 sTarget[j++] = ((unsigned char)sSource[i] / 0x10) + '0';
		sTarget[j++] = ((unsigned char)sSource[i] % 0x10) + '0';
  printf("\n --> sTarget[%d-%d]= 0x%.02X 0x%.02X", j-2,j-1,sTarget[j-2], sTarget[j-1]); 
	}
 printf("\n");
	sTarget[j] = '\0';
}



void toDecimal(char sString[], int nLen){

	int i;

	for (i=0; i<nLen; i++)
		if (sString[i] >= 0x3a && sString[i] <= 0x3f)
			sString[i] -= 0x0a;
}



/* -------- make a character odd parity ---------- */
static unsigned char oddParity(unsigned char s)
{
   unsigned char c = s | 0x80;
   while (s)    {
       if (s & 1)
           c ^= 0x80;
       s = (s >> 1) & 0x7f;
   }
   return c;
}

/* ------ make a key odd parity ------- */
void adjustParity(char* sKey)
{
   int i;
   for (i=0; i<8; i++)
       *(sKey+i) = oddParity(*(sKey+i));
}

