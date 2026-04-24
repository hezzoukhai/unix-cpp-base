#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <int1_struct.h>
#include <int1.h>


void* int1_thread_CTL(void *arg)
{
	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) {
		pause();
	}
}
