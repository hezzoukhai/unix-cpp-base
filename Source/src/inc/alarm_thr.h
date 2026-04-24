#ifndef HPS_ALRAM_THR_H  
#define HPS_ALRAM_THR_H  
#include <pthread.h>
#include <stdio.h>
#include <thread_var.h>

typedef struct timeou_s
{
   int indice;
   int delta;
}timeout_t;

typedef struct sig_inf
{
   int             flag;
   pthread_t       tid;
   int             done;
   pthread_cond_t  cond;
   pthread_mutex_t lock;
}alrm_inf_t;

/*EBE140421: Adding extern */
extern alrm_inf_t tab_alrm[];

void   InitTabAlarm();
void   THR_Alarm(int delta,int * flag);

#endif /* HPS_SIG_ALRM_H   */

