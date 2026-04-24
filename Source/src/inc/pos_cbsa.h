/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | pos_cbsa.h                                                 |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme       | shell                                                      |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  youssef KHALOUFI       | Date de Creation  | 30/09/2002   |
+-----------------+-----------------------------+-------------------+----------+
| Lst. mod.       |                                                            |
+-----------------+------------------------------------------------------------+
| Public          |                                                            |
| Objects         |                                                            |
| of the source   |                                                            |
|                 |                                                            |
+-----------------+------------------------------------------------------------+
| Dependances     |                                                            |
+-----------------+------------------------------------------------------------+
*/

#ifndef PWC_POS_CBSA__F__H
#define PWC_POS_CBSA__F__H


#include<iso_hps.h> 
#include<resources.h>
#include<queue.h>
#include <iso_cbsa.h>

/* youssef KHALOUFI       07/10/2002                                           |*/

 

#define CBSA_MAX_LEN			10000

typedef struct SSPos_cbsaMsg
{
    int		len;
    char	data[CBSA_MAX_LEN];
	
} SPos_cbsaMsg;

/* Definition Parametre POS POS_CBSA */
#define     POS_CBSA_AUTHO_REQ			    10


#define		PROCESSED         1  /* trx deja inserer dans autho activity     */
#define		NOT_PROCESSED     2  /* trx n'est pas inserer dans Autho_activity*/
#define		WAITING_UPD       3  /* trx doit faire la maj de autho_activity  */





/*		pos_cbsa_convert_functinality1.c		*/

int		CbsaAuthoRecPosToIso( int nIndiceCtx , SPos_cbsaMsg * SpPos_cbsaMsg , TSIsoInfo * IsoInfo);









/*		pos_cbsa_ctl.c		*/

void *	thread_CTL(void * arg);





/*		functinality cbsa		*/

int		CbsaAuthoRecReqReplyToPos(int nIndiceCtx , TSIsoInfo * IsoInfo , SPos_cbsaMsg * spPos_cbsaMsg, char *sTlvBuffer, int nFlag );

int		FCT_PROCESS_CBSA_AUTHO_REQ( int nIndiceCtx , SPos_cbsaMsg *SpPos_cbsaMsg  );

int		FCT_INVALID_TRANSACTION(int nIndiceCtx , char * Pos_cbsaBuffer );


/*     functinality cbpr     */

int    FCT_PROCESS_CBPR_REF_REQ( int nIndiceCtx );
int    FCT_PROCESS_CBPR_STOP_LIST( int nIndiceCtx );
int    FCT_PROCESS_CBPR_AUT_24_64(  int nIndiceCtx, SPos_cbsaMsg *SpPos_cbsaMsg, int type  );
int    FCT_PROCESS_CBPR_AUTHO_04_REQ( int nIndiceCtx );
int    FCT_PROCESS_CBPR_TRANSFERT( int nIndiceCtx , SPos_cbsaMsg *SpPos_cbsaMsg  );
int    FCT_PROCESS_CBPR_REVERSAL( int nIndiceCtx , SPos_cbsaMsg *SpPos_cbsaMsg , int type );
int    CBPR_TAC_DLE_DCB( int nIndiceCtx, int type );
int    attente_acquittement(int nIndiceCtx );
int    attente_acquittement_fin(int nIndiceCtx);
int    send_acquittement(int nIndiceCtx, int ack_type);
int    attente_mess_24_64(int nIndiceCtx , char *autho_request, int type);
char GetCbprResponseCode (int response_code);
void   format_card_number(char		*card_number);








/*		pos_cbsaini.c		*/

void	ThreadMaskSignals();

void	sig_segv(int X);

void	GetGlobals (TSResources  *sResource);

int		InitServer(char  *ResourceId);



/*		pos_cbsa_proc.c		*/

void	* thread_proc_POS(void * arg);

int		WaitEvent( int nIndiceCtx,char *sBuffRec,int *nLenPos_cbsa, SPos_cbsaMsg *sPos_cbsaMsg);

int		GetPos_cbsaEvent(int nIndiceCtx, char * sMsgPos_cbsa , int nLenPos_cbsa , SPos_cbsaMsg    *SpPos_cbsaMsg);

int		ProcessEvent( int nIndiceCtx , char *sMsgPos_cbsa, int nMsgLen, int nEvent , SPos_cbsaMsg  *SpPos_cbsaMsg );

int ParseDemandeEchanche(int nIndiceCtx, char * sMsgPos_cbsa, int nLenPos_cbsa);


/*		pos_cbsa_services.c		*/

void	FCT_STOP_SERVER (int nIndexCtx, TSIsoInfo *IsoInfo);

void	FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);

void	FCT_STOP_SERVER (int nIndexCtx, TSIsoInfo *IsoInfo);

void	FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);

void	FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);

void	FCT_STATISTICS(int nIndexCtx, TSIsoInfo *IsoInfo);



/*		pos_cbsa_sig.c		*/

void *	thread_SIG(void * arg);



/*		pos_cbsa_trait_adm.c		*/

void   AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);


#endif








