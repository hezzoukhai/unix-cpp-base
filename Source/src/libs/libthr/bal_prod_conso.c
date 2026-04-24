#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <queue.h>
#include <thread_var.h>

static pthread_mutex_t BalTabMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t BalReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  BalReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t BalWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  BalWriteCond  = PTHREAD_COND_INITIALIZER;

static int iBalGet;
static int iBalPut;
static int nBalSlots;
static int nBalItem ;

static TSBalExchange   tab_BalDemande[MAX_WAIT_REQUEST];

void InitBalProdConso()
{
   if (max_request > MAX_WAIT_REQUEST)
      max_request = MAX_WAIT_REQUEST;
   iBalGet   = 0;
   iBalPut   = 0;
   nBalSlots = max_request;
   nBalItem  = 0;
}

int PBalWaitEntry()
{
   pthread_mutex_lock(&BalWriteMutex);
   while (nBalSlots <= 0)
       pthread_cond_wait(&BalWriteCond, &BalWriteMutex);
   nBalSlots--;
   pthread_mutex_unlock(&BalWriteMutex);
   return(OK);
}

int PBalPutMsg(TSBalExchange * sBalMsg)
{
   pthread_mutex_lock(&BalTabMutex);
   memcpy ((char *) &(tab_BalDemande[iBalPut]), (char *) sBalMsg,
                                                sizeof(TSBalExchange));
   iBalPut = (iBalPut +1) % ( max_request);
   pthread_mutex_unlock(&BalTabMutex);
   return(OK);
}

int PBalConsoInfo()
{
   pthread_mutex_lock(&BalReadMutex);
   nBalItem++;
   pthread_cond_signal(&BalReadCond);
   pthread_mutex_unlock(&BalReadMutex);
    return(OK);
}

int CBalWaitMsg()
{
   pthread_mutex_lock(&BalReadMutex);
   while (nBalItem <= 0)
       pthread_cond_wait(&BalReadCond, &BalReadMutex);
   nBalItem--;
   pthread_mutex_unlock(&BalReadMutex);
    return(OK);
}

int CBalGetMsg(TSBalExchange *sBalMsg)
{
   pthread_mutex_lock(&BalTabMutex);
   memcpy ((char *) sBalMsg, (char *) &(tab_BalDemande[iBalGet]),
                                                sizeof(TSBalExchange));
   memset ((char *) &(tab_BalDemande[iBalGet]),0,sizeof(TSBalExchange)); /*NAK 20160728 PADSS Fix*/
  
   iBalGet = (iBalGet +1) % (max_request);
   pthread_mutex_unlock(&BalTabMutex);
    return(OK);
}

int CBalFreeEntry()
{
   pthread_mutex_lock(&BalWriteMutex);
   nBalSlots++;
   pthread_cond_signal(&BalWriteCond);
   pthread_mutex_unlock(&BalWriteMutex);
    return(OK);
}

