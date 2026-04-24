#ifndef P7_COM_SSL__H
#define P7_COM_SSL__H


#define TLS_SRV_VERIF_CLT_NO		0
#define TLS_SRV_VERIF_CLT_YES		1


typedef struct {
	char				szCltAddress[256];
	E_CONN_STATUS		eStatus;
	int					bConnected;
	BIO *				conn_bio;
} TSCtlSSLComInfo;

typedef struct {
	char						szHostAddr[256];
	int							nPortNbr;
	int							nNbConn;
	char						szCertificateFile[256];
	char						szPrivateKeyFile[256];
	char						szCA[256];
	const TSComInfoProperties*	ComInfoProperties;
	TSCtlSSLComInfo*			tab_conn;
	SSL_CTX*					ctx;
	SSL*						ssl;
	BIO*						root_bio; 
	BIO*						accept_bio;
	BIO*						buffer_bio;
	int							nVerifyPeer;
} TSSSLComInfo;



extern void		InitCltSSLComInfo(TSCtlSSLComInfo* pkCtlSSLComInfo);
extern void		InitSSLComInfo(TSSSLComInfo* pkSSLComInfo);


extern int		SSL_InitCltCom(TSSSLComInfo*	pkSSLComInfo);
extern int		SSL_InitSrvCom(TSSSLComInfo*	pkSSLComInfo);
extern int		SSL_AcceptCallSrv(TSSSLComInfo*	pkSSLComInfo, int* nClientConnIndex);
extern void		SSL_StopCom(TSSSLComInfo*	pkSSLComInfo, int nClientConnIndex);
extern void		SSL_StopSrvCom(TSSSLComInfo*	pkSSLComInfo);
extern int		SSL_ReadLineMsg (TSSSLComInfo*	pkSSLComInfo, int nConnIndex, char *sBuffer, int* nLength);
extern int		SSL_WriteLineMsg(TSSSLComInfo*	pkSSLComInfo, int nConnIndex, const char *sBuffer, int nLength);

#endif