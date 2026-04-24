#ifndef P7_INT1___H
#define P7_INT1___H


#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <resources.h>
#include <p7_define.h>
#include <p7_sid.h>

#include <queue.h>
#include <struct_line.h>
#include <struct_time.h>
#include <p7_com.h>
#include <iso8583.h>
#include <msg_flow_mapping.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


/*Below functions need to be implemented by the user interface*/
/*int Int1UserNwMsgToIso(int nIndexCtx,MsgInfo* pkMsgInfo, TSIsoInfo* pkIsoInfo, TSIsoInfo* pkOrigIsoInfo);
int Int1UserNwMsgFromIso(int nIndexCtx,TSIsoInfo* pkIsoInfo, MsgInfo* pkMsgInfo, MsgInfo* pkOrigMsgInfo);
*/

int UserInt1BalProcMsg(int nIndexCtx, TSBalExchange* pkBalExchange);
int UserInt1AdmProcMsg(int nIndexCtx, TSBalExchange* pkBalExchange);
int UserInt1TimeProcMsg(int nIndexCtx, TSTimeExchange* pkTimeExchange);
int UserInt1LineProcMsg(int nIndexCtx, TSLineExchange* pkLineExchange);
int UserInt1SafProcMsg(int nIndiceCtx , char* sMti, char* sStan, char* sSrcResource, int nSendCounter, char* sResponseCode, char* sCardNumber , char* sTlvBuffer);

int UserInt1OnShutdownResource(int nIndexCtx , char *ResourceId  );
int UserInt1OnConnect(int nIndexCtx );

extern conn_id_t			nConnId;
extern conn_id_t            		nSrvConnId;






/*Below functions are provide by int1 library */
void* int1_thread_ADMIN(void *arg);
void* int1_thread_wait_BAL(void *arg);
void* int1_thread_CTL(void *arg);
void* int1_thread_wait_LINE(void *arg);
void* int1_thread_SAF(void *arg);
void* int1_thread_SIG(void * arg);
void* int1_thread_TIMEOUT(void *arg);


int Int1AdmProcMsg(int nIndexCtx, TSBalExchange * sBalMsg);
int Int1BalProcMsg(int nIndexCtx, TSBalExchange * sBalMsg);
int Int1LineProcMsg(int nIndexCtx, TSLineExchange * sLineMsg);
int Int1TimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
int Int1SafProcMsg(int nIndiceCtx , char* sMti, char* sStan, char* sSrcResource, int nSendCounter, char* sResponseCode, char* sCardNumber , char* sTlvBuffer);

#include <int1_struct.h>
int Int1InitModule(int nIndexCtx, char *ResourceId);
int int1_shutdown_resource ( int nIndexCtx , char *sResourceId  );

int Int1SafResponseTraitement(	int nIndiceCtx , 
							char *sTlvBuffer , 
							char *sSlfMti,   
							char *sSlfResponseCode , 
							char *sInternalRespCode );
int int1SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  );

int Int1ReplyToNw(int nIndexCtx, TSIsoInfo* pkIsoInfo, MsgInfo* pkOrigMsgInfo);


/******************************************************************************/


#define I1RET_PROC_ERR			9
#define I1RET_PROCESSED			10
#define I1RET_ROUTED			11


/******************************************************************************/
int INT1_FCT_AUT_REQ_FROM_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo*  pkIsoInfo, char* sTblInd);


#endif


