#ifndef P7__SERVICES__H
#define P7__SERVICES__H


#define CONTAINER_NAME_MAX_LEN			1024
#define SERVICE_NAME_MAX_LEN			256
#define MAX_SHLIB_HANDLES				128

/* indicator when calling fnServiceReply */
#define FWD_MSG_REPLIED					1
#define FWD_MSG_MALFUNCTION				9

/* Intial service to be called when receiving a request from line*/
typedef int (*fnService)(int nIndexCtx,char* sTlvBuffer);

/* Service to be called on timeout if the request has been forwarded */
typedef int (*fnServiceTimeOut)(int nIndexCtx,char queue_type, char* sTlvBuffer);

/* Service to be called when a response is received if the request has been forwarded */
typedef int (*fnServiceReply)(int nIndiceCtx, TSIsoInfo *IsoInfo , char *sTlvBuffer, int nEvtInd);

/* Service to be called if ever we have to process the service on behalf of the destination */
typedef int (*fnServiceStandin)(int nIndexCtx,char* sTlvBuffer);

/* User custom service */
typedef int (*fnUserService)(int nIndexCtx,char* sTlvBuffer, char* sParam);


fnServiceStandin	GetStandinService(char* szServiceContainer, char* sServiceName);
fnServiceReply		GetServiceReply(char* szServiceContainer, char* sServiceName);
int					CloseTransactionsContainers();
int					ReloadTransactionsContainers();


#endif

