#ifndef  HPS_THR_EMEA_H
#define  HPS_THR_EMEA_H
#include <pthread.h>
#include <define.h>

typedef struct SJccHawkCtx
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
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
} TSJccHawkCtx;

TSJccHawkCtx  tab_JccHawkCtx [ MAX_THREAD ];

void  InitTabJccHawkCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_EMEA_H ***/
