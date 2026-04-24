#ifndef HPS_JCC_PARAM_H
#define HPS_JCC_PARAM_H
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
/*EBE140922: are now declared in resources.h
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];
*/
char  BankCode           [ 6 +1];
char  CenterCode         [ 2 +1];
char  NetworkCode        [ 2 +1];
char  sInterfaceMode     [ 1 +1];

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
char   MCElectronicMngFlag; /** MAW20051109: Europay 05.2 **/
char   UCAFMngFlag;         /** MAW20051123: UCAF and RPI Mng Flags **/
char   RPIMngFlag;          /** MAW20051123: UCAF and RPI Mng Flags **/
char  CutOffStatus;
char  CutOverTime[ 4 + 1];
char  gAcqBankCode [ 6 + 1];
char  ReversalGeneration [ 1 +1];


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
    char description 	[ 256 ];
}t_action_and_reason;

#endif /*** HPS_JCC_PARAM_H ***/
