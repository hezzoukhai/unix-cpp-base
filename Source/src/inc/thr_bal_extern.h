#ifndef  HPS_THR_BAL_H
#define  HPS_THR_BAL_H
#include <pthread.h>
#include <define.h>
#include <cmd_globals.h>



typedef struct SBalCtx
{

   char             szBufferBal  [LG_MAX];
   int              flag_bal;
   int              flag_command;
   int 				nLenBal;
   pthread_t        tid;
   pthread_mutex_t  BalMutex;
   pthread_cond_t   BalCond ;
   int              co_number;
} TSBalCtx;



int   GetBalThreadIndex(pthread_t tid);

#endif /***  HPS_THR_BAL_H ***/
