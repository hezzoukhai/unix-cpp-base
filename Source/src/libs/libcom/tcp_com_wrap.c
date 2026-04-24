/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_com.c                                                              */
/*                                                                             */
/* Description                                                                 */
/*      TCP-IP Tools Management                                                */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
0002NG             15-07-2003          Nabil   Gamrane               Modification de l'addresse IP 
dans les fcts : AcceptCallSrv et AcceptCall
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <fcntl.h>
#include <define.h>
#include <tcp_var.h>
#include <globals.h>


#define CON_MODE_TCP	0
#define CON_MODE_TLS	1



extern char RemoteAppName [];
extern char RemoteAppId [];

static int nConMode = CON_MODE_TCP;

void	InitTLSCom(char* szCertificatePath, char* szPrivateKeyPath, char* szCACertificatePath, int nVerifClt);

int	InitSSL(char* szCertificatePath, char* szPrivateKeyPath, char* szCACertificatePath, int nVerifClt)
{
	trace_event ("Start  InitSSL()", PROCESSING);
	nConMode = CON_MODE_TLS;

#ifndef P7_COMM_NO_TLS
	InitTLSCom(szCertificatePath,szPrivateKeyPath,szCACertificatePath,nVerifClt);
#else
	trace_event("TLS not defined ", ERROR);
	return NOK;
#endif

	trace_event ("End  InitSSL()", PROCESSING);
	return OK;
}

int InitCltCom(char *sServerName, char *sPort)
{
	char                         ligne[134];
	int                          sock;
	struct hostent               *hp;
	struct sockaddr_in           sAddrServ;
	struct servent               *serveur;
	unsigned 					nPort;




	nPort = atoi ( sPort );

	sprintf(ligne, "Start InitCltCom(%s, %d)", sServerName, nPort);
	trace_event(ligne,  PROCESSING);

	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		return ( InitCltTLSCom( sServerName , sPort ) );
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}

	/* creation et attachement du socket en local                       */
	if ( (nFdConnect = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
		trace_event(ligne, ERROR);
		trace_event("End   InitCltCom(NOK)", PROCESSING);
		return(-1);
	}

	if ((hp=gethostbyname(sServerName)) == NULL)
	{
		sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
		trace_event(ligne, ERROR);
		trace_event("End   InitCltCom(NOK)", PROCESSING);
		return(-1);
	}



	sAddrServ.sin_family = AF_INET;
#ifdef INTEL
	sAddrServ.sin_port   = serveur->s_port;
#else
	sAddrServ.sin_port   = htons(nPort);
#endif
	memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

	if (connect(nFdConnect,(struct sockaddr*)&sAddrServ,
		sizeof(struct sockaddr)) == -1)
	{
		sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
		trace_event(ligne, ERROR);
		trace_event("End   IniCltCom(NOK)", PROCESSING);
		return(-1);
	}

	trace_event("End   IniCltCom(OK)", PROCESSING);
	/*return (OK);*/
	return nFdConnect;  /*SKA20221111 PLUTONL-5394*/
}

int InitSrvComms(int nPort)
{

	char		     sLine[MAX_LINE_TRC];
	int               sDesc;
	struct sockaddr_in adr_local;
	unsigned int     Longueur = sizeof(struct sockaddr_in);
	int                       retour;
	struct servent            *serveur;
	int 		on;

	sprintf(sLine, "Start InitSrvComms(%d)", nPort);
	trace_event(sLine, PROCESSING);

	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		return (InitSrvTLSComms(nPort));
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}



	if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		sprintf(sLine, "InitSrvComms: SOCKET ERROR : errno = %d ", errno); 
		trace_event(sLine, ERROR);
		return(NOK);
	}

	adr_local.sin_family          = AF_INET;
	adr_local.sin_addr.s_addr     = htonl(INADDR_ANY);
	adr_local.sin_port            = htons(nPort);
	on=1 ;
	retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
	/* retour = setsockopt (sDesc, IPPROTO_TCP , SO_REUSEADDR, &on,sizeof(on)); */
	if(retour != 0)
	{
		sprintf( sLine,"Can not Use Socket Option SO_REUSEADDR %d|%d",retour,errno);
		trace_event(sLine,TRACE);
		sprintf( sLine,"End   open_device(NOK)");
		trace_event(sLine,TRACE);
		return(NOK);
	}


	if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
	{
		sprintf(sLine, "BIND FCT ERROR : errno = %d", errno);
		trace_event(sLine, ERROR);
		close(sDesc);
		trace_event("End   InitSrvComms(NOK)", PROCESSING);
		return(NOK);
	}

	if(&adr_local != NULL)
		getsockname(sDesc, (struct sockaddr *)&adr_local, &Longueur);

	retour = listen(sDesc, 5);
	if ( retour == -1 )
	{
		sprintf(sLine, "LISTEN ERROR : errno = %d", errno);   
		trace_event(sLine, ERROR);
		trace_event("End  InitSrvComms(NOK)", PROCESSING);
		return(NOK);
	}

	nFdLocal = sDesc;
	trace_event("End   InitSrvComms(OK)", PROCESSING);
	return(OK);
}

int AcceptCallSrv(char calling_address[16])
{
	int                long_inet;
	struct sockaddr_in adr_client;
	char               ligne[134];
	int                nFdConn;


	sprintf(ligne, "Start AcceptCallSrv()");
	trace_event(ligne, PROCESSING);

	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		return (AcceptTLSCallSrv(calling_address));
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}

	long_inet = sizeof(struct sockaddr_in);
	nFdConn = accept(nFdLocal,
		(struct sockaddr *)&adr_client,
		&long_inet);
	if (nFdConn < 0 )
	{
		sprintf(ligne, "AcceptCall: ACCEPT ERROR : errno = %d", errno);
		trace_event(ligne, ERROR);
		return(NOK);
	}

	inet_ntop( AF_INET, &adr_client.sin_addr.s_addr, calling_address, INET_ADDRSTRLEN );
	sprintf(ligne,"CALL Ad In: [%s]", calling_address);
	trace_event(ligne, PROCESSING);


	sprintf(ligne, "End   AcceptCallSrv(%d)", nFdConn);
	trace_event(ligne, PROCESSING);
	return(nFdConn);
}

void StopCltCom(int nFd)
{
	char		sLine[MAX_LINE_TRC];	
	trace_event ( "Stop Communication...Begin", PROCESSING );
	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		StopCltTLSCom(nFd);
#else
		trace_event("SSL not defined ", ERROR);
#endif

		return;
	}

	shutdown (  nFd  , 2 );
	if (close(nFd) == NOK) {
		sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
		trace_event(sLine, ERROR);
	}
	trace_event ( "Stop Communication...End", PROCESSING );
}





int ReadLineMsg(int nConnId, char *sBuffer, int *nLength)
{
	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		return ReadLineMsgTLS(nConnId,sBuffer,nLength);
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}

	return ReadLineMsgTCP(nConnId,sBuffer,nLength);
}

int WriteLineMsg(int nConnId,char *sBuffer, int nLength)
{
	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		return WriteLineMsgTLS(nConnId,sBuffer,nLength);
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}

	return WriteLineMsgTCP(nConnId,sBuffer,nLength);
}

int TcpReadcheck( int nFdConnect , int nTimer )
{
	if (nConMode == CON_MODE_TLS)
	{
#ifndef P7_COMM_NO_TLS
		 return tls_readcheck(nFdConnect,nTimer);
#else
		trace_event("SSL not defined ", ERROR);
		return NOK;
#endif
	}

	return tcp_readcheck(nFdConnect,nTimer);
}


/* Return codes:
1) LINE_INTR: If the waiting has been interruped by a signal.
	-> The caller should do some work and call tcp_readcheck again.
2) LINE_TIMEOUT: If the timer expires
	-> The caller should do some work and call tcp_readcheck again.
3) NOK: if an error occured
	-> should not happen, the caller should disconnect and reconnects
4) OK: if data is available or end party disconnects.
	-> The caller should try to read from the line in a non-blocking mode, either data is received or we get to know the socket is closed.
*/

int tcp_readcheck( int nFdConnect , int nTimer )
{
	int nResult ;
	fd_set rfds ;
	struct timeval tv;
	char  sLine[ MAX_LINE_TRC ];

	sprintf(sLine,"Start tcp_readcheck(): [Fd:%d,Timer:%d]",nFdConnect,nTimer);
	trace_event(sLine,PROCESSING);


	if( nFdConnect < 0 ) return -1 ;             /* bad socket id */
	if( nTimer <= 0 ) return -2 ;                /* bad timer */

	FD_ZERO(&rfds); FD_SET(nFdConnect , &rfds) ; /* check only sd */

	tv.tv_sec = nTimer ;                         /* set timer */
	tv.tv_usec = 0 ;                         /* set timer */

	nResult = select(nFdConnect+1, &rfds, 0, 0, &tv ) ;  /* check it */
	if( nResult == -1 )
	{

		if( errno == EINTR )
		{
			trace_event ("tcp_readcheck: Signal caught" , PROCESSING); 
			return LINE_INTR;
		}

		trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING); 
		return (NOK);
	}

	if( nResult == 0 )
	{
		trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING); 
		return (LINE_TIMEOUT);
	}
	return (OK);
}

/*
* Return codes:
* LINE_DISCONECT: sockets is closed
* LINE_INTR: read has been interrupted by a signal
* OK: data received.
*/
/* TODO additional option: nMaxLength, nWaitForLengh, received length*/
int TCP_ReadMsg (	int  nConnId,
					int  nWaitMsgOpt,					/* indicates if the read request is a blocking call */
					int  nMsgLength,
					int	 nTimer,						/* time to wait if the read call is non blocking*/
					int  nExceptionTimer,				/* time to wait once we received at a least a byte */
					char *sBuffer)
{
	int 	nRead = 0;
	int 	i;
	int		nResult;
	char   	sLine[MAX_LINE_TRC];
	int		flags;
	int		nOrigFlags;
	int		nLocTimer;


	sprintf ( sLine,"Start TCP_ReadMsg(Conn: %d,Wait:%d,Len:%d,Time:%d)", nConnId,nWaitMsgOpt,nMsgLength,nTimer );
	trace_event(sLine,PROCESSING);

	i=0;

	if ( (flags = fcntl(nConnId, F_GETFL, 0)) == -1 )
		flags = 0;

	nOrigFlags = flags;



	
	/*the following call is a blocking one*/
	if( nWaitMsgOpt > 0)
	{
		/*set socket as blocking */
		flags = flags & (~O_NONBLOCK);
		nResult = fcntl(nConnId, F_SETFL, flags);

		nRead = recv(nConnId, sBuffer, 1, 0);
		if( nRead == 0 )
		{
			sprintf(sLine, "P7_ERROR: 1: TCP_ReadMsg( [%d][%d][%d] )", errno ,nRead,i);
			trace_event(sLine, FATAL);
			nResult = fcntl(nConnId, F_SETFL, nOrigFlags);
			return LINE_DISCONECT;
		}
		if( nRead < 0 )
		{
			sprintf(sLine, "P7_ERROR: 1: TCP_ReadMsg( [%d][%d][%d] )", errno ,nRead,i);
			trace_event(sLine, FATAL);
			nResult = fcntl(nConnId, F_SETFL, nOrigFlags);
			if( errno == EINTR )
			{
				return LINE_INTR;
			}
			/*an unexpected error, return disconnect to renew the socket */
			return LINE_DISCONECT;
		}

		i= nRead;
	}

	/*set the socket as non-blocking*/
	nResult = fcntl(nConnId, F_SETFL, nOrigFlags | O_NONBLOCK);

	/*
	* Fix: use tcp_readcheck and while loop instead of MSG_PEEK&MSG_WAITALL. suppose we receive a byte or 2 and then
	* the other peer crashs/get killed. our connections stays in close_wait and we do not detects the disconnection because
	* we have some data to consume on the current line. we should not keep this data in the system in order for the disconnection
	* to be detected
	*/
	do
	{
		if( i > 0)
		{
			nLocTimer = nExceptionTimer;
		}
		else
		{
			nLocTimer = nTimer;
		}

		/*we have a received a single byte, we expect to receive the remaining by the following N seconds,
		* if nothing is received, this means something wrong with the line. let's renew the connection
		*/
		nResult = tcp_readcheck(nConnId,nLocTimer);
		if( nResult != OK)
		{
			if( nResult == LINE_INTR )
			{
				/*we have been interrupted in the middle of our wait. If we did'nt yet consume anything we may return,
				* if we already consumed then let's wait for the remaining time
				*/
				if( i == 0 )
				{
					nResult = fcntl(nConnId, F_SETFL, nOrigFlags);
					return LINE_INTR;
				}
				continue;
			}
			if( nResult == LINE_TIMEOUT )
			{
				break;
			}

			/* for all other return codes, we are not sure if socket is still valid, data received or end party disappeared,...etc
			the following recv will tell for sure
			*/
			/*break;*/
		}



		nRead = recv(nConnId, sBuffer  + i, nMsgLength - i, 0);
		if( nRead > 0 )
		{
			i+=nRead;
		}
		else if( nRead == 0 )
		{
			sprintf(sLine, "P7_ERROR: 2: TCP_ReadMsg( [%d][%d][%d] )", errno ,nRead,i);
			trace_event(sLine, FATAL);
			break;
		}
		else
		{
			sprintf(sLine, "P7_ERROR: 2: TCP_ReadMsg( [%d][%d][%d] )", errno ,nRead,i);
			trace_event(sLine, FATAL);
			if( nRead < 0 )
			{
				if( errno == EINTR )
				{
					if( i == 0 )
					{
						/* see previous comment */
						nResult = fcntl(nConnId, F_SETFL, nOrigFlags);
						return LINE_INTR;
					}
					continue;
				}
				break;
			}
		}
	} while ( (nRead > 0) && i < nMsgLength);



	nResult = fcntl(nConnId, F_SETFL, nOrigFlags);
	if ( i < nMsgLength )
	{
		sprintf(sLine, "END   TCP_ReadMsg( NOK.errno=[%d][%d][%d] )", errno ,nRead,i);
		trace_event(sLine, FATAL);
		return (LINE_DISCONECT);
	}

	
	sprintf(sLine, "End   TCP_ReadMsg(%d)", nMsgLength);
	trace_event(sLine, PROCESSING);

	return OK;
}


