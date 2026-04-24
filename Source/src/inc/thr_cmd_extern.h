#ifndef  HPS_THR_CMDSRV_H
#define  HPS_THR_CMDSRV_H
#include <pthread.h>
#include <define.h>
#include <cmd_globals.h>


typedef struct SCmdsrvCtx
{
   int              free;
   int              status;
   int              fd;
   pthread_t        tid;
   pthread_mutex_t  CmdsrvMutex;
   pthread_cond_t   CmdsrvCond ;
} TSCmdsrvCtx;

extern TSCmdsrvCtx  tab_CmdsrvCtx [  ];

void  InitTabCmdsrvCtx();
int   GetFreeIndex();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_CMDSRV_H ***/
