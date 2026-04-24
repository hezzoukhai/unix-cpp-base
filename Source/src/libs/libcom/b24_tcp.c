/*#include <base24.h>*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <atmopcode.h>
#include <netinet/in.h> /*NAB08102020  PLUTONL-2288 */
#include <syslog.h> /*NAB08102020 */
#include <base24/base24_define.h>
#include <netdb.h> 
/******************************************************************************/

/******************************************************************************/
extern char RemoteAppName [];
extern char RemoteAppId [];


/******************************************************************************/
/*
int InitCltComms()
{
   return ( InitCltCom( RemoteAppName , RemoteAppId ) );
}
*/
/******************************************************************************/
int InitCltCom( char *sServerName, int nPort)
{

    int                          sock;
    struct hostent              *hp;
    struct sockaddr_in           sAddrServ;
    struct servent              *serveur;
    int                          nFd;

    trace_message(PROCESSING,"Start  InitCltCom(%.64s:%d)",sServerName, nPort);

    /* creation et attachement du socket en local                       */
    if ( (nFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        trace_message(ERROR,"Error on create socket: %d:%s",errno,strerror(errno));
        return NOK;
    }



    if ((hp=gethostbyname(sServerName)) == NULL)
    {
        trace_message(ERROR,"Error on gethostbyname: %d:%s",errno,strerror(errno));
        return NOK;
    }



    sAddrServ.sin_family = AF_INET;
    sAddrServ.sin_port   = htons(nPort);

    memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

    if (connect(nFd,(struct sockaddr*)&sAddrServ, sizeof(struct sockaddr)) == -1)
    {
		close(nFd);
        trace_message(ERROR,"Unable to connect: %d:%s",errno,strerror(errno));
        return NOK;
    }

	nFdConnect = nFd;
    trace_message(PROCESSING,"End  InitCltCom(OK): Fd: %d", nFd);
    return nFd;
}
/******************************************************************************/
void StopCltCom(int nFd)
{

    int  nResult;

    trace_message(PROCESSING,"Start  StopCltCom(%d)", nFd);

    if ((nResult = shutdown (  nFd  , 2 )) < 0)
    {
        trace_message(ERROR,"Shutdown Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }

    if ((nResult = close (  nFd )) < 0)
    {
        trace_message(ERROR,"Close Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }


    trace_event("End StopCltCom(OK)",PROCESSING);
}
/******************************************************************************/
int ReadLineMsg ( int nFd, char *sBuffer, int *nLength)
{
    char    sBuffRcv [ LG_MAX ];
	char    sDest[ LG_MAX ];
	int     nLenMsg;
	int		nEncrLen;
	int		nRead;


   trace_message(PROCESSING,"Start  ReadLineMsg(%d)", nFd);

    memset (sBuffRcv, 0, sizeof(sBuffRcv));
    memset (sDest, 0, sizeof(sDest));

    do
    {
        nRead = recv (nFd, sBuffRcv, B24_MSG_LEN_LEN , MSG_PEEK);
    }   while ((nRead != B24_MSG_LEN_LEN) && (nRead > 0));

    if (nRead < 0)
	{
		trace_message(ERROR,"recv_nok_hdr: errno: %u:%s",errno, strerror(errno));
		p7_syslog(LOG_ALERT,P7_ERR_0201);
		return NOK;
	}

    if (nRead == 0)
	{
		trace_message(ERROR,"disconnected: errno: %u:%s",errno, strerror(errno));
		p7_syslog(LOG_ALERT,P7_ERR_0201);
		return DISCONNECT;
	}

    nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] ;

	if (nLenMsg > LG_MAX)
	{
		trace_message(ERROR,"End ReadLineMsg (ERROR): Msg too BIG : %d",nLenMsg);
		p7_syslog(LOG_ALERT,P7_ERR_0201);
		return NOK;
	}

	nRead = recv (nFd, sBuffRcv, nLenMsg + B24_MSG_LEN_LEN, MSG_WAITALL);
    if (nRead < 0)
    {
		trace_message(ERROR,"End ReadLineMsg (ERROR): error while reading message data : %d:%s",errno,strerror(errno));
		p7_syslog(LOG_ALERT,P7_ERR_0201);
		return NOK;
	}
    if (nRead == 0)
    {
        trace_message(ERROR,"End ReadLineMsg (ERROR): disconnect while reading message data : %d:%s",errno,strerror(errno));
		p7_syslog(LOG_ALERT,P7_ERR_0201);
		return NOK;
    }

	if( debug_option == 1 )
		dump_buffer((unsigned char *) sBuffRcv, nLenMsg + B24_MSG_LEN_LEN, 'A', 'L', 'I');
	else
		EncryptBuffer(sBuffRcv,nLenMsg + B24_MSG_LEN_LEN,sDest,&nEncrLen,"ALI");

    memcpy ( sBuffer, sBuffRcv +  B24_MSG_LEN_LEN  , nLenMsg);
    *nLength = nLenMsg;

    trace_event("End  ReadLineMsg(OK)", PROCESSING);
    return OK;
}
/******************************************************************************/
int WriteLineMsg( int nFd, char *sBuffer, int nLength)
{

    char    sBuffSnd[ LG_MAX ];
    int     nResult;
	int		nEncrLen;
	char    sDest[ LG_MAX ]; /*MKB060309 PCI Norme Integration */


    trace_message(PROCESSING,"Start  WriteLineMsg(%d)", nFd);

    memset (sBuffSnd, '\0', sizeof(sBuffSnd));

    sBuffSnd [ 0 ] = (nLength) / 256;
    sBuffSnd [ 1 ] = (nLength) % 256;
    memcpy (sBuffSnd + B24_MSG_LEN_LEN , sBuffer, nLength);

	if (debug_option == 1)
		dump_buffer((unsigned char *) sBuffSnd, nLength + B24_MSG_LEN_LEN, 'A', 'L', 'O');
	else
		EncryptBuffer(sBuffSnd,nLength,sDest,&nEncrLen,"ALO");

    nResult = write ( nFd, sBuffSnd, nLength + B24_MSG_LEN_LEN);
    if (nResult <= 0)
    {
        trace_message(ERROR,"End WriteLineMsg (ERROR): write error : %d:%s",errno,strerror(errno));
		p7_syslog(LOG_ALERT,P7_ERR_0203);
		return NOK;
    }

    trace_event("End  WriteLineMsg(OK)", PROCESSING);
    return OK;
}
/******************************************************************************/
int AcceptCallSrv(char calling_address[16])
{
   int                long_inet;
   struct sockaddr_in adr_client;
   char               ligne[134];
   int                nFdConn;
   unsigned char      adr[4];
   int 				  offset;

   sprintf(ligne, "Start AcceptCallSrv()");
   trace_event(ligne, PROCESSING);

   long_inet = sizeof(struct sockaddr_in);
   nFdConn = accept(nFdLocal,
                       (struct sockaddr *)&adr_client,
                       &long_inet);
   if (nFdConn < 0 )
   {
      sprintf(ligne, "AcceptCall: ACCEPT ERROR : errno = %d \n", errno);
      trace_event(ligne, ERROR);
      return(NOK);
   }

	memset(adr, 0, sizeof(adr));

    adr[0]= adr_client.sin_addr.s_addr % 256;
    adr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
    adr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
    adr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

    sprintf(ligne,"CALL Ad In: %u.%u.%u.%u",
                adr[0], adr[1], adr[2], adr[3]);
     trace_event(ligne, PROCESSING);
     offset = 0;
     sprintf(calling_address + offset, "%03d", adr[3]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[2]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[1]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[0]); offset += 3;
   sprintf(ligne, "End   AcceptCallSrv(%d)", nFdConn);
   trace_event(ligne, PROCESSING);
   return(nFdConn);
}
/******************************************************************************/

int InitSrvComms(int nPort)
{
   char              ligne[134];
   char		     sLine[MAX_LINE_TRC];
   int               sDesc;
   struct sockaddr_in adr_local;
   int     Longueur = sizeof(struct sockaddr_in);
   int                       retour;
   struct servent            *serveur;
   int 		on;

   sprintf(sLine, "Start InitSrvCom(%d)", nPort);
   trace_event(sLine, PROCESSING);

/*****
   serveur = getservbyname(nom_service, "tcp");
   if (serveur == NULL)
   {
      sprintf(ligne, "conect_incoming Service name %.16s not found in /etc/services", nom_service);
      trace_event(ligne, ERROR);
      return(NOK);
   }
******/

   if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "InitSrvCom: SOCKET ERROR : errno = %d ", errno); 
      trace_event(ligne, ERROR);
      return(NOK);
   }

   adr_local.sin_family          = AF_INET;
   adr_local.sin_addr.s_addr     = htonl(INADDR_ANY);
   adr_local.sin_port            = htons(nPort);
   on=1 ;
   retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
   if(retour != 0)
   {
      sprintf( sLine,"Can not Use Socket Option SO_REUSEADDR %d|%d",retour,errno);
      trace_event(sLine,ERROR);
      sprintf( sLine,"End   open_device(NOK)");
      trace_event(sLine,TRACE);
      return(NOK);
   }
   retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
   if(retour != 0)
   {
      sprintf( sLine,"Can not Use Socket Option SO_REUSEPORT %d|%d",retour,errno);
      trace_event(sLine,ERROR);
      sprintf( sLine,"End   open_device(NOK)");
      trace_event(sLine,TRACE);
      return(NOK);
   }


   if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
   {
      sprintf(ligne, "BIND FCT ERROR : errno = %d", errno);
      trace_event(ligne, ERROR);
      close(sDesc);
      trace_event("End   InitSrvCom(NOK)", PROCESSING);
      return(NOK);
   }

   if(&adr_local != NULL)
      getsockname(sDesc, (struct sockaddr *)&adr_local, &Longueur);

   retour = listen(sDesc, 5);
   if ( retour == -1 )
   {
      sprintf(ligne, "LISTEN ERROR : errno = %d \n", errno);   
      trace_event(ligne, ERROR);
      trace_event("End  InitSrvCom(NOK)", PROCESSING);
      return(NOK);
   }

   nFdLocal = sDesc;
   trace_event("End   InitSrvCom(OK)", PROCESSING);
   return(OK);
}
