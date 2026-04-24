#ifndef HPS_THREAD_H
#define HPS_THREAD_H
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>

/* Mehdi Elyajizi this Macro is already diefined by the system in
* /usr/include/pthread.h */
#ifndef PTHREAD_THREADS_MAX
	#define PTHREAD_THREADS_MAX  512
#endif

int max_request;
int max_thread;
int nUserThread;


typedef struct SThrInfo
{
   pthread_t    tid;
} TSThrInfo;

/*EBE140421: Adding extern */
extern TSThrInfo   tab_ThrInfo[ ];

int CheckAndRefreshDBCon(int nIndexCtx, struct timeval	lastTime);

#endif /*** HPS_THREAD_H ***/

