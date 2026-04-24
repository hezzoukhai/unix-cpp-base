/*******************************************************************************/
/*  (c) Hightech Payment Systems 2003                                          */
/* Module                                                                      */
/*      tcp_b24.c                                                              */
/*                                                                             */
/* Description                                                                 */
/* TCP API for Communication with BASE24 NETWORK                               */
/*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <errno.h>

#include <define.h>
#include <dump.h>

#include <tcp_var.h>
#include <bic_tcp.h>

#define  LG_4  4

#define BIC_USE_EOT			0

/**************************************************************************************/
void  GetFdConnect(char *sFdStr)
{
	sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
	nFdConnect = atoi(sFdStr);
}

/**************************************************************************************/
int ReadLineMsg(char *sBuffer, int *nLength)
{

	int   nLenMsg, nRead;
	int   i;
	int   nFd = nFdConnect;
	char  sLine [ MAX_LINE_TRC ];
	char  sBuffRcv[ LG_MAX ];
	int		nEoT;


	nEoT = 0;
#if BIC_USE_EOT
	nEoT = 1;
#endif

	memset(sBuffRcv, 0,sizeof(sBuffRcv));

	sprintf(sLine, "Start ReadLineMsg( nFd=[%d] )", nFd);
	trace_event(sLine, PROCESSING);


	do
	{
		nRead = recv(nFd, sBuffRcv, LG_4 + 3, MSG_PEEK);
	} while ( (nRead !=LG_4+3) && (nRead > 0));
	

	if (nRead <= 0)
	{
		fprintf(stderr,"Error Reading Msg Length : %d nRead = %d \n", errno, nRead );
		sprintf(sLine, "END   ReadLineMsg( 0.NOK. errno=[%d] nRead = %d)", errno, nRead);
		trace_event(sLine, FATAL);
        if ( errno == EINTR)
		{
			return (LINE_INTR);
		}
		return (NOK);
	}

	nLenMsg = ((unsigned char)sBuffRcv[0] * 256)  +  (unsigned char)sBuffRcv[1] + LG_4 ;


	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	if (nRead < 0)
	{
		fprintf(stderr,"Error Reading Msg : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( 1.NOK. errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (NOK);
	}

	if (nRead == 0)
	{
		fprintf(stderr,"Disconnect : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( 2.OR_DISCONECT. errno=[%d] )", errno);
		trace_event(sLine, FATAL);
		return (OR_DISCONECT);
	}
	if (nRead > LG_MAX )
	{
		fprintf(stderr,"BIG buffer : %d \n", errno);
		sprintf(sLine, "END   ReadLineMsg( Big buffer errno=[%d][%d] )", nRead,errno);
		trace_event(sLine, FATAL);
		return (NOK);
	}

	memcpy( sBuffer, sBuffRcv + LG_4 , nLenMsg - LG_4 - nEoT);
	*nLength = nLenMsg - LG_4 - nEoT ;
	
	if( debug_option == 1 )
	{
		dump_buffer((unsigned char *) sBuffRcv, nLenMsg, 'A', 'L','I');
	}

	sprintf(sLine, "End   ReadLineMsg( OK ) (nRead:%d)",nRead);
	trace_event(sLine, PROCESSING);
	return (OK);
}


/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
int WriteLineMsg(char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	char  sLine[ MAX_LINE_TRC ];
	int   result, nLenSnd;
	int   nFd = nFdConnect;

	int		nEoT;


	nEoT = 0;
#if BIC_USE_EOT
	nEoT = 1;
#endif


	sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFd, nLength);
	trace_event(sLine, PROCESSING);

	/* var init */
	memset (sBuffSnd, 0, sizeof(sBuffSnd));

	/* buffer to be sent */
	/*Why +2
	sBuffSnd[0]=  (nLength+2+1) / 256;
	sBuffSnd[1]=  (nLength+2+1) % 256;
	*/

    sBuffSnd[0]=  (nLength + nEoT) / 256;
    sBuffSnd[1]=  (nLength + nEoT) % 256;
     
	memcpy(sBuffSnd+LG_4, sBuffer, nLength);
	nLenSnd = nLength+LG_4;
#if BIC_USE_EOT
	memcpy(sBuffSnd+LG_4+nLength, "\x03", 1);
	nLenSnd++;
#endif

	
	if( debug_option == 1 )
	{
		dump_buffer((unsigned char*)sBuffSnd, nLenSnd, 'A', 'L', 'O');
	}
	
	if((result = send ( nFdConnect, sBuffSnd, nLenSnd, 0)) <= 0)
	{
		sprintf(sLine, "WriteLineMsg()-send() failed : %d", errno);
		trace_event(sLine, ERROR);
		trace_event("End   WriteLineMsg(NOK)", PROCESSING);
		return (NOK);
	}
	

	trace_event("End   WriteLineMsg(OK)", PROCESSING);
	return(OK);
}

