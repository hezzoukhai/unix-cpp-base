#include <stdio.h>
#include <pthread.h>

#include <define.h>
#include <sign_var.h>


pthread_mutex_t SignManageMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  SignManageCond  = PTHREAD_COND_INITIALIZER;

int WaitSignAnswer ()
{
   int nTime = NOK, nResp = NOK;

   nSignResponse = NOK;
   nSignTimeOut  = NOK;

   pthread_mutex_lock(&SignManageMutex);
   while ( (nSignResponse == NOK) && 
           (nSignTimeOut  == NOK))
       pthread_cond_wait(&SignManageCond, &SignManageMutex);
   if (nSignResponse == OK)
      nResp = OK;
   else
      nTime = OK;
   pthread_mutex_unlock(&SignManageMutex);
   if (nResp == OK)
      return(SIGN_RESPONSE);
   else
      return(SIGN_TIMEOUT);
         
}

int SignRespReceived()
{
   pthread_mutex_lock(&SignManageMutex);
   nSignResponse = OK;
   pthread_cond_signal(&SignManageCond);
   pthread_mutex_unlock(&SignManageMutex);
   /*SKO290509*/ return(0);
}

int SignTimeOutDetected()
{
   pthread_mutex_lock(&SignManageMutex);
   nSignTimeOut = OK;
   pthread_cond_signal(&SignManageCond);
   pthread_mutex_unlock(&SignManageMutex);
   /*SKO290509*/ return(0);
}

