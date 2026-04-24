/*****************************************************************************/
/* (c) Hightech Payment Systems 1998                                         */
/*     Database encrypted password generation                                */
/*     This object generates from a plain 8 characters password an encrypted */
/*     password on 16 Hex caracters for batch loging program to database.    */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <define.h>


main()
{
	/*START MOU09032015:#Evolution #45090
	char    entree1[17];
	char    entree2[17];
	char    sortie1[17];
	*/
	char    entree1[32+1];
	char    entree2[32+1];
	char    sortie1[62+4+1];
	/*END MOU09032015:Evolution #45090*/
	
	char	entree3[17];
	/*START MOU09032015:#Evolution #45090char	cryptogram  [512];/ * AER28022013 Password encryption with 3DES method*/
	int     i;
	
	system("clear");
	
	memset(entree1, 0, sizeof(entree1));
	memset(entree2, 0, sizeof(entree2));
	memset(entree3, 0, sizeof(entree3));
	memset(sortie1, 0, sizeof(sortie1));

	
	/*START MOU09032015:#Evolution #45090memset(cryptogram,   0, sizeof(cryptogram));/ *AER28022013 Password encryption with 3DES method*/

	system("stty -echo");
	fprintf(stdout,"Enter database password    : ");
	fflush(stdin);
	scanf("%s",entree1);
	system("stty echo");
	fprintf(stdout,"\n");
   
	if (strlen(entree1) < 8) /*(strlen(entree1) != 8)*/
	{
		fprintf(stderr,"Password length must be between 8 and 32 caracteres\n");
		exit(1);
	}
	
	if (strlen(entree1) > 32 ) 
	{
		fprintf(stderr,"Password length must be between 8 and 32 caracteres\n");
		exit(1);
	}
	
	system("stty -echo");
	fprintf(stdout,"Re enter database password : ");
	fflush(stdin);
	scanf("%s",entree2);
	system("stty echo");
	fprintf(stdout,"\n");
	
	/*START MOU09032015:#Evolution #45090
	if (strlen(entree2) != 8) 
   {
	  fprintf(stderr,"Password length must be 8 caracteres\n");
	  exit(2);
   }
	 END MOU09032015:Evolution #45090 */
	
	
	if (memcmp(entree1, entree2, strlen(entree1)) != 0)
	{
		fprintf(stderr,"Password mismatch\n");
		exit(3);
	}

	cipherPassword(entree1,sortie1);

/*
   for(i=0;i<8;i++)
      sprintf(entree3 + ( 2 * i), "%0X", *(entree1 + i));
   / * DES(1, 'H', entree3, KEY_PASS, sortie1); * /
   GetCryptogram(cryptogram);/*AER28022013 Password encryption with 3DES method* /
   fct_3DES(1,cryptogram,entree3,sortie1);/*AER28022013 Password encryption with 3DES method* /
   fprintf(stdout,"Database password          :  %.16s\n", sortie1);
   */
   
   exit(0);
   
}

