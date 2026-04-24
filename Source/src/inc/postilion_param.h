#ifndef HPS_SID_PARAM_H
#define HPS_SID_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#include <resources.h>

extern char  ResourceStatusSAF  [ ];
extern char  CenterCode         [ ];
extern char  NetworkCode        [ ];
extern char  RoutingPid        	[ ];/*R.HANDAL*/
extern char  RoutingCode        [ ];/*R.HANDAL*/


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

extern char  g_mac_mng_flag;
extern char	 g_auth_proc_mode;
extern char	 g_dyn_key_exchange;

extern char  gvOutletVerifFlag;
extern char  gvVirtualOutlet[ ];
extern char  g_pris_connect_mode[ ];
extern char	 g_t_des_option;

/* Resource Echo Test Timer */
extern int   EchoTestTimer;
extern int   MaxSendEchoTest;

extern char  gvHostNotification[ ];
extern char  ResourceName       [ ];       
extern char  ResourceStatus     [ ];
extern char  ResourceStatusSAF  [ ];
extern char  ResourceInternalId [ ];
extern char  AcquirerBank		[ ];


#define DEFAULT_BANK 				"CA0001"
#define HOST_ZENIT_RESOURCE         "020001"
#define BENEFICIARY_ACCOUNT_NAME	"P70"
#define SPEC_RESP_TAG				"P54" 
char test_CSC [2+1];
char FLAG_CSC  [1+1];


#endif /*** HPS_SID_PARAM_H ***/