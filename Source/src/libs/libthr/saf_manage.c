#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <saf_var.h>

static pthread_mutex_t SafManageMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  SafManageCond  = PTHREAD_COND_INITIALIZER;

static int nSafResponse;
static int nSafTimeOut;


int WaitSafAnswer ()
{
   int nTime = NOK, nResp = NOK;

   nSafResponse = NOK;
   nSafTimeOut  = NOK;

   pthread_mutex_lock(&SafManageMutex);
   while ( (nSafResponse == NOK) && 
           (nSafTimeOut  == NOK))
       pthread_cond_wait(&SafManageCond, &SafManageMutex);

   if (nSafResponse == OK)
      nResp = OK;
   else
      nTime = OK;
   
   pthread_mutex_unlock(&SafManageMutex);
   if (nResp == OK)
      return(SAF_RESPONSE);
   else
      return(SAF_TIMEOUT);
         
}

int WaitSafAnswerML ()
{
   int nTime = NOK, nResp = NOK;

   nSafResponse = NOK;
   nSafTimeOut  = NOK;

  /* pthread_mutex_lock(&SafManageMutex);*/
   while ( (nSafResponse == NOK) && 
           (nSafTimeOut  == NOK))
       pthread_cond_wait(&SafManageCond, &SafManageMutex);

   if (nSafResponse == OK)
      nResp = OK;
   else
      nTime = OK;
   
   /*pthread_mutex_unlock(&SafManageMutex);*/
   if (nResp == OK)
      return(SAF_RESPONSE);
   else
      return(SAF_TIMEOUT);
         
}

void LockSafMutex ()
{
   pthread_mutex_lock(&SafManageMutex);
}
void UnlockSafMutex ()
{
   pthread_mutex_unlock(&SafManageMutex);
}

int SafRespReceived()
{
   pthread_mutex_lock(&SafManageMutex);
   nSafResponse = OK;
   pthread_cond_signal(&SafManageCond);
   pthread_mutex_unlock(&SafManageMutex);
   return(0);
}

int SafTimeOutDetected()
{
   pthread_mutex_lock(&SafManageMutex);
   nSafTimeOut = OK;
   pthread_cond_signal(&SafManageCond);
   pthread_mutex_unlock(&SafManageMutex);
   return(0);
}

