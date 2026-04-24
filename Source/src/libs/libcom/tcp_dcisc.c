#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>


#define DCISC_MSG_LEN_LENGTH		2
#define DCISC_MAX_MSG_LENGTH		2048
#define DCISC_EXCPT_TIME			10



int ReadLineMsgTLS(int nConnId, char *sBuffer, int *nLength)
{
	char  sLine   [ MAX_LINE_TRC ]; 

	sprintf(sLine, "End   ReadLineMsgTLS(%d)", 0);
	trace_event(sLine, PROCESSING);

	return (OK);
}

int WriteLineMsgTLS(int nConnId, char *sBuffer, int nLength)
{
	char  sLine   [ MAX_LINE_TRC ]; 
	
	sprintf ( sLine,"End   WriteLineMsgTLS(%d)", 0 );
	trace_event(sLine,PROCESSING);
	return(OK);
}



int ReadLineMsgTCP(int nConnId, char *sBuffer, int *nLength)
{
	int   	nLenMsg, nRead=0;
	char   	sLine[MAX_LINE_TRC];
	int   	i=0;
	int		nResult;
	int		nLocLen;

	sprintf ( sLine,"Start ReadLinedMsg(%d)", nConnId );
	trace_event(sLine,PROCESSING);

	
	nResult = TCP_ReadMsg(		nConnId,
								1,
								DCISC_MSG_LEN_LENGTH,
								DCISC_EXCPT_TIME,
								DCISC_EXCPT_TIME,
								sBuffer);
	if (nResult != OK)
	{
		sprintf(sLine, "End   ReadLineMsg(%d)", nResult);
		trace_event(sLine, PROCESSING);
		return (nResult);
	}
	
	nLocLen = ((unsigned char)sBuffer[0] * 256) + (unsigned char)sBuffer[1] ;


	if( nLocLen > DCISC_MAX_MSG_LENGTH || nLocLen <= DCISC_MSG_LEN_LENGTH )
	{
		sprintf(sLine, "END   ReadLineMsg( NOK.Inv msg[%d])", nLocLen );
		trace_event(sLine, FATAL);
		return (LINE_DISCONECT);
	}
	

	nResult = TCP_ReadMsg(		nConnId,
								0,
								/*nLocLen - DCISC_MSG_LEN_LENGTH,*/
								nLocLen + 1, /* add 1 for ETX */
								DCISC_EXCPT_TIME,
								DCISC_EXCPT_TIME,
								sBuffer );
	if (nResult != OK)
	{
		sprintf(sLine, "End   ReadLineMsg(%d)", nResult);
		trace_event(sLine, PROCESSING);
	}

	/*nLength = nLocLen - DCISC_MSG_LEN_LENGTH;*/
	*nLength = nLocLen;

	if (debug_option == 1)
		dump_buffer ( sBuffer, *nLength , 'A', 'L', 'I' );

	sprintf(sLine, "End   ReadLineMsg(%d)", *nLength);
	trace_event(sLine, PROCESSING);

	return (OK);
}

int WriteLineMsgTCP(int nConnId, char *sBuffer, int nLength)
{
	char	sBuffSnd[ LG_MAX ];
	int retour;
	int i;
	char  sLine   [ MAX_LINE_TRC ]; 


	sprintf(sLine, "Start WriteLineMsg(%d, %d)", nConnId, nLength);
	trace_event(sLine, PROCESSING);



	if (debug_option == 1)
		dump_buffer ( sBuffer, nLength , 'A', 'L', 'O' );


	sBuffSnd [ 0 ] = (nLength+2+1) / 256;
	sBuffSnd [ 1 ] = (nLength+2+1) % 256;

	memcpy (sBuffSnd + DCISC_MSG_LEN_LENGTH, sBuffer, nLength);

	sBuffSnd [ nLength + DCISC_MSG_LEN_LENGTH ] = 0x03;

	
	retour = send ( nConnId, sBuffSnd, nLength + DCISC_MSG_LEN_LENGTH + 1, 0);
	if (retour <= 0)
	{
		sprintf(sLine, "END   WriteLineMsg( NOK ). errno=[%d]", errno);
		trace_event(sLine, ERROR);
		return(NOK);
	}


	sprintf ( sLine,"End   WriteLineMsg(%d)", retour );
	trace_event(sLine,PROCESSING);
	return(OK);
}

