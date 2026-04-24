#ifndef  HPS_THR_GABD_H
#define  HPS_THR_GABD_H
#include <pthread.h>
#include <define.h>
#include <ndcglobls.h>
#include <account.h>


#ifdef _INTER_VAR_
        pthread_mutex_t gabDTabMutex   = PTHREAD_MUTEX_INITIALIZER;
#endif
#ifdef _EXT_VAR_
        pthread_mutex_t gabDTabMutex;
#endif



typedef struct SgabDCtx
{
   char             szBufferLine	[LG_MAX];
   char             Gabd_Buffer 	[LG_MAX];
   char             szBufferBal 	[LG_MAX];
   char  			gTlvBuffer		[LG_MAX];
   char             adress 			[  17  ];
   char             terminal		[  16  ];
   char 			TransLanguage;
   char             gSourceAccount  [  25  ];
   char             gTargetAccount  [  25  ];
   int              free;
   int              status;
   int              fd;
   int              fd_centre;
   int              flag_bal;
   int              nLenLine;
   int              nLenBal;
   int  			PrintFlag;
   int  			etat;
   int  			key;
   int  			key_trsf;
   account_s        gAccountList[MAX_ACCOUNT];
   SContext			gContext;
   STrnRef			gTrnRef;
   pthread_t        tid;  
   pthread_mutex_t  gabDMutex;
   pthread_cond_t   gabDCond ;
} TSgabDCtx;

TSgabDCtx  tab_gabDCtx [ MAX_THREAD ];

void  InitTabgabDCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_GABD_H ***/
