#ifndef HPS_VISA_PARAM_H
#define HPS_VISA_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#include <resources.h>

/* Resource Specific Parameters read from P7_BASE1_RESOURCE_PARAM table */
extern char  StationId    [ ];
extern char  CountryCode  [ ];
extern char  AlphaCode3   [ ];
extern char  AlphaCode2   [ ];
extern char  BankLocation [ ];
extern char  AcquirerId   [ ];
extern char  ReversalGeneration [ ];
extern char  sInterfaceMode [ ];
extern char	 g_pris_connect_mode[ ];
extern int   EchoTestTimer; /*HAL26082020 PLUTONL-2185*/

void VisaToIsoAcceptorName(int  nIndexCtx,char *VisaAcceptName,char *IsoAcceptName);/*PLUTONL-3660*/


#define		BASE1_STATION_ID            "B01"
#define		BASE1_ACQUIRER_ID           "B02"
#define		BASE1_COUNTRY               "B03"
#define		BASE1_ALPHA_CNTR_1          "B04"
#define		BASE1_ALPHA_CNTR_2          "B05"
#define		BASE1_BANK_LOCATION         "B06"
/*Start IBO20200622 PLUTONL-1967*/
#define		BASE1_INC_REQ	            "B07"
#define		BASE1_OUTG_RESP             "B08"
#define		BASE1_OUTG_REQ              "B09"
#define		BASE1_INC_RESP              "B10"
/*End IBO20200622 PLUTONL-1967*/


#endif /*** HPS_VISA_PARAM_H ***/
