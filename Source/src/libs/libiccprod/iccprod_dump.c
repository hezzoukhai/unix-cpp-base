/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      iccprod_dump.c                                                         */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   25/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <iso_iccprod.h>
#include <tools.h>
#include <define.h>

/************************************************************************************************************/
/************************************************************************************************************/
void DumpIccBuffer (char *sIccData, int nIccLen)
{

int  j=0,i = 0;
char sWork[LG_MAX];

memset(sWork, 0, sizeof(sWork));
for (i = 0; i< nIccLen; i++)
{
	if ( !(i%30)){
		trace_event(sWork,TRACE);
		memset(sWork, 0, sizeof(sWork));
		j=0;
	}
	sprintf(sWork+j,"%02X ", sIccData[i]);
	j+=2;
}
if ( j>0 )
	trace_event(sWork,TRACE);

}
