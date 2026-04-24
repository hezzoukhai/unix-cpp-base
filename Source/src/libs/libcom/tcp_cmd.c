#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>


#include <define.h>
#include <srv_com.h>
#include <tcp_var.h>
#include <ascii.h>
#include <atmopcode.h>
#include <globals.h>

/*EBE140523 */
#undef MAX_WAIT_LINE
#define  MAX_WAIT_LINE  60

void  CloseCom(int nFdConn)
{
	shutdown(nFdConn, 2);
}

int ReadLineMsg( int nFdConn, char *sBuffer, int *nLength)
{
	char  sBuffRcv[ LG_MAX ];
	char  sLine[ MAX_LINE_TRC ];
	int   nLenMsg, nRead;
	int   nRetCode;
	int retour;


	trace_event("Start ReadLineMsg()",PROCESSING);

	memset (sBuffRcv, 0, sizeof(sBuffRcv));


	nRead = recv(nFdConn, sBuffRcv, LG_MAX, 0);

	if (nRead <= 0)
	{
		sprintf(sLine,"End   ReadLineMsg(NOK) nRead %d errno : %d , nFdConn : %d",nRead,errno,nFdConn);
		trace_event(sLine,TRACE);
		return (NOK);
	}

	memcpy( sBuffer, sBuffRcv , nRead );
	*nLength = nRead;

	if (debug_option == 1)
		dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');


	memset (sBuffRcv, 0, sizeof(sBuffRcv)); /*PADSS2020*/

	trace_event("End   ReadLineMsg(OK)",TRACE);
	return (OK);
}

int WriteLineMsg( int nFdConn, char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	char  sLine[ MAX_LINE_TRC ];
	int retour;


	sprintf (sLine, "Start WriteLineMsg(nFdConn=[%d])", nFdConn);
	trace_event(sLine,PROCESSING);

	if (debug_option == 1)
		dump_buffer(sBuffer, nLength, 'A', 'L', 'O');


	memcpy (sBuffSnd, sBuffer, nLength);
	retour = write ( nFdConn, sBuffSnd, nLength);
	if (retour <= 0)
	{
		fprintf(stderr,"Error Sending Msg : %d \n", errno);
		return(NOK);
	}

	memset (sBuffSnd, 0, sizeof(sBuffSnd)); /*PADSS2020*/

	trace_event("End   WriteLineMsg(OK)",TRACE);
	return(OK);
}


int tcp_readcheck( int nFdConnect , int nTimer )
{
	int nResult ;
	fd_set rfds ;
	struct timeval tv;
	char  sLine[ MAX_LINE_TRC ];

	memset(sLine,0,sizeof(sLine));
	sprintf(sLine,"Start tcp_readcheck(%d)",nTimer);
	trace_event(sLine,PROCESSING);


	if( nFdConnect < 0 ) return -1 ;             /* bad socket id */
	if( nTimer <= 0 ) return -2 ;                /* bad timer */

	FD_ZERO(&rfds); FD_SET(nFdConnect , &rfds) ; /* check only sd */

	tv.tv_sec = nTimer ;                         /* set timer */
	tv.tv_usec = 0 ;                         /* set timer */

	nResult = select(nFdConnect+1, &rfds, NULL, NULL, &tv ) ;  /* check it */

	if( nResult == -1 )
	{
		trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING); 
		return (NOK);
	}

	if( nResult == 0 )
	{
		trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING); 
		return (NOK);
	}
	return (OK);
}

int  CloseCldCom()
{
	return(OK);
}
