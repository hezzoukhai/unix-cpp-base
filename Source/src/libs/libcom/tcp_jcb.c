
/*******************************************************************************/
/*  (c) Hightech Payment Systems 2004                                          */
/* Module                                                                      */
/*      tcp_jcb.c                                                              */
/*                                                                             */
/* Description                                                                 */
/*      TCP JCB Tools Management                                               */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MAW    		26-08-2004   		M. Amine WAHBI    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <atmopcode.h>

#define   JCB_TCP_HDR_LEN     4

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

	memset (sBuffRcv, 0, sizeof(sBuffRcv));

	/*** Lecture de La Longueuer du message ****/
	sprintf(sLine, "START ReadLineMsg( nFd=[%d] )", nFd);
	trace_event(sLine, PROCESSING);

	do
	{
		nRead = recv(nFd, sBuffRcv, JCB_TCP_HDR_LEN, MSG_PEEK); 
	} while ( (nRead !=JCB_TCP_HDR_LEN) && (nRead > 0));


	if (nRead <= 0)
	{
		sprintf(sLine, "END ReadLineMsg( NOK .1 errno=[%d], nRead = %d)", errno, nRead);
		trace_event(sLine,  ERROR);
		return (NOK);
	}

	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + JCB_TCP_HDR_LEN;

	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL); 

	if (nRead < 0)
	{
		sprintf(sLine, "END ReadLineMsg( NOK .2 errno=[%d], nRead = %d)", errno, nRead);
		trace_event(sLine, PROCESSING);
		return (NOK);
	}

	if (nRead == 0)
	{
		sprintf(sLine, "END ReadLineMsg( OR_DISCONECT ) errno=[%d], nRead = %d)", errno, nRead);
		trace_event(sLine, ERROR);
		return (OR_DISCONECT);
	}

	memcpy( sBuffer, sBuffRcv + JCB_TCP_HDR_LEN, nLenMsg - JCB_TCP_HDR_LEN);
	*nLength = nLenMsg - JCB_TCP_HDR_LEN ;
	
	if (debug_option == 1)
		dump_buffer ( sBuffRcv, nLenMsg , 'E', 'L', 'I' );


	trace_event("END ReadLineMsg( OK )", PROCESSING);


	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	int retour;
	int i;
	int nFdC = nFdConnect;
	char  sLine [ MAX_LINE_TRC ];

	sprintf(sLine, "START WriteLineMsg( nFdC=[%d] Len:%d)", nFdC,nLength);
	trace_event(sLine, PROCESSING);


	memset (sBuffSnd, 0, sizeof(sBuffSnd));



	sBuffSnd [ 0 ] = (unsigned char)(nLength / 256);	/* AEA19082021 PLUTONL-3406 */
	sBuffSnd [ 1 ] = (unsigned char)(nLength % 256);	/* AEA19082021 PLUTONL-3406 */



	memcpy (sBuffSnd + JCB_TCP_HDR_LEN, sBuffer, nLength);
	
	if (debug_option == 1)
		dump_buffer ( sBuffSnd, nLength + JCB_TCP_HDR_LEN , 'E', 'L', 'O' );


	retour = send ( nFdC, sBuffSnd, nLength + JCB_TCP_HDR_LEN, 0);
	if (retour <= 0)
	{
		sprintf(sLine, "END   WriteLineMsg( NOK ): errno:%d", errno);
		trace_event(sLine, PROCESSING);
		return(NOK);
	}

	trace_event("END   WriteLineMsg( OK )", PROCESSING);
	return(OK);
}

int  CloseCldCom()
{
	return(OK);
}
