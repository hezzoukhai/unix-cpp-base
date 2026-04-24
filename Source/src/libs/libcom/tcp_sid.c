#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>

#define SID_MSG_LEN_LENGTH		4
#define SID_MAX_MSG_LENGTH		2048
#define SID_EXCPT_TIME			10

BIO* get_bio(int nConnDesc);


int ReadLineMsgTLS(int nConnId, char *sBuffer, int *nLength)
{
	int				nLenMsg, nRead=0;
	char			sLine[MAX_LINE_TRC];
	int				i;
	
	BIO *			conn_bio;

	sprintf ( sLine,"Start ReadLineMsgTLS(%d)", nConnId );
	trace_event(sLine,PROCESSING);

	conn_bio = get_bio(nConnId);

	if(BIO_do_handshake(conn_bio) <= 0) 
	{
		sprintf(sLine, "Error in SSL handshake");
		trace_event(sLine,TRACE);
		return (NOK);
	}


	trace_event("Handshake Successful",PROCESSING);


	nRead = BIO_read(conn_bio,sBuffer,SID_MSG_LEN_LENGTH); 
	if (nRead <= 0)
	{
		sprintf(sLine, "END   ReadLineMsgTLS( NOK.errno=[%d][%d] )", errno ,nRead);
		trace_event(sLine, FATAL);
		return (NOK);
	}
	sBuffer[SID_MSG_LEN_LENGTH]='\0';

	*nLength = atoi(sBuffer);

	
	nRead = BIO_read(conn_bio,sBuffer + SID_MSG_LEN_LENGTH,*nLength);
	if (nRead < 0)
	{
		sprintf(sLine, "Error Reading Msg : %d|%d", errno,nRead);
		trace_event(sLine, PROCESSING);
		if ( errno == EINTR)
		{
			return (LINE_INTR);
		}

		return(LINE_DISCONECT);
	}
	if (nRead == 0)
	{
		sprintf(sLine, "Disconnect : %d", errno);
		trace_event(sLine, PROCESSING);
		return (LINE_DISCONECT);
	}


	if (debug_option == 1)
		dump_buffer ( sBuffer, *nLength , 'A', 'L', 'I' );

	sprintf(sLine, "End   ReadLineMsgTLS(%d)", *nLength);
	trace_event(sLine, PROCESSING);

	return (OK);
}



int WriteLineMsgTLS(int nConnId, char *sBuffer, int nLength)
{
	int retour;
	int i;
	char  sLine   [ MAX_LINE_TRC ]; 
	
	BIO *			conn_bio;

	sprintf(sLine, "Start WriteLineMsgTLS(%d, %d)", nConnId, nLength);
	trace_event(sLine, PROCESSING);

	conn_bio = get_bio(nConnId);

	if (debug_option == 1)
		dump_buffer ( sBuffer, nLength , 'A', 'L', 'O' );


	retour = BIO_write(conn_bio,sBuffer,nLength);
	BIO_flush(conn_bio);
	if (retour <= 0)
	{
		sprintf(sLine, "Error Sending Msg retour, nFd, errno :(%d, %d, %d)", retour, nConnId, errno);
		trace_event(sLine, PROCESSING); 
		return(NOK);
	}

	sprintf ( sLine,"End   WriteLineMsgTLS(%d)", retour );
	trace_event(sLine,PROCESSING);
	return(OK);
}



int ReadLineMsgTCP(int nConnId, char *sBuffer, int *nLength)
{
	int   	nLenMsg, nRead=0;
	char   	sLine[MAX_LINE_TRC];
	char	sLengthStr[SID_MSG_LEN_LENGTH + 1];
	int   	i=0;
	int		nResult;
	int		nLocLen;

	sprintf ( sLine,"Start ReadLinedMsg(%d)", nConnId );
	trace_event(sLine,PROCESSING);

	memset(sLengthStr,'\0',sizeof(sLengthStr));


	nResult = TCP_ReadMsg(		nConnId,
								1,
								SID_MSG_LEN_LENGTH,
								SID_EXCPT_TIME,
								SID_EXCPT_TIME,
								sBuffer);
	if (nResult != OK)
	{
		sprintf(sLine, "End   ReadLineMsg(%d)", nResult);
		trace_event(sLine, PROCESSING);
		return (nResult);
	}

	memcpy(sLengthStr,sBuffer,SID_MSG_LEN_LENGTH);

	nLocLen = atoi(sLengthStr);

	if( nLocLen > SID_MAX_MSG_LENGTH || nLocLen == 0 )
	{
		sprintf(sLine, "END   ReadLineMsg( NOK.Inv msg[%d])", nLocLen );
		trace_event(sLine, FATAL);
		return (LINE_DISCONECT);
	}


	nResult = TCP_ReadMsg(		nConnId,
								0,
								nLocLen,
								SID_EXCPT_TIME,
								SID_EXCPT_TIME,
								sBuffer + SID_MSG_LEN_LENGTH );
	if (nResult != OK)
	{
		sprintf(sLine, "End   ReadLineMsg(%d)", nResult);
		trace_event(sLine, PROCESSING);
	}


	*nLength = nLocLen + SID_MSG_LEN_LENGTH;


	if (debug_option == 1)
		dump_buffer ( sBuffer, *nLength , 'A', 'L', 'I' );

	sprintf(sLine, "End   ReadLineMsg(%d)", *nLength);
	trace_event(sLine, PROCESSING);

	return (OK);
}

int WriteLineMsgTCP(int nConnId, char *sBuffer, int nLength)
{
	int retour;
	int i;
	char  sLine   [ MAX_LINE_TRC ]; 


	sprintf(sLine, "Start WriteLineMsg(%d, %d)", nConnId, nLength);
	trace_event(sLine, PROCESSING);



	if (debug_option == 1)
		dump_buffer ( sBuffer, nLength , 'A', 'L', 'O' );


	retour = send ( nConnId, sBuffer, nLength , 0);
	if (retour <= 0)
	{
		sprintf(sLine, "Error Sending Msg retour, nFd, errno :(%d, %d, %d)", retour, nConnId, errno);
		trace_event(sLine, PROCESSING); 
		return(NOK);
	}

	sprintf ( sLine,"End   WriteLineMsg(%d)", retour );
	trace_event(sLine,PROCESSING);
	return(OK);
}




