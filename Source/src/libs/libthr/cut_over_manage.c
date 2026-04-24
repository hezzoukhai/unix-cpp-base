#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include <define.h>
#include <sys/time.h> /* Mehdi Elyajizi Linux */
#include <cut_over_var.h>


pthread_mutex_t CutOverManageMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  CutOverManageCond  = PTHREAD_COND_INITIALIZER;

pthread_mutex_t CutOverTimeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  CutOverTimeCond  = PTHREAD_COND_INITIALIZER;

int WaitCutOverAnswer ()
{
   int nTime = NOK, nResp = NOK;

   nCutOverResponse = NOK;
   nCutOverTimeOut  = NOK;

   pthread_mutex_lock(&CutOverManageMutex);
   while ( (nCutOverResponse == NOK) && 
           (nCutOverTimeOut  == NOK))
       pthread_cond_wait(&CutOverManageCond, &CutOverManageMutex);
   if (nCutOverResponse == OK)
      nResp = OK;
   else
      nTime = OK;
   pthread_mutex_unlock(&CutOverManageMutex);
   if (nResp == OK)
      return(CUT_OVER_RESPONSE);
   else
      return(CUT_OVER_TIMEOUT);
         
}

int CutOverRespReceived()
{
   pthread_mutex_lock(&CutOverManageMutex);
   nCutOverResponse = OK;
   pthread_cond_signal(&CutOverManageCond);
   pthread_mutex_unlock(&CutOverManageMutex);
    /*SKO290509*/ return(OK);
}

int CutOverTimeOutDetected()
{
   pthread_mutex_lock(&CutOverManageMutex);
   nCutOverTimeOut = OK;
   pthread_cond_signal(&CutOverManageCond);
   pthread_mutex_unlock(&CutOverManageMutex);
    /*SKO290509*/ return(OK);
}

int WaitCutOverTime (int nHeure, int nMinute)
{
   time_t nNow;
   time_t nCutOverTime;
   int    nWaitTime;
   struct tm *date;
   struct tm date_r;
   struct timeval  tv;
   struct timespec ts;
   int    retour;
   int    result = NOK;
   char   sLine[MAX_LINE_TRC];

   sprintf(sLine,"Start WaitCutOverTime Hour : %d| Min : %d",nHeure,nMinute);
   trace_event(sLine,PROCESSING);

   time(&nNow);
   date = localtime_r(&nNow, &date_r);
   date_r.tm_sec = 0;
   date_r.tm_min = nMinute;
   date_r.tm_hour = nHeure;
   nCutOverTime = mktime(&date_r);
   if (nCutOverTime < nNow)
      nWaitTime = nCutOverTime - nNow +(3600*24);
   else
      nWaitTime = nCutOverTime - nNow;

   if (gettimeofday(&tv, NULL) < 0)
   {
      fprintf(stderr,"gettimeofday Error : %d\n", errno);
      pthread_exit(NULL);
   }
   ts.tv_sec  = tv.tv_sec + nWaitTime;
   ts.tv_nsec = tv.tv_usec * 1000;

   pthread_mutex_lock(& CutOverTimeMutex );
   if ( (retour =pthread_cond_timedwait(&CutOverTimeCond, 
                                        &CutOverTimeMutex, 
                                        &ts)) < 0)
   {
      if (retour == ETIMEDOUT)
      {
         fprintf(stderr," THREAD CutOver Waked Up  \n");
         result = OK;
      }
      else
      {
         fprintf(stderr,"Error Waiting Time Out : %d\n", errno);
      }
   }
   else
   {
      if (retour == ETIMEDOUT)
      {
         fprintf(stderr," THREAD CutOver Waked Up  \n");
         result = OK;
      }
   }
   pthread_mutex_unlock(& CutOverTimeMutex );
   
   trace_event("End   WaitCutOverTime()",PROCESSING );
   return(result);
}
