#ifndef  HPS_THR_POSISO_H
#define  HPS_THR_POSISO_H
#include <pthread.h>
#include <define.h>



typedef struct SPisoCtx
{
   int              free;
   int              status;
   pthread_t        tid;  
   int              fd;
   int              flag_bal;
   char             szBufferBal [ LG_MAX ];
   int              nLenBal;
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
} TSPisoCtx;

#define THR_IDX_FROM_CTX(a)		(a) - RESERVED_THREAD


extern TSPisoCtx  tab_PisoCtx [ ];

void  InitTabPisoCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_POSISO_H ***/
