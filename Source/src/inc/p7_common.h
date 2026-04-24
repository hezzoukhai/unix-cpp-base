#ifndef P7_COMMON_H
#define P7_COMMON_H


#include <stdio.h>
/*#include <stdlib.h>*/
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#if defined(_WIN32)

	#include <winsock2.h>
	#include <windows.h>
	#include <signal.h>
	#include <sys/timeb.h>
	#include <io.h>

	#define		PATH_SEPERATOR		'/'

#else
	#include <sys/types.h>
	#include <sys/shm.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/time.h>
	#include <sys/msg.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <sys/socketvar.h>
	#include <unistd.h>
	#define		PATH_SEPERATOR		'/'
#endif

#include <pthread.h>


/**
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
**/

#include <p7_globals.h>

#include <p7_macros.h>
#include <p7_trace.h>
/*
#include <p7_errno.h>
#include <p7_tools.h>
*/


#if defined(_WIN32)
#include <win32_utils.h>
#endif


#endif

