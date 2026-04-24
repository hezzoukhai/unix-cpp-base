#include <p7_common.h>
#include <p7_globals.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_tcp.h>


/******************************************************************************/
void InitCltTCPComInfo(TSTCPCltComInfo* pkTCPCltComInfo)
{
	memset(pkTCPCltComInfo->szCltAddress,'\0',sizeof(pkTCPCltComInfo->szCltAddress));
	pkTCPCltComInfo->eStatus=CLT_CONN_FREE;
	pkTCPCltComInfo->bConnected=NOK;
	pkTCPCltComInfo->nFd=0;
}
/******************************************************************************/
void InitTCPComInfo(TSTCPComInfo* pkTCPComInfo)
{
	pkTCPComInfo->nNbConn = 0;
	memset(pkTCPComInfo->szHostAddr,'\0',sizeof(pkTCPComInfo->szHostAddr));
	pkTCPComInfo->nPortNbr = 0;
}
/******************************************************************************/
int TCP_InitCltCom(TSTCPComInfo* pkTCPComInfo)
{

    struct hostent				*hp;
    struct sockaddr_in			sAddrServ;
	int							nFd;
    int							nIndex,nClientIndex;
	char						sLine[MAX_LINE_TRC];
	
	nIndex = 0;
	
	trace_event("Start TCP_InitCltCom()",PROCESSING);
	
	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_CLIENT )
	{
		trace_event("Entity is not a client...",ERROR);
		return NOK;
	}
	if( pkTCPComInfo->ComInfoProperties->nTotalLengthLen < pkTCPComInfo->ComInfoProperties->nLengthLen )
	{
		sprintf(sLine, "Invalid length len VS total length len values %d , %d",pkTCPComInfo->ComInfoProperties->nLengthLen,pkTCPComInfo->ComInfoProperties->nTotalLengthLen);
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitCltCom(NOK)",PROCESSING);
		return NOK;
	}
	if( pkTCPComInfo->ComInfoProperties->nLengthOffset > 0 && pkTCPComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_event("Invalid length Length offset VS LengthExcluded",ERROR);
		return NOK;
	}
	
    if ( (nFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
        sprintf(sLine, "socket(%d): %s", errno, strerror(errno));
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitCltCom(NOK)",PROCESSING);
		return NOK;
	}

	if ((hp=gethostbyname(pkTCPComInfo->szHostAddr)) == NULL)
	{
		sprintf(sLine,"gethostbyname(%s)", pkTCPComInfo->szHostAddr);
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitCltCom(NOK)",PROCESSING);
		return NOK;
	}

	sAddrServ.sin_family    = AF_INET;
	sAddrServ.sin_port   	= htons(pkTCPComInfo->nPortNbr);
    memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

    if (connect(nFd,(struct sockaddr*)&sAddrServ, sizeof(struct sockaddr)) == -1)
    {
		close(nFd);
		sprintf(sLine,"connect(%d): %s", errno, strerror(errno));
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitCltCom(NOK)",PROCESSING);
        return NOK;
    }

	pkTCPComInfo->tab_conn[nIndex].nFd = nFd;

	sprintf(sLine,"End   TCP_InitCltCom(OK)[%d|%d]",nIndex,nFd);
	trace_event(sLine,PROCESSING);
	
	return OK;

}
/******************************************************************************/
int TCP_InitSrvCom(TSTCPComInfo* pkTCPComInfo)
{

	int					nIndex;

	int					sDesc;
	struct sockaddr_in	localAddr;
	int					structLen = sizeof(struct sockaddr_in);
	int					nResult;
	struct servent		*server;
	int					on;
	char				sLine[MAX_LINE_TRC];
	
	trace_event("Start TCP_InitSrvCom()",PROCESSING);
	
	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_event("Entity is not a server...",ERROR);
		return NOK;
	}
	if( pkTCPComInfo->ComInfoProperties->nTotalLengthLen < pkTCPComInfo->ComInfoProperties->nLengthLen )
	{
		sprintf(sLine, "Invalid length len VS total length len values %d , %d",pkTCPComInfo->ComInfoProperties->nLengthLen,pkTCPComInfo->ComInfoProperties->nTotalLengthLen);
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitSrvCom(NOK)",PROCESSING);
		return NOK;
	}
	
	if( pkTCPComInfo->ComInfoProperties->nLengthOffset > 0 && pkTCPComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_event("Invalid length Length offset VS LengthExcluded",ERROR);
		trace_event("End   TCP_InitSrvCom(NOK)",PROCESSING);
		return NOK;
	}
	
	nIndex = 0;

	if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		sprintf(sLine, "InitSrvCom: SOCKET ERROR : errno = %d ", errno); 
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitSrvCom(NOK)",PROCESSING);
		return(NOK);
	}

	localAddr.sin_family          = AF_INET;
	localAddr.sin_addr.s_addr     = htonl(INADDR_ANY);
	localAddr.sin_port            = htons(pkTCPComInfo->nPortNbr);
	on=1 ;

	nResult = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
	if(nResult != 0)
	{
		sprintf(sLine,"Can not Use Socket Option SO_REUSEADDR %d|%d",nResult,errno);
		trace_event(sLine,ERROR);
		trace_event("End   TCP_InitSrvCom(NOK)",PROCESSING);
		return(NOK);
	}
	

	if (bind(sDesc, (struct sockaddr *)&localAddr, structLen) == -1)
	{
		close(sDesc);
		sprintf(sLine, "BIND FCT ERROR : errno = %d", errno);
		trace_event(sLine,ERROR);
		trace_event( "End   InitSrvCom(NOK)",PROCESSING);
		return(NOK);
	}

	nResult = listen(sDesc, SERVER_BACKLOG);
	if ( nResult == -1 )
	{
		sprintf(sLine, "LISTEN ERROR : errno = %d \n", errno);   
		trace_event(sLine,ERROR);
		trace_event("End  InitSrvCom(NOK)",PROCESSING);
		return(NOK);
	}

	pkTCPComInfo->tab_conn[nIndex].nFd = sDesc;
	sprintf(sLine,"End   InitSrvCom(OK)[%d||%d]",nIndex,sDesc);
	trace_event(sLine,PROCESSING);
	return(OK);


}
/******************************************************************************/
int TCP_AcceptCallSrv(TSTCPComInfo* pkTCPComInfo, int* nClientConnIndex)
{
	int					long_inet;
	struct sockaddr_in	client_addr;
	char				szCallingAddress[24];
	int					nFdConn;
	unsigned char		addr[4];
	int					offset;
	int					nIndex,nClientIndex;
	int					i;

	nIndex = 0;

	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return NOK;
	}

	memset(szCallingAddress,'\0',sizeof(szCallingAddress));

	long_inet = sizeof(struct sockaddr_in);
	nFdConn = accept(	pkTCPComInfo->tab_conn[nIndex].nFd,
						(struct sockaddr *)&client_addr,
						&long_inet);
   if (nFdConn < 0 )
   {
      trace_message(P7_TL_ERROR, "AcceptCall: ACCEPT ERROR : errno = %d \n", errno);
      return(NOK);
   }

	memset(addr, 0, sizeof(addr));

    addr[0]= client_addr.sin_addr.s_addr % 256;
    addr[1]= (client_addr.sin_addr.s_addr >> 8 ) % 256;
    addr[2]= (client_addr.sin_addr.s_addr >> 16) % 256;
    addr[3]= (client_addr.sin_addr.s_addr >> 24) % 256;

    trace_message(P7_TL_PROCESSING,"CALL Ad In: %u.%u.%u.%u \n",
                addr[0], addr[1], addr[2], addr[3]);

	offset = 0;
	sprintf(szCallingAddress + offset, "%03d", addr[3]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[2]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[1]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[0]); offset += 3;

	nClientIndex = -1;
	for(i=1; i < pkTCPComInfo->nNbConn + 1; i++ )
	{
		if( pkTCPComInfo->tab_conn[i].eStatus == CLT_CONN_ONLINE &&
			strcmp(szCallingAddress,pkTCPComInfo->tab_conn[i].szCltAddress) == 0 )
		{
			nClientIndex = i;
			break;
		}
	}

	if( nClientIndex > 0 && pkTCPComInfo->ComInfoProperties->bMultiConnPerClient == NOK)
	{
		
		TCP_StopCom(pkTCPComInfo,nClientIndex);
	}
	else
	{
		nClientIndex = -1;
		for(i=1; i < pkTCPComInfo->nNbConn + 1; i++ )
		{
			if( pkTCPComInfo->tab_conn[i].eStatus == CLT_CONN_FREE)
			{
				nClientIndex = i;
				break;
			}
		}
		if( nClientIndex < 0 )
		{
			if( pkTCPComInfo->nNbConn >= pkTCPComInfo->ComInfoProperties->nMaxClients )
	{
		trace_message(P7_TL_FATAL, "MAX clients reached(%d)", pkTCPComInfo->nNbConn);
		return NOK;
	}

		pkTCPComInfo->nNbConn++;
		nClientIndex = pkTCPComInfo->nNbConn;
	}
	}

	(*nClientConnIndex) = nClientIndex;

	strncpy(	pkTCPComInfo->tab_conn[nClientIndex].szCltAddress,
				szCallingAddress,
				sizeof(pkTCPComInfo->tab_conn[nClientIndex].szCltAddress) - 1);

	pkTCPComInfo->tab_conn[nClientIndex].nFd=nFdConn;
	pkTCPComInfo->tab_conn[nClientIndex].eStatus = CLT_CONN_ONLINE;
	
	
	trace_message(P7_TL_TRACE, "Accept(fd:%d;nCltIndx:%d)",nFdConn,nClientIndex);

	return(OK);
}

/******************************************************************************/
void TCP_StopCom(TSTCPComInfo* pkTCPComInfo, int nClientConnIndex)
{
	int		nResult;
	int		nIndex,nClientIndex;

	nIndex = 0;

	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nIndex = nClientConnIndex;
	}
	else
	{
		nIndex = 0;
	}

	if ((nResult = shutdown (  pkTCPComInfo->tab_conn[nIndex].nFd  , 2 )) < 0)
    {
        trace_message(P7_TL_ERROR,"Shutdown Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }

    if ((nResult = close (  pkTCPComInfo->tab_conn[nIndex].nFd )) < 0)
    {
        trace_message(P7_TL_ERROR,"Close    Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }

	pkTCPComInfo->tab_conn[nIndex].eStatus=CLT_CONN_FREE;
	pkTCPComInfo->tab_conn[nIndex].nFd = -1;
	memset(pkTCPComInfo->tab_conn[nIndex].szCltAddress,'\0',sizeof(pkTCPComInfo->tab_conn[nIndex].szCltAddress));

}
/******************************************************************************/
void TCP_StopSrvCom(TSTCPComInfo* pkTCPComInfo)
{
	int		nResult;
	int		nFd;
	int		nIndex,nClientIndex;


	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return ;
	}


	nIndex = 0;
	
	nFd = pkTCPComInfo->tab_conn[nIndex].nFd;
	
	if ((nResult = shutdown (  nFd  , 2 )) < 0)
    {
        trace_message(P7_TL_ERROR,"Shutdown Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }

    if ((nResult = close (  nFd )) < 0)
    {
        trace_message(P7_TL_ERROR,"Close    Ret : %d, Errno : %d->%s",nResult, errno, strerror(errno));
    }
}
/******************************************************************************/
int TCP_readcheck(TSTCPComInfo* pkTCPComInfo, int nConnIndex, int nTimer)
{

	int			nIndex;
	int			nFd;
	int			nResult;
	fd_set 		rfds ;
    struct 		timeval tv;
	
	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nIndex = nConnIndex;
	}
	else
	{
		nIndex = 0;
	}
	nFd = pkTCPComInfo->tab_conn[nIndex].nFd;

	if( nFd < 0 )		return -1 ;             /* bad socket id */
	if( nTimer <= 0 )	return -2 ;             /* bad timer */

	FD_ZERO(&rfds); FD_SET(nFd , &rfds) ;		/* check only sd */

	tv.tv_sec = nTimer ;                         /* set timer */
	tv.tv_usec = 0 ;                         /* set timer */

	nResult = select(nFd+1, &rfds, NULL, NULL, &tv ) ;  /* check it */

	if( nResult == -1 )
	{
		/*trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING); */
		return (NOK);
	}

	if( nResult == 0 )
	{
		/*trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING); */
		return (NOK);
	}


	return OK;
}
/******************************************************************************/
