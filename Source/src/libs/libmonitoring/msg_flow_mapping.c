#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <limits.h>


#include <define.h>
#include <general_macros.h>
#include <resources.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <event_log.h>
#include <thread_var.h>
#include <param_arg.h>
#include <main_thr.h>
#include <queue.h>
#include <line_com.h>
#include <tlv_switch_header.h>
#include <struct_line.h>
#include <struct_time.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <iso_hps.h>
#include <service.h>


#include <msg_flow_mapping.h>
#include <msgflow_fields.h>
#include <iso8583.h>

/******************************************************************************/
static msg_id_t IntMsgLineReqReceived(char* resource_id,struct timeval*  pktime);/*PLUTONL-3762*/
static void IntMsgBalFwdReqRetrieved(msg_id_t msgId,struct timeval*  pktime);/*PLUTONL-3762*/
static void IntMsgBalRepRetrieved(msg_id_t msgId,struct timeval*  pktime);/*PLUTONL-3762*/

/******************************************************************************/
static int PSwMonWaitEntry();
static msg_id_t PSwMonPutMsg(TSMsgMappingFlow * sMsgMappingFlow);
/******************************************************************************/
typedef msg_id_t (*FnMsgReqReceived)(char* resource_id, char* msg_type, char* processing_code, char* msg_stan, char* bank_code, char* terminal_id );
typedef msg_id_t (*FnMsgIsoReqReceived)(char* resource_id, TSIsoInfo* msgInfo);
/*PLUTONL-3762*/
typedef msg_id_t (*FnMsgLineReqReceived)(char* resource_id, struct timeval*  pktime);
typedef void (*FnMsgLog)(msg_id_t msgId, char *sTlvBuffer);
typedef void (*FnMsgBalFwdReqRetrieved)(msg_id_t msgId,struct timeval* pktime);
typedef void (*FnMsgBalRepRetrieved)(msg_id_t msgId,struct timeval* pktime);
/*PLUTONL-3762*/
typedef void (*FnMsgReqForwarded)(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid);
typedef void (*FnMsgFwdReqRetrieved)(msg_id_t msgId);
typedef void (*FnMsgFwdReqSent)(msg_id_t msgId);
typedef void (*FnMsgFwdRepReceived)(msg_id_t msgId);
typedef void (*FnMsgFwdRepReplied)(msg_id_t msgId);
typedef void (*FnMsgRepRetrieved)(msg_id_t msgId);
typedef void (*FnMsgRepSent)(msg_id_t msgId, char* response_code);
typedef void (*FnMsgIsoRepSent)(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id);
typedef void (*FnMsgTimeout)(msg_id_t msgId, char* resource_id);
typedef void (*FnMsgIsOrphan)(msg_id_t msgId);
typedef void (*FnMsgMalfunctionProcessing)(msg_id_t msgId);
typedef void (*FnMsgIsDone)(msg_id_t msgId);
typedef void (*FnMsgSetMsgSAF)(msg_id_t msgId, char saf_ind);
/*SKARROUMI PLUTONL-4833*/
typedef void (*FnMsgFwdReqRetrievedTS)(msg_id_t msgId, struct timeval*  pktime);
typedef void (*FnMsgFwdReqSentTS)(msg_id_t msgId, struct timeval*  pktime);
typedef void (*FnMsgFwdRepReceivedTS)(msg_id_t msgId, struct timeval*  pktime);
typedef void (*FnMsgFwdRepRepliedTS)(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id);
/*SKARROUMI PLUTONL-4833*/
typedef enum {
	ST_NO_MONITORING	=0	,
	ST_MONITORING			,
	ST_INT_MON_QTY
}E_INT_MON_STATE;

static E_INT_MON_STATE eIntMonState = ST_NO_MONITORING;
static int dummy=0;
/******************************************************************************/
static msg_id_t IntMsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan, char* bank_code, char* terminal_id);
/*PLUTONL-3762*/
static void IntMsgLog (msg_id_t msgId, char *sTlvBuffer);
/*PLUTONL-3762*/
static void IntMsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid);
static void IntMsgFwdReqRetrieved(msg_id_t msgId);
static void IntMsgFwdReqSent(msg_id_t msgId);
static void IntMsgFwdRepReceived(msg_id_t msgId);
static void IntMsgFwdRepReplied(msg_id_t msgId);
static void IntMsgRepRetrieved(msg_id_t msgId);
static void IntMsgRepSent(msg_id_t msgId, char* response_code);
static void IntMsgIsoRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id);
static void IntMsgTimeout(msg_id_t msgId, char* resource_id);
/*static */void IntMsgIsOrphan(msg_id_t msgId);
/*static */void IntMsgMalfunctionProcessing(msg_id_t msgId);
static void IntMsgIsDone(msg_id_t msgId);
static void IntMsgSetMsgSAF(msg_id_t msgId, char saf_ind);

static msg_id_t DummyMsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan , char* bank_code, char* terminal_id);
static void DummyMsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid);
static void DummyMsgFwdReqRetrieved(msg_id_t msgId);
static void DummyMsgFwdReqSent(msg_id_t msgId);
static void DummyMsgFwdRepReceived(msg_id_t msgId);
static void DummyMsgFwdRepReplied(msg_id_t msgId);
static void DummyMsgRepRetrieved(msg_id_t msgId);
static void DummyMsgRepSent(msg_id_t msgId, char* response_code);
static void DummyMsgIsoRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* resource_id, char* dst_node_id);
static void DummyMsgTimeout(msg_id_t msgId, char* resource_id);
static void DummyMsgIsOrphan(msg_id_t msgId);
static void DummyMsgMalfunctionProcessing(msg_id_t msgId);
static void DummyMsgIsDone(msg_id_t msgId);
static void DummyMsgSetMsgSAF(msg_id_t msgId, char saf_ind);
/*PLUTONL-3762*/
static void DummyMsgLog(msg_id_t msgId, char *sTlvBuffer);
static msg_id_t DummyMsgLineReqReceived(char* resource_id, struct timeval*  pktime);
static void DummyMsgBalFwdReqRetrieved(msg_id_t msgId, struct timeval*  pktime);
static void DummyMsgBalRepRetrieved(msg_id_t msgId, struct timeval*  pktime);

/*PLUTONL-3762*/

/*SKARROUMI PLUTONL-4833*/
static void IntMsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime);
static void IntMsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime);
static void IntMsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime);
static void IntMsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id);

static void DummyMsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime);
static void DummyMsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime);
static void DummyMsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime);
static void DummyMsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id);

static int BuildMsgFlowMapping(TSMsgMappingFlow* MsgMappingFlow, char* buffer);
int GetMsgMappingFlowSlot(msg_id_t msgId, char* buffer, int* nLength);


/*SKARROUMI PLUTONL-4833*/
/******************************************************************************/
/*PLUTONL-3762*/
static FnMsgLog					UserMsgLog[ST_INT_MON_QTY]				= {DummyMsgLog,		IntMsgLog		};
static FnMsgLineReqReceived		UserMsgLineReqReceived[ST_INT_MON_QTY]  = {DummyMsgLineReqReceived,	IntMsgLineReqReceived};
static FnMsgBalFwdReqRetrieved	UserMsgBalFwdReqRetrieved[ST_INT_MON_QTY]= {DummyMsgBalFwdReqRetrieved,		IntMsgBalFwdReqRetrieved		};
static FnMsgBalRepRetrieved	   UserMsgBalRepRetrieved	[ST_INT_MON_QTY]= {DummyMsgBalRepRetrieved	,		IntMsgBalRepRetrieved			};
/*SKARROUMI PLUTONL-4833*/
static FnMsgFwdReqRetrievedTS	UserMsgFwdReqRetrievedTS[ST_INT_MON_QTY]= {DummyMsgFwdReqRetrievedTS, IntMsgFwdReqRetrievedTS};
static FnMsgFwdReqSentTS		UserMsgFwdReqSentTS[ST_INT_MON_QTY]= {DummyMsgFwdReqSentTS, IntMsgFwdReqSentTS};
static FnMsgFwdRepReceivedTS	UserMsgFwdRepReceivedTS[ST_INT_MON_QTY]= {DummyMsgFwdRepReceivedTS, IntMsgFwdRepReceivedTS};
static FnMsgFwdRepRepliedTS		UserMsgFwdRepRepliedTS[ST_INT_MON_QTY]= {DummyMsgFwdRepRepliedTS, IntMsgFwdRepRepliedTS};
/*SKARROUMI PLUTONL-4833*/
/*PLUTONL-3762*/
static FnMsgReqReceived			UserMsgReqReceived[ST_INT_MON_QTY]		= {DummyMsgReqReceived,		IntMsgReqReceived		};
static FnMsgReqForwarded		UserMsgReqForwarded[ST_INT_MON_QTY]		= {DummyMsgReqForwarded,	IntMsgReqForwarded		};
static FnMsgFwdReqRetrieved		UserMsgFwdReqRetrieved[ST_INT_MON_QTY]		= {DummyMsgFwdReqRetrieved,	IntMsgFwdReqRetrieved	};
static FnMsgFwdReqSent			UserMsgFwdReqSent[ST_INT_MON_QTY]		= {DummyMsgFwdReqSent,		IntMsgFwdReqSent		};
static FnMsgFwdRepReceived		UserMsgFwdRepReceived[ST_INT_MON_QTY]		= {DummyMsgFwdRepReceived,	IntMsgFwdRepReceived	};
static FnMsgFwdRepReplied		UserMsgFwdRepReplied[ST_INT_MON_QTY]		= {DummyMsgFwdRepReplied,	IntMsgFwdRepReplied		};
static FnMsgRepRetrieved		UserMsgRepRetrieved[ST_INT_MON_QTY]		= {DummyMsgRepRetrieved,	IntMsgRepRetrieved		};
static FnMsgRepSent			UserMsgRepSent[ST_INT_MON_QTY]			= {DummyMsgRepSent,		IntMsgRepSent			};
static FnMsgIsoRepSent			UserMsgIsoRepSent[ST_INT_MON_QTY]		= {DummyMsgIsoRepSent,		IntMsgIsoRepSent			};
static FnMsgTimeout			UserMsgTimeout[ST_INT_MON_QTY]			= {DummyMsgTimeout,		IntMsgTimeout			};
static FnMsgIsOrphan			UserMsgIsOrphan[ST_INT_MON_QTY]			= {DummyMsgIsOrphan,		IntMsgIsOrphan			};
static FnMsgMalfunctionProcessing	UserMsgMalfunctionProcessing[ST_INT_MON_QTY]	= {DummyMsgMalfunctionProcessing,IntMsgMalfunctionProcessing };
static FnMsgIsDone			UserMsgIsDone[ST_INT_MON_QTY]			= {DummyMsgIsDone,		IntMsgIsDone			};
static FnMsgSetMsgSAF                   UserMsgSetMsgSAF[ST_INT_MON_QTY]                = {DummyMsgSetMsgSAF,		IntMsgSetMsgSAF                    };
/******************************************************************************/
static int				nMonitoringSharedMemId;
static int				nMonitoringSemId;
/*static int				nMonitoringTabSemId;*/
/*static int				tab_nMonitoringTabSemId[QMD_TAB_SEM_SIZE];*/
static volatile int*			nMonitoringDaemonState=&dummy;
static volatile int*			nBalPut;
static char*				memRefPointer;
/* YDA20171016(PROD00048309) */
static volatile int*			nSwiMonVer;
static volatile int*			nSwiMonStVer;
static volatile int*			nLocMaxNbMsg;

static TSMsgMappingList*		tab_SwitchMsgsList;



static const TSMsgMappingFlow InitialMsgMappingFlow = {
	.msgId			= 0,
	.msg_type		="",
	.processing_code	="",
	.src_node_id		="",
	.src_resource_id	="",
	.src_resource_pid	= 0,
	.dst_node_id		="",
	.dst_resource_id	="",
	.dst_resource_pid	= 0,
	.times			= {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	.status			=QMS_MSG_FLOW_FREE,
	.msg_stan		="",
	.origin_code		='0',
	.response_code		="",
	.isOrphanFlag		='N',
	.isSrcTimeOutFlag	='N',
	.isDstTimeOutFlag	='N',
	.isMalfunctionFlag	='N',
	.dirty_flag		='N',
	/*.time_exception	= {0,0},*/ /*YDA20180209 : Use times[0] instead of time_exception*/
	.isDone			= 'N',
	.c_ver			= SWIMON_ST_VER, /*YDA20171021(PROD00048309)*/
	/* YDA20171113 (PROD00048309) */
	.reason_code		="",
	.terminal_id		="",
	.bank_code		="",
	.saf_ind 		='0'

};
/******************************************************************************/
void InitMsgMappingFlow(TSMsgMappingFlow* MsgMappingFlow)
{
	memcpy((char*)MsgMappingFlow,(char*)&InitialMsgMappingFlow,sizeof(TSMsgMappingFlow));
}
/******************************************************************************/
/******************************************************************************\
* OpenMsgFlowInfoQueue()
* This function should be called by the monitored interface to initialize
* the memory used to save msg mapping infos
\******************************************************************************/

int OpenMsgFlowInfoQueue(int nIndexCtx)
{
	char*				ptr;
	struct sembuf		sb;
	pid_t           	nPid;
	int					nResult;
	int					i;

	trace_event ("Start  OpenMsgFlowInfoQueue()", PROCESSING);

	nMonitoringDaemonState = &dummy;

	/* There's no need to do this check, we will know once the daemon is up
	nResult = GetSwitchMonResourceId(nIndexCtx, NodeId, SMResourceId);
	if (nResult != OK )
	{
		eIntMonState = ST_NO_MONITORING;
		trace_event("End  OpenMsgFlowInfoQueue(NOK): Switch monitoring daemon not defined", PROCESSING);
		return NOK;
	}

	nResult = CheckResource(nIndexCtx, SMResourceId, &nPid);
	if( nResult != OK || ( kill ( nPid , 0 ) != 0 ))
	{
		eIntMonState = ST_NO_MONITORING;
		trace_event ("End  OpenMsgFlowInfoQueue(NOK): Switch monitoring daemon not running", PROCESSING);
		return NOK;
	}*/

	eIntMonState = ST_NO_MONITORING;/* YDA20171021(PROD00048309) */

	/*get the Shared memory id */
	/* YDA20171021(PROD00048309) : set size to 0, it should already exist, we never create here */
	nMonitoringSharedMemId = shmget(QMD_MEM_KEY, 0 /* sh_memory_size + sizeof(nMonitoringDaemonState) + sizeof(nBalPut) */, 0666);
	if( nMonitoringSharedMemId < 0 )
	{
		trace_message (TRACE,"End  OpenMsgFlowInfoQueue(NOK): Unable to get shared memory, shmget failed [%d]", errno);
		return NOK;
	}

	/* map the shared memory to our space and get a pointer to it*/
	memRefPointer = shmat(nMonitoringSharedMemId, NULL, 0);
	if( memRefPointer == (char *)(-1) )
	{
		trace_message (TRACE,"End  OpenMsgFlowInfoQueue(NOK): Unable to map shared memory, shmat failed [%d]", errno);
		return NOK;
	}

	/* the shared memory structure is: 
	SWMON version || SWMON Max number of messages || SWMON daemon state || SWMON last position of putted message || space for data */

	ptr=memRefPointer;

	/* Get SWMON version */
	nSwiMonVer = (int*)ptr; ptr+=sizeof(int);

	/* Get SWMON Struct version */
	nSwiMonStVer = (int*)ptr; ptr+=sizeof(int);

	/* Get SWMON Max number of messages */
	nLocMaxNbMsg = (int*)ptr; ptr+=sizeof(int);

	/* Get SWMON daemon state */
	nMonitoringDaemonState = (int*)ptr; ptr+=sizeof(int);

	/* Get SWMON last position of putted message*/
	nBalPut = (int*)ptr; ptr+=sizeof(int);

        /* Get SWMON messages list */
	tab_SwitchMsgsList = (TSMsgMappingList*)ptr;

	trace_message (TRACE,"dbParam[SwmonVers %d,structVers %d]",SWIMON_VER, SWIMON_ST_VER);
	trace_message (TRACE,"smParam[SwmonVers %d,structVers %d,MaxNbrMsg %d]",*nSwiMonVer,*nSwiMonStVer,*nLocMaxNbMsg);
	trace_message (TRACE,"smParam[SwmonState %d,nBalPut %d,structSize %d]",*nMonitoringDaemonState,*nBalPut, sizeof(*tab_SwitchMsgsList));


	/* allocate 1 semaphore */
	nMonitoringSemId = semget(QMD_SEM_KEY, 1, 0666);
	if( nMonitoringSemId < 0 )
	{
		trace_message (ERROR, "End  OpenMsgFlowInfoQueue(NOK): Unable to get semaphore[%d]",errno);
		return NOK;
	}


	/*
	for (i = 0; i < QMD_TAB_SEM_SIZE; i++)
	{
		tab_nMonitoringTabSemId[i] = semget(QMD_TAB_SEM_SIZE + i, QMD_TAB_SEM_PER_SET, 0666);
		if (tab_nMonitoringTabSemId[i] < 0)
		{
			eIntMonState = ST_NO_MONITORING;
			trace_message(ERROR, "End  OpenMsgFlowInfoQueue(NOK): Unable to get msg array semaphores(%d)[%d]", i, errno);
			return NOK;
		}
	}
	*/
	if( (*nSwiMonVer) != SWIMON_VER )
	{
		trace_message(ERROR,"==== Shared mem exists with different version: Existing[%lu] Daemon Version[%lu]",(*nSwiMonVer),SWIMON_VER);
		return NOK;
	}

	if( (*nSwiMonStVer) != SWIMON_ST_VER )
	{
		trace_message(ERROR,"==== Shared mem exists with different struct version: Existing[%lu] Daemon Version[%lu]",(*nSwiMonStVer),SWIMON_ST_VER);
		return NOK;
	}

	eIntMonState = ST_MONITORING;

	trace_message (TRACE,"End  OpenMsgFlowInfoQueue(OK)");
	return OK;
}
/******************************************************************************\
* CloseMsgFlowInfoQueue()
* This function should be called by the monitored interface before exiting
\******************************************************************************/
int CloseMsgFlowInfoQueue()
{
	trace_event ("Start  CloseMsgFlowInfoQueue()", PROCESSING);

	/*Unmap the shared memory from our space*/
	shmdt(memRefPointer);
	eIntMonState = ST_NO_MONITORING;

	trace_event ("End  CloseMsgFlowInfoQueue(OK)", PROCESSING);
	return OK;
}
/******************************************************************************/
P7_INLINE int swimon_check_msg_id(msg_id_t msgId)
{
	int 	nMagicVal = GET_MAGIC_VALUE(msgId);

	if( nMagicVal == SWIMON_MAGIC_VAL ){
		trace_message(PROCESSING, "[SWMON]: swimon_check_msg_id (OK)");
		return OK;
	}
	trace_message(PROCESSING, "[SWMON]: swimon_check_msg_id (NOK)[ %d != %lu ]", nMagicVal,SWIMON_MAGIC_VAL);

	return NOK;

}
/******************************************************************************/
P7_INLINE msg_id_t MsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan )
{
	/* YDA20171113 (PROD00048309) */
	char	sBankCode[6 + 1];
	char	sTerminalId[8 + 1];

	trace_message(PROCESSING, "[SWMON]: MsgReqReceived(%d,%d)", (*nMonitoringDaemonState),eIntMonState);

	/* YDA20171113 (PROD00048309) */
	memset(sBankCode,'\0',sizeof(sBankCode));
	memset(sTerminalId,'\0',sizeof(sTerminalId));

	return (*UserMsgReqReceived[eIntMonState])(resource_id,msg_type,processing_code,msg_stan, sBankCode, sTerminalId);
}
/******************************************************************************/
P7_INLINE msg_id_t MsgIsoReqReceived(char* resource_id, TSIsoInfo* msgInfo)
{
	char	sMsgType[4 + 1];
	char	sProcessingCode[6 + 1];
	char	sSTAN[6 + 1];
	int		nLength;
	/* YDA20171113 (PROD00048309) */
	msg_id_t	nWrappedMsgId, nIndexedMsgId;
	char	sBankCode[6 + 1];
	char	sTerminalId[8 + 1];
	char	sSecurityData[99 + 1];
	char	sSourceNodeId[NODE_ID_LEN + 1];


	trace_message(PROCESSING, "[SWMON]: MsgIsoReqReceived(%d,%d)", (*nMonitoringDaemonState),eIntMonState);

	memset(sMsgType,'\0',sizeof(sMsgType));
	memset(sProcessingCode,'\0',sizeof(sProcessingCode));
	memset(sSTAN,'\0',sizeof(sSTAN));
	/* YDA20171113 (PROD00048309) */
	memset(sBankCode,'\0',sizeof(sBankCode));
	memset(sTerminalId,'\0',sizeof(sTerminalId));
	memset(sSecurityData,'\0',sizeof(sSecurityData));
	memset(sSourceNodeId,'\0',sizeof(sSourceNodeId));

	sprintf(sMsgType,"%.04d",msgInfo->nMsgType);
	GetIsoField(ISO_PROC_CODE,msgInfo,sProcessingCode,&nLength);
	GetIsoField(ISO_AUDIT_NBR,msgInfo,sSTAN,&nLength);
	/* YDA20171113 (PROD00048309) */
	GetIsoField(ISO_ACQR_ID,msgInfo,sBankCode,&nLength);
	GetIsoField(ISO_TERMINAL_NBR,msgInfo,sTerminalId,&nLength);

/*Start SKA17102022 PLUTONL-5303*/
	nWrappedMsgId = (*UserMsgReqReceived[eIntMonState])(resource_id,sMsgType,sProcessingCode,sSTAN, sBankCode, sTerminalId);
	if( nWrappedMsgId == INVALID_MSG_ID )
	{
		trace_message(ERROR,"[SWMON]: MsgIsoReqReceived: Invalid Msg Id");
		return INVALID_MSG_ID;
	}
	nIndexedMsgId = GET_MSG_ID(nWrappedMsgId);

	GetIsoField(ISO_SECURITY_DATA, msgInfo, sSecurityData, &nLength);
	GetSecurityData( SOURCE_NODE_ID, sSecurityData, sSourceNodeId, &nLength);

	LockMsgFlowMapping(nIndexedMsgId);
	memcpy(tab_SwitchMsgsList[nIndexedMsgId].MsgMappingFlow.src_node_id, sSourceNodeId, sizeof(tab_SwitchMsgsList[nIndexedMsgId].MsgMappingFlow.src_node_id) - 1);
	UnlockMsgFlowMapping(nIndexedMsgId);

	return nWrappedMsgId;
/*End SKA17102022 PLUTONL-5303*/
}
/******************************************************************************/
/*PLUTONL-3762*/
P7_INLINE msg_id_t MsgIsoLineReqReceived(char* resource_id, struct timeval*  pktime)
{

	trace_message(PROCESSING, "[SWMON]: MsgIsoLineReqReceived(%d,%d)", (*nMonitoringDaemonState),eIntMonState);
	return (*UserMsgLineReqReceived[eIntMonState])(resource_id,pktime);
	

}

/******************************************************************************/
P7_INLINE void MsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid)
{
	trace_message(PROCESSING, "[SWMON]: MsgReqForwarded(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgReqForwarded[eIntMonState])(GET_MSG_ID(msgId),dst_resource_id,dst_resource_pid);
}
/******************************************************************************/
P7_INLINE void MsgFwdReqRetrieved(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgFwdReqRetrieved(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdReqRetrieved[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgFwdReqSent(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgFwdReqSent(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdReqSent[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgFwdRepReceived(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgFwdRepReceived(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdRepReceived[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgFwdRepReplied(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgFwdRepReplied(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdRepReplied[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgRepRetrieved(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgRepRetrieved(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgRepRetrieved[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
/*PLUTONL-3762*/
P7_INLINE void MsgBalRepRetrieved(msg_id_t msgId,struct timeval* pktime)
{
	trace_message(PROCESSING, "[SWMON]: MsgBalRepRetrieved(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgBalRepRetrieved[eIntMonState])(GET_MSG_ID(msgId),pktime);
}
/***************************************************************************/
/*PLUTONL-3762*/
P7_INLINE void MsgBalFwdReqRetrieved(msg_id_t msgId,struct timeval*  pktime)
{
	trace_message(PROCESSING, "[SWMON]: (%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgBalFwdReqRetrieved[eIntMonState])(GET_MSG_ID(msgId),pktime);

}
/******************************************************************************/
P7_INLINE void MsgRepSent(msg_id_t msgId, char* response_code)
{
	trace_message(PROCESSING, "[SWMON]: MsgRepSent(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgRepSent[eIntMonState])(GET_MSG_ID(msgId),response_code);
}
/******************************************************************************/
P7_INLINE void MsgIsoRepSent(msg_id_t msgId,  TSIsoInfo* msgInfo)
{
	char	sReasonCode[6 + 1];
	char	sDestResourceId[8 + 1];
	char	sDestNodeId[4 + 1];
	char	sRespCode[6 + 1];
	int	nLength;
	char	sTmpField  [  999];

	trace_message(PROCESSING, "[SWMON]: MsgIsoRepSent(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}

	memset(sReasonCode, '\0', sizeof(sReasonCode)-1);
	memset(sDestResourceId, '\0', sizeof(sDestResourceId)-1);
	memset(sDestNodeId, '\0', sizeof(sDestNodeId)-1);
	memset(sRespCode, '\0', sizeof(sRespCode)-1);

	if (GetIsoField ( ISO_MSG_REASON, msgInfo, sTmpField, &nLength) == SUCCES)
	{
		memcpy(sReasonCode, sTmpField, sizeof(sReasonCode)-1);
	}
	if (GetIsoField ( ISO_RESPONSE_CODE, msgInfo, sTmpField, &nLength) == SUCCES)
	{
		memcpy(sRespCode, sTmpField, sizeof(sRespCode)-1);
	}
	switch ( msgInfo->nMsgType )
	{
		case 1420 :
		case 1421 :
		case 1120 :
		case 1121 :
			if (GetIsoField ( ISO_ADTNL_DATA_PRIV, msgInfo, sTmpField, &nLength) == SUCCES)
			{
				TSTlvPrivate	PrivateTlvInfo;
				InitTlvPrivate(&PrivateTlvInfo);
				AnalyseTlvPrivate(sTmpField,nLength,&PrivateTlvInfo);
				if (GetTlvPrivate(ROUTING_CODE,&PrivateTlvInfo,sTmpField,&nLength) == SUCCES)
				{
					memcpy(sDestResourceId, sTmpField, sizeof(sDestResourceId)-1);
					memcpy(sDestNodeId, NodeId, sizeof(sDestResourceId)-1);
				}
			}
			break;
	}

	(*UserMsgIsoRepSent[eIntMonState])(GET_MSG_ID(msgId),sRespCode,sReasonCode,sDestResourceId,sDestNodeId);
}
/******************************************************************************/
P7_INLINE void MsgBrsRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id)
{

	trace_message(PROCESSING, "[SWMON]: MsgBrsRepSent(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}

	(*UserMsgIsoRepSent[eIntMonState])(GET_MSG_ID(msgId),response_code,reason_code,dst_resource_id,dst_node_id);
}
/******************************************************************************/
P7_INLINE void MsgTimeout(msg_id_t msgId, char* resource_id)
{
	trace_message(PROCESSING, "[SWMON]: MsgTimeout(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgTimeout[eIntMonState])(GET_MSG_ID(msgId),resource_id);
}
/******************************************************************************/
P7_INLINE void MsgIsOrphan(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgIsOrphan(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgIsOrphan[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgMalfunctionProcessing(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgMalfunctionProcessing(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgMalfunctionProcessing[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgIsDone(msg_id_t msgId)
{
	trace_message(PROCESSING, "[SWMON]: MsgIsDone(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgIsDone[eIntMonState])(GET_MSG_ID(msgId));
}
/******************************************************************************/
P7_INLINE void MsgSetMsgSAF(msg_id_t msgId, char saf_ind)
{
	trace_message(PROCESSING, "[SWMON]: MsgSetMsgSAF(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgSetMsgSAF[eIntMonState])(GET_MSG_ID(msgId), saf_ind);
}
/****************************************************************************/
/*PLUTONL-3762*/
P7_INLINE void MsgLog(msg_id_t msgId, char *sTlvBuffer )
{

	trace_message(PROCESSING, "[SWMON]: MsgLog(%d,%d,%lu)", (*nMonitoringDaemonState),eIntMonState,(unsigned long)msgId);
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgLog[eIntMonState])(GET_MSG_ID(msgId),sTlvBuffer);
}

/******************************************************************************/
/*SKARROUMI PLUTONL-4833*/
P7_INLINE void MsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime){
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdReqRetrievedTS[eIntMonState])(GET_MSG_ID(msgId),pktime);
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]*/
}
/******************************************************************************/
P7_INLINE void MsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime){
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdReqSentTS[eIntMonState])(GET_MSG_ID(msgId),pktime);
}
/******************************************************************************/
P7_INLINE void MsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime){
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdRepReceivedTS[eIntMonState])(GET_MSG_ID(msgId),pktime);
}
/******************************************************************************/
P7_INLINE void MsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id){
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	(*UserMsgFwdRepRepliedTS[eIntMonState])(GET_MSG_ID(msgId),pktime, dst_node_id);
}
/*SKARROUMI PLUTONL-4833*/
/******************************************************************************/
int	LockMsgFlowMapping(msg_id_t msgId)
{
	/*
	struct sembuf		sb;
	struct timeval		tv;
	struct timespec		ts;
	int					nResult;
	int					nMonitoringTabSemId;

	nMonitoringTabSemId = tab_SwitchMsgsList[msgId].nSemTabIndex;
	sb.sem_num = tab_SwitchMsgsList[msgId].nSemIndex;
	sb.sem_op = -1;
	sb.sem_flg = 0;


	gettimeofday(&tv, NULL);

	ts.tv_sec  = 1;
	ts.tv_nsec = 0;
	nResult = semtimedop(nMonitoringTabSemId, &sb, 1,&ts);
	if( nResult < 0 )
	{
		trace_message(ERROR,">>>>>>>> LockMsgFlowMapping(ERROR[%d]) <<<<<<<<<<<",errno);
		return NOK;
	}
	*/
	return OK;
}
/******************************************************************************/
int	LockBlkMsgFlowMapping(msg_id_t msgId)
{
	/*
	struct sembuf		sb;
	int					nMonitoringTabSemId;

	nMonitoringTabSemId = tab_SwitchMsgsList[msgId].nSemTabIndex;
	sb.sem_num = tab_SwitchMsgsList[msgId].nSemIndex;
	sb.sem_op = -1;
	sb.sem_flg = 0;

	semop(nMonitoringTabSemId, &sb, 1);
	*/

	return OK;
}
/******************************************************************************/
int	UnlockMsgFlowMapping(msg_id_t msgId)
{
	/*
	struct sembuf		sb;
	int					nMonitoringTabSemId;

	nMonitoringTabSemId = tab_SwitchMsgsList[msgId].nSemTabIndex;
	sb.sem_num = tab_SwitchMsgsList[msgId].nSemIndex;
	sb.sem_op = 1;
	sb.sem_flg = 0;
	semop(nMonitoringTabSemId, &sb, 1);
	*/

	return OK;
}
/******************************************************************************/
/*PROD00055494(3): Make sure we are following the correct state flow*/
int CtrlMsgFlowState(unsigned int curr_status, unsigned int next_status )
{
	if( curr_status == QMS_MSG_FLOW_FREE )
		return NOK;

	return OK;
}
/******************************************************************************/
static msg_id_t IntMsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan, char* bank_code, char* terminal_id)
{
	msg_id_t			nMsgId,nLocMsgId;
	TSMsgMappingFlow	MsgMappingFlow;
	char				sResourceCode[2 + 1];


	InitMsgMappingFlow(&MsgMappingFlow);

	memcpy(MsgMappingFlow.msg_type,msg_type,sizeof(MsgMappingFlow.msg_type) - 1);
	memcpy(MsgMappingFlow.processing_code,processing_code, sizeof(MsgMappingFlow.processing_code) - 1);
	memcpy(MsgMappingFlow.src_node_id,NodeId,sizeof(MsgMappingFlow.src_node_id) - 1);
	memcpy(MsgMappingFlow.src_resource_id,resource_id,sizeof(MsgMappingFlow.src_resource_id) - 1);
	memcpy(MsgMappingFlow.msg_stan,msg_stan,sizeof(MsgMappingFlow.msg_stan) - 1 );

	memset(sResourceCode,'\0',sizeof(sResourceCode));
	memcpy(sResourceCode,resource_id,sizeof(sResourceCode));

	if( memcmp(sResourceCode,POS_PPWM_RES_CODE,2) == 0 ||
		memcmp(sResourceCode,POS_CB2A_RES_CODE,2) == 0 ||
		memcmp(sResourceCode,ATM_RES_CODE,2) == 0)
	{
		memcpy(MsgMappingFlow.bank_code,bank_code,sizeof(MsgMappingFlow.bank_code) - 1);
		memcpy(MsgMappingFlow.terminal_id,terminal_id,sizeof(MsgMappingFlow.terminal_id) - 1);
	}

	MsgMappingFlow.status = QMS_REQ_RECV;
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&MsgMappingFlow.times[MsgMappingFlow.status],NULL);*/

	get_local_time(&MsgMappingFlow.times[MsgMappingFlow.status]);

	/*NAB2508 PLUTONL-3482*/
    
	MsgMappingFlow.src_resource_pid=getpid();
	if( msg_type[2] == '4' ) /* there will be no response for notifications */
	{
		MsgMappingFlow.isDone = 'Y';
	}
	/*MsgMappingFlow.dirty_flag='Y';*/

	nLocMsgId = PSwMonPutMsg(&MsgMappingFlow);
	/* 20210918 *//*PLUTONL-3549*/
	if( nLocMsgId == INVALID_MSG_ID )
	{
		trace_message(ERROR,"[SWMON]: IntMsgReqReceived: Invalid msg id[%s,%s,%s,%s,%s]",msg_type, processing_code, msg_stan, bank_code, terminal_id);
		return INVALID_MSG_ID;
	}
	
	nMsgId = 0;
	nMsgId=SET_MAGIC_VALUE(nMsgId,SWIMON_MAGIC_VAL);
	nMsgId=SET_MSG_ID(nMsgId,nLocMsgId);


	trace_message(ERROR,"[SWMON]: IntMsgReqReceived: %lu[%s,%s,%s,%s,%s]",nMsgId, msg_type, processing_code, msg_stan, bank_code, terminal_id);
	return nMsgId;
}
/******************************************************************************/
static msg_id_t IntMsgLineReqReceived(char* resource_id,struct timeval*  pktime)
{
	msg_id_t			nMsgId,nLocMsgId;
	TSMsgMappingFlow	MsgMappingFlow;
	char				sResourceCode[2 + 1];


	InitMsgMappingFlow(&MsgMappingFlow);

	
	memcpy(MsgMappingFlow.src_node_id,NodeId,sizeof(MsgMappingFlow.src_node_id) - 1);

	memcpy(MsgMappingFlow.src_resource_id,resource_id,sizeof(MsgMappingFlow.src_resource_id) - 1);



	MsgMappingFlow.status = QMS_REQ_RECV;
	if( pktime != 0 )
	{
		MsgMappingFlow.times[MsgMappingFlow.status] = (*pktime);
	}
	else
	{
		/*gettimeofday(&MsgMappingFlow.times[MsgMappingFlow.status],NULL);*//*NAB2508 PLUTONL-3482*/
		get_local_time(&MsgMappingFlow.times[MsgMappingFlow.status]);

	}

	MsgMappingFlow.src_resource_pid=getpid();

	nLocMsgId = PSwMonPutMsg(&MsgMappingFlow);
	/* ZKO28022022 *//*PLUTONL-4429*/
	if( nLocMsgId == INVALID_MSG_ID )
	{
		trace_message(ERROR,"[SWMON]: IntMsgLineReqReceived: Invalid msg id");
		return INVALID_MSG_ID;
	}
	nMsgId = 0;
	nMsgId=SET_MAGIC_VALUE(nMsgId,SWIMON_MAGIC_VAL);
	nMsgId=SET_MSG_ID(nMsgId,nLocMsgId);


	trace_message(ERROR,"[SWMON]: IntMsgLineReqReceived: %lu",nMsgId);

	return nMsgId;
}

/***************************************************************************/
static  void IntMsgBalFwdReqRetrieved(msg_id_t msgId,struct timeval*  pktime)
{
	if( msgId == INVALID_MSG_ID )
		return;
	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REQ_RETRIEVED) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgBalFwdReqRetrieved(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}
	LockMsgFlowMapping(msgId);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id, NodeId, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id) - 1);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = QMS_FWD_REQ_RETRIEVED;

	if (pktime != 0)
	{

		tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]=(*pktime);
	}
	else
	{
		/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status], NULL);*/
		get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);

	}

	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgBalFwdReqRetrieved: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/*PLUTONL-3762*/
/******************************************************************************/
static void IntMsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_REQ_FWD) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgReqForwarded(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	LockMsgFlowMapping(msgId);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_resource_id,dst_resource_id,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_resource_id) - 1);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = QMS_REQ_FWD;
	tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_resource_pid=dst_resource_pid;
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);
	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgReqForwarded: %lu[%s,%s,%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan,dst_resource_id,dst_resource_pid);
}
/******************************************************************************/
static P7_INLINE void SetMsgMappingFlowStatus(msg_id_t msgId, int nStatus)
{
	if( msgId == INVALID_MSG_ID )
		return;

	LockMsgFlowMapping(msgId);

	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = nStatus;
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);
	/*NAB2508 PLUTONL-3482*/

	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/

	UnlockMsgFlowMapping(msgId);
}
/******************************************************************************/
static void IntMsgFwdReqRetrieved(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REQ_RETRIEVED) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgFwdReqRetrieved(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id, NodeId, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id) - 1);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = QMS_FWD_REQ_RETRIEVED;
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status], NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);

	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag = 'Y';*/

	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgFwdReqRetrieved: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);

}
/******************************************************************************/
static void IntMsgFwdReqSent(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REQ_SENT) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgFwdReqSent(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	SetMsgMappingFlowStatus(msgId,QMS_FWD_REQ_SENT);

	trace_message(ERROR,"[SWMON]: IntMsgFwdReqSent: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgFwdRepReceived(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REP_RECV) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgFwdRepReceived(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	SetMsgMappingFlowStatus(msgId,QMS_FWD_REP_RECV);

	trace_message(ERROR,"[SWMON]: IntMsgFwdRepReceived: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgFwdRepReplied(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REP_REPLIED) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgFwdRepReplied(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	SetMsgMappingFlowStatus(msgId,QMS_FWD_REP_REPLIED);

	trace_message(ERROR,"[SWMON]: IntMsgFwdRepReplied: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgRepRetrieved(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_REP_RETRIEVED) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgRepRetrieved(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	SetMsgMappingFlowStatus(msgId,QMS_REP_RETRIEVED);

	trace_message(ERROR,"[SWMON]: IntMsgRepRetrieved: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
/*PLUTONL-3762*/
static void IntMsgBalRepRetrieved(msg_id_t msgId,struct timeval*  pktime)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_REP_RETRIEVED) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgBalRepRetrieved(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	SetMsgMappingFlowStatus(msgId,QMS_REP_RETRIEVED);
	
	if (pktime != 0)
	{

		tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]=(*pktime);
	}
	else
	{
		/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status], NULL);*/
		get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);

	}
	

	trace_message(ERROR,"[SWMON]: IntMsgBalRepRetrieved: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgRepSent(msg_id_t msgId, char* response_code)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_REP_SENT) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgRepSent(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.response_code,response_code,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.response_code) - 1);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = QMS_REP_SENT;

    /*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);
	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/
	tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone='Y';

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgRepSent: %lu[%s,%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan,response_code);
}
/******************************************************************************/
static void IntMsgIsoRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): make sure we are following the expected state chronology*/
	if( CtrlMsgFlowState(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_REP_SENT) != OK )
	{
		trace_message(ERROR,"[SWMON]: IntMsgRepSent(Invalid status): %lu[%d]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.status);
		IntMsgMalfunctionProcessing(msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.response_code,response_code,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.response_code) - 1);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = QMS_REP_SENT;
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[tab_SwitchMsgsList[msgId].MsgMappingFlow.status]);
	/*NAB2508 PLUTONL-3482*/

	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/
	if( strlen(reason_code) > 0 )
	{
		memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.reason_code,reason_code,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.reason_code) - 1);
	}
	if( strlen(dst_resource_id) > 0 )
	{
		memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_resource_id,dst_resource_id,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_resource_id) - 1);
		memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id,dst_node_id,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id) - 1);
	}
	tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone='Y';

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgIsoRepSent: %lu[%s,%s,%s,%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan, response_code, reason_code, dst_resource_id);
}
/******************************************************************************/
/*PLUTONL-3762*/
static void IntMsgLog(msg_id_t msgId, char *sTlvBuffer )
{
	int nLength;
	char sMsgType[4+1];
	char sProcCode[24 + 1];
	char sSTAN[6+1];
	char sResourceCode[6+1];
	char bank_code[6+1];
	char terminal_id[8+1];

	memset(sMsgType,'\0',sizeof(sMsgType));
	memset(sProcCode,'\0',sizeof(sProcCode));
	memset(sSTAN,'\0',sizeof(sSTAN));
	memset(sResourceCode,'\0',sizeof(sResourceCode));
	memset(bank_code,'\0',sizeof(bank_code));
	memset(terminal_id,'\0',sizeof(terminal_id));

	if( msgId == INVALID_MSG_ID )
		return;

	/*MsgType*/
	GetTlvBuffer (AUTO_MSG_TYPE, sTlvBuffer, sMsgType ,  &nLength);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_type, sMsgType, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_type) - 1);
	if( sMsgType[2] == '4' ) /* there will be no response for notifications */
	{
		tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone = 'Y';
	}
	/*ProcCode*/
	GetTlvBuffer (AUTO_PROCESSING_CODE, sTlvBuffer, sProcCode ,  &nLength);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.processing_code, sProcCode+6, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.processing_code) - 1);
    /*InternalStan*/
	GetTlvBuffer (AUTO_INTERNAL_STAN, sTlvBuffer, sSTAN ,  &nLength);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan, sSTAN, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan) - 1);
    /*ResourceCode*/
	GetTlvBuffer (AUTO_CAPTURE_CODE, sTlvBuffer, sResourceCode ,  &nLength);

	GetTlvBuffer (AUTO_ACQ_INST_IDENT_CODE, sTlvBuffer, bank_code ,  &nLength);

	GetTlvBuffer (AUTO_CARD_ACC_TERMINAL, sTlvBuffer, terminal_id ,  &nLength);

	if( memcmp(sResourceCode,POS_PPWM_RES_CODE,2) == 0 ||
		memcmp(sResourceCode,POS_CB2A_RES_CODE,2) == 0 ||
		memcmp(sResourceCode,ATM_RES_CODE,2) == 0)
	{
		memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.bank_code,bank_code,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.bank_code) - 1);
		memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.terminal_id,terminal_id,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.terminal_id) - 1);
	}

	trace_message(ERROR,"[SWMON]: IntMsgLog: %lu",msgId);

}

/******************************************************************************/
static void IntMsgTimeout(msg_id_t msgId, char* resource_id)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): if current slot is in free state then do not log as exception, there's no detail to be logged*/
	if( tab_SwitchMsgsList[msgId].MsgMappingFlow.status == QMS_MSG_FLOW_FREE )
	{
		trace_message(ERROR,"[SWMON]: IntMsgTimeout(Free) do nothing: %lu",msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	if( strncmp(tab_SwitchMsgsList[msgId].MsgMappingFlow.src_resource_id,resource_id,sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.src_resource_id) - 1) == 0 )
	{
		tab_SwitchMsgsList[msgId].MsgMappingFlow.isSrcTimeOutFlag = 'Y';
	}
	else
	{
		tab_SwitchMsgsList[msgId].MsgMappingFlow.isDstTimeOutFlag = 'Y';
	}
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE]);
	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone = 'Y';*//*time out function should not terminate the flow*/

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgTimeout: %lu[%s,%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan,resource_id);
}
/******************************************************************************/
void IntMsgIsOrphan(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): if current slot is in free state then do not log as exception, there's no detail to be logged*/
	if( tab_SwitchMsgsList[msgId].MsgMappingFlow.status == QMS_MSG_FLOW_FREE )
	{
		trace_message(ERROR,"[SWMON]: IntMsgIsOrphan(Free) do nothing: %lu",msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	tab_SwitchMsgsList[msgId].MsgMappingFlow.isOrphanFlag = 'Y';
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE]);
	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/
	tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone = 'Y';

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgIsOrphan: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
void IntMsgMalfunctionProcessing(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): if current slot is in free state then do not log as exception, there's no detail to be logged*/
	if( tab_SwitchMsgsList[msgId].MsgMappingFlow.status == QMS_MSG_FLOW_FREE )
	{
		trace_message(ERROR,"[SWMON]: IntMsgMalfunctionProcessing(Free) do nothing: %lu",msgId);
		return;
	}

	LockMsgFlowMapping(msgId);

	tab_SwitchMsgsList[msgId].MsgMappingFlow.isMalfunctionFlag = 'Y';
	/*NAB2508 PLUTONL-3482*/
	/*gettimeofday(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE],NULL);*/
	get_local_time(&tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_MSG_FLOW_FREE]);
	/*NAB2508 PLUTONL-3482*/
	/*tab_SwitchMsgsList[msgId].MsgMappingFlow.dirty_flag='Y';*/
	tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone = 'Y';

	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgMalfunctionProcessing: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgIsDone(msg_id_t msgId)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): if current slot is in free state then do not log as exception, there's no detail to be logged*/
	if( tab_SwitchMsgsList[msgId].MsgMappingFlow.status == QMS_MSG_FLOW_FREE )
	{
		trace_message(ERROR,"[SWMON]: IntMsgIsDone(Free) do nothing: %lu",msgId);

		return;
	}

	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.isDone = 'Y';
	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgIsDone: %lu[%s]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan);
}
/******************************************************************************/
static void IntMsgSetMsgSAF(msg_id_t msgId, char saf_ind)
{
	if( msgId == INVALID_MSG_ID )
		return;

	/*PROD00055494(3): if current slot is in free state then do not log as exception, there's no detail to be logged*/
	if( tab_SwitchMsgsList[msgId].MsgMappingFlow.status == QMS_MSG_FLOW_FREE )
	{
		trace_message(ERROR,"[SWMON]: IntMsgSetMsgSAF(Free) do nothing: %lu",msgId);

		return;
	}

	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.saf_ind = saf_ind;
	UnlockMsgFlowMapping(msgId);

	trace_message(ERROR,"[SWMON]: IntMsgSetMsgSAF: %lu[%s,%c]",msgId,tab_SwitchMsgsList[msgId].MsgMappingFlow.msg_stan,saf_ind);
}
/******************************************************************************/
/*SKARROUMI PLUTONL-4833*/
static void IntMsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime){
	if( msgId == INVALID_MSG_ID )
		return;

	if (pktime == 0)
		return;
	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = MAX(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REQ_RETRIEVED);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_FWD_REQ_RETRIEVED] = (*pktime);
	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgFwdReqRetrievedTS with timestamp %lu:%lu", pktime->tv_sec, pktime->tv_usec);
}
/******************************************************************************/
static void IntMsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime){
	if( msgId == INVALID_MSG_ID )
		return;

	if (pktime == 0)
		return;
	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = MAX(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REQ_SENT);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_FWD_REQ_SENT] = (*pktime);
	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgFwdReqSentTS with timestamp %lu:%lu", pktime->tv_sec, pktime->tv_usec);
}
/******************************************************************************/
static void IntMsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime){
	if( msgId == INVALID_MSG_ID )
		return;

	if (pktime == 0)
		return;
	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = MAX(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REP_RECV);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_FWD_REP_RECV] = (*pktime);
	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgFwdRepReceivedTS with timestamp %lu:%lu", pktime->tv_sec, pktime->tv_usec);
}
/******************************************************************************/
static void IntMsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id){
	if( msgId == INVALID_MSG_ID )
		return;
	
	if (pktime == 0)
		return;
	LockMsgFlowMapping(msgId);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.status = MAX(tab_SwitchMsgsList[msgId].MsgMappingFlow.status, QMS_FWD_REP_REPLIED);
	tab_SwitchMsgsList[msgId].MsgMappingFlow.times[QMS_FWD_REP_REPLIED] = (*pktime);
	memcpy(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id, dst_node_id, sizeof(tab_SwitchMsgsList[msgId].MsgMappingFlow.dst_node_id) - 1);
	UnlockMsgFlowMapping(msgId);
	trace_message(ERROR,"[SWMON]: IntMsgFwdRepRepliedTS with timestamp %lu:%lu", pktime->tv_sec, pktime->tv_usec);
}
/*SKARROUMI PLUTONL-4833*/
/******************************************************************************/
/******************************************************************************/
/*PLUTONL-3762*/
static msg_id_t DummyMsgLineReqReceived(char* resource_id, struct timeval*  pktime)
{
	trace_event("DummyMsgLineReqReceived",PROCESSING);
	return INVALID_MSG_ID;
}
/******************************************************************************/
/*PLUTONL-3762*/
static void DummyMsgLog(msg_id_t msgId, char *sTlvBuffer)
{
	trace_event("DummyMsgLog",PROCESSING);
	return ;
}
/******************************************************************************/
/*PLUTONL-3762*/

static void DummyMsgBalFwdReqRetrieved(msg_id_t msgId,struct timeval*  pktime)
{
	trace_event("DummyMsgBalFwdReqRetrieved",PROCESSING);
	return;
}
/*PLUTONL-3762*/
/******************************************************************************/

static msg_id_t DummyMsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan, char* bank_code, char* terminal_id)
{
	trace_event("DummyMsgReqReceived",PROCESSING);
	return INVALID_MSG_ID;
}
/******************************************************************************/
static void DummyMsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid)
{
	trace_event("DummyMsgReqForwarded",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgFwdReqRetrieved(msg_id_t msgId)
{
	trace_event("DummyMsgFwdReqRetrieved",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgFwdReqSent(msg_id_t msgId)
{
	trace_event("DummyMsgFwdReqSent",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgFwdRepReceived(msg_id_t msgId)
{
	trace_event("DummyMsgFwdRepReceived",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgFwdRepReplied(msg_id_t msgId)
{
	trace_event("DummyMsgFwdRepReplied",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgRepRetrieved(msg_id_t msgId)
{
	trace_event("DummyMsgRepRetrieved",PROCESSING);
	return;
}
/******************************************************************************/
/*PLUTONL-3762*/
static void DummyMsgBalRepRetrieved(msg_id_t msgId, struct timeval*  pktime)
{
	trace_event("DummyMsgBalRepRetrieved",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgRepSent(msg_id_t msgId, char* response_code)
{
	trace_event("DummyMsgRepSent",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgIsoRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id)
{
	trace_event("DummyMsgIsoRepSent",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgTimeout(msg_id_t msgId, char* resource_id)
{
	trace_event("DummyMsgTimeout",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgIsOrphan(msg_id_t msgId)
{
	trace_event("DummyMsgIsOrphan",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgMalfunctionProcessing(msg_id_t msgId)
{
	trace_event("DummyMsgMalfunctionProcessing",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgIsDone(msg_id_t msgId)
{
	trace_event("DummyMsgIsDone",PROCESSING);
	return;
}
/******************************************************************************/
static void DummyMsgSetMsgSAF(msg_id_t msgId, char saf_ind)
{
        trace_event("DummyMsgSetMsgSAF",PROCESSING);
        return;
}
/******************************************************************************/
/*SKARROUMI PLUTONL-4833*/
static void DummyMsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime)
{
        trace_event("DummyMsgFwdReqRetrievedTS",PROCESSING);
        return;
}
/******************************************************************************/
static void DummyMsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime)
{
        trace_event("DummyMsgFwdReqSentTS",PROCESSING);
        return;
}
/******************************************************************************/
static void DummyMsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime)
{
        trace_event("DummyMsgFwdRepReceivedTS",PROCESSING);
        return;
}
/******************************************************************************/
static void DummyMsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id)
{
        trace_event("DummyMsgFwdRepRepliedTS",PROCESSING);
        return;
}
/*SKARROUMI PLUTONL-4833*/
/******************************************************************************/
/* YDA20171021(PROD00048309) */
int DoOrphanResourceMsg(TSMsgMappingFlow* pkMsgMappingFlow, char *orphanFlag)
{
	/* Empty bal, gen event, log message in auth_saf*/
	pid_t				nPid;
	TSBalMessage		sBalMsg;

	trace_message(TRACE,"Start DoOrphanResourceMsg [%lu][%d][%d][%d]", pkMsgMappingFlow->msgId, pkMsgMappingFlow->status, pkMsgMappingFlow->src_resource_pid, pkMsgMappingFlow->dst_resource_pid);

	if(memcmp(orphanFlag,"OD",2) != 0)
	{
		nPid = pkMsgMappingFlow->dst_resource_pid;
	}
	else if(memcmp(orphanFlag,"OS",2) != 0)
	{
		nPid = pkMsgMappingFlow->src_resource_pid;
	}

	if( memcmp(NodeId,pkMsgMappingFlow->src_node_id,4) == 0 &&
		kill ( nPid , 0 ) != 0 )
	{
		trace_message(TRACE,">>>>>>PID [%d] Down",nPid);
		while(msgrcv( nBalMsg , &sBalMsg, LG_MAX, nPid , IPC_NOWAIT) != -1)
		{
			trace_message(TRACE,">>>>>>>>sBalMsg[%lu]", sBalMsg.sText.sHeader.msgId);
		}
	}

	trace_event ("End DoOrphanResourceMsg(OK)", PROCESSING);
	return OK;
}
/******************************************************************************/
static msg_id_t PSwMonPutMsg(TSMsgMappingFlow * sMsgMappingFlow)
{
	struct sembuf 		sb;
	struct timeval  	tv;
	struct timespec 	ts;
	int					nResult;
	struct timeval		currTime;
	TSMsgMappingFlow	kMsgMappingFlow;
	char 				sMsgState[3]="OK";

	/*gettimeofday(&tv, NULL);*/
	get_local_time(&tv);/*PLUTONL-3482*/
	
	ts.tv_sec  = 1;
	ts.tv_nsec = 0;

	sb.sem_num = SEM_ID_TAB;
	sb.sem_op = -1;
	/* sb.sem_flg = 0; 20210918*//*PLUTONL-3549*/
	sb.sem_flg = SEM_UNDO;
	
	/*TODO: Why don't we wait for this a fixed time instead of blocking the interface forever?*/
	nResult = semtimedop(nMonitoringSemId, &sb, 1,&ts);
	/*nResult = semop(nMonitoringSemId, &sb, 1);*/
	if( nResult < 0 )
	{
		trace_message(ERROR,"END PSwMonPutMsg(KO): semtimedop failed [%d]",errno);
		return INVALID_MSG_ID;
	}

	/*EBE140528: msgId 0 will be reserved to differentiate msgId's that are not managed*/
	/*YDA20171021(PROD00048309) : msgId 0 not reserved anymore, we use magic value to differentiate msgId's that are not managed*/
	/*
	if( (*nBalPut) == 0 )
		(*nBalPut)= (*nBalPut) + 1;
	*/

	/*Check if we are overwriting a busy slot, it's ok to do that if the swi mon is not freeing the slots for some reason,
 	however we should help to watch for orphan messages
	PS: we should never get here: the time_to_orphan VS the switch TPS VS the size of shared mm should be enough to let the swi_mon clear old messages*/

	/* if the message is 2 minutes old, then something is wrong*/
	/*gettimeofday(&currTime,NULL);*/
	get_local_time (&currTime);/*PLUTONL-3482*/
	if(tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_REQ_FWD].tv_sec > 0 &&
			tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_FWD_REQ_RETRIEVED].tv_sec <= 0 &&
			tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_REQ_FWD].tv_sec + 120 <  currTime.tv_sec){
		memcpy(sMsgState,"OD",2);
	}

	if( tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_FWD_REP_REPLIED].tv_sec > 0 &&
			tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_REP_RETRIEVED].tv_sec <= 0 &&
			tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow.times[QMS_FWD_REP_REPLIED].tv_sec + 120 <  currTime.tv_sec)
	{
		memcpy(sMsgState,"OS",2);
	}


	if(memcmp(sMsgState,"OK",2) != 0)
	{
		memcpy((char*)&kMsgMappingFlow,(char *) &(tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow),sizeof(TSMsgMappingFlow));
	}


	/*YDA20180103 : msgId 0 not used anymore*/
	/*EBE140528: msgId 0 will be reserved to differentiate msgId's that are not managed*/
	/*if( (*nBalPut) == 0 )
	{
		(*nBalPut)= (*nBalPut) + 1;
	}*/

	sMsgMappingFlow->msgId = (*nBalPut);

	memcpy ((char *) &(tab_SwitchMsgsList[(*nBalPut)].MsgMappingFlow), (char *) sMsgMappingFlow,sizeof(TSMsgMappingFlow));
	(*nBalPut) = ((*nBalPut) +1) % ( (*nLocMaxNbMsg) );
	sb.sem_op = 1;
	semop(nMonitoringSemId, &sb, 1);

	if(memcmp(sMsgState,"OK",2) != 0)
	{
		DoOrphanResourceMsg(&kMsgMappingFlow,sMsgState);
		memcpy(sMsgState,"OK",2);
	}

	return(sMsgMappingFlow->msgId);
}
/******************************************************************************/
/*
static int PSwMonWaitEntry()
{
	struct sembuf sb;
	sb.sem_num = SEM_ID_SLOTS;
	sb.sem_op = -1;
	sb.sem_flg = 0;
	semop(nMonitoringSemId, &sb, 1);
	return(OK);
}
*/
/******************************************************************************/
/*SKARROUMI PLUTONL-4833*/
static int BuildMsgFlowMapping(TSMsgMappingFlow* MsgMappingFlow, char* buffer)
{
	int						nLength;
	char					data[1024];
	int						i;
	TlvInfo		TlvMsgMappingFlow;

	trace_event("Start BuildMsgFlowMapping()",PROCESSING);

	memset(data,'\0',sizeof(data));
    InitTSMsgMappingFlow(&TlvMsgMappingFlow);

	sprintf(data,"%.012lu",MsgMappingFlow->msgId);
	AddTlvInfoField(MSGFLOW_MSG_ID,&TlvMsgMappingFlow,data,12);

	AddTlvInfoField(MSGFLOW_MSG_TYPE,&TlvMsgMappingFlow,MsgMappingFlow->msg_type,4);
	AddTlvInfoField(MSGFLOW_PROCESSING,&TlvMsgMappingFlow,MsgMappingFlow->processing_code,2);
	AddTlvInfoField(MSGFLOW_SRC_NODE_ID,&TlvMsgMappingFlow,MsgMappingFlow->src_node_id,4);
	AddTlvInfoField(MSGFLOW_SRC_RES_ID,&TlvMsgMappingFlow,MsgMappingFlow->src_resource_id,6);

	sprintf(data,"%.08d",MsgMappingFlow->src_resource_pid);
	AddTlvInfoField(MSGFLOW_SRC_PID,&TlvMsgMappingFlow,data,8);

	trace_event("Again BuildMsgFlowMapping()",PROCESSING);

	if( strlen(MsgMappingFlow->dst_node_id) > 0 )
	{
		AddTlvInfoField(MSGFLOW_DST_NODE_ID,&TlvMsgMappingFlow,MsgMappingFlow->dst_node_id,4);
	}

	if( strlen(MsgMappingFlow->dst_resource_id) > 0 )
	{
		AddTlvInfoField(MSGFLOW_DST_RES_ID,&TlvMsgMappingFlow,MsgMappingFlow->dst_resource_id,6);
	}

	/*if( strlen(MsgMappingFlow->dst_resource_pid) > 0)
	{
		
	}*/
	
	sprintf(data,"%.08d",MsgMappingFlow->dst_resource_pid);
	AddTlvInfoField(MSGFLOW_TAG_DST_PID,&TlvMsgMappingFlow,data,8);

	sprintf(data,"%.01d",MsgMappingFlow->status);
	AddTlvInfoField(MSGFLOW_STATUS,&TlvMsgMappingFlow,data,1);

	AddTlvInfoField(MSGFLOW_MSG_STAN,&TlvMsgMappingFlow,MsgMappingFlow->msg_stan,6);

	data[0] = MsgMappingFlow->origin_code;
	AddTlvInfoField(MSGFLOW_ORIGIN_CODE,&TlvMsgMappingFlow,data,1);

	
	for( i = QMS_REQ_RECV; i <= QMS_REP_SENT; i++ )
	{
		if( MsgMappingFlow->status < i || MsgMappingFlow->times[i].tv_sec <= 0)
		{
			continue;
		}

		sprintf(data,"%.010lu",MsgMappingFlow->times[i].tv_sec);
		sprintf(data+10,"%.06lu",MsgMappingFlow->times[i].tv_usec);
		data[13]='\0';
		switch(i)
		{
		case	QMS_REQ_RECV:
			AddTlvInfoField(MSGFLOW_TIME_REQ_RECV,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_REQ_FWD:
			AddTlvInfoField(MSGFLOW_TIME_REQ_FWD,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_FWD_REQ_RETRIEVED:
			AddTlvInfoField(MSGFLOW_TIME_FWD_REQ_RETRV,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_FWD_REQ_SENT:
			AddTlvInfoField(MSGFLOW_TIME_REQ_SENT,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_FWD_REP_RECV:
			AddTlvInfoField(MSGFLOW_TIME_REP_RECV,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_FWD_REP_REPLIED:
			AddTlvInfoField(MSGFLOW_TIME_REP_REPLIED,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_REP_RETRIEVED:
			AddTlvInfoField(MSGFLOW_TIME_REP_RETRV,&TlvMsgMappingFlow,data,13);
			break;
		case 	QMS_REP_SENT:
			AddTlvInfoField(MSGFLOW_TIME_REP_SENT,&TlvMsgMappingFlow,data,13);
			break;
		default:
			trace_event("Unknown state!",ERROR);
			break;

		}

	}

	if( strlen(MsgMappingFlow->response_code) > 0 )
	{
		AddTlvInfoField(MSGFLOW_RESP_CODE,&TlvMsgMappingFlow,MsgMappingFlow->response_code,3);
	}

	data[0] = MsgMappingFlow->isOrphanFlag;
	AddTlvInfoField(MSGFLOW_ORPHAN_FLAG,&TlvMsgMappingFlow,data,1);

	data[0] = MsgMappingFlow->isSrcTimeOutFlag;
	AddTlvInfoField(MSGFLOW_SRC_TIMEOUT_FLAG,&TlvMsgMappingFlow,data,1);

	data[0] = MsgMappingFlow->isDstTimeOutFlag;
	AddTlvInfoField(MSGFLOW_DST_TIMEOUT_FLAG,&TlvMsgMappingFlow,data,1);

	data[0] = MsgMappingFlow->isMalfunctionFlag;
	AddTlvInfoField(MSGFLOW_MALFUNCTION_FLAG,&TlvMsgMappingFlow,data,1);

	if( MsgMappingFlow->isOrphanFlag == 'Y'		||
		MsgMappingFlow->isSrcTimeOutFlag == 'Y'	||
		MsgMappingFlow->isDstTimeOutFlag == 'Y'	||
		MsgMappingFlow->isMalfunctionFlag == 'Y' )
	{
		sprintf(data,"%.010lu",MsgMappingFlow->times[QMS_MSG_FLOW_FREE].tv_sec);
		sprintf(data+10,"%.06lu",MsgMappingFlow->times[QMS_MSG_FLOW_FREE].tv_usec);
		data[13]='\0';
		AddTlvInfoField(MSGFLOW_TIME_EXCEPTION,&TlvMsgMappingFlow,data,13);
	}

	data[0] = MsgMappingFlow->isDone;
	AddTlvInfoField(MSGFLOW_DONE_FLAG,&TlvMsgMappingFlow,data,1);

	if( strlen(MsgMappingFlow->reason_code) > 0 )
	{
		AddTlvInfoField(MSGFLOW_REASON_CODE,&TlvMsgMappingFlow,MsgMappingFlow->reason_code,4);
	}
	if( strlen(MsgMappingFlow->terminal_id) > 0 )
	{
		AddTlvInfoField(MSGFLOW_TERMINAL_ID,&TlvMsgMappingFlow,MsgMappingFlow->terminal_id,8);
	}
	if( strlen(MsgMappingFlow->bank_code) > 0 )
	{
		AddTlvInfoField(MSGFLOW_BANK_CODE,&TlvMsgMappingFlow,MsgMappingFlow->bank_code,6);
	}

	data[0] = MsgMappingFlow->saf_ind;
	AddTlvInfoField(MSGFLOW_SAF_IND,&TlvMsgMappingFlow,data,1);

	DumpTlvInfo(&TlvMsgMappingFlow);
	
	nLength = BuildTlvInfoBuffer(buffer,&TlvMsgMappingFlow);
	trace_event("End BuildMsgFlowMapping()",PROCESSING);
	return nLength;

}
/******************************************************************************/
int GetMsgMappingFlowSlot(msg_id_t msgId, char* buffer, int* nLength){

	TSMsgMappingFlow localTSMsgMappingFlow;
	if( swimon_check_msg_id(msgId) != OK )
	{
		trace_event("[SWMON]: Msg not monitored",PROCESSING);
		return;
	}
	localTSMsgMappingFlow = tab_SwitchMsgsList[GET_MSG_ID(msgId)].MsgMappingFlow;
	*nLength = BuildMsgFlowMapping(&localTSMsgMappingFlow, buffer);
	if (*nLength <= 0)
		return NOK;
	
	return OK;
}
/*SKARROUMI PLUTONL-4833*/
/******************************************************************************/
