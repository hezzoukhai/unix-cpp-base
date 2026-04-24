#ifndef HPS_PARAM_CTX_H
#define HPS_PARAM_CTX_H
#include <pthread.h>
#include <sqlca.h>

/* Mehdi Elyajizi this Macro is already diefined by the system in
/usr/include/pthread.h */
#ifndef PTHREAD_THREADS_MAX
	#define PTHREAD_THREADS_MAX 512
#endif

typedef struct SParamCtx
{
   pthread_t   ID;
   sql_context ctx;

} TSParamCtx ;

/*EBE140421: Adding extern to "explicitly not define" this variable by including this header file */
extern TSParamCtx     tab_ParamCtx[ ];

#endif /*** HPS_PARAM_CTX_H ***/
