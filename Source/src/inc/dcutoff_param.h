#ifndef HPS_DCUTOFF_PARAM_H
#define HPS_DCUTOFF_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
extern char  ResourceId[];
extern char  ResourceName[];

extern char  ResourceStatus[];
extern char  ResourceInternalId[];
/*
char  BankCode           [ 6 +1];
char  CenterCode         [ 2 +1];
char  NetworkCode        [ 2 +1];
*/

/* Resource Specific Parameters read from P7_BANK_RESOURCE_PARAM table */
extern char  CutOverTime[ ];
extern int  CutOverPeriod;


#endif /*** HPS_DCUTOFF_PARAM_H ***/
