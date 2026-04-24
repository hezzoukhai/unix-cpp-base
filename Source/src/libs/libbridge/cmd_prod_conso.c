#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <queue.h>
#include <thread_var.h>
/******************************************************************************/
static pthread_mutex_t CmdTabMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t CmdReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  CmdReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t CmdWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  CmdWriteCond  = PTHREAD_COND_INITIALIZER;

static int iCmdGet;
static int iCmdPut;
static int nCmdSlots;
static int nCmdItem ;

static TSBridgeExchange   tab_CmdDemande[MAX_WAIT_REQUEST];
/******************************************************************************/

void InitCmdProdConso()
{
   iCmdGet   = 0;
   iCmdPut   = 0;
   nCmdSlots = MAX_WAIT_REQUEST;
   nCmdItem  = 0;
}

int PCmdWaitEntry()
{
   pthread_mutex_lock(&CmdWriteMutex);
   while (nCmdSlots <= 0)
       pthread_cond_wait(&CmdWriteCond, &CmdWriteMutex);
   nCmdSlots--;
   pthread_mutex_unlock(&CmdWriteMutex);
   return(0);
}

int PCmdPutMsg(TSBridgeExchange * kCmdMsg)
{
   pthread_mutex_lock(&CmdTabMutex);
   memcpy ((char *) &(tab_CmdDemande[iCmdPut]), (char *) kCmdMsg,
                                                sizeof(TSBridgeExchange));
   iCmdPut = (iCmdPut +1) % ( MAX_WAIT_REQUEST);
   pthread_mutex_unlock(&CmdTabMutex);
   return(OK);
}

int PCmdConsoInfo()
{
   pthread_mutex_lock(&CmdReadMutex);
   nCmdItem++;
   pthread_cond_signal(&CmdReadCond);
   pthread_mutex_unlock(&CmdReadMutex);
    return(OK);
}

int CCmdWaitMsg()
{
   pthread_mutex_lock(&CmdReadMutex);
   while (nCmdItem <= 0)
       pthread_cond_wait(&CmdReadCond, &CmdReadMutex);
   nCmdItem--;
   pthread_mutex_unlock(&CmdReadMutex);
    return(OK);
}

int CCmdGetMsg(TSBridgeExchange *kCmdMsg)
{
   pthread_mutex_lock(&CmdTabMutex);
   memcpy ((char *) kCmdMsg, (char *) &(tab_CmdDemande[iCmdGet]),
                                                sizeof(TSBridgeExchange));
   iCmdGet = (iCmdGet +1) % (MAX_WAIT_REQUEST);
   pthread_mutex_unlock(&CmdTabMutex);
    return(OK);
}

int CCmdFreeEntry()
{
   pthread_mutex_lock(&CmdWriteMutex);
   nCmdSlots++;
   pthread_cond_signal(&CmdWriteCond);
   pthread_mutex_unlock(&CmdWriteMutex);
    return(OK);
}

