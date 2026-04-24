#ifndef HPS_CBCOM_H
#define HPS_CBCOM_H

#define DISTINCT_PI24_PERIOD					3600 /* 1 hour */


#define CONNEXION_ISS							(1<<0)
#define CONNEXION_ACQ							(1<<1)

/*Channel states */
#define CR_ST_DISCONNECTED					0
#define CR_ST_INIT						1
#define CR_ST_WAITING_CONNEXION					2
#define CR_ST_OPEN						3
#define CR_ST_FROZEN_KEEP_ALIVE					4
#define CR_ST_FROZEN_TO_CLOSE					5

/*Channel Manager states*/
#define CRM_INIT						0
#define CRM_OPEN						1


/*Node lists types*/
#define CBCOM_NL_PI24						'P'
#define CBCOM_NL_IPDU						'I'


#define CBCOM_MAX_INCIDENTS					100

/* Error codes */
#define ERR_CBCOM_DISCONNECT					700

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <hps_iso_8583.h>
#include <cbcom/cbcom_node_list.h>
#include <cbcom/tlv_cbcom.h>
#include <cbcom/cbcom_tcp.h>
#include <time.h>


typedef struct SCBComChannel {
	char			Id[PI23_LEN];
	int			state;
	unsigned int		nConsecutiveIncidents;
	time_t			CBComIncidents[CBCOM_MAX_INCIDENTS];
	unsigned int		nIncidentNextEntry;
	int			nWindowSize;
	int			nNbWaitingRD;
	int			bTNRequester;
	int			nChannelFd; /*EBE130805*/
} TSCBComChannel;


extern TSCBComChannel	CBComChannels[];

extern unsigned int	CBComCRMState;

extern int		nFdConnect;
extern char		ConnectedEntity [24 +1];
extern char		UserApplication [24 +1];
extern int		nConnexionMode;


extern unsigned int nTRC;			/* Temporisation de relance de connexion */
extern unsigned int nTC;			/* Temporisation de connexion */
extern unsigned int nTNR;			/* Temporisation de non reponse */
extern unsigned int nTTN;			/* Temporisation de terminaison normale de connexion */
extern unsigned int nTDG;			/* Timer de d�gel */
extern unsigned int nTDC;			/* Temporisation de disponibilite */
extern unsigned int nMaxNbLostTD;		/* Tol�rance de non-r�ponse d'un test de disponibilit�*/
extern unsigned int nMaxWindowSize;		/* Taille de la fenetre d'anticipation */
extern unsigned int nMaxConsecutiveIncidents;
extern unsigned int nMaxNonConsecutiveIncidents;
extern unsigned int nNonConsecutiveIncidentsPer;
extern unsigned int nNbChannels;

/*ICH 20190427 use resource param*/
#define		CB_COM_ENTITY_NAME                  "E02"
#define		CB_COM_TIMER_TRC                    "E03"
#define		CB_COM_TIMER_TC                     "E04"
#define		CB_COM_TIMER_TNR                    "E05"
#define		CB_COM_TIMER_TTN                    "E06"
#define		CB_COM_TIMER_TDG                    "E07"
#define		CB_COM_TIMER_TDC                    "E08"
#define		CB_COM_MAX_NB_CHANNELS              "E09"
#define		CB_COM_MAX_LOST_TD                  "E10"
#define		CB_COM_MAX_WINDOW_SIZE              "E11"
#define		CB_COM_MAC_CONS_INC                 "E12"
#define		CB_COM_MAX_NON_CONS_INC             "E13"
#define		CB_COM_NON_CONS_INC_PERIOD          "E14"


extern const unsigned int nNbCBComFields;

#endif
