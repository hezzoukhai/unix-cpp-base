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
	char  sLine [ MAX_LINE_TRC ];


	sprintf(sLine, "START ReadLineMsg( nFd=[%d] )", nFd);
	trace_event(sLine, PROCESSING);

	/*** Lecture de La Longueuer du message ****/
	do
	{
		nRead = recv(nFd, sBuffRcv, LG_MTCP_HDR, MSG_PEEK);
	} while ( (nRead !=LG_MTCP_HDR) && (nRead > 0));

	if (nRead <= 0)
	{
		fprintf(stderr,"Error Reading Msg Length : %d nRead = %d \n", errno, nRead );

		sprintf(sLine, "END   ReadLineMsg( NOK. 1. errno=[%d] )", errno);
		trace_event(sLine, FATAL);

		return (NOK);
	}

	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + LG_MTCP_HDR;

	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	if (nRead < 0)
	{
		fprintf(stderr,"Error Reading Msg : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( NOK. 2. errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (NOK);
	}
	if (nRead == 0)
	{
		fprintf(stderr,"Disconnect : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( OR_DISCONECT. 3. errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (OR_DISCONECT);
	}
	memcpy( sBuffer, sBuffRcv + LG_MTCP_HDR, nLenMsg - LG_MTCP_HDR);
	*nLength = nLenMsg;
	
	

	
	if (debug_option == 1)
		dump_buffer(sBuffer, *nLength, 'E', 'L', 'I');
	
	/*ACHPCI*/
	memset(sBuffRcv, 0, sizeof(sBuffRcv));

	sprintf(sLine, "END   ReadLineMsg( %d )",*nLength);
	trace_event(sLine, PROCESSING);
	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	int retour;
	int i;
	int nFdC = nFdConnect;
	char  sLine [ MAX_LINE_TRC ];


	sprintf(sLine, "START WriteLineMsg( nFdC=[%d] )", nFdC);
	trace_event(sLine, PROCESSING);

	memset (sBuffSnd, 0, sizeof(sBuffSnd));


	sBuffSnd [ 0 ] = (unsigned char) (nLength / 256);
	sBuffSnd [ 1 ] = (unsigned char) (nLength % 256);

	memcpy (sBuffSnd + LG_MTCP_HDR, sBuffer, nLength);


	if (debug_option == 1)
		dump_buffer(sBuffSnd, nLength + LG_MTCP_HDR, 'E', 'L', 'O');

	retour = send ( nFdC, sBuffSnd, nLength + LG_MTCP_HDR, 0);
	if (retour <= 0)
	{
		fprintf(stderr,"Error Sending Msg : %d \n", errno);
		sprintf(sLine, "END   WriteLineMsg( NOK. errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return(NOK);
	}

	memset(sBuffSnd, 0, sizeof(sBuffSnd));

	sprintf(sLine, "END   WriteLineMsg( OK )");
	trace_event(sLine, FATAL);
	return(OK);
}

int  CloseCldCom()
{
	return(OK);
}
