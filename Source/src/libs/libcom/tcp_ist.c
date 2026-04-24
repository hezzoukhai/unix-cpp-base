/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity              Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                 Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/


#include <ist.h>


static int nFdLocal;

#define MAX_IST_MSG_LEN		2048

/***********************************************************************
**  name      : InitCltCom()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int InitCltCom(char *sServerName, char *sPort)
{
    struct hostent               *hp;
    struct sockaddr_in           sAddrServ;
    struct servent               *serveur;
    unsigned                     nPort = atoi(sPort);
	char                         sLine[MAX_LINE_TRC];
    int                          sock;
    int                          nFd;


    trace_event("Start   InitCltCom()", PROCESSING);
	
	memset(sLine, '\0',  sizeof (sLine ));

    /* creation et attachement du socket en local*/
    if ( (nFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
	   sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitCltCom(NOK)", PROCESSING);
       return (NOK);
    }


    if ((hp=gethostbyname(sServerName)) == NULL)
	{
	   sprintf(sLine, "sServerName : %s", sServerName);
	   trace_event(sLine, ERROR);
       trace_event("End   InitCltCom(NOK)", PROCESSING);
       return (NOK);
    }

    sAddrServ.sin_family = AF_INET;
	
    #ifdef INTEL
        sAddrServ.sin_port   = serveur->s_port;
    #else
        sAddrServ.sin_port   = htons(nPort);
    #endif
    
	memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

    if (connect(nFd,(struct sockaddr*)&sAddrServ, sizeof(struct sockaddr)) == -1)
    {
	   close(nFd);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitCltCom(NOK)", PROCESSING);
       return (NOK);
    }
    
	sprintf(sLine, "End   InitCltCom(%d)", nFd);
	trace_event(sLine, PROCESSING);
    return (nFd);
	
}


/***********************************************************************
**  name      : InitSrvComms()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int InitSrvComms(int nPort)
{
    char                      sLine[MAX_LINE_TRC];
    struct sockaddr_in        adr_local;
    struct servent            *serveur;
    struct linger             sLinger;
    int                       sDesc;
    int                       on;
    int                       nFd;
    int                       Longueur = sizeof(struct sockaddr_in);
    int                       retour;

    
	
	sprintf(sLine, "Start   InitSrvComms(%d)", nPort);
	trace_event(sLine, PROCESSING);
    
    memset(sLine, '\0',  sizeof (sLine ));
	
    if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
	   sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    }

    adr_local.sin_family          = AF_INET;
    adr_local.sin_addr.s_addr     = htonl(INADDR_ANY);
    adr_local.sin_port            = htons(nPort);
    on=1 ;

    if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on))) != 0)
    {
       close(sDesc);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    
    }

    if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEPORT, &on,sizeof(on))) != 0)
    {
       close(sDesc);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    
    }

    sLinger.l_onoff    = 0;  /* Wait for unsent data */
    sLinger.l_linger    = 5; /* Close socket after 5 sec even if there is unsent data */
    if((retour = setsockopt (sDesc, SOL_SOCKET, SO_LINGER, &sLinger,sizeof(sLinger))) != 0)
    {
       close(sDesc);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    
    }

    if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
    {
       close(sDesc);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    
    }

    if(&adr_local != NULL) 
	getsockname(sDesc, (struct sockaddr *)&adr_local, (int*)&Longueur);

    if ((retour = listen(sDesc, 1)) == -1)
    {
       close(sDesc);
       sprintf(sLine, "Error : %d", errno);
	   trace_event(sLine, ERROR);
       trace_event("End   InitSrvComms(NOK)", PROCESSING);
       return (NOK);
    }

    nFdLocal = sDesc;
    nFd      = sDesc;

    sprintf(sLine, "End   InitSrvComms(%d,%d)", nFdLocal,nFd);
	trace_event(sLine, PROCESSING);
    return (OK);
}

/***********************************************************************
**  name      : AcceptCall()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int AcceptCall(char  calling_address [])
{
    char               sLine[MAX_LINE_TRC];
    int                nStatus;

    sprintf(sLine, "Start   AcceptCall(%d)", nFdLocal);
	trace_event(sLine, PROCESSING);
	
	memset(sLine, '\0',  sizeof (sLine));

    if ((nStatus = AcceptCallSrv(calling_address)) < 0)
	{
	   sprintf(sLine, "End   AcceptCall(%d)", nStatus);
	   trace_event(sLine, PROCESSING);
       return (NOK);
	}
    sprintf(sLine, "End   AcceptCall(%d,%.20s)", nStatus,calling_address);
	trace_event(sLine, PROCESSING);
    return (OK);
}

/***********************************************************************
**  name      : AcceptCallSrv()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int AcceptCallSrv(char calling_address[])
{
    unsigned char          adr[4];
	struct sockaddr_in     adr_client;
    char                   sLine[MAX_LINE_TRC];
    int                    long_inet;
    int                    nFd;
    

    
	sprintf(sLine, "Start   AcceptCallSrv(%d)", nFdLocal);
	trace_event(sLine, PROCESSING);	

    memset(sLine, '\0', sizeof (sLine));
	memset(adr,     0 , sizeof(adr));

    long_inet = sizeof(struct sockaddr_in);
    
	nFd = accept(nFdLocal,(struct sockaddr *)&adr_client,(int*)&long_inet);
    if (nFd < 0 )
	{
	   sprintf(sLine, "Error(%d)", errno);
	   trace_event(sLine, PROCESSING);
	   trace_event("End   AcceptCallSrv(NOK)", PROCESSING);
       return (NOK);
	}
    
    adr[0]= adr_client.sin_addr.s_addr % 256;
    adr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
    adr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
    adr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

    sprintf(calling_address,"%.03u.%.03u.%.03u.%.03u", adr[3], adr[2], adr[1], adr[0]);

    sprintf(sLine, "End   AcceptCallSrv(%d,%.20s)", nFd,calling_address);
	trace_event(sLine, PROCESSING);
    return (nFd);
}

/***********************************************************************
**  name      : StopCltCom()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
void StopCltCom(int nFd)
{
    char  sLine[MAX_LINE_TRC];
    int   nRetCode;

    sprintf(sLine, "Start   StopCltCom(%d)", nFd);
	trace_event(sLine, PROCESSING);
	
	memset(sLine, '\0', sizeof (sLine));

    if ((nRetCode = shutdown (  nFd  , 2 )) < 0)
    {
        sprintf(sLine, "Shutdown Ret : %d, Errno : %d->%s",nRetCode, errno, strerror(errno));
        trace_event(sLine,TRACE);
    }

    if ((nRetCode = close (  nFd )) < 0)
    {
        sprintf(sLine,"Close    Ret : %d, Errno : %d->%s",nRetCode, errno, strerror(errno));
        trace_event(sLine,TRACE);
    }
    
    trace_event("End   StopCltCom()", PROCESSING);
    return;
}



/***********************************************************************
**  name      : ReadLineMsg()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int ReadLineMsg ( int nFd, char *sBuffer, int *nLength)
{
    char    sBuffRcv [ LG_MAX ];
    char    sLine [ MAX_LINE_TRC ];
    int     nLenMsg =0;
    int   	nRead=0;

    
	sprintf(sLine, "Start   ReadLineMsg(%d)", nFd);
	trace_event(sLine, PROCESSING);

    memset (sBuffRcv,  '\0', sizeof(sBuffRcv));
	memset (sLine,     '\0', sizeof (sLine));

    do
    {
        nRead = recv (nFd, sBuffRcv, 2 , MSG_PEEK);
    }   while ((nRead !=2) && (nRead > 0));

    if (nRead < 0)
	{
	  sprintf(sLine, "Error : (%d)", errno);
	  trace_event(sLine, ERROR);
      trace_event("End   ReadLineMsg(NOK)", PROCESSING);
      return (NOK);
	}   

    if (nRead == 0)
	{
	  sprintf(sLine, "Error : (%d)", errno);
	  trace_event(sLine, ERROR);
      trace_event("End   ReadLineMsg(NOK)", PROCESSING);
      return (DISCONNECT);
	}

	

    nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] ;

    memset (sBuffRcv, 0, sizeof(sBuffRcv));

    /*EBE150915 nRead = recv (nFd, sBuffRcv, nLenMsg, 0);*/
	nRead = recv (nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
	
    if (nRead < 0)
	{
	  sprintf(sLine, "Error : (%d)", errno);
	  trace_event(sLine, ERROR);
      trace_event("End   ReadLineMsg(NOK)", PROCESSING);
      return (NOK);
	} 	

    if (nRead == 0)
	{
	  sprintf(sLine, "Error : (%d)", errno);
	  trace_event(sLine, ERROR);
      trace_event("End   ReadLineMsg(NOK)", PROCESSING);
      return (NOK);
	} 	

	if( nRead > MAX_IST_MSG_LEN )
	{
		dump_buffer((unsigned char *) sBuffRcv, 32, 'A', 'L', 'I');
	}

	if( debug_option == 1 )
	{
		dump_buffer((unsigned char *) sBuffRcv, nLenMsg, 'A', 'L', 'I');
	}

    memcpy ( sBuffer, sBuffRcv +  2  , nLenMsg - 2);
    /*EBE150915 *nLength = nLenMsg;*/
	*nLength = nLenMsg- 2;

	trace_event("End   ReadLineMsg(OK)", PROCESSING);
    return (OK);
}


/***********************************************************************
**  name      : WriteLineMsg()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int WriteLineMsg( int nFd, char *sBuffer, int nLength)
{
    char szFuncName[] = "WriteLineMsg", sLine[MAX_LINE_TRC];
    char  sBuffSnd[ LG_MAX ];
    int retour;
    int i;

    sprintf(sLine, "Start   WriteLineMsg(%d,%d)", nFd, nLength);
	trace_event(sLine, PROCESSING);

    memset(sBuffSnd,  '\0', sizeof (sBuffSnd));
	memset(sLine,     '\0', sizeof (sLine));

    sBuffSnd [ 0 ] = (nLength ) / 256;
    sBuffSnd [ 1 ] = (nLength ) % 256;
    memcpy (sBuffSnd +2 , sBuffer, nLength);
	if( debug_option == 1 )
	{
		dump_buffer((unsigned char *) sBuffSnd, nLength+2, 'A', 'L', 'O');
	}
    
	retour = write ( nFd, sBuffSnd, nLength+2);
    if (retour <= 0)
    {
	  sprintf(sLine, "Error : (%d)", errno);
	  trace_event(sLine, ERROR);
      trace_event("End   WriteLineMsg(NOK)", PROCESSING);
      return (NOK);
	} 		
    
    trace_event("End   WriteLineMsg(OK)", PROCESSING);
    return (OK);
}

