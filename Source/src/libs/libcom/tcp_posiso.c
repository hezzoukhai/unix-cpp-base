#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>




#include <define.h>
/*#include <srv_com.h>*/
/*#include <tcp_var.h>*/
/*#include <ascii.h>*/
#include <atmopcode.h>


#undef		MAX_WAIT_LINE
#define		MAX_WAIT_LINE  60

#define		PRE_HDR_LEN		7





BIO* get_bio(int nConnDesc);


int ReadLineMsgTLS( int nConnDesc, char *sBuffer, int *nLength)
{
	
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nRead;
   

   int	nTimerType;
   int	result;

   BIO *			conn_bio;

	sprintf(sLine,"Start ReadLineMsgTLS(%d)",nConnDesc);
	trace_event(sLine,PROCESSING);
	
	memset (sBuffRcv, 0, sizeof(sBuffRcv));
 
	conn_bio = get_bio(nConnDesc);

	result = tls_readcheck(nConnDesc,MAX_WAIT_LINE); 
	if ( result != OK ) 
	{ 
		sprintf(sLine,"tcp_readcheck result %d",result);
		trace_event(sLine, FATAL);
		return(result);
	}

	trace_event("End readcheck",PROCESSING);

	if(BIO_do_handshake(conn_bio) <= 0) 
	{
		sprintf(sLine, "Error in SSL handshake");
		trace_event(sLine,TRACE);
		return (NOK);
	}
	trace_event("Handshake Successful",PROCESSING);

	nRead = BIO_read(conn_bio,sBuffRcv,PRE_HDR_LEN); 
	if (nRead == 0)
	{
		sprintf(sLine,"End   ReadLineMsgTLS(OR_DISCONECT:1) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (NOK);
	}
	else if (nRead < 0)
	{
		sprintf(sLine,"End   ReadLineMsgTLS(NOK:1) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (-1);
	}
	if( memcmp(sBuffRcv,"ISO",3) != 0 )
	{
		sprintf(sLine,"End   ReadLineMsgTLS(NOK:1) Invalid buffer received [%s], nFdConn : %d",sBuffRcv,nConnDesc);
		trace_event(sLine,ERROR);
		return (-1);	
	}

	sBuffRcv[PRE_HDR_LEN]='\0';
    *nLength =    atoi(sBuffRcv+3);

	if (*nLength > LG_MAX)
	{
		sprintf(sLine , "End   ReadLineMsgTLS (ERROR) --- Msg too BIG : %d",*nLength);
		trace_event(sLine, FATAL);
		return NOK;
	}
	
	if (*nLength <= PRE_HDR_LEN)
	{
		sprintf(sLine , "End   ReadLineMsgTLS (ERROR) --- invalid length : %d",*nLength);
		trace_event(sLine, FATAL);
		return NOK;
	}
	
	nRead = BIO_read(conn_bio,sBuffRcv + PRE_HDR_LEN,*nLength - PRE_HDR_LEN);

	sprintf(sLine,"Read : %d ",nRead);
	trace_event(sLine,TRACE);

	if (nRead == 0)
	{
		sprintf(sLine,"End   ReadLineMsg(OR_DISCONECT) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (-1);
	}
	else if (nRead < 0)
	{
		sprintf(sLine,"End   ReadLineMsg(NOK) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return -1;
	}
       
	
	memcpy( sBuffer, sBuffRcv , *nLength);
	
	if( debug_option == 1 )
	{
   		dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');
	}



	memset (sBuffRcv, 0, sizeof(sBuffRcv)); 

	sprintf(sLine,"End ReadLineMsgSSL(OK) %d,%d",nConnDesc, *nLength);
	trace_event(sLine,PROCESSING);
         
	return OK;

}
/*****************************************************************************/
int WriteLineMsgTLS( int nConnDesc, char *sBuffer, int nLength)
{
	char	sLine[ MAX_LINE_TRC ];
	int		nRet;
	BIO *			conn_bio;
   
	
	sprintf(sLine,"Start WriteLineMsgSSL(%d)",nConnDesc);
	trace_event(sLine,PROCESSING);
	 
	conn_bio = get_bio(nConnDesc);


	if( debug_option == 1 )
	{
		dump_buffer(sBuffer, nLength, 'A', 'L', 'O');
	}

	nRet = BIO_write(conn_bio,sBuffer,nLength);
	BIO_flush(conn_bio);

	if (nRet <= 0)
	{
		sprintf(sLine,"End   WriteLineMsgSSL(NOK) nRet %d errno : %d , nConnDesc : %d",nRet,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (NOK);
	}

	/*sprintf(sLine,"End WriteLineMsgSSL(OK) fd:%d, Length:%d",tab_bio[nConnDesc].fd, nLength);*/
	sprintf(sLine,"End WriteLineMsgSSL(OK) fd:%d, Length:%d",nConnDesc, nLength);
	trace_event(sLine,PROCESSING);

	return OK;
}
/*****************************************************************************/


int ReadLineMsgTCP( int nFdConn, char *sBuffer, int *nLength)
{

	char  sLine[ MAX_LINE_TRC ];
	char	szLength[8];
	int   nLenMsg, nRead;
	int		retour;
	int		i;


	sprintf(sLine,"Start ReadLineMsgTCP(%d)",nFdConn);
	trace_event(sLine,PROCESSING);

	retour = tcp_readcheck(nFdConn,MAX_WAIT_LINE); 

	if ( retour != OK ) 
	{ 
		trace_event("Timeout in tcp_readcheck", FATAL);
		return(OR_DISCONECT);
	}

	do
	{
		nRead = recv(nFdConn, sBuffer, PRE_HDR_LEN, MSG_PEEK);
	} while ( (nRead !=PRE_HDR_LEN) && (nRead > 0));


	if( memcmp(sBuffer,"ISO",3) != 0 )
	{
		/* Incorrect data */
		trace_event("Incorrect data received from line(ISO)",ERROR);
		dump_buffer(sBuffer, PRE_HDR_LEN, 'A', 'X', 'X');
		return (NOK);
	}

	memcpy(szLength,sBuffer + 3,4);

	for(i=0; i < 4; i++ )
	{
		if( !isdigit(szLength[i]) )
		{
			/* Incorrect data */
			trace_event("Incorrect data received from line(LEN)",ERROR);
			dump_buffer(sBuffer, PRE_HDR_LEN, 'A', 'X', 'X');
			return (NOK);
		}
	}

	szLength[4]='\0';

	nLenMsg = atoi(szLength);
	nRead = recv(nFdConn, sBuffer, nLenMsg, MSG_WAITALL);
	if (nRead <= 0)
	{
		sprintf(sLine,"End   ReadLineMsgTCP(NOK) nRead %d errno : %d , nFdConn : %d",nRead,errno,nFdConn);
		trace_event(sLine,TRACE);
		return (NOK);
	}

	*nLength = nRead;

	if (debug_option == 1)
	{
   		dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');
	}

	sprintf(sLine,"End   ReadLineMsgTCP(OK)[%d]",*nLength);
	trace_event(sLine,TRACE);
	return (OK);
}

int WriteLineMsgTCP( int nFdConn, char *sBuffer, int nLength)
{

	char  sLine[ MAX_LINE_TRC ];
	int retour;


	sprintf (sLine, "Start WriteLineMsg(nFdConn=[%d])", nFdConn);
	trace_event(sLine,PROCESSING);

	retour = write ( nFdConn, sBuffer, nLength);
	if (retour <= 0)
	{
		sprintf (sLine, "Error Sending Msg : %d", errno);
		trace_event(sLine,ERROR);
		return(NOK);
	}

	if (debug_option == 1)
	{
   		dump_buffer(sBuffer, nLength, 'A', 'L', 'O');
	}

	trace_event("End   WriteLineMsg(OK)",TRACE);
	return(OK);
}

/*
int tcp_readcheck( int nFdConnect , int nTimer )
{
	int nResult ;
	fd_set rfds ;
	struct timeval tv;
	char  sLine[ MAX_LINE_TRC ];

	
	sprintf(sLine,"Start tcp_readcheck(%d)",nTimer);
	trace_event(sLine,PROCESSING);


	if( nFdConnect < 0 ) return -1 ;             
	if( nTimer <= 0 ) return -2 ;                

	FD_ZERO(&rfds); FD_SET(nFdConnect , &rfds) ; 

	tv.tv_sec = nTimer ;                         
	tv.tv_usec = 0 ;                        

	nResult = select(nFdConnect+1, &rfds, NULL, NULL, &tv ) ;  

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
*/


