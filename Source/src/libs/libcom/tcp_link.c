#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <define.h>
#include <limits.h>
#include <p7_globals.h>
#include <p7_com.h>
/******************************************************************************/
#define LINK_COM_PROP		"LinkComProp"
/******************************************************************************/
static TSComInfoProperties		gs_kLinkComProp = {
		E_CM_CLIENT,
		E_CONN_TCP,
		CLTP_HIGH_BINARY,
		2,
		2,
		0,
		LDT_LENGTH_EXCLUDED,
		1,
		256,
		0,
		LINK_COM_PROP
};
/******************************************************************************/


conn_id_t	InitLinkCom()
{
	conn_id_t	nConnId;
	nConnId = SetupComProperties(&gs_kLinkComProp);
	return nConnId;
}

conn_id_t	InitLinkSrvCom(int nNbClt)
{
	conn_id_t	nConnId;

	gs_kLinkComProp.eConnMode = E_CM_SERVER;
	gs_kLinkComProp.nMaxClients = nNbClt;

	nConnId = SetupComProperties(&gs_kLinkComProp);
	return nConnId;
}




