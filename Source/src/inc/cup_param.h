#ifndef HPS_CUP_PARAM_H
#define HPS_CUP_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <resources.h>


#define   CUP_REVERSAL          "03" 
#define   CUP_CANCELLATION   	"20"



/* Key Reset Params  */
/* char  PikLmk             [ 256 +1];
char  PikZmk             [ 256 +1];
char  CheckValuePik      [ 256 +1];
char  gvTtipleDes	   [ 1 +1];
char  TakLmk             [ 256 +1];
char  TakZmk             [ 256 +1];
char  CheckValueTak      [ 256 +1]; */

/* Resource Specific Parameters read from P7_CUP_PARAM table */
/* char  StationId    [ 6 +1]; */
/* char AcquirerBank [ 6 + 1 ]; */

extern int   nEchoTestTimer; /*AOU echo test*/
extern char  StationId [ ];




extern char  CountryCode  [ ];
/*PLUTONL-3569*/
/*extern char  AlphaCode3   [ ];
extern char  AlphaCode2   [ ];
extern char  BankLocation [ ];*/

extern char  AcquirerId   [ ];
extern char  ReversalGeneration [ ];
extern char  sInterfaceMode [ ];
/*
extern char  LocalCurrency [ ];
extern char  ReceivingId [ ];
extern char  ForwardingId   [ ];
*/

extern char		g_pris_connect_mode[ ];


#define 	CUP_STATION_ID                      "P01"
#define 	CUP_ACQUIRER_ID                     "P02"
#define 	CUP_COUNTRY                         "P03"
/*#define 	CUP_ALPHA_COUNTRY1                  "P04"
#define 	CUP_ALPHA_COUNTRY2                  "P05"
#define 	CUP_BANK_LOCATION                   "P06"
*/

/*HAL 20210427 FOR DBS DBS-14*/
#define		CUP_INC_REQ	              "P07"
#define		CUP_OUTG_RESP             "P08"
#define		CUP_OUTG_REQ              "P09"
#define		CUP_INC_RESP              "P10"
/*HAL 20210427 FOR DBS DBS-14*/
#define 	CUP_BANK_LOCATION                   "P06"

#endif /*** HPS_CUP_PARAM_H ***/