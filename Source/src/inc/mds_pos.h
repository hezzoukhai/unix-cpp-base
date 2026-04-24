/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | mds_pos.h                                             |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme        | templat                                                   |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  Nom et Prenom              | Date de Creation  | 19/09/02 |
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
#ifndef MDS_POS_H
    #define MDS_POS_H
#ifndef HPS_POS_MDS_H
#define HPS_POS_MDS_H


typedef struct SSMdsMsg
{
    char        szField1[13];
	/*TODO*/
	/*Add Other Fields*/;
} SMdsMsg;

/* Definition Parametre POS MDS */
#define     MDS_FUNCTIONALITY1			10
#define     MDS_FUNCTIONALITY2			20
#define     MDS_EVENT_1				01
#define     MDS_EVENT_2				02

#define		PROCESSED         1  /* trx deja inserer dans autho activity     */
#define		NOT_PROCESSED     2  /* trx n'est pas inserer dans Autho_activity*/
#define		WAITING_UPD       3  /* trx doit faire la maj de autho_activity  */


/*		mds_convert_functinality1.c		*/
int		Functionality1_PosToIso( int nIndiceCtx , SMdsMsg * SpMdsMsg , TSIsoInfo * IsoInfo);


/*		mds_convert_functinality2.c		*/
int		Functionality2_PosToIso( int nIndiceCtx , SMdsMsg * SpMdsMsg , TSIsoInfo * IsoInfo);


/*		mds_ctl.c		*/
void *	thread_CTL(void * arg);


/*		mds_functinality1.c		*/
int		Functionality1_ReqReplyToPos(int nIndiceCtx , TSIsoInfo * IsoInfo , SMdsMsg * spMdsMsg, char *sTlvBuffer, int nFlag );
int		FCT_PROCESS_Functionality1( int nIndiceCtx , SMdsMsg *SpMdsMsg  );
int		FCT_INVALID_TRANSACTION(int nIndiceCtx , char * MdsBuffer , int MsgLen) 


/*		mds_functinality1.c		*/
int		FCT_PROCESS_Functionality2( int nIndiceCtx , SMdsMsg *SpMdsMsg  );
int		Functionality2_ReqReplyToPos(int nIndiceCtx , TSIsoInfo * IsoInfo , SMdsMsg * spMdsMsg, char *sTlvBuffer, int nFlag );

/*		mdsini.c		*/
void	ThreadMaskSignals();
void	sig_segv(int X);
void	GetGlobals (TSResources  *sResource);
int		InitServer(char  *ResourceId);

/*		mds_proc.c		*/
void	* thread_proc_POS(void * arg);
int		WaitEvent( int nIndiceCtx,char *sBuffRec,int *nLenMds, SMdsMsg *sMdsMsg);
int		GetMdsEvent( char * sMsgMds , int nLenMds , SMdsMsg    *SpMdsMsg);
int		ProcessEvent( int nIndiceCtx , char *sMsgMds, int nMsgLen, int nEvent , SMdsMsg  *SpMdsMsg );

/*		mds_services.c		*/
void	FCT_STOP_SERVER (int nIndexCtx, TSIsoInfo *IsoInfo);
void	FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
void	FCT_STOP_SERVER (int nIndexCtx, TSIsoInfo *IsoInfo);
void	FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
void	FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);
void	FCT_STATISTICS(int nIndexCtx, TSIsoInfo *IsoInfo);

/*		mds_sig.c		*/
void *	thread_SIG(void * arg);

/*		mds_trait_adm.c		*/
void   AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);




#endif /*** HPS_POS_MDS_H ***/


#endif /* MDS_POS_H */
