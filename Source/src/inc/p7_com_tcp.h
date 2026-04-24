#ifndef P7_COM_TCP__H
#define P7_COM_TCP__H

typedef struct {
	char					szCltAddress[256];
	E_CONN_STATUS			eStatus;
	int						bConnected;
#ifdef _WIN32
	SOCKET					kSocket;
	WSADATA					kWsaData;
#endif
	int						nFd;

} TSTCPCltComInfo;

typedef struct {
	int							nNbConn;
	char						szHostAddr[256];
	int							nPortNbr;
	TSTCPCltComInfo*			tab_conn;
	const TSComInfoProperties*	ComInfoProperties;
} TSTCPComInfo;

extern void		InitCltTCPComInfo(TSTCPCltComInfo* pkTCPCltComInfo);
extern void		InitTCPComInfo(TSTCPComInfo* pkTCPComInfo);


extern int		TCP_InitCltCom(TSTCPComInfo* pkTCPComInfo);
extern int		TCP_InitSrvCom(TSTCPComInfo* pkTCPComInfo);
extern int		TCP_AcceptCallSrv(TSTCPComInfo* pkTCPComInfo, int* nClientConnIndex);
extern void		TCP_StopCom(TSTCPComInfo* pkTCPComInfo, int nClientConnIndex);
extern void		TCP_StopSrvCom(TSTCPComInfo* pkTCPComInfo);
extern int		TCP_ReadLineMsg (TSTCPComInfo* pkTCPComInfo, int nConnIndex, char *sBuffer, int* nLength);
extern int		TCP_WriteLineMsg(TSTCPComInfo* pkTCPComInfo, int nConnIndex, const char *sBuffer, int nLength);


#endif