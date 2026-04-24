#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <dump.h>
#include <signal.h>

#include <iso_hps.h>
#include <define.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <struct_time.h>
#include <alarm_thr.h>

/************************************************************************/
static	unsigned int	g_nThreadIndex = 0;
static	int				g_nOrigDebugOpt;
/************************************************************************/
void * thread_disable_pci(void * arg)
{
   struct timeval  tv;
   struct timespec ts;
   int    result;
   timeout_t      timeout_st;

#if defined( SOLARIS ) || defined(LINUX)
   sigset_t  set;
#else
   struct sigset_t  set;
#endif

   pthread_detach(pthread_self());
   memcpy( (char *) &timeout_st, (char *)arg, sizeof(timeout_t));
   free(arg);


   sigemptyset(&set);
   sigaddset(&set, SIGPIPE);
   sigaddset(&set, SIGCHLD);
   sigaddset(&set, SIGUSR2);

#ifdef SOLARIS
   thr_sigsetmask (SIG_BLOCK, &set, NULL );
#elif defined(LINUX)
   pthread_sigmask(SIG_BLOCK, &set, NULL );
#else
   sigthreadmask(SIG_BLOCK, &set, NULL);
#endif

   
   if (gettimeofday(&tv, NULL) < 0)
   {
      fprintf(stderr,"gettimeofday Error : %d\n", errno);
      pthread_exit(NULL);
   }

   
   
   
   debug_option = 1;
   sleep(timeout_st.delta);

   if( timeout_st.indice == g_nThreadIndex )
   {
	   debug_option = g_nOrigDebugOpt;
   }

   pthread_exit(NULL);
}



int disable_pci(int nDelta)
{
	pthread_t tid_pci;
	timeout_t * timeout_st;
	pthread_attr_t pattr;
	char*			sPtr;

	trace_event("Start disable_pci ()",PROCESSING);

	if (nDelta < 0)
      return;

	timeout_st = malloc (sizeof(timeout_t)); 
	timeout_st->indice = ++g_nThreadIndex;
	timeout_st->delta  = nDelta*60;
	pthread_attr_init(&pattr);
	pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);

	sPtr = getenv("DEBUG_MODE");
	g_nOrigDebugOpt = 0;
	if (sPtr && sPtr[0] == '1')
	{
		g_nOrigDebugOpt = 1;
	}

	if (pthread_create(&tid_pci, NULL, thread_disable_pci, timeout_st) < 0)
   {
      fprintf(stderr,"pthread_create Error %d\n", errno);
      free(timeout_st);
      pthread_exit(NULL);
   }

	trace_event("End   disable_pci (OK)",PROCESSING);
	return 0;
}

