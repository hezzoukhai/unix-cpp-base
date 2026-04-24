/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_visa.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP VISA Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
/**
#include <line_com.h>
**/
#include <tcp_var.h>
#include <atmopcode.h>

void  GetFdConnect(char *sFdStr)
{
	sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
	nFdConnect = atoi(sFdStr);
}
int ReadLineMsg(char *sBuffer, int *nLength)
{
	char  sBuffRcv[ LG_MAX ];
	int   nLenMsg, nRead;
	int   i;
	int   nFd = nFdConnect;
	char  sLine [MAX_LINE_TRC];


	sprintf(sLine, "Start ReadLineMsg( nFd=[%d] )", nFd);
	trace_event(sLine, PROCESSING);

	/*** Lecture de La Longueuer du message ****/

	do
	{
		nRead = recv(nFd, sBuffRcv, LG_VTCP_HDR, MSG_PEEK);
	} while ( (nRead !=LG_VTCP_HDR) && (nRead > 0));

	if (nRead <= 0)
	{
		fprintf(stderr,"Error Reading Msg Length : %d nRead = %d \n", errno, nRead );
		sprintf(sLine, "END   ReadLineMsg( NOK. 1. ERROR recv errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (NOK);
	}

	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + LG_VTCP_HDR;

	
	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	if (nRead < 0)
	{
		fprintf(stderr,"Error Reading Msg : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( NOK. 2. ERROR recv errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (NOK);
	}
	if (nRead == 0)
	{
		fprintf(stderr,"Disconnect : %d \n", errno);
		return (OR_DISCONECT);
	}
	memcpy( sBuffer, sBuffRcv + LG_VTCP_HDR, nLenMsg - LG_VTCP_HDR);
	*nLength = nLenMsg - LG_VTCP_HDR;

	if (debug_option == 1)
		dump_buffer ( sBuffRcv, nLenMsg , 'E', 'L', 'I' );

	/*  Check Msg Length */
	if ( nRead <= 20)
	{
		sprintf ( sLine,"Error in Visa Msg Length, nRead = %d", nRead );
		trace_event(sLine,PROCESSING);
		return ( MALFUNCTION );
	}

	memset(sBuffRcv,0,sizeof(sBuffRcv)); /**PA DSS**/



	sprintf(sLine, "End   ReadLineMsg( %d )",*nLength);
	trace_event(sLine, PROCESSING);
	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	int retour;
	int i;
	int nFdC = nFdConnect;

	

	sBuffSnd [ 0 ] = nLength / 256;
	sBuffSnd [ 1 ] = nLength % 256;

	memcpy (sBuffSnd + LG_VTCP_HDR, sBuffer, nLength);
	
	if (debug_option == 1)
		dump_buffer ( sBuffSnd, nLength + LG_VTCP_HDR , 'E', 'L', 'O' );

	retour = send ( nFdC, sBuffSnd, nLength + LG_VTCP_HDR, 0);
	if (retour <= 0)
	{
		char sLine[MAX_LINE_TRC];
		sprintf ( sLine,"Error Sending Msg : %d", errno );
		trace_event(sLine,PROCESSING);
		return(NOK);
	}
	memset(sBuffSnd,0,sizeof(sBuffSnd));
	return(OK);
}

/*
int  CloseCldCom()
{
	return(OK);
}
*/

