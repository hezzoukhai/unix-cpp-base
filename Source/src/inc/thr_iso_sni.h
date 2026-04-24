#ifndef  HPS_THR_ISO_SNI_H
#define  HPS_THR_ISO_SNI_H
#include <pthread.h>
#include <define.h>
#include <iso_sni_globls.h>
#include <account.h>
#include <utility.h>
#include <billing.h>



pthread_mutex_t IsoSniTabMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct SIsoSniCtx
{
   char             szBufferLine	[LG_MAX];
   char             szBufferBal 	[LG_MAX];
   char  			gTlvBuffer		[LG_MAX];
   char             adress 			[  17  ];
   char             terminal		[  16  ];    
   int              free;
   int              status;
   int              fd;
   int              flag_bal;
   int              flag_commande;
   int              nLenLine;
   int              nLenBal;   
   pthread_t        tid;  
   pthread_mutex_t  IsoSniMutex;
   pthread_cond_t   IsoSniCond ;  
   admin_processing_globals   admin_globals;  /*to do initialisation*/
   iso_net_processing_globals iso_net_globals;/*to do initialisation*/
   main_iso_sni_globals       main_globals;   /*to do initialisation*/
} TSIsoSniCtx;

TSIsoSniCtx  tab_IsoSniCtx [ MAX_THREAD ];

extern void  InitTabIsoSniCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

