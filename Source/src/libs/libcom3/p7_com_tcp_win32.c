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
    struct hostent      *hp;
    SOCKADDR_IN			sAddrServ;
    int					nIndex = 0;
	
	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_CLIENT )
	{
		trace_message(P7_TL_FATAL, "Entity is not a client...");
		return NOK;
	}

	if( pkTCPComInfo->ComInfoProperties->nTotalLengthLen < pkTCPComInfo->ComInfoProperties->nLengthLen )
	{
		trace_message(P7_TL_FATAL, "Invalid length len VS total length len values %d , %d",pkTCPComInfo->ComInfoProperties->nLengthLen,pkTCPComInfo->ComInfoProperties->nTotalLengthLen);
		return NOK;
	}
	if( pkTCPComInfo->ComInfoProperties->nLengthOffset > 0 && pkTCPComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_message(P7_TL_FATAL, "Invalid length Length offset VS LengthExcluded");
		return NOK;
	}

	if(WSAStartup(MAKEWORD(2,2),&pkTCPComInfo->tab_conn[nIndex].kWsaData)!=0)
	{
		WSACleanup();
		trace_message(P7_TL_FATAL, "WSAStartup(%d): %s", errno, strerror(errno));
		return NOK;
	}

	pkTCPComInfo->tab_conn[nIndex].kSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( pkTCPComInfo->tab_conn[nIndex].kSocket == INVALID_SOCKET)
	{
		WSACleanup();
        trace_message(P7_TL_FATAL, "socket(%d): %s", errno, strerror(errno));
		return NOK;
	}

	if ((hp=gethostbyname(pkTCPComInfo->szHostAddr)) == NULL)
	{
		WSACleanup();
        trace_message(P7_TL_FATAL,"gethostbyname(%s)", pkTCPComInfo->szHostAddr);
		return NOK;
	}

	sAddrServ.sin_family	 = AF_INET;
	sAddrServ.sin_port   = htons(pkTCPComInfo->nPortNbr);
    memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

   if(connect(pkTCPComInfo->tab_conn[nIndex].kSocket,(SOCKADDR*)(&sAddrServ),sizeof(sAddrServ))!=0)
    {
		WSACleanup();
		trace_message(P7_TL_FATAL,"connect(%d): %s", errno, strerror(errno));
        return NOK;
    }

	return OK;
}
/******************************************************************************/
int TCP_InitSrvCom(TSTCPComInfo* pkTCPComInfo)
{
    
	SOCKADDR_IN			sAddrServ;
    int					nIndex;


	nIndex = 0;
	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return NOK;
	}

	if( pkTCPComInfo->ComInfoProperties->nTotalLengthLen < pkTCPComInfo->ComInfoProperties->nLengthLen )
	{
		trace_message(P7_TL_FATAL, "Invalid length len VS total length len values %d , %d",pkTCPComInfo->ComInfoProperties->nLengthLen,pkTCPComInfo->ComInfoProperties->nTotalLengthLen);
		return NOK;
	}
	if( pkTCPComInfo->ComInfoProperties->nLengthOffset > 0 && pkTCPComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_message(P7_TL_FATAL, "Invalid length Length offset VS LengthExcluded");
		return NOK;
	}

	if(WSAStartup(MAKEWORD(2,2),&pkTCPComInfo->tab_conn[nIndex].kWsaData)!=0)
	{
		WSACleanup();
		trace_message(P7_TL_FATAL, "WSAStartup(%d): %s", errno, strerror(errno));
		return NOK;
	}

	pkTCPComInfo->tab_conn[nIndex].kSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( pkTCPComInfo->tab_conn[nIndex].kSocket == INVALID_SOCKET )
	{
		WSACleanup();
        trace_message(P7_TL_FATAL, "socket(%d): %s", errno, strerror(errno));
		return NOK;
	}

	sAddrServ.sin_family=AF_INET;
	sAddrServ.sin_addr.s_addr=INADDR_ANY;
	sAddrServ.sin_port=htons(pkTCPComInfo->nPortNbr);

	if(bind(pkTCPComInfo->tab_conn[nIndex].kSocket,(SOCKADDR*)(&sAddrServ),sizeof(sAddrServ))==SOCKET_ERROR)
	{
		WSACleanup();
        trace_message(P7_TL_FATAL, "bind(%d): %s", errno, strerror(errno));
		return NOK;
	}

	
	pkTCPComInfo->tab_conn[nIndex].bConnected = OK;

	listen(pkTCPComInfo->tab_conn[nIndex].kSocket,SERVER_BACKLOG);

	

	return OK;

}
/******************************************************************************/
int TCP_AcceptCallSrv(TSTCPComInfo* pkTCPComInfo, int* nClientConnIndex)
{
	SOCKET				kTempSock;
	
	unsigned char		addr[4];
	int					long_inet;
	struct sockaddr_in	client_addr;
	int					offset;
	int					nIndex;
	char				szCallingAddress[24];
	int					nClientIndex;
	int					i;
	

	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server. Cannot listen");
		return NOK;
	}

	memset(szCallingAddress,'\0',sizeof(szCallingAddress));

	nIndex = 0;

	do
	{
		long_inet = sizeof(struct sockaddr_in);
		kTempSock=accept(pkTCPComInfo->tab_conn[nIndex].kSocket,(struct sockaddr *)&client_addr,&long_inet);
	} while( kTempSock == SOCKET_ERROR );



	memset(addr, 0, sizeof(addr));

	addr[0]= client_addr.sin_addr.s_addr % 256;
	addr[1]= (client_addr.sin_addr.s_addr >> 8 ) % 256;
	addr[2]= (client_addr.sin_addr.s_addr >> 16) % 256;
	addr[3]= (client_addr.sin_addr.s_addr >> 24) % 256;

	
	offset = 0;
	sprintf(szCallingAddress + offset, "%03d", addr[3]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[2]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[1]); offset += 3;
	szCallingAddress[offset++]='.';
	sprintf(szCallingAddress + offset, "%03d", addr[0]); offset += 3;

	nClientIndex = -1;
	for(i=1; i <= pkTCPComInfo->nNbConn; i++ )
	{
		if( strcmp(szCallingAddress,pkTCPComInfo->tab_conn[i].szCltAddress) == 0 )
		{
			nClientIndex = i;
			break;
		}
	}
	if( nClientIndex > 0 && pkTCPComInfo->ComInfoProperties->bMultiConnPerClient == NOK)
	{
		
		TCP_StopCom(pkTCPComInfo,nClientIndex);
	}
	else if( pkTCPComInfo->nNbConn >= pkTCPComInfo->ComInfoProperties->nMaxClients )
	{
		trace_message(P7_TL_FATAL, "MAX clients reached(%d)", pkTCPComInfo->nNbConn);
		return NOK;
	}
	else
	{
		pkTCPComInfo->nNbConn++;
		nClientIndex = pkTCPComInfo->nNbConn;
		
	}

	(*nClientConnIndex) = nClientIndex;

	strncpy(	pkTCPComInfo->tab_conn[nClientIndex].szCltAddress,
				szCallingAddress,
				sizeof(pkTCPComInfo->tab_conn[nClientIndex].szCltAddress) - 1);
	pkTCPComInfo->tab_conn[nClientIndex].kSocket=kTempSock;
	pkTCPComInfo->tab_conn[nClientIndex].eStatus = CLT_CONN_ONLINE;

	return OK;

}
/******************************************************************************/
void TCP_StopCom(TSTCPComInfo* pkTCPComInfo, int nClientConnIndex)
{
	int		nIndex;
	
	SOCKET*	pkSocket;

	nIndex = 0;

	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nIndex = 0;
	}
	else
	{
		nIndex = nClientConnIndex;
	}

	pkSocket = &pkTCPComInfo->tab_conn[nIndex].kSocket;
	pkTCPComInfo->tab_conn[nIndex].eStatus=CLT_CONN_FREE;
	

	shutdown(*pkSocket,SD_BOTH);
	closesocket(*pkSocket);	
}
/*****************************************************************************/
void TCP_StopSrvCom(TSTCPComInfo* pkTCPComInfo)
{
	int		nIndex;

	nIndex = 0;
	if( pkTCPComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return ;
	}

	shutdown(pkTCPComInfo->tab_conn[nIndex].kSocket,SD_BOTH);
	closesocket(pkTCPComInfo->tab_conn[nIndex].kSocket);
	/*WSACleanup(); */ /* Should not clean if clients are supposed to keep connected */
}
/******************************************************************************/
int TCP_readcheck(TSTCPComInfo* pkTCPComInfo, int nConnIndex, int nTimer)
{

	int			nIndex;
	SOCKET*		pkSocket;
	int			nFd;
	int			nResult;
	


	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nIndex = nConnIndex;
	}
	else
	{
		nIndex = 0;
	}
	pkSocket = &pkTCPComInfo->tab_conn[nIndex].kSocket;

	/*WIN32: TODO*/

	return OK;
}
/******************************************************************************/

