#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <define.h>
#include <pthread.h>
#include <alarm_thr.h>
#include <signal.h>
#include <p7_trace.h>

pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;

/* EBE140421: adding tab_alrm definition */
P7_DEF_SYM alrm_inf_t tab_alrm[ PTHREAD_THREADS_MAX / 2];

void * thread_wait_TIME(void * arg)
{
   struct timeval  tv;
   struct timespec ts;
   int    retour;
   timeout_t      demande;

#ifdef SOLARIS
   sigset_t  set;
#elif defined(LINUX)
   sigset_t  set;/* Mehdi Elyajizi Linux */
#else
   struct sigset_t  set;
#endif

   pthread_detach(pthread_self());
   memcpy( (char *) &demande, (char *)arg, sizeof(timeout_t));
   free(arg);

#ifdef LINUX
   sigemptyset(&set);/* Mehdi Elyajizi Linux */
   sigaddset(&set, SIGPIPE);/* Mehdi Elyajizi Linux */
   sigaddset(&set, SIGCHLD);/* Mehdi Elyajizi Linux */
   sigaddset(&set, SIGUSR2);/* Mehdi Elyajizi Linux */
#else
   sigemptyset(&set);
   sigaddset(&set, SIGPIPE);
   sigaddset(&set, SIGCHLD);
   sigaddset(&set, SIGUSR2);
#endif

#ifdef SOLARIS
   thr_sigsetmask (SIG_BLOCK, &set, NULL );
#elif LINUX
   pthread_sigmask(SIG_BLOCK, &set, NULL ); /* Mehdi Elyajizi Linux */
#else
   sigthreadmask(SIG_BLOCK, &set, NULL);
#endif

   /*sigthreadmask(SIG_BLOCK, &set, NULL);*/
   if (gettimeofday(&tv, NULL) < 0)
   {
	  trace_message(P7_TL_ERROR,	"gettimeofday Error : %d", errno); 
      pthread_exit(NULL);
   }
   ts.tv_sec  = tv.tv_sec + demande.delta;
   ts.tv_nsec = tv.tv_usec * 1000;
   
   pthread_mutex_lock(& (tab_alrm[demande.indice].lock) );
   if (tab_alrm[demande.indice].done == NOK)
   {
      if ( (retour =pthread_cond_timedwait(&(tab_alrm[demande.indice].cond), 
                                  &(tab_alrm[demande.indice].lock), 
                                  &ts)) < 0)
      {
         if (retour == ETIMEDOUT)
         {
			if(tab_alrm[demande.indice].done == NOK)
			{
				trace_message(P7_TL_ERROR,	"THREAD TimedOut 1 , tid = %lu", tab_alrm[demande.indice].tid);
				tab_alrm[demande.indice].flag = OK;
				pthread_kill(tab_alrm[demande.indice].tid, SIGALRM);
			}
			else
			{
				trace_message(P7_TL_ERROR,	"THREAD TimedOut ERROR 1, tid = %lu", tab_alrm[demande.indice].tid); 
			}
         }
         else
         {
			trace_message(P7_TL_ERROR,	"Error Waiting Time Out : errno = %d, tid = %lu", errno, tab_alrm[demande.indice].tid); 
         }
      }
      else
      {
         if (retour == ETIMEDOUT)
         {
			if(tab_alrm[demande.indice].done == NOK)
			{
				trace_message(P7_TL_ERROR,	"THREAD TimedOut 2 , tid = %lu", tab_alrm[demande.indice].tid);
				tab_alrm[demande.indice].flag = OK;
				pthread_kill(tab_alrm[demande.indice].tid, SIGALRM);
			}
			else
			{
				trace_message(P7_TL_ERROR,	"THREAD TimedOut ERROR 2, tid = %lu", tab_alrm[demande.indice].tid); 
			}
         }
      }
   }
   else
   {
      tab_alrm[demande.indice].tid = -1;
   }
   pthread_mutex_unlock(& (tab_alrm[demande.indice].lock) );
   pthread_exit(NULL);
}

void InitTabAlarm()
{
   int i;
 
   pthread_mutexattr_t mattr;
   pthread_condattr_t cattr;

   pthread_mutexattr_init(&mattr);
   pthread_condattr_init(&cattr);
   for(i = 0; i<(PTHREAD_THREADS_MAX/2); i++)
   {
      tab_alrm[i].tid  = -1;
      tab_alrm[i].tid  = NOK;
      pthread_mutex_init( &(tab_alrm[i].lock) , &mattr);
      pthread_cond_init(  &(tab_alrm[i].cond) , &cattr);
   }
   pthread_mutexattr_destroy(&mattr);
   pthread_condattr_destroy(&cattr);
}

void THR_Alarm(delta, flag)
int delta;
int * flag;
{
   int i=0;
   pthread_t tid_time;
   timeout_t * timeout_st;
   pthread_attr_t pattr;
   
   if (delta < 0)
      return;

   if (delta == 0)
   {
      while ( i < (PTHREAD_THREADS_MAX/2) )
      {
         if (tab_alrm[i].tid == pthread_self()) break;
         i++;
      }
      if (i < (PTHREAD_THREADS_MAX / 2))
      {
         pthread_mutex_lock(&flag_mutex);
         pthread_mutex_lock (&tab_alrm[i].lock);
         pthread_cond_signal(&tab_alrm[i].cond);
         tab_alrm[i].done = OK;
         tab_alrm[i].tid  = -1;
         *flag = tab_alrm[i].flag;
         pthread_mutex_unlock (&tab_alrm[i].lock);
         pthread_mutex_unlock(&flag_mutex);
      }
      return;
   }

   pthread_mutex_lock(&flag_mutex);
   while ( i < (PTHREAD_THREADS_MAX/2) )
   {
      if (tab_alrm[i].tid == -1) break;
      i++;
   }

   if (i < (PTHREAD_THREADS_MAX / 2))
   {
	  tab_alrm[i].tid  = pthread_self();
	  tab_alrm[i].done = NOK;
	  tab_alrm[i].flag = NOK;
	  pthread_mutex_unlock(&flag_mutex);
	  *flag = NOK;

	  timeout_st = malloc (sizeof(timeout_t)); 
	  timeout_st->indice = i;
	  timeout_st->delta  = delta;
	  pthread_attr_init(&pattr);
	  pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
	  if (pthread_create(&tid_time, NULL, thread_wait_TIME, timeout_st) < 0)
	  {
		 trace_message(P7_TL_ERROR,	"pthread_create Error %d", errno);
		 free(timeout_st);
		 pthread_exit(NULL);
	  }
   }
   else
   {
	  trace_message(P7_TL_ERROR,	"No Empty Slot"); 
   }
}


