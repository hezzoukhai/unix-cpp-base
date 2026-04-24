#ifndef  HPS_THR_DBL_H
#define  HPS_THR_DBL_H

#include <pthread.h>
#include <define.h>
#include <dblglobls.h>
#include <account.h>
#include <dbl_utility.h>
#include <billing.h>

/*BEGIN Adil HMAMI: 20/08/2005*/
/*EBE150203: Commented as not used + never initialize in a header file!
pthread_mutex_t DblTabMutex   = PTHREAD_MUTEX_INITIALIZER;
/*END Adil HMAMI*/


typedef struct SDblCtx
{
   char         	szBufferLine	[LG_MAX];
   char         	szBufferBal 	[LG_MAX];
   char  		gTlvBuffer		[LG_MAX];
   char             	adress 			[  17  ];
   char             	terminal		[  16  ];
   char             	AcquirerBank	[  6+1  ];
   char 		TransLanguage;
   char  		Coord_number	[   4  ];
   char             	gSourceAccount  [  25  ];
   char             	gTargetAccount  [  25  ];
   char             	gUtility  		[  25  ];
   char             	gBill_Id  		[  31  ];
   char             	gBill_Ref  		[  31  ];
   char             	gBill_Currency  [  4   ];
   char             	gBill_Date  	[  11  ];
   char             	gBill_Type  	[  3   ];
   int              	free;
   int              	status;
   int              	fd;
   int              	flag_bal;
   int              	flag_commande;
   int              	nLenLine;
   int              	nLenBal;
   int  		PrintFlag;
   int  		OpReq;
   long             	Periode;
   long             	Tempexp;
   int              	gBill_Nbr ;
   double           	gBill_TMontant ;
   double           	gBill_Montant ;
   account_s        	gAccountList[MAX_ACCOUNT];
   utility_s        	gUtilityList[MAX_UTILITY];
   billing_s        	gBillList	[MAX_BILLING];
   pthread_t        	tid;  
   pthread_mutex_t  	DblMutex;
   pthread_cond_t   	DblCond ;
   SContext		gContext;
   STrnRef		gTrnRef;
   SSystem		gSystem;
} TSDblCtx;

TSDblCtx  tab_DblCtx [ MAX_THREAD ];

void  InitTabDblCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);


#endif /***  HPS_THR_DBL_H ***/
