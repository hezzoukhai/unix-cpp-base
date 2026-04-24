#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <unistd.h>
#endif



main()
{
	unsigned char	EncryptedData   [1024];
	unsigned char	EncryptionKey   [1024];
	unsigned char	ClearData       [1024];
	unsigned char	verif [17];
	unsigned char	verif1[17];

	system("clear");

	memset(verif,  0, sizeof(verif));
	memset(verif1, 0, sizeof(verif1));
	memset(EncryptionKey, 0, sizeof(EncryptionKey));
	memset(ClearData, 0, sizeof(ClearData));
	memset(EncryptedData, 0, sizeof(EncryptedData));



        step1:
	fprintf(stdout,"ENCRYPTION KEY  (16-32-48  HEX)  : ");
	fflush(stdin);
	scanf("%s",EncryptionKey);
	if (   strlen(EncryptionKey)!=16 
            && strlen(EncryptionKey)!=32 
            && strlen(EncryptionKey)!=48) 
            {  fprintf(stderr,"\n Error in Key Length \n"); goto step1;  }


        step2:
	fprintf(stdout,"CLEAR DATA      (16-32-48  HEX)  : ");   
	fflush(stdin);
	scanf("%s",ClearData);
        if (   strlen(ClearData)%16 )
            {  fprintf(stderr,"\n Error in Data Length (Input Len [%lu] not muliple of 16) \n", strlen(ClearData)); goto step2;  }


/** Encrypting The Input Data **/
        fct_3DES ( 1, EncryptionKey, ClearData, EncryptedData);

/** Printing Out the Results **/

	fprintf(stdout,"Encryption Key                   : %.48s\n", EncryptionKey);
        fct_KCV_calc(EncryptionKey, verif); 
	fprintf(stdout,"KCV of Encryption Key            : %.16s\n", verif);
	fprintf(stdout,"Clear Data                       : %.1024s\n", ClearData);
        if ( strlen(ClearData) < 48 )
        {
           fct_KCV_calc(ClearData, verif1); 
	   fprintf(stdout,"KCV of Clear Data                : %.16s\n", verif1);
        }
	fprintf(stdout,"Encrypted Data                   : %.1024s\n", EncryptedData);
}
