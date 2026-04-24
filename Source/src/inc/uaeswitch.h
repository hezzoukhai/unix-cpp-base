/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/

#ifndef UAESWITCH_H
#define UAESWITCH_H

/* - Header File Inclusion Section ----------------------------------------- */
/* - System Header Files */
#ifdef SOLARIS
   #include <thread.h>
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*#include <string.h>*/
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#ifdef SOLARIS
#include <limits.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
/*
#include <sys/stream.h>
*/
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

/* - Oracle Header Files */
#include <sqlca.h>
#include <sqlcpr.h>

/* - Private Header Files */
#include <define.h>
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
#include <iso_uaeswitch.h>
#include <iso_uaeswitch_fields.h>
#include <uaeswitch_tcp.h>
#include <p7_services.h>


/* - Constantes Definition Section ----------------------------------------- */
#define UAESWITCH_NETWORK_CODE     "21"
#define UAESWITCH_INST_ID          "784419236"
#define UAESWITCH_ACQ_COUNTRY_CODE "784"
#define UAESWITCH_DFLT_ACCNT_TYPE  "00"
#define UAESWITCH_DFLT_CRNCY       "784"
#define UAESWITCH_RSC_ID           "NTUAES"
#define EI_APPROVED_REQ            "00"
#define MAX_ECHO_TEST              3

#define PROCESSED     1 /* Trx already on autho_activity            */
#define NOT_PROCESSED 2 /* Trx not on autho_activity                */
#define WAITING_UPD   3 /* Trx should make update of autho_activity */

/* key types */
#define AWK           1
#define IWK           2

/* key inedx */
#define UNKOWN        0
#define FIRST         1
#define SECOND        2
#define THIRD         3

#define BAL_CURRENT   "01"
#define BAL_AVAILABLE "02"

/* bitmaps field presence values */
#define A  0 /* Absent */
#define M  1 /* Mandatory */
#define C  2 /* Conditional */
#define MR 3 /* Mandatory and as in Request */
#define CR 4 /* Conditional and as in Request */

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSUaeSwitch {
   char *F_szRsrcId;
   char *F_szResourceStatus;
   int  F_nSendNbr;
   int  F_nEchoTestPeriod;
} SUaeSwitch;

/* BitMap Generic Structure */
typedef struct SMsgBitMap {
   int         nPresenceFlag;
} TSMsgBitMap;


/* - Function Declaration Section ------------------------------------------ */
/* - Public Functions Declaration */
extern int  ThreadMaskSignals(void);

extern int  InitModule(char * P_szResourceId);
extern void DumpGlobals (void);
extern int  GetMaxSafCounter(void);
extern char GetDeclineAdvicesFlag ( char * Resource );

void *thread_ADMIN(void * arg);
extern void  AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);
extern void  FCT_STOP_INTERFACE (int nIndexCtx, TSIsoInfo *IsoInfo);
extern void  FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
extern void  FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);
extern void  FCT_STATISTICS (int nIndexCtx, TSIsoInfo *IsoInfo);
extern void  FCT_LINE_DOWN (int nIndexCtx, TSIsoInfo *IsoInfo);
extern void  FCT_LINE_UP (int nIndexCtx, TSIsoInfo *IsoInfo);
extern int   shutdown_resource(int nIndexCtx, char *ResourceId);

extern void * thread_wait_BAL(void * arg);
extern void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

extern void * thread_CTL(void * arg);

extern void * thread_wait_LINE(void * arg);
extern void   LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

extern int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo, char * pReqType);
extern int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx, TSUaeSwitchInfo * UaeSwitchInfo);
extern int  FCT_NW_MNG_REQ_FROM_NW (int , TSUaeSwitchInfo *);
extern int  FCT_NW_ADV_REQ_FROM_NW (int nIndexCtx, TSUaeSwitchInfo  *NapsInfo);
extern int  initial_sign_on_to_nw (int nIndexCtx );
extern int  sign_off (int nIndexCtx );
extern int  sign_on (int nIndexCtx );

void * thread_SAF(void * arg);
extern int SafManagement( int nIndiceCtx , char *sResource );
extern int SafResponseTraitement(int nIndiceCtx ,
                          char *sTlvBuffer ,
                          char *sSlfMti,
                          char *sSlfResponseCode ,
                          char *sInternalRespCode );
extern int SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  );

void * thread_SIG(void * arg);

void * thread_signin(void * arg);
extern int GenerateEchoTest( int P_nIndiceCtx );
int PrepareNetMsg (int P_nIndexCtx, char *P_szFctCode, TSIsoInfo *P_psIsoInfo);

extern void * thread_TIMEOUT(void * arg);
extern int TimeProcMsg(int P_nIndexCtx, TSTimeExchange *sTimeMsg);

extern int ConvUaeSwitchToISO( int nIndexCtx, TSUaeSwitchInfo *UaeSwitchInfo, TSIsoInfo *IsoInfo );
extern int IsoToNwRsp (char *sSrc, char *sTar);

extern int GetStlmntDate(int nIndexCtx, char * szNetCode, char *szBusinessDate, char * szStlmntDate);
extern int SetStlmntDate(int nIndexCtx, char * szNetCode, char * szStlmntDate);
extern int GetOrgnlData(int nIndexCtx,
                 char * szTlvBuffer,
                 char * szRespCode,
                 char * szOrgnlData,
                 char * szOrgnlRefNbr,
                 char * szOrgnlAuthId);

extern int Buffer_To_UaeSwitch(int nIndexCtx , TSUaeSwitchInfo *UaeSwitchInfo);
extern int  TrnReplyToUaeSwitch(
               int               nIndexCtx,
               TSIsoInfo        *IsoInfo,
               TSUaeSwitchInfo  *OrigUaeSwitchInfo);

extern int  GetKey (
               int   nIndexCtx,
               char *szRsrcId,
               int   nKeyType,
               int   *nKeyIndex,
               char *szKey);
extern int ProcessKeyExchange(int nIndexCtx, char * szNtlnData);

extern int BuildReqToNw(
              int              nIndexCtx,
              TSMsgBitMap      sMsgBitMap[],
              TSIsoInfo       *sMyIsoInfo,
              TSUaeSwitchInfo *sNwIsoInfo);
extern int BuildReqFromNw(
              int              nIndexCtx,
              TSMsgBitMap      sMsgBitMap[],
              TSUaeSwitchInfo *sNwIsoInfo,
              TSIsoInfo       *sMyIsoInfo);
extern int BuildRepToNw(
              int              nIndexCtx,
              TSMsgBitMap      sMsgBitMap[],
              TSUaeSwitchInfo  *sOrgNwInfo,
              TSIsoInfo       *sMyIsoInfo,
              TSUaeSwitchInfo *sNewNwInfo);
extern int BuildRepFromNw(
              int              nIndexCtx,
              TSMsgBitMap      sMsgBitMap[],
              TSUaeSwitchInfo *sNwInfo,
              TSIsoInfo       *sOrgMyIsoInfo,
              TSIsoInfo       *sNewMyIsoInfo);

/* Transaction request & response processing */
extern int FCT_TRN_REQ_FROM_NW(int nIndexCtx, TSUaeSwitchInfo  *UaeSwitchInfo);
extern int FCT_TRN_REP_TO_NW(int nIndexCtx, TSIsoInfo *IsoInfo);
extern int FCT_TRN_REQ_TO_NW(int nIndexCtx, TSIsoInfo  *IsoInfo);
extern int FCT_TRN_REP_FROM_NW(int nIndexCtx, TSUaeSwitchInfo  *UaeSwitchInfo);
extern int FCT_RECONC_ADV(int nIndexCtx, TSUaeSwitchInfo  *UaeSwitchInfo, char *ReconcType);
extern int FCT_REV_REQ_FROM_NW(int nIndexCtx, TSUaeSwitchInfo  *UaeSwitchInfo);
extern int FCT_REV_REQ_TO_NW(int nIndexCtx, TSIsoInfo  *IsoInfo, char *pTlvBuffer );
extern int FCT_REV_REP_FROM_NW(int nIndexCtx, TSUaeSwitchInfo  *UaeSwitchInfo);

/* - Variables Definition Section ----------------------------------------- */
/* - Module Variables Declaration --------------------------------------- */

/* - Module Variables Publication --------------------------------------- */
extern SUaeSwitch G_sUaeSwitch;
extern char       AcquirerBank [ 6 + 1 ];
extern int        nFdConnect;
extern int        nFdLocal;
extern char       ResourceId         [ 6 +1];
extern char       ResourceName       [15 +1];
extern char       ResourceStatus     [ 3 +1];
extern char       ResourceInternalId [32 +1];

#endif /* UAESWITCH_H */


