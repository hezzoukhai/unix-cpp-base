#ifndef  HPS_THR_POSCB2A_H
#define  HPS_THR_POSCB2A_H
#include <pthread.h>
#include <define.h>


#ifdef _INTER_VAR_
	pthread_mutex_t PCb2aTabMutex;   = PTHREAD_MUTEX_INITIALIZER;
#endif
#ifdef _EXT_VAR_
	pthread_mutex_t PCb2aTabMutex; 
#endif

#define CB2A_NO_SERVICE			0
#define CB2A_SERV_TLC			(1<<0)
#define CB2A_SERV_TLP			(1<<1)
#define CB2A_SA_REF_DATA		(1<<2)
#define CB2A_PA_REF_DATA		(1<<3)
#define CB2A_SA_PARAMS			(1<<4)
#define CB2A_INIT_ID			(1<<5)

#define	TLP_MAX_WINDOW_SIZE		20

/* POS State values */
enum {

	CB2A_ST_NONE		= 0	,
	
	CB2A_ST_DLG_OPEN_TLC		,
	CB2A_ST_COLLECTION		,
	CB2A_ST_RECON			,

	CB2A_ST_DLG_OPEN_TLP		,
	CB2A_ST_TLP_INIT_TRANSFER	,
	CB2A_ST_TLP_UPLOAD		,
	CB2A_ST_TLP_ACTIVATION		,
	CB2A_ST_FUNC_STATE_REQ		,
	
};

/* Timers */
enum {
	CB2A_TIMER_TSI		= 0	,
	CB2A_TIMER_TNR			,
	CB2A_TIMER_TMA			,
	CB2A_TIMER_TGR			,
	CB2A_TIMER_TSM			,

	CB2A_TIMER_QTY
};

/* Recovery types */
enum {
	CB2A_REC_NONE		=0	,
	CB2A_REC_LAST_EXCHANGE		,
	CB2A_REC_DIALOG_OPEN		,
	/*CB2A_REC_DISCONNECT		,
	*/
	CB2A_RECOVERY
};

/* services */
enum {
	POS_SRV_ECHO_TEST	=0	,
	POS_SRV_RESYNCH_MNG		,

};

/* Processing mode */
enum {
	POS_MODE_AUT		=0	,
	POS_MODE_TLPTLC			,
};

#define POS_TYPE_POINT_ACC		'P'
#define POS_TYPE_CLUSTER		'C'

#define POS_TYPE_MASTER			'M'
#define POS_TYPE_SLAVE			'S'
#define POS_TYPE_AUTONOMOUS		'A'

/*Start SWI24122013*/
#define PI29_FLUX_PRODUCTION    "01"
#define PI29_FLUX_HABILITATION  "02"
#define PI29_FLUX_TEST          "03"
/*End SWI24122013*/

typedef struct SPCb2aGlobals
{
  char  szFunctionCode[ 3 + 1 ];
  char  szCurrency[ 3 + 1 ];
  char  szTerminal[8 + 1];
  char  szBankCode[6 + 1];
  char  szOutlet[15 + 1];
  char	szLogicalNumber[4 + 1];
  char	szDownloadControl[6 + 1];
  char	szAvailableServices[8 + 1];
  int	nState;
  int	nSignOnStatus;
  int   nType;
  int	nPendingServices;
  int	nCurrService;
  int	nRecoveryFlag;
  int	nHasDialogRight;
  int	nNbDialogRightExchange;
  char	nPOSType;
  int	nNbPointAcceptors;
  int	nCurrPointAcceptor;
 
  int   nNbCurrentMsg;				/* current msg in current TLP file */
  int   nNumberOfMsg;				/* Nbr of message in current TLP file */
  int   nWindowTlp;					/* Window size for current TLP file */
  int	nProcessedTlpMsgs;			/* nbr of confirmed msg received by the POS */
  int	nLastActivationResult;		/* File activation result in case we need to resend the message a second time */
  char	sFileVersion[4 + 1];

  /*Start Recon fields */
  int	nNbReconFiles;				/* nbr of remittances in current TLC */
  int	nNbReconMsg;				/* nbr of trx in current remittance */
  int	nReconWindowSize;			/* nbr of trx sent by POS before a confirmation is required from server */
  int	nCurrNbReconFiles;			/* Current remittance being processed */
  int	nCurrNbReconMsgs;			/* current trx being processed on current remittance "window" */
  int	nTotalCurrNbReconMsgs;		/* current trx being processed on current remittance */
  int	nReconIncidentMsg;			/* Msg number of the first trx on which we faced an incident during TLC*/
  int	nConsecutiveErrorsOnMsg;	/* Counter on nbr of incidents on the same message nbr */
  int	nConsecutiveMsgRetrans;		/* counter on nbr of times the same msg is being sent from the POS */
  int	nProcessedReconMsg;			/* Nbr of trx processed and saved on DB	*/
  int	nReconResult;
  /*End Recon fields */

  int	nSendDateAndTime;
  char	szLastDigest[32];
  char	szLineBackupBuffer[LG_MAX];
  int	nLineBackupLen;
  int	nLineBackupOffset;

  int	nCurrTimer;
  int	nTimerActiveFlag;
  int	nTimers[CB2A_TIMER_QTY];

  int	nNbIncidents;

  char  sDigestBuffer[LG_MAX];
  int   nOffset[TLP_MAX_WINDOW_SIZE];
  int	nCurrOffset;

  /*SWI12112013*/
  char  sSA_LogicalNo[4];
  char  sSA_ArchType[3];
  char	sSiteType[8 + 1];
  char  appl_status;
  char  sTASA[33];
  char  sERT[3];
  /*SWI12112013*/

  
  int	nInitFlag;
  char	sLogBuffer[2048];
  int	nLogBufferOffset;
} TSPCb2aGlobals;

typedef struct SPCb2aCtx
{
   int              free;
   int              status;
   pthread_t        tid;
   pthread_t        p7_tid;
   int              fd;
   int              flag_bal;
   char             szBufferLine[ LG_MAX ];
   int              nLenLine;
   char             szBufferBal [ LG_MAX ];
   int              nLenBal;
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
   TSPCb2aGlobals   stGlobals;
} TSPCb2aCtx;

TSPCb2aCtx  tab_PCb2aCtx [ MAX_THREAD ];

void  InitTabPCb2aCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

extern char g_acquirer_id[];


#endif /***  HPS_THR_POSCB2A_H ***/
