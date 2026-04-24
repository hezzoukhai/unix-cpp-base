#ifndef HPS_DEVENT_PARAM_H
#define HPS_DEVENT_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#define NULL_STATUS			0
#define INIT_STATUS 		1
#define IN_PROCESS_STATUS 	2
#define SUCCES_STATUS 		3
#define ERROR_STATUS 		-1

/*


#define MAX_NOTIF_RECORDS   20




int   notif_polling_time;
int		NbrNotifMsg;

                                                                                                    
extern TSNotifMsg NotifMsg[];
*/

typedef struct 
{
	char	CardNumber			[22 	+ 1];
	char	Mti					[4 		+ 1];
	char	ManageType			[1		+ 1];
	char	Stan				[6 		+ 1];
	char	Message				[3072 	+ 1];
	char	SrcResource			[6 		+ 1];
	char	DstResource			[6 		+ 1];
	char	ResponseCode		[3		+ 1];
	int		SendCounter;
	int		Status;
}TSSafMsg;


typedef enum {
	DEVENT_ST_SAF_WS		= 0,

	DEVENT_ST_QT
} E_DEVENT_SERV_TP;

typedef enum {
	DEVENT_ECD_RESOURCE		= 0,

	DEVENT_ECD_QT
} E_DEVENT_ENT_CD;


typedef struct {
	E_DEVENT_SERV_TP	service_type;
	int					service_id;
	E_DEVENT_ENT_CD		entity_code;
	char				entity_id[24 + 1];
	int					polling_time;
	int					polling_nbr;
	int					call_period;
	int					retry_nbr;
} TSDeventServices;


typedef struct {
	TSSafMsg*	tab_msg;
	int			nIndexCtx;
	int			nNbMsg;
	int			nServiceIdx;
} TSThrSWData;


#define DEVENT_MAX_LIST_SIZE			256
#define DEVENT_MAX_ENT_SERVICES			32
#define DEVENT_MAX_THR_SAF				6

extern char					g_devent_services[];
/*extern int					g_list_size;*/
extern int					g_nb_services;
extern TSDeventServices		g_tab_devent_serv[];
extern TSThrSWData			g_tab_thr_saf[];



#endif /*** HPS_DEVENT_PARAM_H ***/
