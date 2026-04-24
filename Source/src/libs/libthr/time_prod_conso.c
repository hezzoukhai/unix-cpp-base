#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <struct_time.h>
#include <thread_var.h>

static pthread_mutex_t TimeTabMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t TimeReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  TimeReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t TimeWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  TimeWriteCond  = PTHREAD_COND_INITIALIZER;

static int iTimeGet;
static int iTimePut;
static int nTimeSlots;
static int nTimeItem ;

#define MAX_TIME_OUT    100

static TSTimeExchange   tab_TimeDemande[MAX_TIME_OUT];

void InitTimeProdConso()
{
   iTimeGet =0;
   iTimePut =0;
   nTimeSlots = MAX_TIME_OUT;
   nTimeItem =0;
}

int PTimeWaitEntry()
{
   pthread_mutex_lock(&TimeWriteMutex);
   while (nTimeSlots <= 0)
       pthread_cond_wait(&TimeWriteCond, &TimeWriteMutex);
   nTimeSlots--;
   pthread_mutex_unlock(&TimeWriteMutex);
   return(OK);
}

int PTimePutMsg(TSTimeExchange * sTimeMsg)
{
   pthread_mutex_lock(&TimeTabMutex);
   memcpy ((char *) &(tab_TimeDemande[iTimePut]), (char *) sTimeMsg,
                                                sizeof(TSTimeExchange));
   iTimePut = (iTimePut +1) % ( MAX_TIME_OUT);
   pthread_mutex_unlock(&TimeTabMutex);
   return(OK);
}

int PTimeConsoInfo()
{
   pthread_mutex_lock(&TimeReadMutex);
   nTimeItem++;
   pthread_cond_signal(&TimeReadCond);
   pthread_mutex_unlock(&TimeReadMutex);
   return(OK);
}

int CTimeWaitMsg()
{
   pthread_mutex_lock(&TimeReadMutex);
   while (nTimeItem <= 0)
       pthread_cond_wait(&TimeReadCond, &TimeReadMutex);
   nTimeItem--;
   pthread_mutex_unlock(&TimeReadMutex);
   return(OK);
}

int CTimeGetMsg(TSTimeExchange *sTimeMsg)
{
   pthread_mutex_lock(&TimeTabMutex);
   memcpy ((char *) sTimeMsg, (char *) &(tab_TimeDemande[iTimeGet]),
                                                sizeof(TSTimeExchange));
   memset ((char *) &(tab_TimeDemande[iTimeGet]),0,sizeof(TSTimeExchange));        /*NAK 20160728 PADSS Fix*/
   iTimeGet = (iTimeGet +1) % (MAX_TIME_OUT);
   pthread_mutex_unlock(&TimeTabMutex);
   return(OK);
}

int CTimeFreeEntry()
{
   pthread_mutex_lock(&TimeWriteMutex);
   nTimeSlots++;
   pthread_cond_signal(&TimeWriteCond);
   pthread_mutex_unlock(&TimeWriteMutex);
   return(OK);
}

