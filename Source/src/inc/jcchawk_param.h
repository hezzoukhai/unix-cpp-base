#ifndef HPS_JCCHAWK_PARAM_H
#define HPS_JCCHAWK_PARAM_H
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

#endif /*** HPS_JCCHAWK_PARAM_H ***/
