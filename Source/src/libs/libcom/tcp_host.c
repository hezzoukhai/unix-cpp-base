/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_host.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP HOST Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <atmopcode.h>

#define LG_HOST_HDR 4

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
	char  sLine[ MAX_LINE_TRC ];
	char  sLenMsg[ 256 ];
	int   nLenMsg, nRead;
	int   i;
	int   nFd = nFdConnect;
	

	sprintf(sLine, "Start ReadLineMsg(%d)", nFd);
	trace_event(sLine, PROCESSING);

	memset(sBuffRcv, 0, sizeof(sBuffRcv));
	
	do
	{
		nRead = recv(nFd, sBuffRcv, LG_HOST_HDR, MSG_PEEK);
	} while ( (nRead !=LG_HOST_HDR) && (nRead > 0));

	if (nRead < 0)
	{
		sprintf(sLine, "Error Reading Msg Length : %d nRead = %d", errno, nRead);
		trace_event(sLine, ERROR);
		sprintf(sLine, "End   ReadLineMsg(NOK)");
		trace_event(sLine, PROCESSING);
		return (LINE_INTR);
	}
	if (nRead == 0)
	{
		sprintf(sLine, "Disconnect : %d", errno);
		trace_event(sLine, ERROR);
		sprintf(sLine, "End   ReadLineMsg(DISCONNECT)");
		trace_event(sLine, PROCESSING);
		return (LINE_DISCONECT);
	}

	memset(sLenMsg , '\0', sizeof(sLenMsg)); /*IBO20230208 PLUTONL-5868*/
	memcpy(sLenMsg, sBuffRcv,4);
	/* La longueur dans le header du message n'inclu pas la longueur de la longueur */
	nLenMsg = atoi(sLenMsg) + LG_HOST_HDR;

	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	if (nRead < 0)
	{
		sprintf(sLine, "Error Reading Msg : %d", errno);
		trace_event(sLine, ERROR);
		sprintf(sLine, "End   ReadLineMsg(NOK)");
		trace_event(sLine, PROCESSING);
		return (LINE_INTR);
	}
	if (nRead == 0)
	{
		sprintf(sLine, "Disconnect : %d", errno);
		trace_event(sLine, ERROR);
		sprintf(sLine, "End   ReadLineMsg(DISCONNECT)");
		trace_event(sLine, PROCESSING);
		return (OR_DISCONECT);
	}

	
	if (debug_option == 1)
		dump_buffer ( sBuffRcv, nLenMsg , 'A', 'L', 'I' );
	

	memcpy( sBuffer, sBuffRcv , nLenMsg );
	*nLength = nLenMsg;

	memset(sBuffRcv, 0, sizeof(sBuffRcv));

	sprintf(sLine, "End   ReadLineMsg(%d)", *nLength);
	trace_event(sLine, PROCESSING);

	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	char  sLine   [ MAX_LINE_TRC ];
	int retour;
	int i;
	int nFdC = nFdConnect;


	sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFdC, nLength);
	trace_event(sLine, PROCESSING);

	memcpy (sBuffSnd , sBuffer, nLength);


	if (debug_option == 1)
		dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );

	retour = send ( nFdC, sBuffSnd, nLength , 0);
	if (retour <= 0)
	{
		sprintf(sLine, "Err When Calling send() : %d", errno);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "End   WriteLineMsg(NOK)");
		trace_event(sLine, PROCESSING);
		return(NOK);
	}

	memset(sBuffSnd, 0, sizeof(sBuffSnd));
	
	sprintf(sLine, "End   WriteLineMsg(OK)");
	trace_event(sLine, PROCESSING);
	return(OK);
}

int  CloseCldCom()
{
	return(OK);
}



