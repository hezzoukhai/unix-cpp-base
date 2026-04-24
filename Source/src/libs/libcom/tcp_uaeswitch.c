/**
**  name      : uaeswitch_tcp.h
**  descr     :
**  author    : Y. OUADI
** (C) 1999-2005 : Hightech Payment Systems
*/
#include <uaeswitch.h>

/* ------------------------------------------------------------------------- */
/* --                        PRIVATE INTERFACE                            -- */
/* ------------------------------------------------------------------------- */
/* - Constantes Definition Section ----------------------------------------- */

/* - Macros Definition Section --------------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Modules Private Functions --------------------------------------------- */

/* - Modules Private Data -------------------------------------------------- */
/*static int nFdLocal;*/


/* ------------------------------------------------------------------------- */
/* --                   PUBLIC FUNCTIONS DEFINITION                       -- */
/* ------------------------------------------------------------------------- */
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
    char                         szFuncName[] = "InitCltCom", sLine[134];
    int                          sock;
    struct hostent              *hp;
    struct sockaddr_in           sAddrServ;
    struct servent              *serveur;
    unsigned                     nPort = atoi(sPort);
    int                          nFd;


    START_TRACE_PAR2(szFuncName, "%.64s, %.64s", sServerName, sPort);

    /* creation et attachement du socket en local                       */
    if ( (nFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        return_CALL_FAILURE_ERRNO(szFuncName, "socket", errno, NOK);


    if ((hp=gethostbyname(sServerName)) == NULL)
        return_CALL_FAILURE_STR_VAR(szFuncName, "gethostbyname",
            "unknown machine (check /etc/hosts)", sServerName, NOK);


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
        return_CALL_FAILURE_ERRNO(szFuncName, "connect", errno, NOK);
    }

    return_INT(szFuncName, nFd);
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
    char              szFuncName[] = "InitSrvComms", sLine[MAX_LINE_TRC];
    int               sDesc;
    struct sockaddr_in adr_local;
    int     Longueur = sizeof(struct sockaddr_in);
    int                       retour;
    struct servent            *serveur;
    int            on;
    int            nFd;

    struct  linger sLinger;

    START_TRACE_PAR1(szFuncName, "%d", nPort);


    if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        return_CALL_FAILURE_ERRNO(szFuncName, "socket", errno, NOK);

    adr_local.sin_family          = AF_INET;
    adr_local.sin_addr.s_addr     = htonl(INADDR_ANY);
    adr_local.sin_port            = htons(nPort);
    on=1 ;

  /*  if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on))) != 0)
   */
#ifdef LINUX
	if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on))) != 0)
#else
	if((retour = setsockopt (sDesc, IPPROTO_TCP, SO_REUSEADDR, &on,sizeof(on))) != 0)
#endif
    {
        close(sDesc);
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "setsockopt",
            "Can not Use Socket Option SO_REUSEADDR", errno, NOK);
    }

  /*  if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEPORT, &on,sizeof(on))) != 0)
   */
#ifdef LINUX
         if((retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on))) != 0)
#else
        if((retour = setsockopt (sDesc, IPPROTO_TCP, SO_REUSEADDR, &on,sizeof(on))) != 0)
#endif
    {
        close(sDesc);
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "setsockopt",
            "Can not Use Socket Option SO_REUSEADDR", errno, NOK);
    }

    sLinger.l_onoff    = 0; /* Wait for unsent data */
    sLinger.l_linger    = 5; /* Close socket after 5 sec even if there is unsent data */
    if((retour = setsockopt (sDesc, SOL_SOCKET, SO_LINGER, &sLinger,sizeof(sLinger))) != 0)
    {
        close(sDesc);
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "setsockopt",
            "Can not Use Socket Option SO_LINGER", errno, NOK);
    }

    if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
    {
        close(sDesc);
        return_CALL_FAILURE_ERRNO(szFuncName, "bind", errno, NOK);
    }

    if(&adr_local != NULL)
        getsockname(sDesc, (struct sockaddr *)&adr_local, (int*)&Longueur);

    if ((retour = listen(sDesc, 1)) == -1)
    {
        close(sDesc);
        return_CALL_FAILURE_ERRNO(szFuncName, "listen", errno, NOK);
    }

    nFdLocal = sDesc;
    nFd      = sDesc;

    return_INT_PAR2(szFuncName, OK, "%d, %d", nFdLocal, nFd);
}

/***********************************************************************
**  name      : InitCltCom()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    :
***********************************************************************/
int AcceptCall(char  calling_address [])
{
    char               szFuncName[] = "AcceptCall", sLine[134];
    int                nStatus;

    START_TRACE_PAR1(szFuncName, "%d", nFdLocal);

    if ((nStatus = AcceptCallSrv(calling_address)) < 0)
        return_CALL_FAILURE_INT(szFuncName, "AcceptCallSrv", nStatus, NOK);

    return_INT_PAR2(szFuncName, OK, "%d, %.20s", nStatus, calling_address);
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
    int                long_inet;
    struct sockaddr_in adr_client;
    char               szFuncName[] = "AcceptCallSrv", sLine[134];
    int                nFd;
    unsigned char      adr[4];

    START_TRACE(szFuncName);

    memset(adr, 0, sizeof(adr));

    long_inet = sizeof(struct sockaddr_in);
    nFd = accept(nFdLocal,
        (struct sockaddr *)&adr_client,
        (int*)&long_inet);
    if (nFd < 0 )
        return_CALL_FAILURE_ERRNO(szFuncName, "accept", errno, NOK);

    adr[0]= adr_client.sin_addr.s_addr % 256;
    adr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
    adr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
    adr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

    sprintf(calling_address,"%.03u.%.03u.%.03u.%.03u", adr[3], adr[2], adr[1], adr[0]);

    return_INT_PAR2(szFuncName, nFd, "%d, %.20s", nFd, calling_address);
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
    char szFuncName[] = "StopCltCom", sLine[MAX_LINE_TRC];
    int  nRetCode;

    START_TRACE_PAR1(szFuncName, "%d", nFd);

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

    return_VOID(szFuncName);
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
    char  sBuffRcv [ LG_MAX ];
    char  szFuncName[] = "ReadLineMsg", sLine [ MAX_LINE_TRC ];
    int   nLenMsg =0, nRead=0;

    START_TRACE_PAR1(szFuncName, "%d", nFd);

    memset (sBuffRcv, 0, sizeof(sBuffRcv));

    do
    {
        nRead = recv (nFd, sBuffRcv, 2 , MSG_PEEK);
    }   while ((nRead !=2) && (nRead > 0));

    if (nRead < 0)
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "recv_nok_hdr",
            "while reading message length", errno, NOK);

    if (nRead == 0)
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "recv_disc_hdr",
            "disconnected while reading message length", errno, DISCONNECT);


    /* nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ; */
    nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] ;

    memset (sBuffRcv, 0, sizeof(sBuffRcv));

    nRead = recv (nFd, sBuffRcv, nLenMsg, 0);

    if (nRead < 0)
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "recv_nok",
            "while reading message data", errno, NOK);

    if (nRead == 0)
        return_CALL_FAILURE_ERRNO_VAR(szFuncName, "recv_disc",
            "disconnect while reading message data", errno, NOK);

    dump_buffer((unsigned char *) sBuffRcv, nLenMsg, 'A', 'L', 'I');

    memcpy ( sBuffer, sBuffRcv +  2  , nLenMsg - 2);
    *nLength = nLenMsg;

    return_OK(szFuncName);
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

    START_TRACE_PAR2(szFuncName, "%d, %d ", nFd, nLength);

    memset (sBuffSnd, 0, LG_MAX);

    sBuffSnd [ 0 ] = (nLength + 2 ) / 256;
    sBuffSnd [ 1 ] = (nLength + 2) % 256;
    memcpy (sBuffSnd +2 , sBuffer, nLength);

    dump_buffer((unsigned char *) sBuffSnd, nLength+2, 'A', 'L', 'O');
    retour = write ( nFd, sBuffSnd, nLength+2);
    if (retour <= 0)
        return_CALL_FAILURE_ERRNO(szFuncName, "write", errno, NOK);

    return_OK(szFuncName);
}

