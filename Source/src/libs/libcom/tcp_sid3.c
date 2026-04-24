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
#define SID3_COM_PROP		"Sid3ComProp"
/******************************************************************************/
static TSComInfoProperties		gs_kSid3ComProp = {
	E_CM_CLIENT,
	E_CONN_TCP,
	CLTP_HIGH_ASCII,
	4,
	4,
	0,
	LDT_LENGTH_EXCLUDED,
	1,
	0,
	0,
	SID3_COM_PROP
};
/******************************************************************************/


conn_id_t	InitSidCom()
{
	conn_id_t	nConnId;
	nConnId = SetupComProperties(&gs_kSid3ComProp);
	return nConnId;
}

conn_id_t	InitSidSrvCom(int nNbClt)
{
	conn_id_t	nConnId;

	gs_kSid3ComProp.eConnMode = E_CM_SERVER;
	gs_kSid3ComProp.nMaxClients = nNbClt;
	
	nConnId = SetupComProperties(&gs_kSid3ComProp);
	return nConnId;
}





