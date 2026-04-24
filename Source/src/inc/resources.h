
#ifndef HPS_RESOURCES_H
#define HPS_RESOURCES_H
#include <sys/types.h>

#define NO_ROUTING_RESOURCE 		"000000"
#define STAND_IN_RESOURCE 			"000001"

/* EBE140723: Should no more be used. We have many resources of each type in multiple nodes*/
#define DAEMON_SCREEN_RESOURCE     	"000002"
#define POS_RESOURCE		     	"000010"
/* EBE140920
#define HSM_VERIF   		     	"070000"
#define BNT_VERIF   		     	"07BNT0"
*/
#define VRS_RESOURCE		     	"050000"
#define VISA_RESOURCE		     	"110000"

#define DSCREEN_RES_CODE			"DS"
#define BRIDGE_RES_CODE				"BR"

/* YDA20171113 (PROD00048309)*/
#define ATM_RES_CODE				"60"
#define POS_CB2A_RES_CODE			"20"
#define POS_PPWM_RES_CODE			"21"

#define RES_TYPE_PRIV_GETWAYS                	'G'
#define RES_TYPE_INT_NET                   		'N'
#define RES_TYPE_NAT_SWITCH						'S'
#define RES_TYPE_POS                          	'P'
#define RES_TYPE_ATM                          	'A'
#define RES_TYPE_BLACK_BOX                    	'B'
#define RES_TYPE_HOST_INTERFACE               	'H'
#define RES_TYPE_VIRTUAL                      	'V'


#define COM_TP_ASYNC		"00" 	/* ASYNC			*/
#define COM_TP_ASYNC_SEL	"01" 	/* ASYNC POLL/S		*/
#define COM_TP_X25			"02" 	/* X25 / FR			*/
#define COM_TP_TCPIP		"03" 	/* TCP/IP			*/
#define COM_TP_SNA			"04" 	/* SNA				*/
#define COM_TP_TLS			"05"

/* EBE140723: Define global lengths*/
#define NODE_ID_LEN					4
#define RESOURCE_ID_LEN				6
#define RESOURCE_CODE_LEN			2
#define RESOURCE_NAME_LEN			15
#define RESOURCE_STATUS_LEN			3
#define RESOURCE_INTERNAL_ID_LEN	32
#define ACQUIRER_BANK_LEN			6
#define RES_COMMS_ID_LEN			2
#define RES_PROTO_ID_LEN			2
#define RES_DEV_NAME_LEN			64
#define RES_REMOTE_APP_NAME_LEN		64
#define RES_REMOTE_APP_ID_LEN		64
#define RES_LOCAL_APP_NAME_LEN		64
#define RES_LOCAL_APP_ID_LEN		64
#define RES_CONNECT_MODE_LEN            1


/*Start IBO20200622 PLUTONL-1967*/
#define NW_INC_REQ			0
#define NW_OUTG_RESP		1
#define NW_OUTG_REQ			2
#define NW_INC_RESP			3
/*End IBO20200622 PLUTONL-1967*/


/* EBE140922: Global variables shared by every resource */
extern char     NodeId[];
extern char		ResourceId[];
extern char		ResourceInternalId[];
extern char		ResourceName[];
extern char		ResourceStatus[];
extern char 	CommsId[];
extern char 	ProtocolId[];
extern char 	DeviceName[];
extern char 	RemoteAppName[];
extern char 	RemoteAppId[];
extern char 	LocalAppName[];
extern char 	LocalAppId[];
extern int 		nTimerAcq;
extern int 		nIssAcqDelta;
extern int 		MinTroublesInd;
extern int              nTroublesInd;/*ICH20160809*/
extern int 		MaxTroublesInd;
extern int 		max_thread;
extern int 		nUserThread;
extern char		AcquirerBank[];
extern int      nEchoTestTimer;
extern char     ConnectMode[];
extern char     ReversalGeneration[];
extern char		g_pris_connect_mode[  ];
extern pid_t 	g_nSwiMonPid; /*IBO20200527 RawMsg*/
extern char		g_sCaptureMsg[]; /*IBO20200622 PLUTONL-1967*/
extern int   	EchoTestTimer;
extern char     AcquirerId   [ ];
extern char     CountryCode  [ ];
extern char     BankLocation [ ];

extern int      g_nAuthReplyBackPeriod; /*ZKO10032022 PLUTONL-4411*/
extern char   	g_keyfmt; /*AHA20220929 PLUTONL-4980*/

#define DBINST_LEN		4

extern char g_LocDBServ[];


typedef struct SResources
{

 char     resource_id	[ 6 + 1 ];
 char     resource_code	[ 2 + 1 ];
 char	  node_id		[4 + 1];	/*EBE140708*/
 char     resource_name	[ 16 + 1 ];
 char     component_type	[ 4 + 1 ];
 char     status	[ 1  + 1];
 char     subject_of_supervise	[ 1 + 1 ];
 char     object_name	[ 60 + 1 ];
 char     pris_object_path	[ 60 + 1 ];
 long      ipc_critical_level_nb_msg;
 int      ipc_critical_percent_freespace;
 int      ufs_critical_percent_freespace;
 long      ufs_critical_size_freespace;
 int      dbts_critical_count_extent;
 char     pris_start_script_name	[ 40 + 1 ];
 char     pris_start_script_path	[ 60 + 1 ];
 char     pris_stop_script_name	[ 40 + 1 ];
 char     pris_stop_script_path	[ 60 + 1 ];
 char     pris_subject_of_auto_start	[   1 + 1];
 char     pris_support_adm_message	[ 1 + 1];
 char     pris_comms_id	[ 2  + 1];
 char     pris_protocol_id	[ 2 + 1 ];
 char     pris_connect_mode	[ 1 + 1 ] ;
 char     pris_device_name	[ 64 + 1 ] ;
 char     pris_remote_application_name	[ 64 + 1 ];
 char     pris_remote_application_id	[ 64 + 1 ];
 char     pris_local_application_name	[ 64 + 1 ];
 char     pris_local_application_id	[ 64 + 1 ];
 char     pris_log_file_name	[ 20 + 1 ];
 int      pris_log_max_size;
 int      pris_start_cycle;
 int      pris_end_cycle;
 char     pris_trace_level	[ 1 + 1 ];
 int      pris_nb_static_threads;
 int      pris_nb_user_threads_vs_kernel;
 int      pris_timer_acquirer;
 int      pris_delta_timer_iss_acq;
 long     pris_echo_test_timer;
 int      pris_transaction_sim_max;
 char     pris_gen_reversal_timeout_late	[ 1 + 1 ];
 int      pris_nb_no_succ_response_stop;
 int      pris_nb_timeout_stop;
 int      pris_current_cycle;
 char     resource_live	[ 1 + 1 ];
 char     pris_processing_step	[ 3 + 1 ];
 char     pris_internal_id	[ 32 + 1 ];
 pid_t    pris_pid;
 pid_t    pris_ppid;
 pthread_t   pris_thread_id;
 int      pris_min_troubles_ind;
 int      pris_max_troubles_ind;
 char     pris_troubleshoot	[ 1 + 1 ];
 char     pris_troubleshoot_upd_resid	[ 6 + 1 ];
 char     pris_troubleshoot_update_date	[14 + 1 ]; /* YYYYMMDDhhmmss */
 char     pris_bank_code	[6 + 1 ];
 char     pris_acq_transport_key_number [3 + 1 ];
 char     pris_iss_transport_key_number [3 + 1 ];

}TSResources;

#endif /**  HPS_RESOURCES_H **/
