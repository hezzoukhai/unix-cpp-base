#include <stdio.h>
#include <stdlib.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>
#include <signal.h>
#include <dlfcn.h>
#include <dump.h>
#include <p7_services.h>
#include <hps_iso_8583.h>

/*******************************************************************************/
/*PLUTONL-1844 Versioning */
typedef int (*fnPrintLibtrnVersion)();
/*******************************************************************************/
typedef struct {
	void*	hServiceHandle;
	char	szContainerName[CONTAINER_NAME_MAX_LEN + 1];
} TSServicesContainer;


/*******************************************************************************/
static TSServicesContainer	tab_shlib_handles[MAX_SHLIB_HANDLES];
static int					nNextSlot = 0;
static pthread_mutex_t		NextSlotMutex = PTHREAD_MUTEX_INITIALIZER;
/*******************************************************************************/
int	GetContainerIndex(char* szServiceContainer)
{
	char		sLine[MAX_LINE_TRC];
	int			i;
	char		szPath[1024];
	void*		hServiceHandle;

    /*Start PLUTONL-1844 Versioning */
	int			nResult;
	fnPrintLibtrnVersion	hfnPrintLibtrnVersion;
    /*End PLUTONL-1844 Versioning */

	memset (sLine,  0, sizeof(sLine));
	memset (szPath, 0, sizeof(szPath));
	
	trace_event ( "Start GetContainerIndex ()", PROCESSING );

	sprintf(szPath,"%s/%s",getenv("HPS_LIBPATH"),szServiceContainer);
	trace_event (szPath, PROCESSING);

	pthread_mutex_lock(&NextSlotMutex);
	for(i=0; i < nNextSlot; i++ )
	{
		if( strcmp(tab_shlib_handles[i].szContainerName,szPath) == 0 )
		{
			pthread_mutex_unlock(&NextSlotMutex);
			trace_event ( ">>>>> Container found", ERROR );
			return i;
		}
	}

	if( i >= nNextSlot )
	{
		/*hServiceHandle = dlopen(szPath,RTLD_LAZY|RTLD_GLOBAL);*/
		hServiceHandle = dlopen(szPath,RTLD_LAZY);
		if( hServiceHandle == NULL )
		{
			pthread_mutex_unlock(&NextSlotMutex);
			sprintf(sLine,">>> dlopen() failed: %s",dlerror());
			trace_event (sLine, ERROR);
			fprintf(stderr,sLine);
			trace_event ("End GetContainerIndex ( hServiceHandle == NULL) ", PROCESSING);
			return ( -1 );		
		}
		memset(tab_shlib_handles[nNextSlot].szContainerName,'\0',sizeof(tab_shlib_handles[nNextSlot].szContainerName));
		strncpy(tab_shlib_handles[nNextSlot].szContainerName,szPath,sizeof(tab_shlib_handles[nNextSlot].szContainerName) - 1);
		tab_shlib_handles[nNextSlot].hServiceHandle = hServiceHandle;
		i = nNextSlot;
		nNextSlot++;
		trace_event ( ">>>>> Container First time loaded", ERROR );

     	/*Start PLUTONL-1844 Versioning */
		hfnPrintLibtrnVersion= (fnPrintLibtrnVersion)dlsym(hServiceHandle,"PrintLibtrnVersion");
		if(hfnPrintLibtrnVersion == NULL)
		{
			sprintf(sLine,">>> dlsym() failed: %s",dlerror());
			trace_event (sLine, ERROR);
			return ( -1 );
		}

		nResult = (*hfnPrintLibtrnVersion)();
		/*End PLUTONL-1844 Versioning */

	}
	pthread_mutex_unlock(&NextSlotMutex);

	sprintf(sLine,"End GetContainerIndex (%d)",i);
	trace_event (sLine, PROCESSING);

	return i;
}
/*******************************************************************************/
int	ReloadTransactionsContainers()
{

	int			i;
	int			nIndex;
	char		szPath[1024];
	char		sLine[MAX_LINE_TRC];
	void*		hServiceHandle;

	trace_event ("Start  ReloadTransactionsContainers()", PROCESSING);
	nIndex= 0;
	pthread_mutex_lock(&NextSlotMutex);
	for(i=0; i < nNextSlot; i++ )
	{
		dlclose(tab_shlib_handles[i].hServiceHandle);

		sprintf(szPath,"%s",tab_shlib_handles[i].szContainerName);
		trace_event (szPath, PROCESSING);

		/*hServiceHandle = dlopen(szPath,RTLD_LAZY|RTLD_GLOBAL);*/
		hServiceHandle = dlopen(szPath,RTLD_LOCAL);

		if( hServiceHandle == NULL )
		{
			sprintf(sLine,">>> dlopen() failed: %s",dlerror());
			trace_event (sLine, ERROR);
			fprintf(stderr,sLine);
			continue;
		}
		memset(tab_shlib_handles[nIndex].szContainerName,'\0',sizeof(tab_shlib_handles[nIndex].szContainerName));
		strncpy(tab_shlib_handles[nIndex].szContainerName,szPath,sizeof(tab_shlib_handles[nIndex].szContainerName) - 1);
		tab_shlib_handles[nIndex].hServiceHandle = hServiceHandle;
		nIndex++;

	}

	nNextSlot = nIndex;
	pthread_mutex_unlock(&NextSlotMutex);
	trace_event ("End    ReloadTransactionsContainers(OK)", PROCESSING);

	return OK;
}
/*******************************************************************************/
/*******************************************************************************/
int	CloseTransactionsContainers()
{

	int			i;
	char		szPath[1024];
	char		sLine[MAX_LINE_TRC];
	void*		hServiceHandle;

	trace_event ("Start  CloseTransactionsContainers()", PROCESSING);
	pthread_mutex_lock(&NextSlotMutex);
	for(i=0; i < nNextSlot; i++ )
	{
		dlclose(tab_shlib_handles[i].hServiceHandle);

		memset(tab_shlib_handles[i].szContainerName,'\0',sizeof(tab_shlib_handles[i].szContainerName));
		tab_shlib_handles[i].hServiceHandle = NULL;
	}

	nNextSlot = 0;
	pthread_mutex_unlock(&NextSlotMutex);
	trace_event ("End    CloseTransactionsContainers(OK)", PROCESSING);

	return OK;
}
/*******************************************************************************/
fnServiceStandin	GetStandinService(char* szServiceContainer, char* sServiceName)
{

	int					nIndex;
	fnServiceStandin	fnStandinEntry;

	trace_event ("Start    GetStandinService()", PROCESSING);

	nIndex = GetContainerIndex(szServiceContainer);
	if( nIndex < 0 )
	{
		return NULL;
	}

	fnStandinEntry= (fnServiceStandin)dlsym(tab_shlib_handles[nIndex].hServiceHandle,sServiceName);
	trace_event ("End    GetStandinService(OK)", PROCESSING);

	return fnStandinEntry;
}
/*******************************************************************************/
fnServiceReply	GetServiceReply(char* szServiceContainer, char* sServiceName)
{

	int					nIndex;
	fnServiceReply		fnReplyEntry;
	trace_event ("Start GetServiceReply ()",PROCESSING);
	nIndex = GetContainerIndex(szServiceContainer);
	if( nIndex < 0 )
	{
		return NULL;
	}

	fnReplyEntry= (fnServiceReply)dlsym(tab_shlib_handles[nIndex].hServiceHandle,sServiceName);
        
        
        trace_event ("End GetServiceReply ()",PROCESSING);
	return fnReplyEntry;
}
/*******************************************************************************/
static int ServiceReply(	int			nIndexCtx, 
							char *		sTlvBuffer,
							TSIsoInfo *	IsoInfo,
							int			nProcResult)
{

	TSTlvAutho  sTlvInfo;
	int			nRetCode;
       

        
	trace_event ("Start ServiceReply ()" , PROCESSING);

	PrintTlvBuffer ( sTlvBuffer );
	InitTlvAutho    ( &sTlvInfo );
	nRetCode = AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	if( nProcResult == ROUTAGE )
	{
		nRetCode = TlvToIso        ( nIndexCtx , &sTlvInfo   , IsoInfo  , REQUEST );
	}
	else
	{
		nRetCode = TlvToIso        ( nIndexCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
	}


	InitTlvAutho    ( &sTlvInfo );
	trace_event ("End ServiceReply ()", PROCESSING);

	return OK;
}

/*******************************************************************************/
int	ServiceProcessing(	int			nIndexCtx,
						char *		sMyResource, 
						char *		sAuthoTable, 
						char *		sAcquirerBank,
						char *		InternationalRoutCheck, 
						char *		sServGatewayFlag, 
						char *		sBillCalculFlag, 
						char *		sTlvBuffer , 
						TSIsoInfo *	IsoInfo)
{

	char		szServiceName[512];
	char		szServiceContainer[512];
	char		szServiceEntry[512];
	char		szPath[1024];
	char		sLine[LG_MAX];
	void*		hServiceHandle;
	int			nResult;
	int			nRetCode;
	fnService	fnServiceEntry;
	TSTlvAutho  sTlvInfo;
	int			i;
	int			nIndex;


	trace_event ("Start  ServiceProcessing()", PROCESSING);

	memset(szServiceName,'\0',sizeof(szServiceName));
	memset(szServiceContainer,'\0',sizeof(szServiceContainer));
	memset(szServiceEntry,'\0',sizeof(szServiceEntry));
	memset(szPath,'\0',sizeof(szPath));
	InitTlvAutho(&sTlvInfo);


	nResult = IsoToTlv ( nIndexCtx , IsoInfo , &sTlvInfo );
	if ( nResult != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nResult = AuthProcessingMalfunction(	nIndexCtx,
												sTlvBuffer);
		return ( NOK );
	}

	
	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
	PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
	PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
	PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo , STAND_IN_RESOURCE ,  6);
	PutTlvAutho ( AUTO_NODE_ID, &sTlvInfo , NodeId ,  NODE_ID_LEN);
	PutTlvAutho ( AUTO_DBSERV_ID, &sTlvInfo , g_LocDBServ ,  DBINST_LEN);

	AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

	nResult = LoadIssuerInfo (  nIndexCtx			,
								sTlvBuffer          ,
								InternationalRoutCheck		);
	PrintTlvBuffer ( sTlvBuffer );

	if ( nResult != OK )
	{
		nResult = AuthProcessingMalfunction(	nIndexCtx,
												sTlvBuffer);
		ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo,nResult);
		return nResult;
	}

	nResult = GetService(	nIndexCtx,
							sTlvBuffer,
							szServiceName,
							szServiceContainer,
							szServiceEntry);
	if( nResult != OK )
	{
		trace_event ( "GetService Error ...", ERROR );
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , FUNCT_NOT_AVIALABL , 3 );
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , RESOURCE_SERVICE_NOT_SUPPORTED  , 3);
		nResult = AuthProcessingMalfunction(	nIndexCtx,
												sTlvBuffer);
		ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo,nResult);
		return ( NOK );
	}

	nIndex = GetContainerIndex(szServiceContainer);
	if( nIndex < 0 )
	{
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
			
		nResult = AuthProcessingMalfunction(	nIndexCtx,
												sTlvBuffer);
		ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo,ERROR);
		return ( NOK );		
	}

	hServiceHandle = tab_shlib_handles[nIndex].hServiceHandle;
	
	fnServiceEntry= (fnService)dlsym(hServiceHandle,szServiceEntry);
	if(fnServiceEntry == NULL)
	{
		sprintf(sLine,">>> dlsym() failed: %s",dlerror());
		trace_event (sLine, ERROR);
		fprintf(stderr,sLine);
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nResult = AuthProcessingMalfunction(	nIndexCtx,
												sTlvBuffer);
		ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo,ERROR);
		return ( NOK );
	}

        sprintf(sLine,"Container/Service  %s, %s",szServiceContainer, szServiceEntry);
        trace_event (sLine, PROCESSING);

	PutTlvBuffer ( AUTO_SERV_CONTAINER    , sTlvBuffer , szServiceContainer  , strlen(szServiceContainer));
	PutTlvBuffer ( AUTO_SERV_PROC_ENTRY    , sTlvBuffer , szServiceEntry  , strlen(szServiceEntry));

	nResult = (*fnServiceEntry)(nIndexCtx,sTlvBuffer);

	
	ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo, nResult);

	InitTlvAutho(&sTlvInfo);
	sprintf(sLine,"End  ServiceProcessing(%d)",nResult);
	trace_event (sLine, PROCESSING);
	return(nResult);
}
/*******************************************************************************/

int ServiceReplyProcessing(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer, int nEvtInd)
{

	char				sServContainer	[CONTAINER_NAME_MAX_LEN + 1];
	char				sServReplyEntry	[SERVICE_NAME_MAX_LEN + 1];
	int					nRetCode;
	int					nLength;
	char				sLine[MAX_LINE_TRC];
	fnServiceReply		fnReplyEntry;
	
	trace_event("Start ServiceReplyProcessing()",PROCESSING);

	memset(sServContainer, 0, sizeof(sServContainer));
	memset(sServReplyEntry, 0, sizeof(sServReplyEntry));


	GetTlvBuffer ( AUTO_SERV_CONTAINER , sTlvBuffer , sServContainer ,&nLength);
	GetTlvBuffer ( AUTO_SERV_ROUTING_REPLY_ENTRY , sTlvBuffer , sServReplyEntry ,&nLength);
	if( strlen(sServContainer) == 0 || strlen(sServReplyEntry) == 0 )
	{
		trace_event("End   ServiceReplyProcessing(No Reply process)",PROCESSING);
		return OK;
	}

	fnReplyEntry = GetServiceReply(sServContainer,sServReplyEntry);
	if( fnReplyEntry == NULL )
	{
		
		trace_event("End   ServiceReplyProcessing(Inv Reply Process)",PROCESSING);
		return(OK);
	}
	/*consume the reply entry*/
	/*YDA20170927 see PROD00047328*/
	/*DelTlvBuffer ( AUTO_SERV_CONTAINER , sTlvBuffer , sServContainer ,&nLength);*/
	DelTlvBuffer ( AUTO_SERV_ROUTING_REPLY_ENTRY , sTlvBuffer , sServReplyEntry ,&nLength);

	
	nRetCode = (*fnReplyEntry)(nIndexCtx,IsoInfo, sTlvBuffer,nEvtInd);

	/*pour rester conforme, cette fct fera la conversion vers iso au lieu de AuthReplyProc */
	ServiceReply(nIndexCtx,sTlvBuffer,IsoInfo, nRetCode);

	sprintf(sLine,"End   ServiceReplyProcessing(%d)",nRetCode);
	trace_event(sLine,PROCESSING);
	return nRetCode;

}


/*******************************************************************************/
int AuthProcessingMalfunction(	int			nIndexCtx,
								char *		sTlvBuffer )
{

	char		sActionCode 		[ 3 + 1 ];
	char            sMti                    [ 4 + 1 ];
	int			nResult,nLength;
	
	trace_event ("Start  AuthProcessingMalfunction()", PROCESSING);

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sMti ,        0, sizeof ( sMti ));

	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
	GetTlvBuffer ( AUTO_MSG_TYPE , sTlvBuffer , sMti ,&nLength);

	if (sMti[MSG_TYPE_CLASS] == MSG_TYPE_P2_REV)
            PutTlvBuffer ( AUTO_CUR_TABLE_INDICATOR  , sTlvBuffer, "MSC" , 3);
	

	nResult = GetActionCode( nIndexCtx , sTlvBuffer , sActionCode );                                                            
	if ( nResult != OK )                                                                                                         
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);
	
	nResult = InsertionAuthoActivity ( nIndexCtx , sTlvBuffer );

	if ( nResult != OK )
	{
		trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
	}


	trace_event ("End    AuthProcessingMalfunction()", PROCESSING);
	return OK;
}






/*******************************************************************************/
