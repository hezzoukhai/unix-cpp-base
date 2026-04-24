#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <define.h>
#include <thread_var.h>
#include <queue.h>
#include <param_arg.h>
#include <resources.h>
#include <nw_raw_msg.h>
#include <iso8583.h>



/*pid_t 	g_nSwiMonPid;*//*PLUTONL-3561*/
uid_t		g_nSwiUID;


int 
InitMessagesCaptureModule(int nIndexCtx)
{
	char			sLine[MAX_LINE_TRC];


	trace_event("Start InitMessagesCaptureModule()", PROCESSING);

	nBalNtwRawMsg = OpenBalCom(BAL_NTW_RAW_MSG);
	g_nSwiUID = getuid();/*PLUTONL-3561*/

	trace_event("End   InitMessagesCaptureModule(OK)", PROCESSING);
	return OK;
}

/*****************************************************************************/
int BuildBodyRawMsg(E_NW_MSG_CAT eMsgCat, E_ISO8583_PROTO eProto, char* sKey, int nLength, char* sBuf, char* sBody)
{

	char 	sVersion[2]="1";
	int 	nOffset;

	nOffset = 0;
	sBody[nOffset++]=sVersion[0];
	sprintf(sBody+nOffset,"%02d%-36s%01d%04d",eProto,sKey,eMsgCat,nLength);
	nOffset+=43;
	memcpy(sBody + nOffset,sBuf,nLength);
	nOffset+=nLength;
	return nOffset;
	
}
/*****************************************************************************/
void BuildKeyRawMsg(char* sRefNbr, char* sIntStan, char* sExtStan, char* ScaptureCode, char* sRoutingCode, char* sKey)
{
	sprintf(sKey,"%s%s%s%s%s",sRefNbr,sIntStan,sExtStan,ScaptureCode,sRoutingCode);
}
/*****************************************************************************/
int LogNetworkMessage(int nIndexCtx, E_NW_MSG_CAT eMsgCat, E_ISO8583_PROTO eProto, char* sKey, char* sBuf, int nBufLen)
{

	TSBalExchange   BalMessage;
	char			sLine[MAX_LINE_TRC];
	char			sResMonResId[RESOURCE_ID_LEN + 1];
	int				nResult;
	int				nTotalLen;


	
	trace_event("Start LogNetworkMessage()", PROCESSING);
	memset(sResMonResId,'\0',sizeof(sResMonResId));
	
	/*PLUTONL-3561*/
	/*if (g_nSwiMonPid == 0)
	{
		nResult = GetSwitchMonResourcePid (nIndexCtx, NodeId, sResMonResId, &g_nSwiMonPid );
		if (nResult != OK)
		{
			sprintf(sLine,">>LogNetworkMessage(NOK) No defined switch monitoring for node id [%s]",NodeId);
			trace_event(sLine, ERROR);
			trace_event("END   LogNetworkMessage(NOK)", PROCESSING);
			return NOK;
		}
	}
	if ((kill ( g_nSwiMonPid , 0 ) != 0))
	{
		g_nSwiMonPid = 0;
		trace_event(">>LogNetworkMessage(NOK) PID is Down", ERROR);
		nResult = GetSwitchMonResourcePid (nIndexCtx, NodeId, sResMonResId, &g_nSwiMonPid );
		if (nResult != OK)
		{
			sprintf(sLine,">>LogNetworkMessage(NOK) No defined switch monitoring for node id [%s]",NodeId);
			trace_event(sLine, ERROR);
			trace_event("END   LogNetworkMessage(NOK)", PROCESSING);
			return NOK;
		}

	}
	*/

	nTotalLen = BuildBodyRawMsg(eMsgCat, eProto, sKey, nBufLen, sBuf, BalMessage.sBody);
	
	BalMessage.sHeader.nPartnerId = g_nSwiUID; /*g_nSwiMonPid;*//*PLUTONL-3561*/
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1000;
	BalMessage.sHeader.nLen = nTotalLen;
	
	nResult = WriteBalMsg(nBalNtwRawMsg, &BalMessage);

	trace_event("End   LogNetworkMessage(OK)", PROCESSING);

	return OK;
}

