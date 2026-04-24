/*
** <Name>
**  cbae.h
** 
** <Descr>
**  general header file for "cbae" interface
** 
** <Module History>
**  Version       Date      Author          Comments
**  1.0.0.0       30Sep04   Younes OUADI    Initiale Version
**
*/

#ifndef CBAE_H
#define CBAE_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
   #ifdef SOLARIS
      #include <thread.h>
   #endif
   #include <pthread.h>
/*
   #include <stlib.h>
*/
   #include <stdio.h>
   #include <string.h>
   #include <signal.h>
   #include <fcntl.h>
   #include <errno.h>
   #ifdef SOLARIS
   #include <limits.h>
   #endif

   #include <sys/types.h>
   #include <sys/socket.h>
   /*#include <sys/stream.h>*/
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
   #include <general_macros.h>
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
   #include <p7_sid.h>
   #include <iso_hps.h>
   #include <service.h>
   #include <iso_cbae.h>
   #include <iso_cbae_fields.h>
   #include <cbcom.h>


#define PR_PTRN_CLEAR			0
#define PR_PTRN_HIDE			1
#define PR_PTRN_ENCRYPT			2

typedef struct tlv_info_st
{						
   char    sTlvName[8];	
   int     nType;		
   int     nLength;		
   int	   nPrintPattern;
}tlv_info_type;			


#include <hps_iso_8583.h>
#include <cbcom.h>
#include <cbae_define.h>
#include <iso_cbae_fields.h>
#include <cbae/gen_tlv_add_data.h>
#include <cbae/gen_tlv_nat_data.h>
#include <cbae/gen_tlv_add_resp_data.h>
#include <cbae/gen_tlv_uf_data.h>
#include <cbae/gen_tlv_services_data.h>
#include <cbae/gen_tlv_sectorial_data.h>
#include <cbae/gen_tlv_fund_transfer_data.h>
#include <cbae/gen_tlv_cb_reserved_data.h>
#include <cbae/gen_tlv_add_data_int.h>

/* Not supported by ProC 
#ifdef _DEBUG
#define trace_debug(a,b,args...)		trace_message(a,b,args)
#else
#define trace_debug(a,b,args...)		
#endif
*/





#define PROCESSED     1				/** TRX Already on AUTHO_ACTIVITY            **/
#define NOT_PROCESSED 2				/** TRX Not on AUTHO_ACTIVITY                **/
#define WAITING_UPD   3				/** TRX Should make Update of AUTHO_ACTIVITY **/



typedef struct msg_map_st {
	int		nFieldNo;
	char	info;
	int		nCondition;
} msg_map_t;

typedef struct msg_field_format_st {
	int		nFieldNo;
	char	format;
} msg_field_format_t;

typedef struct msg_subfield_format_st {
	/*int		nFieldNo;*/
	char*	szTag;
	/*int		nIndex;*/
	char	format;
} msg_subfield_format_t;


/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   extern int  ThreadMaskSignals(void);
   extern int  InitModule(char * P_szResourceId);
   extern void DumpGlobals (void);
   extern int  GetMaxSafCounter(void);
   extern char GetDeclineAdvicesFlag ( char * Resource );
   extern void GetGlobals(TSResources * P_sResource);
   void *thread_ADMIN(void * arg);
   extern void  AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);
   extern void  FCT_STOP_INTERFACE (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern void  FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern void  FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern void  FCT_STATISTICS (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern void  FCT_LINE_DOWN (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern void  FCT_LINE_UP (int nIndexCtx, TSIsoInfo *IsoInfo);
   extern int   shutdown_resource(int nIndexCtx);

   extern void * thread_wait_BAL(void * arg);
   extern void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

   extern void * thread_CTL(void * arg);

   extern void * thread_wait_LINE(void * arg);
   extern int   LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

   extern int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo, char * pReqType);
   extern int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx, TSCbaeInfo * CbaeInfo);
   extern int  FCT_NW_MNG_REQ_FROM_NW (int , TSCbaeInfo *);
   extern int  FCT_NW_ADV_REQ_FROM_NW (int nIndexCtx, TSCbaeInfo  *NapsInfo);
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
   

   extern void * thread_TIMEOUT(void * arg);
   extern int TimeProcMsg(int P_nIndexCtx, TSTimeExchange *sTimeMsg);

   
   
   extern int Buffer_To_Cbae(int nIndexCtx , TSCbaeInfo *CbaeInfo);


   /* Transaction request & response processing */
   extern int FCT_TRN_REQ_FROM_NW(int nIndexCtx, TSCbaeInfo  *CbaeInfo);
   extern int FCT_TRN_REP_TO_NW(int nIndexCtx, TSIsoInfo *IsoInfo);
   extern int FCT_TRN_REQ_TO_NW(int nIndexCtx, TSIsoInfo  *IsoInfo);
   extern int FCT_TRN_REP_FROM_NW(int nIndexCtx, TSCbaeInfo  *CbaeInfo);
   extern int FCT_RECONC_ADV(int nIndexCtx, TSCbaeInfo  *CbaeInfo, char *ReconcType);
   extern int FCT_REV_REQ_FROM_NW(int nIndexCtx, TSCbaeInfo  *CbaeInfo);
   extern int FCT_REV_REQ_TO_NW(int nIndexCtx, TSIsoInfo  *IsoInfo, char *pTlvBuffer );
   extern int FCT_REV_REP_FROM_NW(int nIndexCtx, TSCbaeInfo  *CbaeInfo);

/* - Variables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */

   /* - Module Variables Publication --------------------------------------- */
  
	
	extern char  CountryCode	 [];
	extern char  BankLocation	 [];
	extern char  ServerId		 [];
	extern char  SICBIdentifier	 [];
	extern int	 nNbKeyPacks; 


#endif /* CBAE_H */


