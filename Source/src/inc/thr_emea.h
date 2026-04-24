#ifndef  HPS_THR_EMEA_H
#define  HPS_THR_EMEA_H
#include <pthread.h>
#include <define.h>
#include <tlv_emea.h>

typedef struct SEmeaCtx
{
   int              free;
   int              status;
   pthread_t        tid;  
   int              fd;
   int              flag_bal;
   char             szBufferLine[ LG_MAX ];
   int              nLenLine;
   char             szBufferBal [ LG_MAX ];
   char             szTermNbr [ 15 + 1 ];
   int              nLenBal;
   TSTlvEmea        sTlvInfo;
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
} TSEmeaCtx;

TSEmeaCtx  tab_EmeaCtx [ MAX_THREAD ];

void  InitTabEmeaCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_EMEA_H ***/
