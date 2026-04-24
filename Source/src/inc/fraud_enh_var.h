#ifndef HPS_FRAUD_H
#define HPS_FRAUD_H

#include <sys/types.h>
#include <resources.h>

#define PTHREAD_F_THREADS_MAX  512

#define D_FRAUD_REFRESH			"DFRAUD_REFRESH"
#define D_FRAUD_MODE_CALC	    'C'			/*Mode calculating transaction scoring*/
#define D_FRAUD_MODE_PROC	    'P'			/*Mode executing rules on authorizations*/

typedef struct SThrFraud
{
   char     sRule[6 + 1];
   char     sEvent_seq[ 3 + 1];
   char     sBank_code[ 6 + 1]; /*AZD10022014*/
   int   	processed ;
} TSThrFraud;

extern TSThrFraud   SThrFraudRulesList[  ];
extern int nRefreshPeriod;
extern char		JobRole[];

#endif /*** HPS_FRAUD_H ***/
