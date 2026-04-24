#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <struct_line.h>
#include <thread_var.h>

static pthread_mutex_t LineTabMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t LineReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  LineReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t LineWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  LineWriteCond  = PTHREAD_COND_INITIALIZER;

static int iLineGet;
static int iLinePut;
static int nLineSlots;
static int nLineItem ;

static TSLineExchange   tab_LineDemande[MAX_WAIT_REQUEST];

void InitLineProdConso()
{
   if (max_request > MAX_WAIT_REQUEST)
      max_request = MAX_WAIT_REQUEST;
   iLineGet =0;
   iLinePut =0;
   nLineSlots = max_request;
   nLineItem =0;
}

int PLineWaitEntry()
{
   pthread_mutex_lock(&LineWriteMutex);
   while (nLineSlots <= 0)
       pthread_cond_wait(&LineWriteCond, &LineWriteMutex);
   nLineSlots--;
   pthread_mutex_unlock(&LineWriteMutex);
    return(OK);
}

int PLinePutMsg(TSLineExchange * sLineMsg)
{
   pthread_mutex_lock(&LineTabMutex);
   memcpy ((char *) &(tab_LineDemande[iLinePut]), (char *) sLineMsg,
                                                sizeof(TSLineExchange));
   iLinePut = (iLinePut +1) % ( max_request);
   pthread_mutex_unlock(&LineTabMutex);
    return(OK);
}

int PLineConsoInfo()
{
   pthread_mutex_lock(&LineReadMutex);
   nLineItem++;
   pthread_cond_signal(&LineReadCond);
   pthread_mutex_unlock(&LineReadMutex);
    return(OK);
}

int CLineWaitMsg()
{
   pthread_mutex_lock(&LineReadMutex);
   while (nLineItem <= 0)
       pthread_cond_wait(&LineReadCond, &LineReadMutex);
   nLineItem--;
   pthread_mutex_unlock(&LineReadMutex);
    return(OK);
}

int CLineGetMsg(TSLineExchange *sLineMsg)
{
   pthread_mutex_lock(&LineTabMutex);
   memcpy ((char *) sLineMsg, (char *) &(tab_LineDemande[iLineGet]),
                                                sizeof(TSLineExchange));
   memset((char *) &(tab_LineDemande[iLineGet]),0,sizeof(TSLineExchange));        /*NAK 20160728 PADSS Fix*/
   iLineGet = (iLineGet +1) % (max_request);
   pthread_mutex_unlock(&LineTabMutex);
    return(OK);
}

int CLineFreeEntry()
{
   pthread_mutex_lock(&LineWriteMutex);
   nLineSlots++;
   pthread_cond_signal(&LineWriteCond);
   pthread_mutex_unlock(&LineWriteMutex);
    return(OK);
}

