#ifndef HPS_DNOTIF_PARAM_H
#define HPS_DNOTIF_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];
char  BankCode           [ 6 +1];
char  CenterCode         [ 2 +1];
char  NetworkCode        [ 2 +1];
int nSleepParam = 60; /* Default value 60 If not define in resource table timer Acquirer */char  CutOverTime[ 4 + 1]; /* Resource Specific Parameters read from P7_BANK_RESOURCE_PARAM table */

#endif /*** HPS_DNOTIF_PARAM_H ***/
