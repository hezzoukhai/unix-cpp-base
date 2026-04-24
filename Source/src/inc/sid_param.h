#ifndef HPS_SID_PARAM_H
#define HPS_SID_PARAM_H


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

extern char  ResourceStatusSAF  [ ];
extern char  BankCode           [ ];
extern char  CenterCode         [ ];
extern char  NetworkCode        [ ];



/* Resource Specific Parameters read from P7_BANK_RESOURCE_PARAM table */
extern int   SendCounterMax;
extern int   AdviceInterval;
extern char  DeclineAdvicesFlag;
extern char  DynamicAdviceFlag;
extern char  FileUpdateMessagesFlag;
extern char  CVC2Management;
extern char  MultiCurrencyFlag;
extern char  ECommerceManagement;
extern char  ChipManagement;
extern char  RPIMngFlag;
extern char  CutOffStatus;
extern char  CutOverTime[ ];
extern char  gAcqBankCode [ ];
extern char  ReversalGeneration [ ];
/*extern char  MacManagementFlag[ ];*/
extern char  g_mac_mng_flag;
extern char	 g_auth_proc_mode;
extern char	 g_dyn_key_exchange;
extern char  gvOutletVerifFlag[ ];
extern char  gvVirtualOutlet[ ];
extern char  gvTtipleDes[ ];
extern char  g_pris_connect_mode[  ];

/* Resource Echo Test Timer */
extern int   EchoTestTimer;
extern int   MaxSendEchoTest;

#define BANK_MALFUNCTION_RESPONSE  "909"


/* Structure holding fields to be used for settlement purposes */
typedef struct SStlmntInfo
{
  char   Currency        [ 3 + 1];
  char   Date            [ 4 + 1];
  char   CreditNbr       [10 + 1];
  char   DebitNbr        [10 + 1];
  char   CreditRevNbr    [10 + 1];
  char   DebitRevNbr     [10 + 1];
  char   CreditAmount    [16 + 1];
  char   DebitAmount     [16 + 1];
  char   CreditRevAmount [16 + 1];
  char   DebitRevAmount  [16 + 1];
  char   StlmntAmount    [17 + 1];
  char   InqNbr          [10 + 1];
  char   AutNbr          [10 + 1];
   
} TSStlmntInfo;

/*
char  StlmntId  [12 +1];
*/


typedef struct s_action_and_reason
{
    char bank_action	[  3 ];
    char reason_code    [  4 ];
    char iso_action	[  3 ];
    char description 	[ 256 ];
}t_action_and_reason;

/*Start IBO20200622 PLUTONL-1967*/
#define		BANK_INC_REQ	           "K12"
#define		BANK_OUTG_RESP             "K13"
#define		BANK_OUTG_REQ              "K14"
#define		BANK_INC_RESP              "K15"
/*End IBO20200622 PLUTONL-1967*/

#endif /*** HPS_SID_PARAM_H ***/



