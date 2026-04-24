/*******************************************************************************/
/* (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/* trans_key.c                                                                 */
/* */
/* Description                                                                 */
/* Convert Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void 
convert_key(sortie, entree)
	char           *sortie;
	char           *entree;
{
	int             i;
	unsigned char   a;
	unsigned char   b;

	trace_event("Start convert_key()", PROCESSING);


	for (i = 0; i < 8; i++) {

		a = 0x00;
		b = 0x00;

		if (*(entree + 2 * i) == 0x41)
			a = 0xA0;
		else if (*(entree + 2 * i) == 0x42)
			a = 0xB0;
		else if (*(entree + 2 * i) == 0x43)
			a = 0xC0;
		else if (*(entree + 2 * i) == 0x44)
			a = 0xD0;
		else if (*(entree + 2 * i) == 0x45)
			a = 0xE0;
		else if (*(entree + 2 * i) == 0x46)
			a = 0xF0;
		else
			a = (unsigned char) (*(entree + 2 * i) & 0x0F) << 4;



		if (*(entree + 2 * i + 1) == 0x41)
			b = 0x0A;
		else if (*(entree + 2 * i + 1) == 0x42)
			b = 0x0B;
		else if (*(entree + 2 * i + 1) == 0x43)
			b = 0x0C;
		else if (*(entree + 2 * i + 1) == 0x44)
			b = 0x0D;
		else if (*(entree + 2 * i + 1) == 0x45)
			b = 0x0E;
		else if (*(entree + 2 * i + 1) == 0x46)
			b = 0x0F;
		else
			b = (unsigned char) (*(entree + 2 * i + 1) & 0x0F);

		*(sortie + i) = (char) (a | b);
	}

	trace_event("End   convert_key()", PROCESSING);

	return;
}
