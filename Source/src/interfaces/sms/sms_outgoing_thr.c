#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <define.h>
#include <signal.h>
#include <iso_sms.h>
#include <iso_hps.h>
#include <sms_param.h>
#include <sms_var.h>

pthread_mutex_t OutgManageMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  OutgManageCond  = PTHREAD_COND_INITIALIZER;

/*************************************************************/
void * thread_OUTG(void * arg)
{
	char   sLine [ MAX_LINE_TRC ];
	struct timeval	lastTime;
	int			nResult;
	int			nIndiceCtx;

	/****** Initialisation des variables *****/
	memset( sLine , 0 , sizeof(sLine )  );

	pthread_detach(pthread_self());

	ThreadMaskSignals();
	AllocateOracleCtx(OUTG_THREAD_ID);
	ConnectOracle (OUTG_THREAD_ID );

	fprintf ( stderr,"\n Thread OUTGOING ID = %lu ", pthread_self() );
	nIndiceCtx=OUTG_THREAD_ID;
	gettimeofday(&lastTime,NULL);
	while (1){
		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
		OutgoingManagement( OUTG_THREAD_ID , ResourceId );
		gettimeofday(&lastTime,NULL);
	}
	return(NULL);
}

/****************************************************************/
int WaitOutgoingAnswer ()
{
	int nTime = NOK, nResp = NOK;

	nOutgResponse = NOK;
	nOutgTimeOut  = NOK;

	pthread_mutex_lock(&OutgManageMutex);
	while ( (nOutgResponse == NOK) &&
		(nOutgTimeOut  == NOK))
		pthread_cond_wait(&OutgManageCond, &OutgManageMutex);
	if (nOutgResponse == OK)
		nResp = OK;
	else
		nTime = OK;
	pthread_mutex_unlock(&OutgManageMutex);
	if (nResp == OK)
		return(OUTG_RESPONSE);
	else
		return(OUTG_TIMEOUT);

}

int OutgoingRespReceived()
{
	pthread_mutex_lock(&OutgManageMutex);
	nOutgResponse = OK;
	pthread_cond_signal(&OutgManageCond);
	pthread_mutex_unlock(&OutgManageMutex);
	return(OK);
}

int OutgoingTimeOutDetected()
{
	pthread_mutex_lock(&OutgManageMutex);
	nOutgTimeOut = OK;
	pthread_cond_signal(&OutgManageCond);
	pthread_mutex_unlock(&OutgManageMutex);
	return(OK);
}

