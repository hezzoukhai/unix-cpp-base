#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <define.h>
#include <thread_var.h>
#define _INT_VAR_
#include <thr_hsm.h>
#undef _INT_VAR_
#include <queue.h>
#include <param_arg.h>
#include <resources.h>
#include <hsm_inc.h>


/* EBE140421: adding tab_HsmCtx definition */
P7_DEF_SYM TSHsmCtx  tab_HsmCtx [ MAX_THREAD ];
P7_DEF_SYM char	g_dec_table[SIZE_KEY + 1];
/******************************************************************************/

#define HSM_VERIF_ID		0
#define HSM_PROD_ID			1


#define MAX_HSM_RES			2
#define MAX_HSM_TYPES		2

P7_DEF_SYM  char gs_hsm_resources[MAX_HSM_TYPES][MAX_HSM_RES][RESOURCE_ID_LEN + 1];
/******************************************************************************/
/* EBE140920: Hide so nobody use it
void*
thread_HSM(void *arg)
{
	TSHsmExchange   sBalMsg;
	TSArgParam     *p_arg;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) {
		if (ReadBalHsm(nBalHsm, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) {
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}
		if ((sBalMsg.sHeader.nIndex >= 0) && (sBalMsg.sHeader.nIndex < MAX_THREAD)) 
		{
			if (CmpCoNumber(sBalMsg.sHeader.nIndex, sBalMsg.sHeader.nCoNumber) == OK) 
			{
				memcpy(tab_HsmCtx[sBalMsg.sHeader.nIndex].szBufferBal, sBalMsg.sBody,sBalMsg.sHeader.nLen);
				tab_HsmCtx[sBalMsg.sHeader.nIndex].nLenBal = sBalMsg.sHeader.nLen;
				InfoHsmThread(sBalMsg.sHeader.nIndex);
			}
		}
	}
}

void 
InitTabHsmCtx(int mode)
{
	int             i;
	pthread_mutexattr_t mattr;
	pthread_condattr_t cattr;


	pthread_mutexattr_init(&mattr);
	pthread_condattr_init(&cattr);


	for (i = 0; i < MAX_THREAD; i++) {
		tab_HsmCtx[i].co_number = 0;
		tab_HsmCtx[i].free = 1;
		tab_HsmCtx[i].status = -1;
		tab_HsmCtx[i].tid = -1;
		tab_HsmCtx[i].nLenBal = 0;
		memset(tab_HsmCtx[i].szBufferBal, 0, LG_MAX);
		pthread_mutex_init(&(tab_HsmCtx[i].HsmMutex), &mattr);
		pthread_cond_init(&(tab_HsmCtx[i].HsmCond), &cattr);
	}

	if (mode == VERIF_MODE)
		nBalHsm = OpenBalCom(BAL_HSM_VERIF);
	else
		nBalHsm = OpenBalCom(BAL_HSM_PROD);

	if (nBalHsm < 0) {
		trace_event("Can Not Open Hsm Queue", PROCESSING);
		exit(0);
	}
	EmptyQueue(nBalHsm, getpid());

	/ **** Creation du Thread  HSM  *** /
	if (pthread_create(&(tab_ThrInfo[HSM_THREAD_ID].tid), NULL,
			   thread_HSM, NULL) < 0) {
		fprintf(stderr, "pthread_create (HSM) Error %d\n", errno);
		exit(0);
	}
}
*/

int 
GetNextCoNumber(int nIndex)
{
	tab_HsmCtx[nIndex].co_number = (tab_HsmCtx[nIndex].co_number + 1) % 100;
	return (tab_HsmCtx[nIndex].co_number);
}

int 
CmpCoNumber(int nIndex, int nCoNumber)
{
	if (tab_HsmCtx[nIndex].co_number == nCoNumber)
		return (OK);
	else
		return (NOK);
}

int 
GetThreadHsmIndex(pthread_t tid)
{
	int             i;

	for (i = 0; i < MAX_THREAD; i++) {
		if (tab_HsmCtx[i].tid == tid)
			break;
	}
	if (i >= MAX_THREAD)
		return (-1);
	return (i);
}
/*
EBE140921: This function should now be called with HsmMutex acquired or undefined behavior will happen
*/
int WaitHsmResponse(int nIndex, int nTimer, char *szBuffer)
{
	struct timeval  tv;
	struct timespec ts;
	int             retour;

	tab_HsmCtx[nIndex].flag_bal = WAITING;
	if (gettimeofday(&tv, NULL) < 0) {
		fprintf(stderr, "gettimeofday Error : %d\n", errno);
		return (NOK);
	}
	ts.tv_sec = tv.tv_sec + nTimer;
	ts.tv_nsec = tv.tv_usec * 1000;

	/*EBE140921: Moved to calling function(HsmQuery,...etc).
	* The HSM may response before this function get called and hence the response is missed
	* and this function returns with timeout
	pthread_mutex_lock(&(tab_HsmCtx[nIndex].HsmMutex));
	*/

	if ((retour = pthread_cond_timedwait(&(tab_HsmCtx[nIndex].HsmCond),
					     &(tab_HsmCtx[nIndex].HsmMutex),
					     &ts)) < 0) {
		if (retour == ETIMEDOUT) {
			retour = TIMEOUT;
			tab_HsmCtx[nIndex].flag_bal = TIMEOUT;
		} else {
			retour = NOK;
			tab_HsmCtx[nIndex].flag_bal = NOK;
		}
	} else {
		if (retour == ETIMEDOUT) {
			retour = TIMEOUT;
			tab_HsmCtx[nIndex].flag_bal = TIMEOUT;
		} else {
			retour = OK;
			tab_HsmCtx[nIndex].flag_bal = OK;
			memcpy(szBuffer, tab_HsmCtx[nIndex].szBufferBal, tab_HsmCtx[nIndex].nLenBal);
		}
	}
	/*EBE140921: Moved to calling function(HsmQuery,...etc).
	pthread_mutex_unlock(&(tab_HsmCtx[nIndex].HsmMutex));
	*/

	return (retour);
}

int 
InfoHsmThread(int nIndex)
{
	pthread_mutex_lock(&(tab_HsmCtx[nIndex].HsmMutex));
	pthread_cond_signal(&(tab_HsmCtx[nIndex].HsmCond));
	pthread_mutex_unlock(&(tab_HsmCtx[nIndex].HsmMutex));
	/*SKO290509*/ return(0);
}

/* Start EBE140716 */


void* 
thread_HSM_Verif(void *arg)
{
	TSHsmExchange   sBalMsg;
	TSArgParam     *p_arg;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) {
		if (ReadBalHsm(nBalHsmVerf, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) 
		{
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}
		if ((sBalMsg.sHeader.nIndex >= 0) && (sBalMsg.sHeader.nIndex < MAX_THREAD)) 
		{
			if (CmpCoNumber(sBalMsg.sHeader.nIndex, sBalMsg.sHeader.nCoNumber) == OK) 
			{
				memcpy(tab_HsmCtx[sBalMsg.sHeader.nIndex].szBufferBal, sBalMsg.sBody,sBalMsg.sHeader.nLen);
				tab_HsmCtx[sBalMsg.sHeader.nIndex].nLenBal = sBalMsg.sHeader.nLen;
				InfoHsmThread(sBalMsg.sHeader.nIndex);
			}
		}
		memset((char *)&sBalMsg, 0, sizeof(TSHsmExchange));
	}
}


void* 
thread_HSM_Prod(void *arg)
{
	TSHsmExchange   sBalMsg;
	TSArgParam     *p_arg;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) {
		if (ReadBalHsm(nBalHsmProd, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) 
		{
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}
		if ((sBalMsg.sHeader.nIndex >= 0) && (sBalMsg.sHeader.nIndex < MAX_THREAD)) 
		{
			if (CmpCoNumber(sBalMsg.sHeader.nIndex, sBalMsg.sHeader.nCoNumber) == OK) 
			{
				memcpy(tab_HsmCtx[sBalMsg.sHeader.nIndex].szBufferBal, sBalMsg.sBody,sBalMsg.sHeader.nLen);
				tab_HsmCtx[sBalMsg.sHeader.nIndex].nLenBal = sBalMsg.sHeader.nLen;
				InfoHsmThread(sBalMsg.sHeader.nIndex);
			}
		}
	}
}


int 
InitTabHsm(int nIndexCtx, char* szNodeId, int mode)
{
	int             i;
	int				nSize;
	char			sInterfaceMode[12];
	char			sLine[MAX_LINE_TRC];
	pthread_mutexattr_t mattr;
	pthread_condattr_t cattr;


	pthread_mutexattr_init(&mattr);
	pthread_condattr_init(&cattr);

	memset(sInterfaceMode, '\0', sizeof(sInterfaceMode));
	memset(gs_hsm_resources, '\0', sizeof(gs_hsm_resources));
	memset(g_dec_table, '\0', sizeof(g_dec_table));
	
	if (mode & HSM_ACQ_MODE)
	{
		sInterfaceMode[0] = 'A';
	}
	else
	{
		sInterfaceMode[0] = 'I';
	}


	nSize = sizeof(tab_HsmCtx)/sizeof(tab_HsmCtx[0]);

	for (i = 0; i < nSize; i++) 
	{
		tab_HsmCtx[i].co_number = 0;
		tab_HsmCtx[i].free = 1;
		tab_HsmCtx[i].status = -1;
		tab_HsmCtx[i].tid = -1;
		tab_HsmCtx[i].nLenBal = 0;
		memset(tab_HsmCtx[i].szBufferBal, 0, sizeof(tab_HsmCtx[i].szBufferBal));
		pthread_mutex_init(&(tab_HsmCtx[i].HsmMutex), &mattr);
		pthread_cond_init(&(tab_HsmCtx[i].HsmCond), &cattr);
	}

	/*PROD00053984*/
	if (GetGlobalsVar(nIndexCtx,PG_DECIMALISATION_TABLE,g_dec_table)!= OK) 
	{
		memcpy(g_dec_table,Validation_data,SIZE_KEY);
	}

	nBalHsm = -1;
	if (mode & HSM_VERIF_MODE)
	{
		if ( GetHSMVCodes(	nIndexCtx, 
							szNodeId,
							sInterfaceMode,
							gs_hsm_resources[HSM_VERIF_ID][0],
							gs_hsm_resources[HSM_VERIF_ID][1]) == OK)
		{
			nBalHsmVerf = OpenBalCom(BAL_HSM_VERIF);
			if (nBalHsmVerf < 0)
			{
				trace_event("Can Not Open Hsm Verif Queue", FATAL);
				exit(0);
			}
			EmptyQueue(nBalHsmVerf, getpid());
			if (pthread_create(&(tab_ThrInfo[HSM_VERIF_THREAD_ID].tid), NULL, thread_HSM_Verif, NULL) < 0)
			{
				fprintf(stderr, "pthread_create (HSM Verif) Error %d\n", errno);
				exit(0);
			}

			nBalHsm = nBalHsmVerf;
		}
	}

	if (mode & HSM_PROD_MODE)
	{
		if (GetHSMPCodes(	nIndexCtx,
							szNodeId,
							sInterfaceMode,
							gs_hsm_resources[HSM_PROD_ID][0],
							gs_hsm_resources[HSM_PROD_ID][1]) == OK)
		{
			nBalHsmProd = OpenBalCom(BAL_HSM_PROD);
			if (nBalHsmProd < 0)
			{
				trace_event("Can Not Open Hsm Prod Queue", FATAL);
				exit(0);
			}
			EmptyQueue(nBalHsmProd, getpid());

			if (pthread_create(&(tab_ThrInfo[HSM_PROD_THREAD_ID].tid), NULL, thread_HSM_Prod, NULL) < 0)
			{
				fprintf(stderr, "pthread_create (HSM Prod) Error %d\n", errno);
				exit(0);
			}
			if (nBalHsm < 0)
				nBalHsm = nBalHsmProd;
		}

	}

	return OK;
}

/*****************************************************************************/
int CheckHsmResource(int nIndexCtx, int mode, pid_t* nPid)
{
	int	nIndex;

	char	sLine[MAX_LINE_TRC];
	trace_event("Start CheckHsmResource()", PROCESSING);

	if (mode& HSM_PROD_MODE)
		nIndex = HSM_PROD_ID;
	else
		nIndex = HSM_VERIF_ID;

	if (strlen(gs_hsm_resources[nIndex][0]) > 0 && 
		CheckResource(nIndexCtx, gs_hsm_resources[nIndex][0], nPid) == OK)
		return OK;
	if (strlen(gs_hsm_resources[nIndex][1]) > 0 &&
		CheckResource(nIndexCtx, gs_hsm_resources[nIndex][1], nPid) == OK)
		return OK;

	trace_event("End   CheckHsmResource(NOK)", PROCESSING);
	return NOK;
}
/*****************************************************************************/
int GetHsmDecTable(char* sDecTable)
{

	memcpy(sDecTable,g_dec_table,SIZE_KEY);
	return OK;
}
