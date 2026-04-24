#ifndef MQ_COM___H
#define MQ_COM___H

#include <cmqc.h>
#include <cmqxc.h>   


typedef struct
{
	char	sMsgId[128];
	char	sCorrId[128];
	char	sReplyToQ[64];
	int		nType;
	int		nMaxSize;
} TSQMsgOpt;


int OpenMQManager(	MQHCONN*	pHcon, 
					MQCD*		pClientConn, 
					MQCNO*		pConnectOpt,
					char*		sApplicationName, 
					char*		sApplicationId,
					char*		szQManager);
int OpenIncQueue(MQHCONN* pHcon, MQHOBJ* pHobj, char *szQueue );
int OpenOutgQueue(MQHCONN* pHcon, MQHOBJ* pHobj, char *szQueue );
int CloseQueue(MQHCONN Hcon, MQHOBJ* pHobj);
int CloseMQManager(MQHCONN* pHcon);
int ReadMQMsg(MQHCONN Hcon, MQHOBJ Hobj, char *buffer, int nBufLen, int *MsgLen,TSQMsgOpt* pkQMsgOpt, int pnTimer);
int WriteMQMsg (MQHCONN Hcon, MQHOBJ Hobj , char *buffer, int nBufLen,TSQMsgOpt* pkQMsgOpt);


#endif

