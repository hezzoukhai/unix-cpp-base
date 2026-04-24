#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <define.h>
#include <math.h>


/*char *trim (char *String)
{
	int dest;
	int src=0;
	int len = strlen(String);
	
	 Advance src to the first non-whitespace character. 
	while(isspace(String[src])) src++;
	 Copy the string to the "front" of the buffer 
	for(dest=0; src<len; dest++, src++) 
	{
		String[dest] = String[src];
	}
	/* Working backwards, set all trailing spaces to NULL. 
	for(dest=len-1; isspace(String[dest]); --dest)
	{
		String[dest] = '\0';
	}
	return String ;
}
*/

void cipherPassword (unsigned char *password, unsigned char *cipherPassword)
{
	char    subChaine[17];
	char    subChaineHex[17];
	char    subCipherChaine[17];
	char	cryptogram  [512];/* AER28022013 Password encryption with 3DES method*/
	char  	ChainePassword[32+2+1];

	int i ;
	int j;
	int lenToCp ;
	int nToloop = 0;
	memset(cryptogram,   0, sizeof(cryptogram));/*AER28022013 Password encryption with 3DES method*/
	memset(ChainePassword,   0, sizeof(ChainePassword));
	
	sprintf(ChainePassword,"%.2lu%s", strlen(password),password);
	GetCryptogram(cryptogram);/*AER28022013 Password encryption with 3DES method*/

	if((strlen(ChainePassword)%8) ==0)
	{
		nToloop = strlen(ChainePassword)/8 ;
	}
	else
	{
		nToloop = (strlen(ChainePassword)/8) + 1 ;
	}
	
   for (j=0 ; j<nToloop;j++)
   {
		lenToCp = 0 ;
		memset(subChaine, 0, sizeof(subChaine));
		memset(subChaineHex, 0, sizeof(subChaineHex));
		memset(subCipherChaine, 0, sizeof(subCipherChaine));
	
		if(((j+1)*8)<=strlen(ChainePassword))
		{
			lenToCp = 8 ;
		}
		else
		{
			lenToCp = strlen(ChainePassword) - (j*8) ;
		}
	
		memcpy(subChaine,ChainePassword+(j*8),lenToCp);
	
		for(i=0;i<8;i++) 
			sprintf(subChaineHex + ( 2 * i), "%0X", *(subChaine + i));
	
		fct_3DES(1,cryptogram,subChaineHex,subCipherChaine);/*AER28022013 Password encryption with 3DES method*/
		sprintf(cipherPassword,"%s%s",cipherPassword,subCipherChaine);
   }
   fprintf(stdout,"Database password          : %s\n", cipherPassword);
   return ;
}

void decipherPassword (unsigned char *cipherPassword, unsigned char *password)
{
	char    subCipherChaine[17];
	char    subChaineHex[17];
	char    subChaine[17];
	char	cryptogram  [512];/* AER28022013 Password encryption with 3DES method*/
	char    passWordChaine[64+2+2+1];
	char    passLength[2+1];
	int     nLe=0;
	

	int i ;
	int lenToCp ;
	int nToloop = 0;
	memset(cryptogram,   0, sizeof(cryptogram));/*AER28022013 Password encryption with 3DES method*/
	memset(passWordChaine, 0, sizeof(passWordChaine));
	memset(passLength, 0, sizeof(passLength));
	memset(password, 0, sizeof(password));
	
	GetCryptogram(cryptogram);/*AER28022013 Password encryption with 3DES method*/
	
	if((strlen(cipherPassword)%16) ==0)
	{
		nToloop = strlen(cipherPassword)/16 ;
	}
	else
	{
		nToloop = (strlen(cipherPassword)/16) + 1 ;
	}

   for(i=0 ;i<nToloop ; i++)
   {
		lenToCp = 0 ;
		memset(subCipherChaine, 0, sizeof(subCipherChaine));
		memset(subChaineHex, 0, sizeof(subChaineHex));
		memset(subChaine, 0, sizeof(subChaine));

		if(((i+1)*16)<=strlen(cipherPassword))
		{
			lenToCp = 16 ;
		}
		else
		{
			lenToCp = strlen(cipherPassword) - (i*16) ;
		}
		
		memcpy(subCipherChaine,cipherPassword+(i*16),lenToCp);
		fct_3DES(-1,cryptogram,subCipherChaine,subChaineHex);
		/*compress(subChaine, subChaineHex);*/
		PcrdCompress(subChaine, subChaineHex);/*ICH20160811 Change func name*/
		
		sprintf(passWordChaine,"%s%s",passWordChaine,subChaine);
	}  

	lenToCp = 0 ;
	memcpy(passLength,passWordChaine,2);
	lenToCp = atoi(passLength);	
	memcpy(password,passWordChaine+2,lenToCp);

	return ;
}


