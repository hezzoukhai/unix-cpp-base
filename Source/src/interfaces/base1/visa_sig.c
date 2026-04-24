#include <define.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <param_arg.h>
#include <thread_var.h>
#include <visa_param.h>
#include <errno.h>

void * thread_SIG(void * arg)
{
	#if defined (SS_64BIT_SERVER) || defined (SOLARIS) || defined (LINUX) || defined (HPUX)
		sigset_t  set;
	#else
		struct sigset_t  set;
	#endif

	int	sig_recu;
	char    szDetail[80];
	
	trace_event(">>>>>>>>>>>>>>>>> Starting thread_SIG () <<<<<<<<<<<<<<<<<<<<<<", FATAL);
	pthread_detach(pthread_self());

	while (1) {

		/*23062020 PLUTONL-1956*/
		if (memcmp(ResourceStatus, STOPI, 3) == 0 )
		{
			 sleep(5);
			 exit(0);
		}
        /*23062020 PLUTONL-1956*/

		sigemptyset(&set);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGPIPE);
		sigaddset(&set, SIGCHLD);
		sigaddset(&set, SIGUSR2);

		#ifdef SOLARIS
			thr_sigsetmask(SIG_BLOCK, &set, NULL);
		#elif LINUX
			pthread_sigmask(SIG_BLOCK, &set, NULL);
		#elif HPUX
			pthread_sigmask(SIG_BLOCK, &set, "");
		#else
			sigthreadmask(SIG_BLOCK, &set, NULL);
		#endif

		if (sigwait(&set, &sig_recu) < 0) {
			sprintf(szDetail, "SigWait Error : %d", errno);
			trace_event(szDetail, ERROR);
		}
		
		fprintf(stderr, "Signal Received : %d\n", sig_recu);
		if (sig_recu == SIGCHLD)
			while (waitpid(-1, NULL, WNOHANG) > 0);
		if (sig_recu == SIGUSR2) {
			trace_event("**************************************************************", FATAL);
			trace_event("***              Message Halt Received ( SIGUSR2 )          **", FATAL);
			trace_event("**************************************************************", FATAL);
			
			memcpy (ResourceStatus, STOPI, 3);/*ICH20171011*/ /*23062020 PLUTONL-1956*/
			SendStopIsoMessage(ResourceId, getpid());
		}
	}
}
















