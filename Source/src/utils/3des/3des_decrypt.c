#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <unistd.h>
#endif



int main ( void)
{
	unsigned char	EncryptionKey   [1024];
	unsigned char	EncryptedData   [1024];
	unsigned char	ClearData       [1024];
	unsigned char	verif [17];
	unsigned char	verif1[17];

	system("clear");

	memset(verif,  0, sizeof(verif));
	memset(verif1, 0, sizeof(verif1));
	memset(EncryptionKey, 0, sizeof(EncryptionKey));
	memset(EncryptedData, 0, sizeof(EncryptedData));
	memset(ClearData, 0, sizeof(ClearData));



        step1:
	fprintf(stdout,"ENCRYPTION KEY  (16-32-48  HEX)  : ");
	fflush(stdin);
	scanf("%s",EncryptionKey);
	if (   strlen(EncryptionKey)!=16 
            && strlen(EncryptionKey)!=32 
            && strlen(EncryptionKey)!=48) 
            {  fprintf(stderr,"\n Error in Key Length \n"); goto step1;  }

        step2:
	fprintf(stdout,"DATA   (HEX. LEN MULTIPLE OF 16) : ");   
	fflush(stdin);
	scanf("%s",EncryptedData);
        if (   strlen(EncryptedData)%16 )
            {  fprintf(stderr,"\n Error in Data Length (Input Len [%lu] not muliple of 16) \n", strlen(EncryptedData)); goto step2;  }


/** Decrypting The Input Data **/
        fct_3DES ( -1, EncryptionKey, EncryptedData, ClearData);


/** Generating KCV of the Decryoted Data **/

	fprintf(stdout,"Encryption Key                   : %.48s\n", EncryptionKey);
        fct_KCV_calc(EncryptionKey, verif); 
	fprintf(stdout,"KCV of Encryption Key            : %.16s\n", verif);
	fprintf(stdout,"Encrypted Data                   : %.1024s\n", EncryptedData);
	fprintf(stdout,"Clear Data                       : %.1024s\n", ClearData);
        if( strlen(EncryptedData) < 48 )
        {
           fct_KCV_calc(ClearData, verif1); 
	   fprintf(stdout,"KCV of Clear Data                : %.16s\n", verif1);
        }

        return ( 0 ) ;
        
}
