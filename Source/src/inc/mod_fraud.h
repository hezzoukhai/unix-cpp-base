#ifndef MOD_FRAUD__H
#define MOD_FRAUD__H

#include <list_thr.h>

/******************************************************************************/

#define FRD_RT_CD_LEN				6
#define FRD_RT_CD_PID_LEN			8
#define FRD_ALT_RT_CD_LEN			6
#define FRD_ALT_RT_CD_PID_LEN		8
#define FRD_RT_FLAG_LEN				1
#define FRD_TO_PER_LEN				6
#define FRD_DEF_ACTION_LEN			3
#define FRD_PARAM_LEN			(FRD_RT_CD_LEN + FRD_RT_CD_PID_LEN + FRD_ALT_RT_CD_LEN + FRD_ALT_RT_CD_PID_LEN + FRD_RT_FLAG_LEN + FRD_TO_PER_LEN + FRD_DEF_ACTION_LEN)


#define FRD_RT_CD_OFST				0
#define FRD_RT_CD_PID_OFST			(FRD_RT_CD_OFST + FRD_RT_CD_LEN)
#define FRD_ALT_RT_CD_OFST			(FRD_RT_CD_PID_OFST + FRD_RT_CD_PID_LEN)
#define FRD_ALT_RT_CD_PID_OFST		(FRD_ALT_RT_CD_OFST + FRD_ALT_RT_CD_LEN)
#define FRD_RT_FLAG_OFST			(FRD_ALT_RT_CD_PID_OFST + FRD_ALT_RT_CD_PID_LEN)
#define FRD_TO_PER_OFST				(FRD_RT_FLAG_OFST + FRD_RT_FLAG_LEN)
#define FRD_DEF_ACTION_OFST			(FRD_TO_PER_OFST + FRD_TO_PER_LEN)

#define RUN_MODE_LOCAL		'L'
#define RUN_MODE_REMOTE		'R'

/*JOU20170726: define setup modes*/
#define	SETUP_MODE_CLIENT	'C'
#define SETUP_MODE_SERVER	'S'

/******************************************************************************/
int InitFraudModule();
int FrdCheckInquiry(int nIndexCtx,char* sTlvBuffer);
int FrdCheckResponse(int nIndexCtx, char* sTlvBuffer);


int  InsertFraudRequest(TSNodeData * data);
int  UpdateFraudRequest (TSNodeData * data, char* sResponseCode);
int  FetchFraudResponse (TSNodeData * data, char* sResponseCode);
int  FreeTimeOutFraudRequest ();
/******************************************************************************/




#endif


