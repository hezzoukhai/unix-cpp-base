#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <define.h>
#include <queue.h>
#include <resources.h>
#include <commands.h>
#include <param_arg.h>
/******************************************************************************/
#define		MAX_BRIDGE_COMMANDS		64
#define		BRIDGE_TIMEOUT_VALUE	3

/******************************************************************************/
typedef struct {
	char             szBufferBal 	[LG_MAX];
	int              nLenBal;
	int              free;
	int              status;
	int              flag_bal;
	int              co_number;
	pthread_t        tid;  
	pthread_mutex_t  BridgeMutex;
	pthread_cond_t   BridgeCond;
} TSBridgeCommand;
/******************************************************************************/
static	TSBridgeCommand	tab_BridgeCommands[MAX_BRIDGE_COMMANDS];
static	pthread_t	gs_thr_tid;
static	int			gs_nNextSlot = 0;
static  int			gs_nLastSlot = -1;
pthread_mutex_t		gs_NextSlotMutex   = PTHREAD_MUTEX_INITIALIZER;
static	char		gs_sLocalNodeId[NODE_ID_LEN + 1];
static	char		gs_sLocBridgeResId[RESOURCE_ID_LEN + 1];
static	pid_t		gs_nLocBridgePid;

static	char		gs_sIASNodeId[NODE_ID_LEN + 1];
static	char		gs_sIASBridgeResId[RESOURCE_ID_LEN + 1];
static	pid_t		gs_nIASBridgePid;
/******************************************************************************/
static int  GetNextBridgeCoNumber(int nIndex)
{
	tab_BridgeCommands[nIndex].co_number = (tab_BridgeCommands[nIndex].co_number + 1) % 100;
	return (tab_BridgeCommands[nIndex].co_number);
}
/*
static int CmpBridgeCoNumber(int nIndex, int nCoNumber)
{
	if (tab_BridgeCommands[nIndex].co_number == nCoNumber)
		return (OK);
	else
		return (NOK);
}
*/

static int InfoBridgeThread(int nIndex)
{
	pthread_mutex_lock(&(tab_BridgeCommands[nIndex].BridgeMutex));
	pthread_cond_signal(&(tab_BridgeCommands[nIndex].BridgeCond));
	pthread_mutex_unlock(&(tab_BridgeCommands[nIndex].BridgeMutex));
	return(0);
}

static int GetBridgeThreadIndex(pthread_t tid)
{
	int             i;
	int				nSize;

	nSize = sizeof(tab_BridgeCommands)/sizeof(tab_BridgeCommands[0]);

	for (i = 0; i < nSize; i++) {
		if (tab_BridgeCommands[i].tid == tid)
			break;
	}
	if (i >= nSize)
		return (-1);
	return (i);
}
/******************************************************************************/
/*
This function should now be called with BridgeMutex acquired or undefined behavior will happen
*/

int WaitCmdResponse(int nIndex, int nTimer, char *szBuffer)
{
	struct timeval  tv;
	struct timespec ts;
	int             retour;

	tab_BridgeCommands[nIndex].flag_bal = WAITING;
	if (gettimeofday(&tv, NULL) < 0) {
		fprintf(stderr, "gettimeofday Error : %d\n", errno);
		return (NOK);
	}
	ts.tv_sec = tv.tv_sec + nTimer;
	ts.tv_nsec = tv.tv_usec * 1000;


	if ((retour = pthread_cond_timedwait(&(tab_BridgeCommands[nIndex].BridgeCond),
					     &(tab_BridgeCommands[nIndex].BridgeMutex),
					     &ts)) < 0) 
	{
		if (retour == ETIMEDOUT) 
		{
			retour = TIMEOUT;
			tab_BridgeCommands[nIndex].flag_bal = TIMEOUT;
		} 
		else 
		{
			retour = NOK;
			tab_BridgeCommands[nIndex].flag_bal = NOK;
		}
	} 
	else 
	{
		if (retour == ETIMEDOUT) 
		{
			retour = TIMEOUT;
			tab_BridgeCommands[nIndex].flag_bal = TIMEOUT;
		} 
		else 
		{
			retour = OK;
			tab_BridgeCommands[nIndex].flag_bal = OK;
			memcpy(szBuffer, tab_BridgeCommands[nIndex].szBufferBal, tab_BridgeCommands[nIndex].nLenBal);
		}
	}
	
	return (retour);
}

int BridgeQuery(int nIndiceCtx, char* sDstNodeId, char *BuffSnd, char *BuffRcv)
{
	int					BalStatus;
	TSBridgeExchange	sBalMessage;
	char				sLine[MAX_LINE_TRC];
	int					nSlot;
	int					nResult;
	int					i;
	int					nFound;

	
	
	nFound = NOK;
	pthread_mutex_lock(&gs_NextSlotMutex);
	for( i= 0; i < MAX_BRIDGE_COMMANDS; i++ )
	{
		nSlot = gs_nNextSlot;
		gs_nNextSlot = (gs_nNextSlot + 1)%MAX_BRIDGE_COMMANDS;
		if( tab_BridgeCommands[nSlot].free == 1 )
		{
			nFound = OK;
			break;
		}
	}
	pthread_mutex_unlock(&gs_NextSlotMutex);
	if( nFound != OK )
	{
		trace_event(">>> All Slots are busy", ERROR);
		return NOK;
	}

	tab_BridgeCommands[nSlot].free = 0;

	/*Make sure our bridge is still UP*/
	if( gs_nLocBridgePid == 0 || kill ( gs_nLocBridgePid , 0 ) != 0 )
	{
		sprintf(sLine, "Bridge changed PID :%d", gs_nLocBridgePid);
		trace_event(sLine, TRACE);
		nResult = GetBEBridgeResourceId(nIndiceCtx,gs_sLocalNodeId,gs_sLocBridgeResId,&gs_nLocBridgePid,
											 gs_sIASNodeId,gs_sIASBridgeResId,&gs_nIASBridgePid);
		if( nResult != OK )
		{
 			sprintf(sLine, "Error to get bridge resource id:%s", gs_sLocalNodeId);
			trace_event(sLine, ERROR);
			return NOK;
		}
		sprintf(sLine, "New Bridge PID :%d", gs_nLocBridgePid);
		trace_event(sLine, TRACE);
	}

	

	memset((char *) &sBalMessage, 0, sizeof(TSBridgeExchange));
	memset(tab_BridgeCommands[gs_nNextSlot].szBufferBal, 0, sizeof(tab_BridgeCommands[gs_nNextSlot].szBufferBal));
	sBalMessage.sHeader.nIndex = nSlot;
	sBalMessage.sHeader.tid = pthread_self();
	sBalMessage.sHeader.nCoNumber = GetNextBridgeCoNumber(nSlot);

	strncpy(sBalMessage.sHeader.nPartnerNodeId,sDstNodeId,4);
	sBalMessage.sHeader.nPartnerNodeId[4]='\0';

	sBalMessage.sHeader.nPartnerId = gs_nLocBridgePid;
	sBalMessage.sHeader.nSourceId = getpid();
	sBalMessage.sHeader.nCommand = BRIDGE_QUERY;
	memcpy(sBalMessage.sBody, BuffSnd, strlen(BuffSnd));
	sBalMessage.sHeader.nLen = strlen(BuffSnd);

	
	if (debug_option == 1)
	{
 		sprintf(sLine, "TO Bridge:%d-->%s|", sBalMessage.sHeader.nLen, sBalMessage.sBody);
		trace_event(sLine, TRACE);
	}

	pthread_mutex_lock(&(tab_BridgeCommands[nSlot].BridgeMutex));

	if (WriteCmdMsg(nBalBridge, &sBalMessage) != OK) 
	{
		trace_event("Can Not Write Message To Bridge Queue", PROCESSING);
		trace_event("End  BridgeQuery ( NOK )", PROCESSING);
		tab_BridgeCommands[nSlot].free = 1;
		pthread_mutex_unlock(&(tab_BridgeCommands[nIndiceCtx].BridgeMutex));
		return (NOK);
	}
	memset((char *) &sBalMessage, 0, sizeof(TSBridgeExchange));


	BalStatus = WaitCmdResponse(nSlot, BRIDGE_TIMEOUT_VALUE, BuffRcv);


	pthread_mutex_unlock(&(tab_BridgeCommands[nSlot].BridgeMutex));

	tab_BridgeCommands[nSlot].free = 1;

	if (BalStatus != OK) 
	{
		trace_event("Time Out Situation With Bridge", PROCESSING);
		trace_event("End BridgeQuery ( NOK )", PROCESSING);

		/*let's refresh our identifiers*/
		nResult = GetBEBridgeResourceId(nIndiceCtx,	gs_sLocalNodeId,gs_sLocBridgeResId,&gs_nLocBridgePid,
													gs_sIASNodeId,gs_sIASBridgeResId,&gs_nIASBridgePid);
		if( nResult != OK )
		{
 			sprintf(sLine, "Error to get bridge resource id:%s", gs_sLocalNodeId);
			trace_event(sLine, TRACE);
			return OK;
		}
		return (NOT_AVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "FROM Bridge: <--%s|", BuffRcv);
		trace_event(sLine, TRACE);
	}
	trace_event("End BridgeQuery ( OK )", PROCESSING);
	return (OK);
}

int BridgeIASQuery(int nIndiceCtx, char *BuffSnd, char *BuffRcv)
{
	return BridgeQuery(nIndiceCtx,gs_sIASNodeId,BuffSnd,BuffRcv);
}


static void*  thread_Bridge(void *arg)
{
	TSBridgeExchange	kBridgeCmd;
	TSArgParam			*p_arg;
	int					nIndex;
	char				sLine[MAX_LINE_TRC];

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) 
	{
		if (ReadCmdMsg(nBalBridge, getpid(), !IPC_NOWAIT, &kBridgeCmd) < 0) 
		{
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}

		sprintf(sLine,"ReadCmdMsg(nIndex: %d, nCoNumber:%d)",kBridgeCmd.sHeader.nIndex,kBridgeCmd.sHeader.nCoNumber);
		trace_event(sLine, PROCESSING);

		if ((kBridgeCmd.sHeader.nIndex >= 0) && (kBridgeCmd.sHeader.nIndex < MAX_BRIDGE_COMMANDS)) 
		{
			nIndex = kBridgeCmd.sHeader.nIndex;
			if( tab_BridgeCommands[nIndex].co_number == kBridgeCmd.sHeader.nCoNumber )
			{
				memcpy(tab_BridgeCommands[nIndex].szBufferBal, kBridgeCmd.sBody,kBridgeCmd.sHeader.nLen);
				tab_BridgeCommands[nIndex].nLenBal = kBridgeCmd.sHeader.nLen;
				InfoBridgeThread(nIndex);
			}
		}
	}
}
/******************************************************************************/
int  InitTabBridge(int nIndexCtx, char* szNodeId)
{
	int						i;
	int						nSize;
	char					sLine[MAX_LINE_TRC];
	int						nResult;
	pthread_mutexattr_t		mattr;
	pthread_condattr_t		cattr;


	pthread_mutexattr_init(&mattr);
	pthread_condattr_init(&cattr);


	nSize = sizeof(tab_BridgeCommands)/sizeof(tab_BridgeCommands[0]);

	for (i = 0; i < nSize; i++) 
	{
		tab_BridgeCommands[i].co_number = 0;
		tab_BridgeCommands[i].free = 1;
		tab_BridgeCommands[i].status = -1;
		tab_BridgeCommands[i].tid = -1;
		tab_BridgeCommands[i].nLenBal = 0;
		memset(tab_BridgeCommands[i].szBufferBal, 0, sizeof(tab_BridgeCommands[i].szBufferBal));
		pthread_mutex_init(&(tab_BridgeCommands[i].BridgeMutex), &mattr);
		pthread_cond_init(&(tab_BridgeCommands[i].BridgeCond), &cattr);
	}

	nBalBridge = -1;


	nBalBridge = OpenBalCom(BAL_BDG);
	if (nBalBridge < 0)
	{
		trace_event("Can Not Open Bridge Queue", FATAL);
		exit(0);
	}
	EmptyQueue(nBalBridge, getpid());

	if (pthread_create(&gs_thr_tid, NULL, thread_Bridge, NULL) < 0)
	{
		fprintf(stderr, "pthread_create (Bridge thr) Error %d\n", errno);
		exit(0);
	}

	memset(gs_sLocalNodeId,'\0',sizeof(gs_sLocalNodeId));
	memset(gs_sLocBridgeResId,'\0',sizeof(gs_sLocBridgeResId));

	memset(gs_sIASNodeId,'\0',sizeof(gs_sIASNodeId));
	memset(gs_sIASBridgeResId,'\0',sizeof(gs_sIASBridgeResId));

	strncpy(gs_sLocalNodeId,szNodeId,sizeof(gs_sLocalNodeId) - 1);
	
	
	nResult = GetBEBridgeResourceId(nIndexCtx,gs_sLocalNodeId,gs_sLocBridgeResId,&gs_nLocBridgePid,
											 gs_sIASNodeId,gs_sIASBridgeResId,&gs_nIASBridgePid);
	if( nResult != OK )
	{
 		sprintf(sLine, "Error to get bridge resources id:%s", gs_sLocalNodeId);
		trace_event(sLine, TRACE);
		return NOK;
	}


	return OK;
}

