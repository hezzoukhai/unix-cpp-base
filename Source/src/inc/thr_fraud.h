#ifndef  HPS_THR_fraud_H
#define  HPS_THR_fraud_H

#include <pthread.h>
#include <define.h>


typedef struct SfraudCtx
{

   
   char             rule_id 		[6 + 1];
   char             rule_seq		[3 + 1];
   char             bank_code       [6 + 1]; 
   int				RuleIndex ;
   
   int              free;
   int              status;
   pthread_t        tid;  
   pthread_mutex_t  FraudMutex;
   pthread_cond_t   FraudCond ;
   
} TSfraudCtx;

extern TSfraudCtx  tab_FraudCtx [  ];

void  InitTabfraudCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

void * thread_trx_scoring_calc  (void * arg);	/*AMER20180902 PROD00060617: Thread for calculating transaction scoring for authorizations*/
void * thread_proc_Fraud  (void * arg);
void * thread_wait_BAL(void * arg);
void * thread_SIG(void * arg);

#endif /***  HPS_THR_fraud_H ***/
