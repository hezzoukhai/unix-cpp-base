
/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_cis.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP CIS Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		03-03-2003   		Ismail hassani    				Initiale Version	
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

int  CloseCldCom()
{
	return(OK);
}


int ReadLineMsg(char *sBuffer, int *nLength)
{
	char	sBuffRcv[ LG_MAX ];
	char	sLine[MAX_LINE_TRC];
	int		nLenMsg, nRead;
	int		i;
	int		nFd = nFdConnect;
	
	trace_event("Start ReadLineMsg()",PROCESSING);

	/*** Lecture de La Longueuer du message ****/
	do
	{
		nRead = recv(nFd, sBuffRcv, LG_CTCP_HDR, MSG_PEEK);
	} while ( (nRead !=LG_CTCP_HDR) && (nRead > 0));

	if (nRead <= 0)
	{
		sprintf(sLine,"Error Reading Msg Length : %d nRead = %d", errno, nRead );
		trace_event(sLine,ERROR);
		return (NOK);
	}

	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + LG_CTCP_HDR;
	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	if (nRead < 0)
	{
		sprintf(sLine,"Error Reading Msg : %d(%d) ",nRead, errno);
		trace_event(sLine,ERROR);
		return (NOK);
	}
	if (nRead == 0)
	{
		sprintf(sLine,"Disconnect : %d ", errno);
		trace_event(sLine,ERROR);
		return (OR_DISCONECT);
	}
	memcpy( sBuffer, sBuffRcv + LG_CTCP_HDR, nLenMsg - LG_CTCP_HDR);
	*nLength = nLenMsg - LG_CTCP_HDR ;


	
	if (debug_option == 1)
		dump_buffer(sBuffRcv, nLenMsg, 'A', 'L', 'I');
	
	memset(sBuffRcv, 0, sizeof(sBuffRcv));
	sprintf(sLine,"End   ReadLineMsg(%d)", *nLength );
	trace_event(sLine,PROCESSING);
	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	char	sBuffSnd[ LG_MAX ];
	char	sLine[MAX_LINE_TRC];
	int retour;
	int i;
	int nFdC = nFdConnect;
	


	sprintf(sLine,"Start WriteLineMsg(%d)", nLength );
	trace_event(sLine,PROCESSING);

	sBuffSnd [ 0 ] = nLength / 256;
	sBuffSnd [ 1 ] = nLength % 256;

	memcpy (sBuffSnd + LG_CTCP_HDR, sBuffer, nLength);
	
	
	if (debug_option == 1)
		dump_buffer(sBuffSnd, nLength + LG_CTCP_HDR, 'E','L','O');
	

	
	retour = send ( nFdC, sBuffSnd, nLength + LG_CTCP_HDR, 0);
	if (retour <= 0)
	{
		sprintf(sLine,"Error Sending Msg : %d(%d) ",retour, errno);
		trace_event(sLine,ERROR);
		return(NOK);
	}

	memset (sBuffSnd, 0, sizeof(sBuffSnd));

	trace_event("End  WriteLineMsg(OK)",ERROR);
	return(OK);
}
