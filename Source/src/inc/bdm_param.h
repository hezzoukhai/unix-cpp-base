#ifndef HPS_BANK_PARAM_H
#define HPS_BANK_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* File Update Constants */
/*
#define  INSERT  1
#define  UPDATE  2
*/

#include <resources.h>

extern char  BankCode           [ ];
extern char  CenterCode         [ ];
extern char  NetworkCode        [ ];
extern char  ReversalGeneration       [ ];


extern int   SendCounterMax;
extern int   AdviceInterval;
extern char  DeclineAdvicesFlag;
extern char   DynamicAdviceFlag;
extern char   FileUpdateMessagesFlag;
extern char   CVC2Management;
extern char   MultiCurrencyFlag;
extern char   ECommerceManagement;
extern char   ChipManagement;
extern char  CutOffStatus;
extern char  CutOverTime[ ];



#define 			BANK_ACQUIRER_BANK_CODE             "K01"
#define 			BANK_SEND_COUNTER_MAX               "K02"
#define 			BANK_ADVICE_INTERVAL                "K03"
#define 			BANK_CUTOVER_TIME                   "K04"
#define 			BANK_DEC_ADVICES_PROCESSING         "K05"
#define 			BANK_DYNAMIC_ADVICES_FLAG           "K06"
#define 			BANK_FILE_UPDATE_MESSAGES_FLAG      "K07"
#define 			BANK_CVC2_MANAGEMENT_FLAG           "K08"
#define 			BANK_MULTI_CCYY_MNGT_FLAG           "K09"
#define 			BANK_E_COMM_MNGT_FLAG               "K10"
#define 			BANK_CHIP_MANAGEMENT_FLAG           "K11"
/*Start IBO20200622 PLUTONL-1967*/
#define				BANK_OUTG_REQ                  		"K12"
#define				BANK_INC_RESP                  		"K13"
/*End IBO20200622 PLUTONL-1967*/

#endif /*** HPS_BANK_PARAM_H ***/
