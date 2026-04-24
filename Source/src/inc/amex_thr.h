
#ifndef  HPS_THR_AMEX_H
#define  HPS_THR_AMEX_H
#include <pthread.h>
#include <define.h>

pthread_mutex_t AmexTabMutex   = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t  x25_conn_mutex = PTHREAD_MUTEX_INITIALIZER;
int              nX25Conn;
pthread_cond_t   x25_cond = PTHREAD_COND_INITIALIZER ;

typedef struct SAmexCtx
{
   int              free;
   int              status;
   pthread_t        tid;
   int              fd;
   int              flag_bal;
   char             szBufferLine[ LG_MAX ];
   int              nLenLine;
   char             szBufferBal [ LG_MAX ];
   int              nLenBal;
   pthread_mutex_t  AmexMutex;
   pthread_cond_t   AmexCond ;
} TSAmexCtx;

TSAmexCtx  tab_AmexCtx [ MAX_THREAD ];

void  InitTabAmexCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_AMEX_H ***/

