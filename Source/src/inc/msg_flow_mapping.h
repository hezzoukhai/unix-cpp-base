#ifndef MSG_FLOW_MAPPING_H
#define MSG_FLOW_MAPPING_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <define.h>
#include <iso_hps.h>
#include <general_macros.h>
#include <tlv_fields_len.h>/* YDA20180103 */

#define QMD_MEM_KEY                             2150
#define	QMD_MEM_KEY_OLD				2048
#define	QMD_SEM_KEY				2049
#define QMD_TAB_SEM_SIZE		40
#define QMD_TAB_SEM_PER_SET		250


/* YDA20171016(PROD00048309): replaced with nb_conc_msg & sh_memory_size 
#define SW_MON_MAX_MSG			(QMD_TAB_SEM_PER_SET*QMD_TAB_SEM_SIZE)

#define SH_MEMORY_SIZE			(SW_MON_MAX_MSG*sizeof(TSMsgMappingList))
*/

/******************************************************************************/
/* YDA20171021: SWIMON shared memory struct versions */
/* this versions should change whenever the struct of the shared mem or the the structs used in the shared mem changes*/
#define SWIMON_VER			0x50700001
#define SWIMON_ST_VER			0x17

/* YDA20171016: change index size to 20 bits instead of 16 */
#define SWIMON_MAGIC_VAL		0x507				/* P7 */
#define GET_MAGIC_VALUE(a)		(((a)&0xFFF00000)>>20)
#define GET_MSG_ID(a)			(((a)&0x000FFFFF))
#define SET_MAGIC_VALUE(a,b)		(((a)&0x000FFFFF)|(((b)<<20)))
#define SET_MSG_ID(a,b)			(((a)&0xFFF00000)|((b)&0x000FFFFF))


/******************************************************************************/
#define SEM_ID_TAB				0
/******************************************************************************/
/* Queue msg status		*/
enum {
	QMS_MSG_FLOW_FREE	= 0	,
	QMS_REQ_RECV			,
	QMS_REQ_FWD				,
	QMS_FWD_REQ_RETRIEVED	,
	QMS_FWD_REQ_SENT		,
	QMS_FWD_REP_RECV		,
	QMS_FWD_REP_REPLIED		,
	QMS_REP_RETRIEVED		,
	QMS_REP_SENT			,

	QMS_QTY
};



#define INVALID_MSG_ID		UINT_MAX



typedef struct {
	msg_id_t			msgId;
	char				msg_type[4 + 1];
	char				processing_code[2 + 1];
	char				src_node_id[4 + 1];
	char				src_resource_id[6 + 1];
	pid_t				src_resource_pid;
	char				dst_node_id[4 + 1];
	char				dst_resource_id[6 + 1];
	pid_t				dst_resource_pid;
	struct timeval			times[QMS_QTY];
	unsigned int			status;
	char				msg_stan[6 + 1];
	char				origin_code;			/* ONUS, NAT, INTER */
	char				response_code[3 + 1];
	char				isOrphanFlag;			/* Y or N */
	char				isSrcTimeOutFlag;
	char				isDstTimeOutFlag;
	char				isMalfunctionFlag;
	char				dirty_flag;
	char				isDone;
	/*struct timeval		time_exception;*/ /*YDA20180209 : Use times[0] instead of time_exception*/
	char				c_ver;				/*YDA20171021 : TSMsgMappingFlow struct version*/
	/* YDA20171113 (PROD00048309) : add fields for SAF Monitoring*/
	char				reason_code[4 + 1];
	char				terminal_id[8 + 1];
	char				bank_code[6 + 1];
	char				saf_ind;
} TSMsgMappingFlow;

typedef struct SMsgMappingList{
	union {
		TSMsgMappingFlow	MsgMappingFlow;
		char			dummy[304];/*YDA20171023(PROD00048309) : let our struct be always 312 bytes(304 for union + 8 for pointer) */
	};
	struct SMsgMappingList*	next;
} TSMsgMappingList;



extern int OpenMsgFlowInfoQueue(int nIndexCtx);
extern int CloseMsgFlowInfoQueue();

extern void InitMsgMappingFlow(TSMsgMappingFlow* MsgMappingFlow);
extern P7_INLINE msg_id_t MsgReqReceived(char* resource_id, char* msg_type, char* processing_code, char* msg_stan );
extern P7_INLINE msg_id_t MsgIsoReqReceived(char* resource_id, TSIsoInfo* msgInfo);
extern P7_INLINE void  MsgReqForwarded(msg_id_t msgId, char* dst_resource_id, pid_t dst_resource_pid);
extern P7_INLINE void MsgFwdReqRetrieved(msg_id_t msgId);
extern P7_INLINE void MsgFwdReqSent(msg_id_t msgId);
extern P7_INLINE void MsgFwdRepReceived(msg_id_t msgId);
extern P7_INLINE void MsgFwdRepReplied(msg_id_t msgId);
extern P7_INLINE void MsgRepRetrieved(msg_id_t msgId);
extern P7_INLINE void MsgRepSent(msg_id_t msgId, char* response_code);
extern P7_INLINE void MsgIsoRepSent(msg_id_t msgId, TSIsoInfo* msgInfo);
extern P7_INLINE void MsgBrsRepSent(msg_id_t msgId, char* response_code, char* reason_code, char* dst_resource_id, char* dst_node_id);
/*SKARROUMI PLUTONL-4833*/
extern P7_INLINE void MsgFwdReqRetrievedTS(msg_id_t msgId, struct timeval*  pktime);
extern P7_INLINE void MsgFwdReqSentTS(msg_id_t msgId, struct timeval*  pktime);
extern P7_INLINE void MsgFwdRepReceivedTS(msg_id_t msgId, struct timeval*  pktime);
extern P7_INLINE void MsgFwdRepRepliedTS(msg_id_t msgId, struct timeval*  pktime, char* dst_node_id);
/*SKARROUMI PLUTONL-4833*/
extern P7_INLINE void MsgTimeout(msg_id_t msgId, char* resource_id);
extern P7_INLINE void MsgIsOrphan(msg_id_t msgId);
extern P7_INLINE void MsgMalfunctionProcessing(msg_id_t msgId);
extern P7_INLINE void MsgIsDone(msg_id_t msgId);
extern P7_INLINE void MsgSetMsgSAF(msg_id_t msgId, char saf_ind);

extern int	LockMsgFlowMapping(msg_id_t msgId);
extern int	LockBlkMsgFlowMapping(msg_id_t msgId);
extern int	UnlockMsgFlowMapping(msg_id_t msgId);

extern int swimon_check_msg_id(msg_id_t msgId);

/******************************************************************************/
#define MMF_TAG_MSG_ID						"001"
#define MMF_TAG_MSG_TYPE					"002"
#define MMF_TAG_PROCESSING					"003"
#define MMF_TAG_SRC_NODE_ID					"004"
#define MMF_TAG_SRC_RES_ID					"005"
#define MMF_TAG_SRC_PID						"006"
#define MMF_TAG_DST_NODE_ID					"007"
#define MMF_TAG_DST_RES_ID					"008"
#define MMF_TAG_DST_PID						"009"
#define MMF_TAG_TIME_REQ_RECV				"010"
#define MMF_TAG_TIME_REQ_FWD				"011"
#define MMF_TAG_TIME_FWD_REQ_RETRV			"012"
#define MMF_TAG_TIME_REQ_SENT				"013"
#define MMF_TAG_TIME_REP_RECV				"014"
#define MMF_TAG_TIME_REP_REPLIED			"015"
#define MMF_TAG_TIME_REP_RETRV				"016"
#define MMF_TAG_TIME_REP_SENT				"017"
#define MMF_TAG_STATUS						"018"
#define MMF_TAG_MSG_STAN					"019"
#define MMF_TAG_ORIGIN_CODE					"020"
#define MMF_TAG_RESP_CODE					"021"
#define MMF_TAG_ORPHAN_FLAG					"022"
#define MMF_TAG_SRC_TIMEOUT_FLAG			"023"
#define MMF_TAG_DST_TIMEOUT_FLAG			"024"
#define MMF_TAG_MALFUNCTION_FLAG			"025"
#define MMF_TAG_TIME_EXCEPTION				"026"
#define MMF_TAG_DONE_FLAG					"027"
#define MMF_TAG_REASON_CODE					"028"
#define MMF_TAG_TERMINAL_ID					"029"
#define MMF_TAG_BANK_CODE					"030"
#define MMF_TAG_SAF_IND						"031"
/******************************************************************************/
#define SAF_IND_MSG							'1'
#define SAF_IND_OK							'2'
#define SAF_IND_TO							'3'
#define SAF_IND_NR							'4'
/******************************************************************************/

#define MSG_CUR_BLK					1		/* Current Msg block */
#define MSG_NEW_BLK					0		/* New Msg Block		*/

#endif

