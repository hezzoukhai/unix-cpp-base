#ifndef HPS_QUEUE_H
#define HPS_QUEUE_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h> /*IBO20220817 PLUTONL-5088*/
#include <define.h>


#define BAL_MESSAGE                       0
#define BAL_ADMIN                         1
#define BAL_ADM_MSG                       2

/****** QUEUE Management ***/
#define		BAL_MSG 	 	(key_t)256 
#define		BAL_ADM 	 	(key_t)257 
#define		BAL_HSM_VERIF 	        (key_t)258 
#define		BAL_HSM_PROD  	        (key_t)259 
#define		BAL_SHL       	        (key_t)260
#define		BAL_BDG					(key_t)261
#define                BAL_FRD                                 (key_t)262
#define		BAL_NTW_RAW_MSG	        (key_t)263

/*SKARROUMI PLUTONL-4900*/
#define     BAL_BRIDGE_R    (key_t)264
/*SKARROUMI PLUTONL-4900*/
#define        QPID_LAUNCHER_TYPE      250
/************************************/
typedef struct SHsmHeader
    {
        char            sPartner[2];
        char            sSource [2];
        pid_t           nPartnerId;
        pid_t           nSourceId;
        int             nCommand;
        int             nLen;
        int             nCoNumber;
        int             tid;
        int             nIndex;
        time_t          nTimeStamp;
    } TSHsmHeader;

typedef struct SHsmExchange
    {
        TSHsmHeader     sHeader;
        char            sBody[LG_MAX];
    } TSHsmExchange;

typedef struct SHsmMessage
    {
        long            lType;
        TSHsmExchange   sText;
    } TSHsmMessage;
/************************************/


typedef struct SBalHeader
    {
        char            sPartner[2];
        char            sSource [2];
        pid_t           nPartnerId;
        pid_t           nSourceId;
        int             nCommand;
        int             nLen;
		char            nFormat  [3+1]; /*SNO010715*/
        time_t          nTimeStamp;
		msg_id_t		msgId; /*EBE130925:SWMON*/
    } TSBalHeader;


typedef struct SBalExchange
    {
        TSBalHeader     sHeader;
        char            sBody[LG_MAX];
		struct timeval  StimeB;/*PLUTONL-3762*/
    } TSBalExchange;


typedef struct SBalMessage
    {
        long            lType;
        TSBalExchange   sText;
    } TSBalMessage;

/************************************/
typedef struct 
{
    pid_t           nPartnerId;
    pid_t           nSourceId;
	char			nPartnerNodeId[4 + 1];
    int             nCommand;
    int             nLen;
	pthread_t		tid;
    int             nCoNumber;
	int             nIndex;
} TSBridgeHeader;

typedef struct 
{
    TSBridgeHeader		sHeader;
    char				sBody[LG_MAX];
} TSBridgeExchange;

typedef struct 
{
	long            lType;
	TSBridgeExchange   sText;
} TSBridgeMessage;

/*EBE140921: Adding extern */
extern int     nBalMsg;
extern int     nBalAdm;
extern int     nBalHsm;
extern int     nBalShl;
extern int     nBalHsmVerf;
extern int     nBalHsmProd;
extern int	   nBalBridge;
extern int        nBalFrd;
extern int 	   nBalNtwRawMsg;
/*SKARROUMI PLUTONL-4900*/
extern int     nBalBrr;
/*SKARROUMI PLUTONL-4900*/

/* Opens the queue identified by nBalName and returns the queue identifier
* Used to open the queues: BAL_MSG, BAL_ADM, HSM, BAL_BRIDGE_R,...etc
*/
int OpenBalCom( key_t nBalName);

/*  Read a message identified by MsgType from the queue identified by nBalId and returns the message in struct_bal
* Interfaces usually keep reading message type corresponding to their PID
*/
int ReadBalMsg(int nBalId, long MsgType, int mode, TSBalExchange *struct_bal);

/* Write the message content in the queue identified by nBalId.
* An interface A willing to send a message to an interface B will set the destination interface's PID as "message type"
* Interface A's PID is present the message header which can be used by interface B to return the messsage. 
The interface can also use field 53 to retrieve the source interface PID
*/
int WriteBalMsg(int nBalId, TSBalExchange *struct_bal);

int ReadCmdMsg(int nBalId, long MsgType, int mode, TSBridgeExchange *struct_bal);
int WriteCmdMsg(int nBalId, TSBridgeExchange *struct_bal);



#endif /* HPS_QUEUE_H */
