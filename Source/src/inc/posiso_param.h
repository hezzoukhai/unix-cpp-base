#ifndef HPS_POSISO_PARAM_H
#define HPS_POSISO_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
extern char  MacUsage			 [ ];
extern char  DukptUsage			 [ ];




#define		PROCESSED     1 /* transaction deja inserer dans autho activity */
#define		NOT_PROCESSED 2 /* trans n'est pas inserer dans Autho_activity */
#define		WAITING_UPD   3 /* transction doit faire la maj de autho_activity */
#define     INVALID_MAC   4


#define POSISO_PAR_TLS_CERT			"001"
#define POSISO_PAR_TLS_PK			"002"
#define POSISO_PAR_TLS_CA_CERT		"003"
#define POSISO_PAR_TLS_VERIF_CLT	"004"


#endif /*** HPS_POSISO_PARAM_H ***/
