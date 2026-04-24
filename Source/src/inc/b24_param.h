#ifndef HPS_B24_PARAM_H
#define HPS_B24_PARAM_H
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
char  AcquirerBank [ 6 + 1 ];
char  AcquirerId   [11 +1];
char  ReversalGeneration [ 1 +1];
long  GV_EchoTestTimer;

int   nFdLocal;
int   nFdConnect;

#define        szEwb_AcqId     "00000000062"
#define        EWB_ACQ_ID      "4142"
#define        EWB_RECV_ID     "4142"

#endif /*** HPS_B24_PARAM_H ***/
