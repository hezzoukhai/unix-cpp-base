#ifndef P7_COM__H
#define P7_COM__H

#include <p7_com_prop.h>
#include <p7_com_tcp.h>
#include <p7_com_ssl.h>
/******************************************************************************/
int									RegisterComInfoProperties	(TSComInfoProperties* pComInfoProperties);
int									SetTabComInfoProperties		(TSComInfoProperties* tab_ComInfoProperties, int nSize);
const TSComInfoProperties*			GetTabComInfoProperties		( int* nSize);
const TSComInfoProperties*const		GetComInfoProperties		(const char* szPropertiesName);
/******************************************************************************/
extern void			FreeAllComs();

extern conn_id_t	SetupComProperties	(const TSComInfoProperties* pkComInfoProperties);
extern int			ConnectLine (conn_id_t nConnIndex, const char* szHostAddr, int nPortNbr);
extern conn_id_t	InitCom		(const char*	szConnName, const char* szHostAddr, int nPortNbr);

extern conn_id_t	SetupSSLComProperties	( const TSComInfoProperties* pkComInfoProperties);
extern int			ConnectSSLLine (conn_id_t nConnIndex, const char* szHostAddr, int nPortNbr, const char* szCertFile, const char* szPrivKeyFile, const char* szCA, int nVerifyPeer);
extern conn_id_t	InitSSLCom	(const char*	szConnName, const char* szHostAddr, int nPortNbr, const char* szCertFile, const char* szPrivKeyFile, const char* szCA, int nVerifyPeer);



extern int			AcceptCallSrv(conn_id_t nConnIndex, conn_id_t* nCltConnIndex);
extern void			StopCltCom(conn_id_t nConnIndex);
extern void			StopSrvCom(conn_id_t nConnIndex);
extern int			ReadLineMsg (conn_id_t nConnIndex, char *sBuffer, int* nLength);
extern int			WriteLineMsg(conn_id_t nConnIndex, const char *sBuffer, int nLength);
extern int			tcp_readcheck( conn_id_t nConnIndex , int nTimer );

#endif
