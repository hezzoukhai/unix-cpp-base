#ifndef JCB_INT_PARAM_H
#define JCB_INT_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

extern char  CountryCode  [ ];
extern char  AlphaCode3   [ ];
extern char  AlphaCode2   [ ];
extern char  BankLocation [ ];
extern char  AcquirerId   [ ];
extern char  ReversalGeneration [ ];
extern char  sInterfaceMode [ ];
extern char  NegativeDataAccessCode [ ];
extern char  LocalCurrency [ ];
extern char  ReceivingId [ ];
extern char  ForwardingId   [ ];

#define 	JCB_ACQUIRER_ID                     "J01"     
#define 	JCB_FORWARDING_ID                   "J02"                 
#define 	JCB_RECEIVING_ID                    "J03"
#define 	JCB_NEG_DATA_ACCESS_CODE_TAG            "J04"
#define 	JCB_LOCAL_CURRENCY                  "J05"
/*START HAL 20210503 FOR DBS DBSSG-8*/
#define		JCB_INC_REQ	                        "J06"
#define		JCB_OUTG_RESP                       "J07"
#define		JCB_OUTG_REQ                        "J08"
#define		JCB_INC_RESP                        "J09" 
/*END HAL 20210503 FOR DBS DBSSG-8*/

#endif /*** JCB_INT_PARAM_H ***/
