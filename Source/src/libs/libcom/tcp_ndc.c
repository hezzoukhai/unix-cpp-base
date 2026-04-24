/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_ndc.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP NDC Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	 Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		 18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>


#include <define.h>
#include <line_com.h>
#include <tcp_var.h>
#include <atmopcode.h>
#include <globals.h>  
#include <p7_trace.h>

#ifndef P7_COMM_NO_TLS
#include <openssl/bio.h>
#include <openssl/ssl.h>


BIO* get_bio(int nConnDesc);



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


int ReadLineMsgTLS(int nConnId, char *sBuffer, int *nLength)
{
	int				nLenMsg, nRead=0;
	char			sLine[MAX_LINE_TRC];
	int				i;

	BIO *			conn_bio;
	int				fd;

	fd = -999;

	sprintf ( sLine,"Start ReadLineMsgTLS(): [Index:%d]", nConnId);
	trace_event(sLine,PROCESSING);

	conn_bio = get_bio(nConnId);
	if( conn_bio == NULL )
	{
		trace_message(P7_TL_ERROR,"Connection BIO is null: [Index: %d]",nConnId);
		trace_message(P7_TL_INFO,"End ReadLineMsgTLS(NOK)");
	}

	/* not needed
	if(BIO_do_handshake(conn_bio) <= 0) 
	{
		sprintf(sLine, "Error in SSL handshake");
		trace_event(sLine,ERROR);
		return (NOK);
	}


	trace_event("Handshake Successful",PROCESSING);
	*/


	nRead = BIO_read(conn_bio,sBuffer,2); 
	if (nRead <= 0)
	{
		trace_message(P7_TL_ERROR,"Error on BIO_read: [errno:%d,nRead:%d,Index:%d]", errno ,nRead,nConnId);
		trace_message(P7_TL_INFO,"End ReadLineMsgTLS(NOK)");
		return (NOK);
	}
	sBuffer[2]='\0';

	nLenMsg = ((unsigned char)sBuffer[0] * 256) + (unsigned char)sBuffer[1] + 2 ;

	if (nLenMsg > LG_MAX -1 )
	{
		trace_message(P7_TL_ERROR,"Message too big, connection will be closed: [Length:%d,Index:%d]", nLenMsg,nConnId);
		trace_message(P7_TL_INFO,"End ReadLineMsgTLS(NOK)");
		return (OR_DISCONECT);
	}


	nRead = BIO_read(conn_bio,sBuffer,nLenMsg - 2);
	if (nRead <= 0)
	{
		if ( nRead < 0 && errno == EINTR)
		{
			trace_message(P7_TL_ERROR,"Error on BIO_read, Interruption: [errno:%d,nRead:%d,Index:%d]", errno ,nRead,nConnId);
			trace_message(P7_TL_INFO,"End ReadLineMsgTLS(NOK)");
			return (LINE_INTR);
		}
		trace_message(P7_TL_ERROR,"Error on BIO_read, connection will be closed: [errno:%d,nRead:%d,Index:%d]", errno ,nRead,nConnId);
		trace_message(P7_TL_INFO,"End ReadLineMsgTLS(NOK)");
		return(LINE_DISCONECT);
	}


	*nLength = nLenMsg -2;

	if (debug_option == 1)
	{
		dump_buffer ( sBuffer, *nLength , 'A', 'L', 'I' );
	}

	sprintf(sLine, "End   ReadLineMsgTLS(%d)", *nLength);
	trace_event(sLine, PROCESSING);
	return (OK);
}



int WriteLineMsgTLS(int nConnId, char *sBuffer, int nLength)
{
	int retour;
	int i;
	char  sLine   [ MAX_LINE_TRC ]; 
	char  sBuffSnd[ LG_MAX ];
	BIO *			conn_bio;

	sprintf(sLine, "Start WriteLineMsgTLS(%d, %d)", nConnId, nLength);
	trace_event(sLine, PROCESSING);

	conn_bio = get_bio(nConnId);

	sBuffSnd [ 0 ] = (nLength ) / 256;
	sBuffSnd [ 1 ] = (nLength ) % 256;
	memcpy (sBuffSnd +2 , sBuffer, nLength);

	if (debug_option == 1)
	{
		dump_buffer ( sBuffSnd, nLength + 2 , 'A', 'L', 'O' );
	}


	retour = BIO_write(conn_bio,sBuffSnd,nLength + 2);
	BIO_flush(conn_bio);
	if (retour <= 0)
	{
		trace_message(P7_TL_ERROR,"Error on BIO_write: [errno:%d,result:%d,Index:%d]", errno ,retour,nConnId);
		trace_message(P7_TL_INFO,"End WriteLineMsgTLS(NOK)");
		return(NOK);
	}

	sprintf ( sLine,"End   WriteLineMsgTLS(%d)", retour );
	trace_event(sLine,PROCESSING);
	return(OK);
}

#endif



int ReadLineMsgTCP(int nFd, char *sBuffer, int *nLength)
{
	char  sBuffRcv[ LG_MAX ];
	char sLine [MAX_LINE_TRC];
	int   nLenMsg =0, nRead=0;

	memset(sBuffRcv,0,sizeof(sBuffRcv));
	memset(sLine,0,sizeof(sLine));

	sprintf ( sLine,"Start ReadLineMsgTCP(Fd:%d)", nFd );
	trace_event(sLine,PROCESSING);


	do
	{
		nRead = recv(nFd, sBuffRcv, 2 , MSG_PEEK);
	} while ( (nRead !=2) && (nRead > 0));

	if (nRead < 0)
	{
		fprintf(stderr,"Error Reading MSG_PEEK : %d \n", errno);
		sprintf ( sLine,"Error Reading MSG_PEEK : %d", errno);
		trace_event(sLine,ERROR);
		return (OR_BAL);
	}
	if (nRead == 0)
	{
		fprintf(stderr,"Disconnect MSG_PEEK: %d \n", errno);
		sprintf ( sLine,"Disconnect MSG_PEEK: %d ", errno);
		trace_event(sLine,ERROR);
		return (OR_DISCONECT);
	}


	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ;
	memset(sBuffRcv,0,sizeof(sBuffRcv));

	if (nLenMsg > LG_MAX -1 )
	{
		sprintf ( sLine,"ReadLineMsg Msg Too Big %d)", nLenMsg );
		trace_event(sLine,ERROR);
		nRead = recv (nFd, sBuffRcv, LG_MAX -1 , 0);

		trace_event("ATM Will Be Disconnected",ERROR);
		return (OR_DISCONECT);
	}



	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);

	if (nRead < 0)
	{
		fprintf(stderr,"Error Reading Msg : %d \n", errno);
		sprintf ( sLine,"Error Reading Msg : %d ", errno);
		trace_event(sLine,ERROR);
		return (OR_BAL);
	}
	if (nRead == 0)
	{
		fprintf(stderr,"Disconnect : %d \n", errno);
		sprintf ( sLine,"Disconnect : %d ", errno);
		trace_event(sLine,ERROR);
		return (OR_DISCONECT);
	}

	

	memcpy( sBuffer, sBuffRcv +  2  , nLenMsg - 2);
	*nLength = nLenMsg -2;

	dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');


	memset(sBuffRcv, 0, sizeof(sBuffRcv));

	sprintf ( sLine,"End   ReadLineMsgTCP(Len:%d)", *nLength );
	trace_event(sLine,PROCESSING);

	return (OK);
}

int WriteLineMsgTCP(int nFd, char *sBuffer, int nLength)
{
	char  sBuffSnd[ LG_MAX ];
	char sLine [MAX_LINE_TRC];
	int retour;
	int i;


	sprintf ( sLine,"Start WriteLineMsgTCP(Fd:%d,Len:%d)",nFd, nLength );
	trace_event(sLine,PROCESSING);


	
	memset (sBuffSnd, 0, sizeof(sBuffSnd));

	dump_buffer(sBuffer, nLength, 'A', 'L', 'O');


	sBuffSnd [ 0 ] = (nLength ) / 256;
	sBuffSnd [ 1 ] = (nLength ) % 256;
	memcpy (sBuffSnd +2 , sBuffer, nLength);


	retour = write ( nFd, sBuffSnd, nLength+2);
	if (retour <= 0)
	{
		fprintf(stderr,"Error Sending Msg : %d \n", errno);
		memset (sBuffSnd, 0, sizeof(sBuffSnd)); 
		return(NOK);
	}
	memset (sBuffSnd, 0, sizeof(sBuffSnd)); 

	sprintf ( sLine,"End   WriteLineMsgTCP(Len:%d)",retour );
	trace_event(sLine,PROCESSING);

	return(OK);
}




