#ifndef  HPS_THR_HSM_H
#define  HPS_THR_HSM_H
#include <pthread.h>
#include <define.h>
#ifdef _INT_VAR_
pthread_mutex_t HsmTabMutex   = PTHREAD_MUTEX_INITIALIZER;
#endif
#ifdef _EXT_VAR_
pthread_mutex_t HsmTabMutex ;
#endif

typedef struct SHsmCtx
{
   char             szBufferBal 	[LG_MAX];
   int              nLenBal;
   int              free;
   int              status;
   int              flag_bal;
   int              co_number;
   pthread_t        tid;  
   pthread_mutex_t  HsmMutex;
   pthread_cond_t   HsmCond ;
} TSHsmCtx;


/*EBE140421: Adding extern */
extern TSHsmCtx  tab_HsmCtx [ ];

/*EBE140920 void  InitTabHsmCtx     ();*/
int   GetHsmFreeIndex   ();
int   GetHsmThreadIndex (pthread_t tid);
int	  InitTabHsm(int nIndexCtx, char* szNodeId, int mode);	/*EBE140716 */

#endif /***  HPS_THR_HSM_H ***/
