#ifndef BASE24_H
#define BASE24_H

    #ifdef SOLARIS
        #include <thread.h>
    #endif
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <errno.h>
    #ifdef SOLARIS
        #include <limits.h>
    #endif

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/socketvar.h>
    #include <arpa/inet.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <netinet/tcp.h>

	#include <syslog.h>

    /* - Oracle Header Files */
    #include <sqlca.h>
    #include <sqlcpr.h>

    /* - Private Header Files */
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


#include <base24/base24_define.h>
#include <base24/iso_base24.h>
#include <base24/iso_base24_atm_fields.h>
#include <base24/iso_base24_pos_fields.h>
#include <base24/iso_base24_base_fields.h>
#include <base24/iso_base24_host_fields.h>
#include <base24/base24_tokens.h>

#define PROCESSED     1				/** TRX Already on AUTHO_ACTIVITY            **/
#define NOT_PROCESSED 2				/** TRX Not on AUTHO_ACTIVITY                **/
#define WAITING_UPD   3				/** TRX Should make Update of AUTHO_ACTIVITY **/


typedef struct tlv_info_st
{
   char    sTlvName[8];
   int     nType;
   int     nLength;
}tlv_info_type;

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
	char*	szTag;
	char	format;
} msg_subfield_format_t;

enum {

	AUT_REQ_MSG			= 1<<0,
	FIN_REQ_MSG			= 1<<1,
	FILE_UPD_MSG		= 1<<2,
	REV_MSG				= 1<<3,
	REC_MSG				= 1<<4,
	ADM_MSG				= 1<<5,
	FEE_MSG				= 1<<6,
	NET_MNG_MSG			= 1<<7,
};

#define ALL_SERVICES_CHANNEL	(AUT_REQ_MSG|FIN_REQ_MSG|FILE_UPD_MSG|REV_MSG|REC_MSG|ADM_MSG|FEE_MSG|NET_MNG_MSG)

typedef struct {
	int		nFd;
	int		state;
	int		nIndex;
	int		nEchoTestTimer;
	char	sHostAddr[256];
	int		nPort;
	int		bServices;
	char	signon_flag;
	char	status;
} TSComChannel;

extern TSComChannel	Channels[];



/******************************************************************************/
/******************************************************************************/








#define			B24_ISS_MODE		(1<<0)
#define			B24_ACQ_MODE		(1<<1)

/*NAB08102020 PLUTONL-2288*/

#define NOT_CONNECTED                         -18


extern int                   nReconnectInterval;


/*NAB08102020 PLUTONL-2288*/


/******************************************************************************/
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
   extern int   shutdown_resource(int nIndexCtx);

   extern void * thread_wait_BAL(void * arg);
   extern void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

   extern void * thread_CTL(void * arg);

   extern void * thread_wait_LINE(void * arg);
   extern int   LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

   extern int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo);
   extern int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx, TSBase24Info * msgInfo);
   extern int  FCT_NW_MNG_REQ_FROM_NW (int , TSBase24Info *);
   extern int  FCT_NW_ADV_REQ_FROM_NW (int nIndexCtx, TSBase24Info  *msgInfo);
   

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
   


   extern void * thread_TIMEOUT(void * arg);
   extern int TimeProcMsg(int P_nIndexCtx, TSTimeExchange *sTimeMsg);


   extern char        AcquirerBank        [];
   extern char        ResourceInternalId  [];
   extern char        ResourceId          [];
   extern char        ResourceName        [];
   extern char        ResourceStatus      [];
   extern char        ResourceInternalId  [];
   /*extern int         EchoTestTimer;*/
   extern char        ReversalGeneration  [];
   extern char        AcquirerId		  [];
   

   extern int			nNbChannels;
   extern char			sDPCNumber[];
   extern char			sNetworkIdentifier[];
   extern char			sBankId[];
   extern int			nFdConnect;
   extern char			szLogicalNetwork[];
   extern char			szStationIndex[];
   extern int			nInterfaceMode;

   extern char			B24BaseResourceCode[];
   extern char			B24HostResourceCode[];
   extern char			B24POSResourceCode[];
   extern char			B24ATMResourceCode[];
#endif


