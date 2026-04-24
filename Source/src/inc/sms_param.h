#ifndef HPS_SMS_PARAM_H
#define HPS_SMS_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
/*EBE140922: are now declared in resources.h
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];
char AcquirerBank        [ 6 + 1 ];
*/

#include <resources.h>



int  nFdConnect;
int  nFdLocal;


/* Resource Specific Parameters read from P7_SMS_RESOURCE_PARAM table */

/*
char  sInterfaceMode     [ 1 +1];
char  StationId          [ 6 +1 ];
char  PlusPmc            [ 6 + 1];
char  CountryCode        [ 3 +1 ];
char  AlphaCode3         [ 3 +1 ];
char  AlphaCode2         [ 2 +1 ];
char  BankLocation       [ 11 +1 ];
char  AcquirerId         [ 11 +1 ];
char  ReversalGeneration [ 1 +1];
*/

extern char  sInterfaceMode     [ ];
char  StationId          [ 6 +1 ];
extern char  PlusPmc            [ ];
extern char  CountryCode        [  ];
extern char  AlphaCode3         [  ];
extern char  AlphaCode2         [  ];
extern char  BankLocation       [  ];
extern char  AcquirerId         [  ];
extern char  ReversalGeneration [ ];

void SmsToIsoAcceptorName(int  nIndexCtx,char *SmsAcceptName,char *IsoAcceptName);/*PLUTONL-3660*/


#define	SMS_STATION_ID             "S01"
#define	SMS_ACQUIRER_ID            "S02"
#define	SMS_FORWARDER_ID           "S03"
#define	SMS_PLUS_PMC_ID            "S04"
#define	SMS_COUNTRY                "S05"
#define	SMS_ALPHA_CNTR_1           "S06"
#define	SMS_ALPHA_CNTR_2           "S07"
#define	SMS_BANK_LOCATION          "S08"
/*Start SKA04102023*
#define		SMS_INC_REQ	            "B07"
#define		SMS_OUTG_RESP             "B08"
#define		SMS_OUTG_REQ              "B09"
#define		SMS_INC_RESP              "B10"
/*End SKA04102023*/

#endif /*** HPS_SMS_PARAM_H ***/
