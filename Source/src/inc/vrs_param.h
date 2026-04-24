#ifndef HPS_VRS_PARAM_H
#define HPS_VRS_PARAM_H
#include <stdio.h>
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <struct_vrs.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char AcquirerBank [ 6 + 1 ];
/* Resource Variables           */
int        nSeqNum;
int        nProcState;
TSVrsInfo  sVrsInfo;
FILE *fp;
FILE *fperr;

#define  VRS_MAX_STATE       100

/* Protocol State Definition    */
#define   VRS_ERROR              0
#define   VRS_STATE_0            1
#define   VRS_STATE_1            2
#define   VRS_STATE_2            3
#define   VRS_STATE_3            4
#define   VRS_STATE_4            5
#define   VRS_STATE_5            6
#define   VRS_STATE_6            7
#define   VRS_STATE_7            8
#define   VRS_STATE_8            9
#define   VRS_STATE_9            10
#define   VRS_STATE_10           11
#define   VRS_STATE_11           12
#define   VRS_STATE_12           13
#define   VRS_STATE_13           14

#define   VRS_STATE_AUT          30

#define   VRS_STATE_20           21
#define   VRS_STATE_21           22
#define   VRS_STATE_22           23

#define   VRS_PASS_CHECK         53
#define   VRS_MERCHANT_CHECK     54
#define   VRS_CARD_CHECK         55

void (*tab_ProcState[VRS_MAX_STATE])();

#endif /*** HPS_VRS_PARAM_H ***/
