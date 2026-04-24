#ifndef BRIDGE__H
#define BRIDGE__H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <syslog.h>

#include <sqlca.h>
#include <sqlcpr.h>

#include <define.h>
#include <p7_define.h>
#include <pwrmacros.h>
#include <resources.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <event_log.h>
#include <thread_var.h>
#include <param_arg.h>
#include <main_thr.h>
#include <list_thr.h>
#include <queue.h>
#include <line_com.h>
#include <tlv_switch_header.h>
#include <struct_line.h>
#include <struct_time.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <hsm_inc.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <iso_hps.h>
#include <service.h>
#include <hps_iso_8583.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <maj_card_activity.h>

#include <brs_param.h>



#define 	NULL_STATUS			0
#define 	INIT_STATUS 		1
#define 	IN_PROCESS_STATUS 	2
#define 	SUCCES_STATUS 		3
#define 	TIMEOUT_STATUS 		4
#define 	ERROR_STATUS 		-1


/*ICH20160802*/
#define		CONN_USED			'U'
#define		CONN_NEW			'N'
/*ICH20160802*/

/*Bridge replication sent mode Bulk with data compression / sent parallel messages*/
#define		MODE_BULK			'B'
#define		MODE_PARALLEL		'P'

#define		BRIDGE_OFFLINE		'F'
#define		BRIDGE_CONNECTED	'C'
#define		BRIDGE_ONLINE		'O'
#define		BRIDGE_SIGNOFF		'0'
#define		BRIDGE_SIGNON		'1'

#define		BRIDGE_INCOMING		'I'
#define		BRIDGE_OUTGOING		'O'


/* status */
#define		BR_SIGNOFF			0
#define		BR_SIGNON			1
#define		BR_TO_BE_STOPPED	9


#define		BR_PROTO_ISO		"11"
#define		BR_PROTO_TLV		"21"
#define		BR_PROTO_C_TLV		"22"/*COMPRESSED TLV ICH201+0811*/
#define		BR_PROTO_CMD_TLV	"31"
#define		BR_PROTO_LEN		2
#define 	BR_TCP_HDR_LEN		6 /*COMPRESSED TLV ICH201+0811*/


#define BRS_PROCESS_ID_LEN          16
#define BRS_SEQ_ID_LEN				10


#define		START_CTX_BAL_IDX		40
#define		START_CTX_THR_IDX		80
#define		START_CTX_TIME_IDX		120
/*#define		START_CTX_CMD_IDX		120*/
#define		START_CTX_MSG_IDX		140

#define D_BRIDGE_S_TYPE			"D_BRIDGE_S_TYPE"


void InitTabBridgeCtx();
void InitBridgeResStatus();/*ICH15072016*/

int GetBridgeThreadIndex (pthread_t tid);
int GetBridgeFreeIndex ();
int WaitNewRequest (int nIndex);
int WaitBalResponse (int nIndex, int nTimer);
int FreeBridgeIndex (int nIndex);
int InfoBridgeThread(int nIndex);
int GetBridgeFdByOraCtx(int nIndexCtx);
int ShutDownProcessingThreads ();
int IsToBeStopped(int nIndex);
int GetFdByBridgeAddr (char *sAddr, int *Id);


#define		BIG_BUF_MAX_SIZE								(2048*1200)		/*1200 slots, 2.5Mo*/



#endif

