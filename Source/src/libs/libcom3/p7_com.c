#include <p7_common.h>
#include <stdlib.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_tcp.h>
#include <p7_com_ssl.h>
#include <p7_com.h>
/******************************************************************************/
/*
#define MAX_TCP_CONN		32
#define MAX_SSL_CONN		400
*/
/******************************************************************************/
#define GET_COM_TYPE(a)			(((a)&0xF0000000)>>28)
#define GET_COM_IDX(a)			(((a)&0x0FFF0000)>>16)
#define GET_COM_CLT_IDX(a)		(((a)&0x0000FFFF))


#define SET_COM_TYPE(a,b)		(((a)&0x0FFFFFFF)|(((b)<<28)))
#define SET_COM_IDX(a,b)		(((a)&0xF000FFFF)|(((b)<<16)&0x0FFF0000))
#define SET_COM_CLT_IDX(a,b)	(((a)&0xFFFF0000)|((b)&0x0000FFFF))

/******************************************************************************/
static TSComInfoProperties*		gs_tab_ComInfoProperties = NULL;
static int						gs_nComInfoPropertiesSize = 0;
static int						gs_nCurrComInfoPropertiesSize = 0;

static TSTCPComInfo**			gs_tab_TCPComInfo = NULL;
static int						gs_nTCPComInfoSize = 0;
static int						gs_nCurrTCPComInfoSize = 0;

static TSSSLComInfo**			gs_tab_SSLComInfo = NULL;
static int						gs_nSSLComInfoSize = 0;
static int						gs_nCurrSSLComInfoSize = 0;


static int						gs_bSSLInUse=NOK;

static pthread_mutex_t			gs_ComInfoMutex  = PTHREAD_MUTEX_INITIALIZER;

#define DFLT_STEP_COM_INFO_PROP		2
#define DFLT_STEP_COM_TBL			2
/******************************************************************************/
int	RegisterComInfoProperties(TSComInfoProperties* pComInfoProperties)
{
	if( gs_nCurrComInfoPropertiesSize >= gs_nComInfoPropertiesSize )
	{
		
		gs_nComInfoPropertiesSize+=DFLT_STEP_COM_INFO_PROP;
		gs_tab_ComInfoProperties	= (TSComInfoProperties*)realloc(gs_tab_ComInfoProperties, gs_nComInfoPropertiesSize*sizeof(TSComInfoProperties));
	}
	gs_tab_ComInfoProperties[gs_nCurrComInfoPropertiesSize] = (*pComInfoProperties);
	gs_nCurrComInfoPropertiesSize++;

	return 0;
}
/******************************************************************************/
int SetTabComInfoProperties		(TSComInfoProperties* tab_ComInfoProperties, int nSize)
{
	gs_tab_ComInfoProperties = tab_ComInfoProperties;
	gs_nComInfoPropertiesSize = nSize;
	return OK;
}
/******************************************************************************/
const TSComInfoProperties*	GetTabComInfoProperties			( int* nSize)
{
	(*nSize) = gs_nComInfoPropertiesSize;
	return gs_tab_ComInfoProperties;
}
/******************************************************************************/
const TSComInfoProperties*const  GetComInfoProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nComInfoPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_ComInfoProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_ComInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
/******************************************************************************/
conn_id_t	SetupComProperties	(const TSComInfoProperties* pkComInfoProperties)
{
	int				i;
	int				nConnIndex = 0;
	int				nResult;
	TSTCPComInfo*	pkTCPComInfo;
	char			sLine[MAX_LINE_TRC];

	trace_event("Start SetupComProperties(START)",PROCESSING);

	pthread_mutex_lock(&gs_ComInfoMutex);
	if( gs_tab_TCPComInfo == NULL ||
		gs_nCurrTCPComInfoSize + 1 >= gs_nTCPComInfoSize )
	{
		gs_nTCPComInfoSize+=DFLT_STEP_COM_TBL;
		gs_tab_TCPComInfo	= (TSTCPComInfo**)realloc(gs_tab_TCPComInfo, gs_nTCPComInfoSize*sizeof(TSTCPComInfo*));
	}

	

	pkTCPComInfo =(TSTCPComInfo*) malloc(sizeof(TSTCPComInfo));
	if( pkTCPComInfo == NULL )
	{
		trace_event("Cannot allocate Communication data",ERROR);
		trace_event("End   SetupComProperties(NOK)",PROCESSING);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return INVALID_CONN_ID;
	}

	InitTCPComInfo(pkTCPComInfo);
	pkTCPComInfo->ComInfoProperties = pkComInfoProperties;

	pkTCPComInfo->tab_conn = (TSTCPCltComInfo*)malloc((pkComInfoProperties->nMaxClients + 1)* sizeof(TSTCPCltComInfo));
	if( pkTCPComInfo->tab_conn == NULL )
	{
		trace_event("Cannot allocate clients Communication data",ERROR);
		trace_event("End   SetupComProperties(NOK)",PROCESSING);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return INVALID_CONN_ID;
	}

	for( i = 0; i < pkComInfoProperties->nMaxClients + 1; i++ )
	{
		InitCltTCPComInfo(&pkTCPComInfo->tab_conn[i]);
	}


	nConnIndex = SET_COM_TYPE(nConnIndex,pkComInfoProperties->eConnType);
	/*nConnIndex = SET_COM_IDX(nConnIndex,gs_nTCPComInfoSize);*/ /*BOC ACT PROD00038868*/
	nConnIndex = SET_COM_IDX(nConnIndex,gs_nCurrTCPComInfoSize);


	/*gs_tab_TCPComInfo[gs_nTCPComInfoSize]=pkTCPComInfo;
	gs_nTCPComInfoSize++;*//*BOC ACT PROD00038868*/
	
	gs_tab_TCPComInfo[gs_nCurrTCPComInfoSize]=pkTCPComInfo;
	gs_nCurrTCPComInfoSize++;
	
	pthread_mutex_unlock(&gs_ComInfoMutex);
	
	trace_event("End   SetupComProperties(OK)",PROCESSING);
	return nConnIndex;

}
/******************************************************************************/
/******************************************************************************/
int	ConnectLine (conn_id_t nConnIndex, const char* szHostAddr, int nPortNbr)
{
	
	int				nIndex;
	int				nResult;
	TSTCPComInfo*	pkTCPComInfo;
	char			sLine[MAX_LINE_TRC];


	sprintf(sLine,"Start ConnectLine(%s,%d)",szHostAddr,nPortNbr);
	trace_event(sLine,PROCESSING);


	nIndex	= GET_COM_IDX(nConnIndex);


	pkTCPComInfo = gs_tab_TCPComInfo[nIndex];
	
	strncpy(pkTCPComInfo->szHostAddr,szHostAddr,sizeof(pkTCPComInfo->szHostAddr) - 1 );
	pkTCPComInfo->nPortNbr = nPortNbr;


	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_CLIENT )
	{
		nResult = TCP_InitCltCom(pkTCPComInfo);
	}
	else
	{
		nResult = TCP_InitSrvCom(pkTCPComInfo);
	}

	if( nResult != OK )
	{
		trace_event("End   ConnectLine(NOK): Error on connect",ERROR);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return NOK;
	}



	trace_event("End   ConnectLine(OK)",PROCESSING);
	return OK;
}
/******************************************************************************/
conn_id_t	InitCom		(const char*	szConnName, const char* szHostAddr, int nPortNbr)
{
	int							i;
	int							nResult;
	conn_id_t					nConn_id;
	const TSComInfoProperties*	pkComInfoProperties;
	char						sLine[MAX_LINE_TRC];


	sprintf(sLine,"Start InitCom([%s],%s,%d)",szConnName,szHostAddr,nPortNbr);
	trace_event(sLine,PROCESSING);

	pkComInfoProperties = GetComInfoProperties(szConnName);
	if( pkComInfoProperties == NULL )
	{
		sprintf(sLine,"End   InitCom(NOK): Connection properties not found [%s]",szConnName);
		trace_event(sLine,PROCESSING);
		return INVALID_CONN_ID;
	}

	nConn_id = SetupComProperties(pkComInfoProperties);
	if( nConn_id == INVALID_CONN_ID )
	{
		sprintf(sLine,"End   InitCom(NOK): Error setup [%s]",szConnName);
		trace_event(sLine,PROCESSING);
		return INVALID_CONN_ID;
	}
	nResult = ConnectLine (nConn_id, szHostAddr, nPortNbr);
	if( nResult != OK )
	{
		sprintf(sLine,"End   InitCom(NOK): Error Connect %s[%s:%d]",szConnName,szHostAddr, nPortNbr);
		trace_event(sLine,PROCESSING);
		return INVALID_CONN_ID;
	}


	trace_event("End   InitCom(OK)",PROCESSING);
	return nConn_id;

}
/******************************************************************************/
conn_id_t	SetupSSLComProperties	( const TSComInfoProperties* pkComInfoProperties)
{
	
	int				i;
	int				nConnIndex = 0;
	int				nResult;
	TSSSLComInfo*	pkSSLComInfo;

	trace_event("Start SetupSSLComProperties()",PROCESSING);

	pthread_mutex_lock(&gs_ComInfoMutex);


	if( gs_tab_SSLComInfo == NULL ||
		gs_nCurrSSLComInfoSize + 1 >= gs_nSSLComInfoSize )
	{
		gs_nSSLComInfoSize+=DFLT_STEP_COM_TBL;
		gs_tab_SSLComInfo	= (TSSSLComInfo**)realloc(gs_tab_SSLComInfo, gs_nSSLComInfoSize*sizeof(TSSSLComInfo*));
	}
	

	pkSSLComInfo =(TSSSLComInfo*) malloc(sizeof(TSSSLComInfo));
	if( pkSSLComInfo == NULL )
	{
		trace_event("Cannot allocate Communication data",ERROR);
		trace_event("End   SetupSSLComProperties(NOK)",PROCESSING);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return INVALID_CONN_ID;
	}

	InitSSLComInfo(pkSSLComInfo);
	pkSSLComInfo->ComInfoProperties = pkComInfoProperties;

	pkSSLComInfo->tab_conn = (TSCtlSSLComInfo*)malloc((pkComInfoProperties->nMaxClients+1)* sizeof(TSCtlSSLComInfo));
	if( pkSSLComInfo->tab_conn == NULL )
	{
		trace_event("Cannot allocate clients Communication data",ERROR);
		trace_event("End   SetupSSLComProperties(NOK)",PROCESSING);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return INVALID_CONN_ID;
	}

	for( i = 0; i < pkComInfoProperties->nMaxClients + 1; i++ )
	{
		InitCltSSLComInfo(&pkSSLComInfo->tab_conn[i]);
	}

	

	nConnIndex = SET_COM_TYPE(nConnIndex,pkComInfoProperties->eConnType);
	nConnIndex = SET_COM_IDX(nConnIndex,gs_nSSLComInfoSize);
	 

	if( gs_bSSLInUse == NOK )
	{
		gs_bSSLInUse = OK;
		SSL_library_init();
		SSL_load_error_strings(); 
		OpenSSL_add_all_algorithms();
	}


	gs_tab_SSLComInfo[gs_nSSLComInfoSize]=pkSSLComInfo;
	gs_nSSLComInfoSize++;
	pthread_mutex_unlock(&gs_ComInfoMutex);


	trace_event("End   SetupSSLComProperties(OK)",PROCESSING);
	return nConnIndex;
	
}
/******************************************************************************/
int	ConnectSSLLine (conn_id_t nConnIndex, const char* szHostAddr, int nPortNbr, const char* szCertFile, const char* szPrivKeyFile, const char* szCA, int nVerifyPeer)
{

	int				nIndex;
	int				nResult;
	TSSSLComInfo* pkSSLComInfo;
	char			sLine[MAX_LINE_TRC];


	sprintf(sLine,"Start ConnectSSLLine(%s,%d)",szHostAddr,nPortNbr);
	trace_event(sLine,PROCESSING);

	sprintf(sLine,"Certficate(%s,%s)",szCertFile,szPrivKeyFile);
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CA(%s,Verify peer%d)",szCA,nVerifyPeer);
	trace_event(sLine,PROCESSING);

	nIndex	= GET_COM_IDX(nConnIndex);

	pkSSLComInfo = gs_tab_SSLComInfo[nIndex];

	strncpy(pkSSLComInfo->szHostAddr,szHostAddr,sizeof(pkSSLComInfo->szHostAddr) - 1 );
	pkSSLComInfo->nPortNbr = nPortNbr;

	if( szCertFile )
	{
		strncpy(pkSSLComInfo->szCertificateFile,szCertFile,sizeof(pkSSLComInfo->szCertificateFile) - 1 );
	}
	if( szPrivKeyFile )
	{
		strncpy(pkSSLComInfo->szPrivateKeyFile,szPrivKeyFile,sizeof(pkSSLComInfo->szPrivateKeyFile) - 1 );
	}
	if( szCA )
	{
		strncpy(pkSSLComInfo->szCA,szCA,sizeof(pkSSLComInfo->szCA) - 1 );
	}

	pkSSLComInfo->nVerifyPeer = nVerifyPeer;

	if( pkSSLComInfo->ComInfoProperties->eConnMode == E_CM_CLIENT )
	{
		nResult = SSL_InitCltCom(pkSSLComInfo);
	}
	else
	{
		nResult = SSL_InitSrvCom(pkSSLComInfo);
	}

	if( nResult != OK )
	{
		trace_event("Error on connect",ERROR);
		trace_event("End   ConnectSSLLine(NOK)",PROCESSING);
		pthread_mutex_unlock(&gs_ComInfoMutex);
		return NOK;
	}

	trace_event("End   ConnectSSLLine(OK)",PROCESSING);
	return OK;
}
/******************************************************************************/
conn_id_t	InitSSLCom	(const char*	szConnName, const char* szHostAddr, int nPortNbr, const char* szCertFile, const char* szPrivKeyFile, const char* szCA, int nVerifyPeer)
{
	
	int			i;
	int			nResult;
	conn_id_t	nConn_id;
	char		sLine[MAX_LINE_TRC];

	const TSComInfoProperties* pkComInfoProperties;
	
	sprintf(sLine,"Start InitSSLCom([%s],%s,%d)",szConnName,szHostAddr,nPortNbr);
	trace_event(sLine,PROCESSING);

	sprintf(sLine,"Certficate(%s,%s)",szCertFile,szPrivKeyFile);
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CA(%s,Verify peer%d)",szCA,nVerifyPeer);
	trace_event(sLine,PROCESSING);


	pkComInfoProperties = GetComInfoProperties(szConnName);
	if( pkComInfoProperties == NULL )
	{
		sprintf(sLine,"Connection properties not found [%s]",szConnName);
		trace_event(sLine,PROCESSING);
		trace_event("End   InitSSLCom(NOK)",PROCESSING);
		return INVALID_CONN_ID;
	}

	nConn_id = SetupSSLComProperties(pkComInfoProperties);
	if( nConn_id == INVALID_CONN_ID )
	{
		sprintf(sLine,"Error setup [%s]",szConnName);
		trace_event(sLine,PROCESSING);
		trace_event("End   InitSSLCom(NOK)",PROCESSING);
		return INVALID_CONN_ID;
	}
	
	nResult = ConnectSSLLine(nConn_id,szHostAddr,nPortNbr,szCertFile,szPrivKeyFile,szCA,nVerifyPeer);
	if( nResult != OK )
	{
		sprintf(sLine,"Error Connect %s[%s:%d]",szConnName,szHostAddr, nPortNbr);
		trace_event(sLine,PROCESSING);
		trace_event("End   InitSSLCom(NOK)",PROCESSING);
		return INVALID_CONN_ID;
	}

	trace_event("End   InitSSLCom(OK)",PROCESSING);
	return nConn_id;
}
/******************************************************************************/
void		FreeAllComs()
{
	int i;

	trace_event("Start FreeAllComs()",PROCESSING);

	for(i=0; i < gs_nTCPComInfoSize; i++ )
	{
		free(gs_tab_TCPComInfo[i]->tab_conn);
		free(gs_tab_TCPComInfo[i]);
	}
	for(i=0; i < gs_nSSLComInfoSize; i++ )
	{
		free(gs_tab_SSLComInfo[i]->tab_conn);
		free(gs_tab_SSLComInfo[i]);
	}
	
	if(gs_bSSLInUse == OK)
	{
		gs_bSSLInUse = NOK;
		
		/*ERR_free_strings();*/
		
		EVP_cleanup();
	}

	trace_event("End   FreeAllComs(OK)",PROCESSING);
}
/******************************************************************************/
int	AcceptCallSrv(conn_id_t nConnIndex,conn_id_t* nCltConnIndex)
{
	int		nType;
	int		nIndex;
	int		nCltIndex;
	int		nLocCltConnIndex;
	int		nResult;
	char	sLine[MAX_LINE_TRC];


	trace_event("Start AcceptCallSrv()",PROCESSING);

	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);
	 
	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{

			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   AcceptCallSrv(NOK)",PROCESSING);
			return NOK;
		}
		nResult = TCP_AcceptCallSrv(gs_tab_TCPComInfo[nIndex],&nCltIndex);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   AcceptCallSrv(NOK)",PROCESSING);
			return NOK;
		}
		nResult = SSL_AcceptCallSrv(gs_tab_SSLComInfo[nIndex],&nCltIndex);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   AcceptCallSrv(NOK)",PROCESSING);
		break;
	}

	if( nResult != OK )
	{
		sprintf(sLine,"Error on AcceptCall[nType=%d][nConnIndex=%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   AcceptCallSrv(NOK)",PROCESSING);
		return NOK;
	}

	nLocCltConnIndex = 0;

	nLocCltConnIndex = SET_COM_TYPE(nLocCltConnIndex,nType);
	nLocCltConnIndex = SET_COM_IDX(nLocCltConnIndex,nIndex);
	nLocCltConnIndex = SET_COM_CLT_IDX(nLocCltConnIndex,nCltIndex);

	(*nCltConnIndex) = nLocCltConnIndex;

	trace_event("End   AcceptCallSrv(OK)",PROCESSING);
	return OK;
}
/******************************************************************************/
void StopCltCom(conn_id_t nConnIndex)
{
	int		nType;
	int		nIndex;
	int		nCltIndex=0;
	char	sLine[MAX_LINE_TRC];

	trace_event("Start StopCltCom()",PROCESSING);

	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);
	nCltIndex = GET_COM_CLT_IDX(nConnIndex);

	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   StopCltCom(NOK)",PROCESSING);
			return;
		}
		TCP_StopCom(gs_tab_TCPComInfo[nIndex],nCltIndex);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   StopCltCom(NOK)",PROCESSING);
			return;
		}
		SSL_StopCom(gs_tab_SSLComInfo[nIndex],nCltIndex);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   StopCltCom(NOK)",PROCESSING);
		break;
	}

	trace_event("End   StopCltCom(OK)",PROCESSING);

}
/******************************************************************************/
void StopSrvCom(conn_id_t nConnIndex)
{
	int		nType;
	int		nIndex;
	char	sLine[MAX_LINE_TRC];

	trace_event("Start StopSrvCom()",PROCESSING);

	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);

	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   StopSrvCom(NOK)",PROCESSING);
			return;
		}
		TCP_StopSrvCom(gs_tab_TCPComInfo[nIndex]);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   StopSrvCom(NOK)",PROCESSING);
			return;
		}
		SSL_StopSrvCom(gs_tab_SSLComInfo[nIndex]);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   StopSrvCom(NOK)",PROCESSING);
		break;
	}

	trace_event("End   StopSrvCom(OK)",PROCESSING);
}
/******************************************************************************/
int	ReadLineMsg (conn_id_t nConnIndex, char *sBuffer, int* nLength)
{
	int		nType;
	int		nIndex;
	int		nCltIndex;
	char	sLine[MAX_LINE_TRC];

	sprintf(sLine,"Start ReadLineMsg(%lu)",nConnIndex);
	trace_event(sLine,PROCESSING);


	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);
	nCltIndex = GET_COM_CLT_IDX(nConnIndex);


	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   ReadLineMsg(NOK)",PROCESSING);
			return NOK;
		}
		return TCP_ReadLineMsg(gs_tab_TCPComInfo[nIndex],nCltIndex,sBuffer,nLength);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   ReadLineMsg(NOK)",PROCESSING);
			return NOK;
		}
		return SSL_ReadLineMsg(gs_tab_SSLComInfo[nIndex],nCltIndex,sBuffer,nLength);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   ReadLineMsg(NOK)",PROCESSING);
		break;
	}

	return NOK;
}
/******************************************************************************/
int	WriteLineMsg(conn_id_t nConnIndex, const char *sBuffer, int nLength)
{

	int		nType;
	int		nIndex;
	int		nCltIndex;
	char	sLine[MAX_LINE_TRC];

	sprintf(sLine,"Start WriteLineMsg(%lu)",nConnIndex);
	trace_event(sLine,PROCESSING);

	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);
	nCltIndex = GET_COM_CLT_IDX(nConnIndex);

	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   ReadLineMsg(NOK)",PROCESSING);
			return NOK;
		}
		return TCP_WriteLineMsg(gs_tab_TCPComInfo[nIndex],nCltIndex,sBuffer,nLength);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   ReadLineMsg(NOK)",PROCESSING);
			return NOK;
		}
		return SSL_WriteLineMsg(gs_tab_SSLComInfo[nIndex],nCltIndex,sBuffer,nLength);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   ReadLineMsg(NOK)",PROCESSING);
		break;
	}
	return NOK;
}
/******************************************************************************/
int tcp_readcheck( conn_id_t nConnIndex , int nTimer )
{
	int		nType;
	int		nIndex;
	int		nCltIndex;
	char	sLine[MAX_LINE_TRC];


	nType	= GET_COM_TYPE(nConnIndex);
	nIndex	= GET_COM_IDX(nConnIndex);
	nCltIndex = GET_COM_CLT_IDX(nConnIndex);

	switch(nType)
	{
	case E_CONN_TCP:
		if( nIndex >= gs_nTCPComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   tcp_readcheck(NOK)",PROCESSING);
			return NOK;
		}
		return TCP_readcheck(gs_tab_TCPComInfo[nIndex],nCltIndex,nTimer);
		break;
	case E_CONN_SSL:
		if( nIndex >= gs_nSSLComInfoSize )
		{
			sprintf(sLine,"Out of range index [%d] for ConnIndex [%lu]",nType,nConnIndex);
			trace_event(sLine,PROCESSING);
			trace_event("End   tcp_readcheck(NOK)",PROCESSING);
			return NOK;
		}
		return SSL_readcheck(gs_tab_SSLComInfo[nIndex],nCltIndex,nTimer);
		break;
	default:
		sprintf(sLine,"Unknown Conn Type [%d] for ConnIndex [%lu]",nType,nConnIndex);
		trace_event(sLine,PROCESSING);
		trace_event("End   tcp_readcheck(NOK)",PROCESSING);
		break;
	}
	return NOK;
}
/******************************************************************************/






