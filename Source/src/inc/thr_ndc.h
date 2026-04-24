#ifndef  HPS_THR_NDC_H
#define  HPS_THR_NDC_H
#include <pthread.h>
#include <ndcglobls.h>
#include <define.h>
#include <paramdef.h>
#include <p7_utility.h>
#include <account.h>
#include <billing.h>
#include <donation.h>
#include <ndc_param.h>
#include <command.h>


#define NOT_CARDLESS                    0
#define CARDLESS_INPUT_ACC              1
#define CARDLESS_INPUT_CARD             2

/*
typedef int (*fnATMServiceProcess)(char*,char*);
typedef int (*fnATMServiceReply)();

#define ATM_MAX_OPERATIONS		256

typedef struct SOperation
{
	char	operation_id	[4 + 1];
	char	operation_code	[12 + 1];
	char	op_container	[64 + 1];
	char	process_entry	[64 + 1];
	char	reply_entry		[64 + 1];
	char	user_param_1	[32 + 1];
	char	user_param_2	[32 + 1];

	fnATMServiceProcess     process;
	fnATMServiceReply		reply;

} TSOperation;

typedef struct SATMOps
{
	TSOperation		gOperations[ATM_MAX_OPERATIONS];
	int				nNbOperations;
	char			terminal_profile[3 + 1];
	char			bank_code[6 + 1];
} TSATMOps;
*/

/*PROD00026076*/
typedef enum  {
	ATM_ST_OFFLINE		= 0	,
	ATM_ST_CONNECTED		,
	ATM_ST_CMD_REQ			,
	ATM_ST_CDC_DL			,

	ATM_ST_QTY
} E_ATM_NDC_STATE;


typedef struct SNdcCtx
{
   char             szBufferLine		[LG_MAX];
   char             szBufferBal 		[LG_MAX];
   char  			gTlvBuffer			[LG_MAX];
   char             adress 				[  17  ];
   char             terminal			[  16  + 1];
   char 			TransLanguage;
   char  			Coord_number		[   4  ];
   char             gSourceAccount		[  NDC_ACC_NBR_LG_MAX + 1  ];
   char             gTargetAccount		[  NDC_ACC_NBR_LG_MAX + 1  ];
   char             gRecordData  		[  999  ];
   char             gService            [  25  ]; 
   char             gUtility            [  25  ];
   char             gBill_Id            [  31  ];
   char             gBill_Currency		[  4   ];
   char             gBill_Date          [  11  ];
   int              free;
   int              status;
   int              fd;
   int              flag_bal;
   int              flag_commande;
   int              nLenLine;
   int              nLenBal;
   int  			PrintFlag;
   int                          Master_idx;
   int  			OpReq;
   time_t  			expiryTime;
   double           gBill_Montant ;
   account_s        gAccountList[MAX_ACCOUNT];
   utility_s        gUtilityList[MAX_UTILITY];
   billing_s        gBillList   [MAX_BILLING];
   pthread_t        tid;  
   pthread_t        tid_spec;
   pthread_mutex_t  NdcMutex;
   pthread_cond_t   NdcCond ;
   SContext			gContext;
   STrnRef			gTrnRef;
   SSystem			gSystem;
   donation_s       gDonation		[1];      /* ADD TBO24112011 */
   sub_donation_s   gSub_donation	[1];     /* ADD TBO24112011 */
   char             gBillAmount  	[  13  ]; /* RJE 16032014 */
   char             gBill_Ref  		[  31  ];
   char             gBill_Type  	[  3   ];
   long             Periode;
   long             Tempexp;
   int              gBill_Nbr ;
   double           gBill_TMontant ;
	pthread_mutex_t NdcBalMutex  ;
	int             NdcBalFlag  ;
    char Luno       [9 + 1];
    char Tvn        [8 + 1];
	char			AcquirerBank[6 + 1];
	char			sProfileCode[3 + 1];
	int				nGmtTimeOffst;
	int					do_free;	/*EBE: PROD00026076 */

	char			App_PID      [ 9];
	char			App_TID      [ 9];
	

	ndc_param*		pkScr;
	ndc_param*		pkPrn;
	int				nCardLessType;	/* 0 - Not Cardless, 1- Card less with account Entry,2 - Card less with card Entry */
    char            sTemrCmdTimeout [1 + 1];
    struct timeval  Stime; /*IBO20221104 PLUTONL-5370*/
} TSNdcCtx;


extern TSNdcCtx  tab_NdcCtx [ ];

void  InitTabNdcCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);


const char* LabelStatusDescriptor(char SD);
const char* LabelStatusInfoValueC(char value);
const char* LabelStatusInfoQualiferC(char *SQ);

const char* ndc_dev_name(char dev);
int ProcSolMsgResponse(int  nIndexCtx, char *Terminal_Number, char  *pAcqBank, char* buffer, int lg_buffer);

#endif /***  HPS_THR_NDC_H ***/
