#ifndef HPS_AMEX_GCA_INT_H
#define HPS_AMEX_GCA_INT_H

#include <queue.h>
#include <iso_hps.h>
/*
#include <iso_amex.h>
#include <iso_amex_f127.h>*/
#include <struct_line.h>
#include <tlv_private.h>

#include <iso_amex.h>
#include <struct_time.h>

   /* - ADM: Public Functions Declaration */
   void *thread_ADMIN(void * arg);
   void  AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);
   void  FCT_STOP_INTERFACE (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_STATISTICS (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_LINE_DOWN (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_LINE_UP (int nIndexCtx, TSIsoInfo *IsoInfo);
   int SendShutDownSignalsToLineThreads(void);
   int shutdown_resource(int nIndexCtx, char *ResourceId);


   /* - BAL: Public Functions Declaration */
   void * thread_proc_BAL(void * arg);
   void * thread_wait_BAL(void * arg);
   void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

	/* - CTL: Public Functions Declaration */
   void * thread_CTL(void * arg);
   
   
	/* - LINE: Function Declaration Section ------------------------------------------ */
   void * thread_proc_LINE(void * arg);
   void * thread_wait_LINE(void * arg);
   int    LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

   /* - Public Functions Declaration */
   int ConnectInterface  (int   P_nIndiceCtx);
   int ReconnectInterface(int   P_nIndiceCtx);
   int  FCT_AUT_REQ_FROM_AMEX(int , TSAmexInfo  *);

   
   /* - Net: Public Functions Declaration */
   int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo);
   /*int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx);*/
   int  FCT_NW_MNG_REP_FROM_NW (int , TSAmexInfo *);
   int  FCT_NW_MNG_REQ_FROM_NW (int , TSAmexInfo *);
   int  initial_sign_on_to_nw (int nIndexCtx );
   int  SendNwMngMsg (int nIndexCtx, char *InfoCode );
   int  sign_off (int nIndexCtx );
   int  sign_on (int nIndexCtx );

   /* - SAF: Public Functions Declaration */
   void * thread_SAF(void * arg);
   int SafManagement( int nIndiceCtx , char *sResource );
   int SafResponseReceived ( int       nIndiceCtx ,
			     char      *sPan, /*MKB111007**/
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode );
   int SafTimeOutReceived (  int       nIndiceCtx ,
			     char      *sPan, /*MKB111007**/
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode,
                             int       nSendCounter );
   int SafResponseTraitement(int nIndiceCtx ,
                             char *sTlvBuffer ,                                
                             char *sSlfMti,
                             char *sSlfResponseCode ,
                             char *sInternalRespCode );
   int SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  );

   /* - SIG: Public Functions Declaration */
   void * thread_SIG(void * arg);
   
   /* - SignIN: Public Functions Declaration */
   void * thread_echo_test(void * arg);
   void * thread_signin(void * arg);
   int GenerateEchoTest( int P_nIndiceCtx );
   int PrepareNetMsg (int P_nIndexCtx, char *P_szFctCode, TSIsoInfo *P_psIsoInfo);

   /* - Timeout: Public Functions Declaration */
   void * thread_proc_TIME(void * arg);
   void * thread_TIMEOUT(void * arg);
   int AcquirerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
   int IssuerTimeProcMsg(int P_nIndexCtx, TSTimeExchange * sTimeMsg);
   int TimeProcMsg(int P_nIndexCtx, TSTimeExchange *sTimeMsg);
   int BuildTimeOutReversalMsg (TSIsoInfo *IsoInfo ,TSIsoInfo * IsoRevInfo);
 
   extern char  AcquirerId      [];
   extern char  InstitutionalId [];
   extern char  AmexId          [];
   extern int   MaxSendEchoTest;
   
#endif /** HPS_AMEX_GCA_INT_H **/
