#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <queue.h>
#include <thread_var.h>
#include <errno.h>

static pthread_mutex_t BalTabMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t BalReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  BalReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t BalWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  BalWriteCond  = PTHREAD_COND_INITIALIZER;

static int iBalGet;
static int iBalPut;
static int nBalSlots;
static int nBalItem ;

static TSHsmExchange   tab_BalDemande[MAX_WAIT_REQUEST];

void InitHsmProdConso()
{
   if (max_request > MAX_WAIT_REQUEST)
      max_request = MAX_WAIT_REQUEST;
   iBalGet   = 0;
   iBalPut   = 0;
   nBalSlots = max_request;
   nBalItem  = 0;
}

int PHsmWaitEntry()
{
   pthread_mutex_lock(&BalWriteMutex);
   while (nBalSlots <= 0)
       pthread_cond_wait(&BalWriteCond, &BalWriteMutex);
   nBalSlots--;
   pthread_mutex_unlock(&BalWriteMutex);
   return(OK);
}

int PHSMTimedPutMsg(TSHsmExchange * sHsmMsg, int nPMaxQueueWaitTimeMs)
{
   struct timeval    tp;
   struct timespec	  maxTime;
   int nResult;

   gettimeofday(&tp, NULL);
   maxTime.tv_sec   = tp.tv_sec;
   maxTime.tv_nsec  = tp.tv_usec * 1000;
   maxTime.tv_nsec += nPMaxQueueWaitTimeMs * 1000000;
   maxTime.tv_sec  += maxTime.tv_nsec / 1000000000;
   maxTime.tv_nsec  = maxTime.tv_nsec % 1000000000;


   pthread_mutex_lock(&BalTabMutex);

   while (nBalSlots <= 0)
   {
      nResult = pthread_cond_timedwait(&BalWriteCond, &BalTabMutex, &maxTime);
      if( nResult == ETIMEDOUT )
      {
         pthread_mutex_unlock(&BalTabMutex);
         return(TIMEOUT);
      }
      else if( nResult != 0 )
      {
         pthread_mutex_unlock(&BalTabMutex);
         return(NOK);
      }
   }

   nBalSlots--;
   pthread_mutex_unlock(&BalTabMutex);
   /*
   PBalConsoInfo();PBalPutMsg(sHsmMsg);
   PBalConsoInfo();
   */
   PHsmPutMsg(sHsmMsg);
   PHsmConsoInfo();
   return OK;
}

int PHsmPutMsg(TSHsmExchange * sBalMsg)
{
   pthread_mutex_lock(&BalTabMutex);
   memcpy ((char *) &(tab_BalDemande[iBalPut]), (char *) sBalMsg,
                                                sizeof(TSHsmExchange));
   iBalPut = (iBalPut +1) % ( max_request);
   pthread_mutex_unlock(&BalTabMutex);
   return(OK);
}

int PHsmConsoInfo()
{
   pthread_mutex_lock(&BalReadMutex);
   nBalItem++;
   pthread_cond_signal(&BalReadCond);
   pthread_mutex_unlock(&BalReadMutex);
    return(OK);
}

int CHsmWaitMsg()
{
   pthread_mutex_lock(&BalReadMutex);
   while (nBalItem <= 0)
       pthread_cond_wait(&BalReadCond, &BalReadMutex);
   nBalItem--;
   pthread_mutex_unlock(&BalReadMutex);
    return(OK);
}

int CHsmGetMsg(TSHsmExchange *sBalMsg)
{
   pthread_mutex_lock(&BalTabMutex);
   memcpy ((char *) sBalMsg, (char *) &(tab_BalDemande[iBalGet]),
                                                sizeof(TSHsmExchange));
   memset ((char *) &(tab_BalDemande[iBalGet]),0,sizeof(TSHsmExchange)); /*NAK 20160728 PADSS Fix*/
  
   iBalGet = (iBalGet +1) % (max_request);
   pthread_mutex_unlock(&BalTabMutex);
    return(OK);
}

int CHsmFreeEntry()
{
   pthread_mutex_lock(&BalWriteMutex);
   nBalSlots++;
   pthread_cond_signal(&BalWriteCond);
   pthread_mutex_unlock(&BalWriteMutex);
    return(OK);
}

