#ifndef HPS_NDC_PARAM_H
#define HPS_NDC_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <ndcglobls.h>
#include <define.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char  AcquirerBank [6 +1];
/* Resource Specific Parameters */
char  Coord_nbr_general  [ 5 ];

char   buffer_rec[LG_MAX];
int    lng;
int    gTransLanguage;
int    gPrintFlag;

#endif /*** HPS_NDC_PARAM_H ***/
