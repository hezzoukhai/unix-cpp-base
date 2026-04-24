#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>

void           *
thread_CTL(void *arg)
{
	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) {
		pause();
	}
}
