#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <define.h>
#include <thread_var.h>
#define _INT_VAR_
#include <thr_hsm.h>
#undef _INT_VAR_
#include <queue.h>
#include <param_arg.h>

void * thread_HSM(void * arg)
{
   TSHsmExchange        sBalMsg;
   TSArgParam     * p_arg;

   pthread_detach(pthread_self());

   ThreadMaskSignals();

   while (1)
   {
      if ( ReadBalHsm(nBalHsm, getpid(), !IPC_NOWAIT, &sBalMsg) < 0)
      {
         fprintf(stderr," Error Reading Message \n");
         pthread_exit(NULL);
      }

      if ((sBalMsg.sHeader.nIndex >= 0) && (sBalMsg.sHeader.nIndex < MAX_THREAD) )
      {
         if (CmpCoNumber(sBalMsg.sHeader.nIndex, sBalMsg.sHeader.nCoNumber) == OK)
         {
            memcpy(tab_HsmCtx[sBalMsg.sHeader.nIndex].szBufferBal, sBalMsg.sBody,
                sBalMsg.sHeader.nLen);
            tab_HsmCtx[sBalMsg.sHeader.nIndex].nLenBal = sBalMsg.sHeader.nLen;
            InfoHsmThread(sBalMsg.sHeader.nIndex);
         }
      }
   }
}

void InitTabHsmCtx(int mode)
{
   int i;
   pthread_mutexattr_t mattr;
   pthread_condattr_t  cattr;


   pthread_mutexattr_init(&mattr);
   pthread_condattr_init(&cattr);


   for (i=0; i < MAX_THREAD; i++)
   {
      tab_HsmCtx[i].co_number = 0;
      tab_HsmCtx[i].free      = 1;
      tab_HsmCtx[i].status    = -1;
      tab_HsmCtx[i].tid       = -1;  
      tab_HsmCtx[i].nLenBal   = 0;
      memset (tab_HsmCtx[i].szBufferBal, 0 , LG_MAX );
      pthread_mutex_init( &(tab_HsmCtx[i].HsmMutex) , &mattr);
      pthread_cond_init ( &(tab_HsmCtx[i].HsmCond) , &cattr);
   }

   if (mode == VERIF_MODE)
      nBalHsm = OpenBalCom(BAL_HSM_VERIF); 
   else
      nBalHsm = OpenBalCom(BAL_HSM_PROD); 

   if ( nBalHsm < 0)
   {
      trace_event("Can Not Open Hsm Queue", PROCESSING);
      exit( 0 );
   }
   EmptyQueue(nBalHsm , getpid());

   /**** Creation du Thread  HSM  ***/
   if (pthread_create(&(tab_ThrInfo[HSM_THREAD_ID].tid), NULL, 
                                                   thread_HSM, NULL) < 0)
   {
      fprintf(stderr,"pthread_create (HSM) Error %d\n", errno);
      exit(0);
   }

}

int GetNextCoNumber(int nIndex)
{
   tab_HsmCtx[nIndex].co_number = (tab_HsmCtx[nIndex].co_number + 1) % 100;
   return(tab_HsmCtx[nIndex].co_number);
}

int CmpCoNumber(int nIndex, int nCoNumber)
{
   if (tab_HsmCtx[nIndex].co_number == nCoNumber)
      return(OK);
   else
      return(NOK);
}

int GetThreadHsmIndex (pthread_t tid)
{
   int i;

   for (i = 0; i < MAX_THREAD; i++)
   {
      if (tab_HsmCtx[i].tid == tid)
         break;
   }
   if ( i >= MAX_THREAD)
      return(-1);
   return (i);
}

int WaitHsmResponse (int nIndex, int nTimer, char *szBuffer)
{
   struct timeval  tv;
   struct timespec ts;
   int    retour;

   tab_HsmCtx[nIndex].flag_bal = WAITING;
   if (gettimeofday(&tv, NULL) < 0)
   {
      fprintf(stderr,"gettimeofday Error : %d\n", errno);
      return(NOK);
   }
   ts.tv_sec  = tv.tv_sec + nTimer;
   ts.tv_nsec = tv.tv_usec * 1000;

   pthread_mutex_lock (&(tab_HsmCtx[nIndex].HsmMutex));
   if ( (retour =pthread_cond_timedwait(&(tab_HsmCtx[nIndex].HsmCond),
                              &(tab_HsmCtx[nIndex].HsmMutex),
                              &ts)) < 0)
   {
      if (retour == ETIMEDOUT)
      {
         retour = TIMEOUT;
         tab_HsmCtx[nIndex].flag_bal = TIMEOUT;
      }
      else
      {
         retour = NOK;
         tab_HsmCtx[nIndex].flag_bal = NOK;
      }
   }
   else
   {
      if (retour == ETIMEDOUT)
      {
         retour = TIMEOUT;
         tab_HsmCtx[nIndex].flag_bal = TIMEOUT;
      }
      else
      {
         retour = OK;
         tab_HsmCtx[nIndex].flag_bal = OK;
         memcpy(szBuffer, tab_HsmCtx[nIndex].szBufferBal, tab_HsmCtx[nIndex].nLenBal); 
      }
   }
   pthread_mutex_unlock (&(tab_HsmCtx[nIndex].HsmMutex));
   return(retour);
}

int InfoHsmThread(int nIndex)
{
   pthread_mutex_lock (&(tab_HsmCtx[nIndex].HsmMutex));
   pthread_cond_signal( &(tab_HsmCtx[nIndex].HsmCond));
   pthread_mutex_unlock (&(tab_HsmCtx[nIndex].HsmMutex));
   /*SKO290509*/ return(0);
}

