#ifndef HPS_T24_PARAM_H
#define HPS_T24_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* File Update Constants */
#define  INSERT  1
#define  UPDATE  2

#include <resources.h>

/* Resource Common Parameters ***/
/*
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];
*/

char  BankCode           [ 6 +1];
char  CenterCode         [ 2 +1];
char  NetworkCode        [ 2 +1];
char  ReversalGeneration       [ 1 +1];




/* Resource Specific Parameters read from P7_BANK_RESOURCE_PARAM table */
int   SendCounterMax;
int   AdviceInterval;
char  DeclineAdvicesFlag;
char   DynamicAdviceFlag;
char   FileUpdateMessagesFlag;
char   CVC2Management;
char   MultiCurrencyFlag;
char   ECommerceManagement;
char   ChipManagement;
char  CutOffStatus;
char  CutOverTime[ 4 + 1];

#define 	T24_ACQUIRER_BANK_CODE             "T01"
#define 	T24_SEND_COUNTER_MAX               "T02"
#define 	T24_ADVICE_INTERVAL                "T03"
#define 	T24_DEC_ADVICES_PROCESSING         "T04"
#define 	T24_CVC2_MANAGEMENT_FLAG           "T05"
#define 	T24_CUTOVER_TIME                   "T06"
#define 	T24_MULTI_CCY_MNGT_FLAG            "T07"
#define 	T24_DYNAMIC_ADVICES_FLAG           "T08"
#define 	T24_E_COMMERCE_MNGT_FLAG           "T09"
#define 	T24_FILE_UPDATE_MESSAGES_FLAG      "T10"
#define 	T24_CHIP_MANAGEMENT_FLAG           "T11"


#define BANK_MALFUNCTION_RESPONSE  "96"

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

char  StlmntId  [12 +1];



typedef struct s_action_and_reason
{
    char bank_action	[  2 ];
    char reason_code    [  4 ];
    char iso_action		[  3 ];
    char description 	[ 20 ];
}t_action_and_reason;

/* START AQE150913 */
/* Resource Echo Test Timer */
int   EchoTestTimer;
int   MaxSendEchoTest;
/* END AQE150913 */


#endif /*** HPS_T24_PARAM_H ***/
