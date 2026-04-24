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
#define AMX_GNS_COM_PROP		"AMXGNSComProp"
/******************************************************************************/
static TSComInfoProperties		gs_kAmexGnsComProp = {
	E_CM_CLIENT,E_CONN_TCP,CLTP_HIGH_BINARY,2,2,0,LDT_LENGTH_INCLUDED,0,256,0,AMX_GNS_COM_PROP
};
/******************************************************************************/


conn_id_t	InitAmexGnsCom()
{
	conn_id_t	nConnId;
	nConnId = SetupComProperties(&gs_kAmexGnsComProp);
	return nConnId;
}




