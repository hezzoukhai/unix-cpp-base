#ifndef HPS_SID_PARAM_H
#define HPS_SID_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#include <resources.h>

extern char         ResourceStatusSAF  [ ];
extern char         CenterCode         [ ];
extern char         NetworkCode        [ ];
extern char         RoutingPid         [ ];/*R.HANDAL*/
extern char         RoutingCode        [ ];/*R.HANDAL*/


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
extern char  gvTtipleDes[ ];
extern char  g_pris_connect_mode[  ];
extern char	 g_t_des_option;
extern char	 g_signoff_saf_option;

/* Resource Echo Test Timer */
extern int   EchoTestTimer;
extern int   MaxSendEchoTest;

/*Start SKA20221117 PLUTONL-5393*/
typedef struct 
{
    int                 free;
    int                 fd;
    pthread_mutex_t     kMutex;
    pthread_cond_t      kCond;
} TSIntCtx;

extern TSIntCtx     tab_IntCtx         [ ];

#define             MAX_INTERFACE_THREAD                 10
/*End SKA20221117 PLUTONL-5393*/
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



#endif /*** HPS_SID_PARAM_H ***/



