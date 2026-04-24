#ifndef HPS_PURGE_TIME_H
#define HPS_PURGE_TIME_H
#include <sys/types.h>
#include <sys/time.h>
int nTimerAcq;
int nIssAcqDelta;
int g_nAuthReplyBackPeriod; /*ZKO10032022 PLUTONL-4411*/

void  GetIssuerPurgeTime   ( time_t * PurgeTime);
void  GetAcquirerPurgeTime ( time_t * PurgeTime);

#endif /** HPS_PURGE_TIME_H **/
