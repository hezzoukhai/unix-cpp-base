/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      dbl_tools_convert.c                                                    */
/*                                                                             */
/* Description                                                                 */
/*      Convert Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include <define.h>
#include <dblmsgcst.h>
#include <account.h>
#include <dbl_utility.h>
#include <dbl_billing.h>
#include <atmopcode.h>
#include <dblopcode.h>
#include <currency.h>
#include <langue.h>
#include <dblconvert.h>
#include <dblstatus.h>
#include <iso_hps.h>


char      destinations[4][31];

#define OR_ONUS_LAST_TRANSACTION 1113

SDblConvert	sDblConvert[]={
{OR_ONUS_WITHDRAWAL,	"010000",	"200"},
{OR_NAT_WITHDRAWAL,	"010000",	"200"},
{OR_INT_WITHDRAWAL,		"010000",	"200"},
{OR_ONUS_ACCOUNT_LIST,		"180000",	"691"},
{OR_RECONC_DATA_REQ,		"180000",	"692"},
{OR_ONUS_AUTHENTICATION,	"300000",	"693"},
{OR_ONUS_LAST_TRANSACTION,	"300000",	"693"},
{OR_ONUS_CASH_DPT,		"180000",	"694"},
{OR_ONUS_CHEQ_DPT,		"390000",	"694"},
{OR_ONUS_DOC_DPT,		"180000",	"694"},
{OR_ONUS_BALANCE,		"310000",	"281"},
{OR_NAT_BALANCE,		"310000",	"281"},
{OR_INT_BALANCE,		"310000",	"281"},
{OR_CR_BALANCE,			"310000",	"281"},
{OR_ONUS_SHORT_STAT,		"380000",	"691"},
{OR_ONUS_CHEQ_BOOK,		"390000",	"692"},
{OR_ONUS_STAT_REQ,		"390000",	"693"},
{OR_ONUS_INT_TFR,		"400000",	"284"},
{OR_ONUS_EXT_TFR,		"400000",	"284"},
{OR_ONUS_PIN_CHANGE,		"900000",	"304"},
{OR_ONUS_LOAN_INQUIRY,		"910000",	"305"},
{-1,				      "", 	   ""}
};

/*N.G 180703 Ajout libel francais */
STransName	sTransName[]={
{OR_ONUS_WITHDRAWAL,		"   ^>OZ &;C",		"   WITHDRAWAL","Retrait"},
{OR_NAT_WITHDRAWAL,		"   ^>OZ &;C",		"   WITHDRAWAL","Retrait"},
{OR_INT_WITHDRAWAL,		"   ^>OZ &;C",		"   WITHDRAWAL","Retrait"},
{OR_ONUS_ACCOUNT_LIST,		"   (#&#C;U\" \\;%V",	"   ACCOUNT LIST REQ",""},
{OR_RECONC_DATA_REQ,		"","RECONCILIATION",""},
{OR_ONUS_AUTHENTICATION,	"      ","   AUTHENTICATION","AUTHENTICATION"},
{OR_ONUS_CASH_DPT,		"   ^>OZ I\">_\"","   CASH DEPOSIT","DEPOT ESP"},
{OR_ONUS_CHEQ_DPT,		"   (#Q_DU\" I\">_\"","   CHEQUE DEPOSIT",""},
{OR_ONUS_DOC_DPT,		"   (\">Z(CXU\" I\">_\"","   DOCUMENT DEPOSIT",""},
{OR_ONUS_PRE_CASH_DPT,		"   ^>OZ I\">_\"","   CASH DEPOSIT","DEPOT ESP"},
{OR_ONUS_PRE_CHEQ_DPT,		"   (#Q_DU\" I\">_\"","   CHEQUE DEPOSIT",""},
{OR_ONUS_PRE_DOC_DPT,		"   (\">Z(CXU\" I\">_\"","   DOCUMENT DEPOSIT",""},
{OR_ONUS_BALANCE,		"   >_E@U\" YJ WVJ(C\"","   BALANCE","SOLDE"},
{OR_NAT_BALANCE,		"   >_E@U\" YJ WVJ(C\"","   BALANCE","SOLDE"},
{OR_INT_BALANCE,		"   >_E@U\" YJ WVJ(C\"","   BALANCE","SOLDE"},
{OR_ONUS_SHORT_STAT,		"   @E(=X &#C; MDQ","   SHORT STATEMENT","RELEVE"},
{OR_ONUS_CHEQ_BOOK,		"   (#Q_DU\" @(M> &UG","   CHEQUE BOOK REQ",""},
{OR_ONUS_STAT_REQ,		"   (#_\"XJU\" Y#_& &UG","   STATEMENT REQ","RELEVE"},
{OR_ONUS_INT_TFR,		"   ^>OZ T_[;(","   TRANSFER",""},
{OR_ONUS_EXT_TFR,		"   ^>OZ T_[;(","   TRANSFER",""},
{OR_ONUS_PIN_CHANGE,		"   ^@(U\" YOU\" @__L(","   PIN CHANGE",""},
{OR_ONUS_LOAN_INQUIRY,		"   !F@OU\" Y#_&","   LOAN INQUIRY",""},
/*{OR_ONUS_HAYRAT_TFR,            "   (\"@_]U\" &#C; ^U\" T_[;(",       "   HAYRAT","Paiement Fact"},
{OR_ONUS_E_REMITTANCE,          "   E-REMITTANCE", "   E-REMITTANCE","Paiement Fact"},
{OR_BILL_PAYMENT,               "   BILL PAYMENT", "   BILL PAYMENT","Paiement Fact"},
*/
/*{OR_BILL_PAYMENT_BATELCO,       "   [QU(&",     "   BATELCO POSTPAID","Paiement Fact"},
*/
{OR_BILL_PAYMENT_MTC,           "   ^C ^( W\"", "   MTC POSTPAID","Paiement Fact"},
{OR_BILL_PAYMENT_ELECTRICITY,   "   $#&@]QU\" \\@[(#M IM>",  "   ELECTRICITY","Paiement Fact"},
{OR_BILL_PAYMENT_CREDIT,        "   Y#X(%V\" (#O#G& \\@[(#M IM>",  "   CREDIT CARD PAYMENT","Paiement Fact"},
{OR_TOP_UP_BETELCO,             "   WC WC [QU(&",  "   SIMSIM","Paiement Fact"},
{OR_TOP_UP_MTC,                 "    ^A ^\"  ^C ^( W\" ",    "   EEZEE","Paiement Fact"},
{-1,				"","",""}
};

/***********************************************************************
  **  nom   : GetLanguageRequested               **
  **  desc   : renvoit le code de la langue choisie         **
  **  entree   : OCBuffer      -> buffer code operation      **
  **  sortie   : aucune                  **
  **  retour   : code de la langue choisie            **
  **        ERROR -> code non reconnu            **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
char   GetLanguageRequested(char *pOCBuffer, char *pTrackii)
{
   char      lLanguageRequested;
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start GetLanguageRequested(%.8s,%.37s)", pOCBuffer, pTrackii);
   trace_event(sLine, PROCESSING);

   switch(pOCBuffer[OC_LANGUAGE_POS])
   {
   case OC_ARABIC :
      lLanguageRequested = ARABIC;
      break;
   case OC_ENGLISH :
      lLanguageRequested = ENGLISH;
      break;
   case OC_FRENSH :
      lLanguageRequested = FRENSH;
      break;
   default :
      switch(pTrackii[strlen(pTrackii) - 2])
      {
      case ISO2_ARABIC   :
         lLanguageRequested = ARABIC;
         break;
      case ISO2_ENGLISH   :
         lLanguageRequested = ENGLISH;
         break;
      case ISO2_FRENSH   :
         lLanguageRequested = FRENSH;
         break;
      default   :
         lLanguageRequested = ENGLISH;
      }
   }

   sprintf(sLine, "End   GetLanguageRequested(%c)", lLanguageRequested);
   trace_event(sLine, PROCESSING);
   return (lLanguageRequested);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetOperationRequested               **
  **  desc   : renvoit le code de l'operation choisie par le    porteur**
  **  entree   : OCBuffer      -> buffer code operation      **
  **  sortie   : aucune                  **
  **  retour   : code de l'operation demandee            **
  **             ERROR -> operation non reconnue      **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
GetOperationRequested(char *OCBuffer)
{
   int             operationRequested;
   int            multi, cpt;
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start GetOperationRequested(%.8s)", OCBuffer);
   trace_event(sLine, PROCESSING);

   switch(OCBuffer[5])
    {
     case 'A' :
      operationRequested = OR_ONUS_ACCOUNT_LIST;
      break;
     /*case 'B' :
      operationRequested = OR_LOAD_UTILITY;
      break;
     case 'C' :
      operationRequested = OR_LOAD_BILLING;
      break;
     */
     default:
      operationRequested = 0;
      multi = 1000;
      for(cpt = 0; cpt < 4; cpt ++)
      {
         if(' ' != OCBuffer[OC_FUNCTION_POS + cpt])
         {
            operationRequested+=((OCBuffer[OC_FUNCTION_POS+cpt]-'A'+1)
                *multi);
         }
         multi /= 10;
      }
   }

   sprintf(sLine, "End   GetOperationRequested(%d)", operationRequested);
   trace_event(sLine, PROCESSING);
   return (operationRequested);
}

int GetOperatioBuffer(char *OCBuffer, char * sOpBuffer)
{

	char sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start GetOperatioBuffer(%.8s)", OCBuffer);
	trace_event(sLine, PROCESSING);
	memcpy(sOpBuffer,OCBuffer + OC_FUNCTION_POS,4);
	sOpBuffer[4]='\0';
	sprintf(sLine, "End   GetOperationIndex(%s)", sOpBuffer);
	trace_event(sLine, PROCESSING);
   return (OK);
}


/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetSelectedAccount               **
  **  desc   : charge le compte selectionne de l'operation      **
  **  entree   : pBBuffer   -> BBuffer code operation      **
  **  sortie   : pAccount   -> compte selectionne         **
  **  retour   : OK   -> operation OK               **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
GetSelectedAccount(char *pBBuffer, account_s pAccountList[], 
				   int pAccountListSize, 
                   char *pAccount, char *pAccountCur, char *pAccountType)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetSelectedAccount(%.32s)", pBBuffer);
   trace_event(sLine, PROCESSING);

   memset(pAccount,       0, 25);
   memset(pAccountCur,    0,  4);

   switch(pBBuffer[0])
   {
   case OC_ACCOUNT_1 :
      memcpy(pAccount, pAccountList[0].accountNumber,
          strlen(pAccountList[0].accountNumber));
      memcpy(pAccountCur, pAccountList[0].accountCurrency,
          strlen(pAccountList[0].accountCurrency));
      memcpy(pAccountType, pAccountList[0].accountType,
          strlen(pAccountList[0].accountType));
      pAccountList[0].accountAvailable = NO;
      break;
   case OC_ACCOUNT_2 :
      memcpy(pAccount, pAccountList[1].accountNumber,
          strlen(pAccountList[1].accountNumber));
      memcpy(pAccountCur, pAccountList[1].accountCurrency,
          strlen(pAccountList[1].accountCurrency));
      memcpy(pAccountType, pAccountList[1].accountType,
          strlen(pAccountList[1].accountType));
      pAccountList[1].accountAvailable = NO;
      break;
   case OC_ACCOUNT_3 :
      memcpy(pAccount, pAccountList[2].accountNumber,
          strlen(pAccountList[2].accountNumber));
      memcpy(pAccountCur, pAccountList[2].accountCurrency,
          strlen(pAccountList[2].accountCurrency));
      memcpy(pAccountType, pAccountList[2].accountType,
          strlen(pAccountList[2].accountType));
      pAccountList[2].accountAvailable = NO;
      break;
   case OC_ACCOUNT_4 :
      memcpy(pAccount, pAccountList[3].accountNumber,
          strlen(pAccountList[3].accountNumber));
      memcpy(pAccountCur, pAccountList[3].accountCurrency,
          strlen(pAccountList[3].accountCurrency));
      memcpy(pAccountType, pAccountList[3].accountType,
          strlen(pAccountList[3].accountType));
      pAccountList[3].accountAvailable = NO;
      break;
   case OC_ACCOUNT_5 :
      memcpy(pAccount, pAccountList[4].accountNumber,
          strlen(pAccountList[4].accountNumber));
      memcpy(pAccountCur, pAccountList[4].accountCurrency,
          strlen(pAccountList[4].accountCurrency));
      memcpy(pAccountType, pAccountList[4].accountType,
          strlen(pAccountList[4].accountType));
      pAccountList[4].accountAvailable = NO;
      break;
   case OC_ACCOUNT_6 :
      memcpy(pAccount, pAccountList[5].accountNumber,
          strlen(pAccountList[5].accountNumber));
      memcpy(pAccountCur, pAccountList[5].accountCurrency,
          strlen(pAccountList[5].accountCurrency));
      memcpy(pAccountType, pAccountList[5].accountType,
          strlen(pAccountList[5].accountType));
      pAccountList[5].accountAvailable = NO;
      break;
   case OC_ACCOUNT_7 :
      memcpy(pAccount, pAccountList[6].accountNumber,
          strlen(pAccountList[6].accountNumber));
      memcpy(pAccountCur, pAccountList[6].accountCurrency,
          strlen(pAccountList[6].accountCurrency));
      memcpy(pAccountType, pAccountList[6].accountType,
          strlen(pAccountList[6].accountType));
      pAccountList[6].accountAvailable = NO;
      break;
   default :
      memset(pAccount, '0', 24);
      memset(pAccountCur, '0',  3);
   }

   sprintf(sLine, "End   GetSelectedAccount(%.24s,%.3s,%.2s)", pAccount, pAccountCur, pAccountType);
   trace_event(sLine, PROCESSING);
   return (OK);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetSelectedUtility 									**
  **  desc   : charge le compte selectionne de l'operation      **
  **  entree   : pBBuffer   -> BBuffer code operation      **
  **  sortie   : pAccount   -> compte selectionne         **
  **  retour   : OK   -> operation OK               **
  ***********************************************************************/
GetSelectedUtility (char 		*pBBuffer, 
					utility_s 	pUtilityList[], 
				   	int 		pUtilityListSize, 
                   	char 		*pUtility)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetSelectedUtility (%.32s)", pBBuffer);
   trace_event(sLine, PROCESSING);

   memset(pUtility,       0, 25);

   switch(pBBuffer[0])
   {
   case OC_UTILITY_1 :
      memcpy(pUtility, pUtilityList[0].utilityNumber,
          strlen(pUtilityList[0].utilityNumber));
      pUtilityList[0].utilityAvailable = NO;
      break;
   case OC_UTILITY_2 :
      memcpy(pUtility, pUtilityList[1].utilityNumber,
          strlen(pUtilityList[1].utilityNumber));
      pUtilityList[1].utilityAvailable = NO;
      break;
   case OC_UTILITY_3 :
      memcpy(pUtility, pUtilityList[2].utilityNumber,
          strlen(pUtilityList[2].utilityNumber));
      pUtilityList[2].utilityAvailable = NO;
      break;
   case OC_UTILITY_4 :
      memcpy(pUtility, pUtilityList[3].utilityNumber,
          strlen(pUtilityList[3].utilityNumber));
      pUtilityList[3].utilityAvailable = NO;
      break;
   case OC_UTILITY_5 :
      memcpy(pUtility, pUtilityList[4].utilityNumber,
          strlen(pUtilityList[4].utilityNumber));
      pUtilityList[4].utilityAvailable = NO;
      break;
   case OC_UTILITY_6 :
      memcpy(pUtility, pUtilityList[5].utilityNumber,
          strlen(pUtilityList[5].utilityNumber));
      pUtilityList[5].utilityAvailable = NO;
      break;
   case OC_UTILITY_7 :
      memcpy(pUtility, pUtilityList[6].utilityNumber,
          strlen(pUtilityList[6].utilityNumber));
      pUtilityList[6].utilityAvailable = NO;
      break;
   default :
      memset(pUtility, '0', 24);
   }

   sprintf(sLine, "End   GetSelectedUtility(%.24s)", pUtility);
   trace_event(sLine, PROCESSING);
   return (OK);
}

/***********************************************************************
  **  nom   : GetSelectedBilling               **
  **  desc   : charge le compte selectionne de l'operation      **
  **  entree   : pBBuffer   -> BBuffer code operation      **
  **  sortie   : pBilling   -> compte selectionne         **
  **  retour   : OK   -> operation OK               **
  ***********************************************************************/
int GetSelectedBilling(char *pBBuffer, 
				   billing_s pBillingList[], 
				   int pBillingListSize, 
                   char *pBilling, 
				   char *pBillingCur, 
				   char *pBillingDate,
				   double *pBillingMontant)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetSelectedBilling(%.32s)", pBBuffer);
   trace_event(sLine, PROCESSING);

   memset(pBilling,       0, 25);
   memset(pBillingCur,    0,  4);

   switch(pBBuffer[0])
   {
   case OC_BILLING_1 :
      memcpy(pBilling, pBillingList[0].billingNumber,
          strlen(pBillingList[0].billingNumber));
      memcpy(pBillingCur, pBillingList[0].billingCurrency,
          strlen(pBillingList[0].billingCurrency));
      memcpy(pBillingDate, pBillingList[0].billingDate,
          strlen(pBillingList[0].billingDate));
	  *pBillingMontant = pBillingList[0].billingAmount;
      pBillingList[0].billingAvailable = NO;
      break;
   case OC_BILLING_2 :
      memcpy(pBilling, pBillingList[1].billingNumber,
          strlen(pBillingList[1].billingNumber));
      memcpy(pBillingCur, pBillingList[1].billingCurrency,
          strlen(pBillingList[1].billingCurrency));
      memcpy(pBillingDate, pBillingList[1].billingDate,
          strlen(pBillingList[1].billingDate));
	  *pBillingMontant = pBillingList[1].billingAmount;
      pBillingList[1].billingAvailable = NO;
      break;
   case OC_BILLING_3 :
      memcpy(pBilling, pBillingList[2].billingNumber,
          strlen(pBillingList[2].billingNumber));
      memcpy(pBillingCur, pBillingList[2].billingCurrency,
          strlen(pBillingList[2].billingCurrency));
      memcpy(pBillingDate, pBillingList[2].billingDate,
          strlen(pBillingList[2].billingDate));
	  *pBillingMontant = pBillingList[2].billingAmount;
      pBillingList[2].billingAvailable = NO;
      break;
   case OC_BILLING_4 :
      memcpy(pBilling, pBillingList[3].billingNumber,
          strlen(pBillingList[3].billingNumber));
      memcpy(pBillingCur, pBillingList[3].billingCurrency,
          strlen(pBillingList[3].billingCurrency));
      memcpy(pBillingDate, pBillingList[3].billingDate,
          strlen(pBillingList[3].billingDate));
	  *pBillingMontant = pBillingList[3].billingAmount;
      pBillingList[3].billingAvailable = NO;
      break;
   case OC_BILLING_5 :
      memcpy(pBilling, pBillingList[4].billingNumber,
          strlen(pBillingList[4].billingNumber));
      memcpy(pBillingCur, pBillingList[4].billingCurrency,
          strlen(pBillingList[4].billingCurrency));
      memcpy(pBillingDate, pBillingList[4].billingDate,
          strlen(pBillingList[4].billingDate));
	  *pBillingMontant = pBillingList[4].billingAmount;
      pBillingList[4].billingAvailable = NO;
      break;
   case OC_BILLING_6 :
      memcpy(pBilling, pBillingList[5].billingNumber,
          strlen(pBillingList[5].billingNumber));
      memcpy(pBillingCur, pBillingList[5].billingCurrency,
          strlen(pBillingList[5].billingCurrency));
      memcpy(pBillingDate, pBillingList[5].billingDate,
          strlen(pBillingList[5].billingDate));
	  *pBillingMontant = pBillingList[5].billingAmount;
      pBillingList[5].billingAvailable = NO;
      break;
   case OC_BILLING_7 :
      memcpy(pBilling, pBillingList[6].billingNumber,
          strlen(pBillingList[6].billingNumber));
      memcpy(pBillingCur, pBillingList[6].billingCurrency,
          strlen(pBillingList[6].billingCurrency));
      memcpy(pBillingDate, pBillingList[6].billingDate,
          strlen(pBillingList[6].billingDate));
	  *pBillingMontant = pBillingList[6].billingAmount;
      pBillingList[6].billingAvailable = NO;
      break;
   default :
      memset(pBilling, '0', 24);
      memset(pBillingCur, '0',  3);
      memset(pBillingDate, '0',  3);
	  *pBillingMontant = 0.000;
	
   }

   sprintf(sLine, "End   GetSelectedBilling(%.24s,%.3s,%.2s)", pBilling, pBillingCur, pBillingDate);
   trace_event(sLine, PROCESSING);
   return (OK);
}
/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetEntredAccount               **
  **  desc   : charge le compte selectionne de l'operation      **
  **  entree   : BBuffer      -> buffer code operation         **
  **  sortie   : account   -> compte selectionne         **
  **  retour   : OK   -> operation OK               **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
GetEntredAccount(char *BBuffer, char *account, 
                 char *accountCur, char *accountType)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetEntredAccount(%.32s)", BBuffer);
   trace_event(sLine, PROCESSING);

   memset(account,    0, 25);
   memset(accountCur,    0,  4);

   /* il ne faut pas copier le dernier caractere car il reprsente le FDK **
    ** choisi, d'ou strlen() - 1                                          */
   memcpy(account,    BBuffer,    strlen(BBuffer) - 1);

   memset(accountCur,   '0', 3);

   memset(accountType,   '0', 2);

   sprintf(sLine, "End   GetEntredAccount(%.24s,%.3s,%.2s)", account, accountCur, accountType);
   trace_event(sLine, PROCESSING);
   return (OK);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetPaymentDestination               **
  **  desc   : charge le nom du destinataire de paiement      **
  **  entree   : OCBuffer      -> buffer code operation      **
  **  sortie   : name   -> nom du destinataire de paiement      **
  **  retour   : OK   -> operation OK               **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
GetPaymentDestination(char *OCBuffer, char *name)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetPaymentDestination(%.8s)", OCBuffer);
   trace_event(sLine, PROCESSING);

   switch(OCBuffer[OC_PAYMENT_DESTINATION_POS])
   {
   case OC_NAME_1 :
      memcpy(name, destinations[0], strlen(destinations[0]));
      break;
   case OC_NAME_2 :
      memcpy(name, destinations[1], strlen(destinations[1]));
      break;
   case OC_NAME_3 :
      memcpy(name, destinations[2], strlen(destinations[2]));
      break;
   case OC_NAME_4 :
      memcpy(name, destinations[3], strlen(destinations[3]));
      break;
   default :
      memset(name, ' ', 20);
      break;
   }

   sprintf(sLine, "End   GetPaymentDestination(%.20s)", name);
   trace_event(sLine, PROCESSING);
   return (OK);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
  **  nom   : GetTransCurrency               **
  **  desc   : charge le code monnaie de la transaction      **
  **  entree   : pOCBuff      -> operation demandee          **
  **        cardNbr   -> numero de la carte         **
  **  sortie   : pTransCurr   -> monnaie de la transaction      **
  **  retour   : OK   -> operation OK               **
  **  auteur   : Younes OUADI (25 Oct 1998)            **
  ***********************************************************************/
GetTransCurrency(char *pOCBuff, char *pTransCurr)
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetTransCurrency(%.8s)---%c", pOCBuff,pOCBuff[OC_CURRENCY_CODE_POS]);
   trace_event(sLine, PROCESSING);

   switch(pOCBuff[OC_CURRENCY_CODE_POS])
   {
   case OC_FIRST_CURRENCY :
      memcpy(pTransCurr, FIRST_CURRENCY, 3);
      break;
   case OC_SECOND_CURRENCY :
   case OC_SECOND_CURRENCY_1 :
      memcpy(pTransCurr, SECOND_CURRENCY, 3);
      break;
   case OC_THIRD_CURRENCY :
      memcpy(pTransCurr, THIRD_CURRENCY, 3);
      break;
   default :
      memcpy(pTransCurr, DEFAULT_CURRENCY, 3);
   }

   sprintf(sLine, "End   GetTransCurrency(%.3s)", pTransCurr);
   trace_event(sLine, PROCESSING);
   return (OK);
}

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetStartingDate(int nIndexCtx,char *OCBuffer, char *startingDate)
{
   char      month      [ 7];
   int      choice;
   int      retour;
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start GetStartingDate(%.9s)", OCBuffer);
   trace_event(sLine, PROCESSING);

   memset(startingDate,   0, 7);
   memset(month,      0, sizeof(month));

   switch(OCBuffer[OC_PERIOD_POS])
   {
   case OC_PERIOD_1   :
      choice = -1;
      break;
   case OC_PERIOD_2   :
      choice = -2;
      break;
   case OC_PERIOD_3   :
      choice = -3;
      break;
   case OC_PERIOD_4   :
      choice = -4;
      break;
   case OC_PERIOD_5   :
      choice = -5;
      break;
   case OC_PERIOD_6   :
      choice = -6;
      break;
   case OC_PERIOD_7   :
      choice = -7;
      break;
   case OC_PERIOD_8   :
      choice = -8;
      break;
   default         :
      choice = -1;
   }

   retour = GetMonth(nIndexCtx,choice, month, 'N');
   if(retour == OK)
   {
      memcpy(startingDate, month, 6);
   }
   else
   {
      sprintf(sLine, "End   GetStartingDate(%d)", retour);
      trace_event(sLine, PROCESSING);
      return (retour);
   }


   sprintf(sLine, "End   GetStartingDate(%.7s)", startingDate);
   trace_event(sLine, PROCESSING);
   return (OK);
}


/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetEndDate(int nIndexCtx,char *OCBuffer, char *endDate)
{
   char      month      [ 7];
   int      choice;
   int      retour;
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start GetEndDate(%.9s)", OCBuffer);
   trace_event(sLine, PROCESSING);

   memset(endDate,      0, 7);
   memset(month,      0, sizeof(month));

   switch(OCBuffer[OC_PERIOD_POS])
   {
   case OC_PERIOD_1   :
      choice = -0;
      break;
   case OC_PERIOD_2   :
      choice = -1;
      break;
   case OC_PERIOD_3   :
      choice = -2;
      break;
   case OC_PERIOD_4   :
      choice = -3;
      break;
   case OC_PERIOD_5   :
      choice = -4;
      break;
   case OC_PERIOD_6   :
      choice = -5;
      break;
   case OC_PERIOD_7   :
      choice = -6;
      break;
   case OC_PERIOD_8   :
      choice = -7;
      break;
   default         :
      choice = -0;
   }

   retour = GetMonth(nIndexCtx,choice, month, 'N');
   if(retour == OK)
   {
      memcpy(endDate, month, 6);
   }
   else
   {
      sprintf(sLine, "End   GetEndDate(%d)", retour);
      trace_event(sLine, PROCESSING);
      return (retour);
   }

   sprintf(sLine, "End   GetEndDate(%.7s)", endDate);
   trace_event(sLine, PROCESSING);
   return (OK);
}


/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetDeliveryMode(char OCBuffer[], char deliveryMode[])
{
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetDeliveryMode(%.9s)", OCBuffer);
   trace_event(sLine, PROCESSING);

   memset(deliveryMode,   0, 2);

   switch(OCBuffer[OC_DELIVERY_MODE_POS])
   {
   case 'A' :
      deliveryMode[0] = '1';
      break;
   case 'I' :
      deliveryMode[0] = '0';
      break;
   default  :
      deliveryMode[0] = '1';
      break;
   }

   sprintf(sLine, "End   GetDeliveryMode(%.2s)", deliveryMode);
   trace_event(sLine, PROCESSING);
   return (OK);
}


/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetChequeNumber(char *BBuffer, int *chequeNbr)
{
   char      strtmp   [33];
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start GetChequeNumber(%.8s)", BBuffer);
   trace_event(sLine, PROCESSING);

   memset(strtmp,   0, sizeof(strtmp));

   memcpy(strtmp,   BBuffer, strlen(BBuffer) - 1);

   *chequeNbr = atoi(strtmp);

   if(*chequeNbr == 0) *chequeNbr = 1;

   sprintf(sLine, "End   GetChequeNumber(%d)", *chequeNbr);
   trace_event(sLine, PROCESSING);
}


/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   LoadTransCode(int pOpReq, char pProcessingCode[], char pFunctionCode[])
{
   int   lCpt;
   char  sLine[ MAX_LINE_TRC ];


   sprintf(sLine, "Start LoadTransCode(%d)", pOpReq);
   trace_event(sLine, PROCESSING);

   memset(pProcessingCode,   0, sizeof(sDblConvert[0].fProcessingCode)+1);
   memset(pFunctionCode,   0, sizeof(sDblConvert[0].fFunctionCode)+1);

   lCpt = 0;
   while(1)
   {
      if ( sDblConvert[lCpt].fOpReq == -1 )
         break;

      if ( sDblConvert[lCpt].fOpReq == pOpReq )
      {
         memcpy(pProcessingCode,
             sDblConvert[lCpt].fProcessingCode,
             sizeof(sDblConvert[lCpt].fProcessingCode));
         memcpy(pFunctionCode,
             sDblConvert[lCpt].fFunctionCode,
             sizeof(sDblConvert[lCpt].fFunctionCode));
         break;
      }
      lCpt ++;
   }

   if (strlen(pProcessingCode) == 0 || strlen(pFunctionCode) == 0)
   {
      sprintf(sLine, "End   LoadTransCode(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }

   sprintf(sLine, "End   LoadTransCode(%s,%s)",
		pProcessingCode, pFunctionCode);
   trace_event(sLine, PROCESSING);
   return (OK);
}
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetPrintFlag(pOCBuff)
char*   pOCBuff;
{
    	trace_event("Start GetPrintFlag", PROCESSING);
    	trace_event("End   GetPrintFlag", PROCESSING);
   	return ((pOCBuff[OC_PRINT_FLAG_POS] != OC_PRINT ? NOK : OK));
}


/*********************************************************************/
/*********************************************************************/
int LoadDblField(char pDblBuff[], int pDblBuffLen, char pSep, 
                                  int pFldNbr, char pFld[])
{
   char tmp_buf[MAX_FIELD_LENGTH];
   char  sLine[ MAX_LINE_TRC ];

   int   lCpt1 = 0;
   int   lCpt2 = 0;
   int   lNbrSep = 0;

   sprintf(sLine, "Start LoadDblField(%d)", pFldNbr);
   trace_event(sLine, PROCESSING);

   memset(tmp_buf,0,sizeof(tmp_buf));

   while ((lNbrSep < pFldNbr) && (lCpt1 < pDblBuffLen-1))
   {
      if (pDblBuff[lCpt1] == pSep) lNbrSep += 1;
      lCpt1 += 1;
   }

   if ((lNbrSep < pFldNbr) || (lCpt1 == pDblBuffLen))
   {
      trace_event("End   LoadDblField(NOK)", PROCESSING);
      return(NOK);
   }

   lCpt2 = lCpt1;
   while ((pDblBuff[lCpt2] != pSep)  &&  (lCpt2 < pDblBuffLen))
      lCpt2+=1;

   memcpy(pFld, pDblBuff+lCpt1, lCpt2 - lCpt1);

   sprintf(sLine, "End   LoadDblField(%.30s)", pFld);
   trace_event(sLine, PROCESSING);
   return(lCpt2 - lCpt1);
}


/*********************************************************************/
/*********************************************************************/
int   GetTransName( int   pOpReq, char   pLang, char   pTransName[])
{
   int   lCpt;
   char  sLine[ MAX_LINE_TRC ];

   sprintf(sLine, "Start GetTransName(%d)", pOpReq);
   trace_event(sLine, PROCESSING);

   for(lCpt = 0; 1; lCpt++)
   {
      if (sTransName[lCpt].fOpReq == -1)
         break;

      if (sTransName[lCpt].fOpReq == pOpReq)
         switch (pLang)
         {
         case ARABIC      :
            memcpy(pTransName,
                sTransName[lCpt].fAraName,
                strlen(sTransName[lCpt].fAraName));
            break;
         case ENGLISH   :
            memcpy(pTransName,
                sTransName[lCpt].fEngName,
                strlen(sTransName[lCpt].fEngName));
            break;
         case FRENSH      :
            memcpy(pTransName,
                sTransName[lCpt].fFreName,
                strlen(sTransName[lCpt].fFreName));
            break;
         default      :
            memcpy(pTransName,
                sTransName[lCpt].fEngName,
                strlen(sTransName[lCpt].fEngName));
         }
   }

   sprintf(sLine, "End   GetTransName(%.25s)", pTransName);
   trace_event(sLine, PROCESSING);
   return(strlen(pTransName));
}

  /***********************************************************************
  **  nom   : GetStatus()                       **
  **  desc  : Construction de la chaine Status ND           **
  **  entree    : pStatusInfo   -> structure des info sur le status **
  **  sortie    : pStatus   -> Chaine status            **
  **  retour    : OK    -> generation OK                **
  **  auteur    : YO990815                      **
  ***********************************************************************/
/*int GetStatus(SStatusInfo* pStatusInfo, char* pStatus)
{
    strcat(pStatus, pStatusInfo->fStatusDesc);
    strcat(pStatus, "|");
    strcat(pStatus, pStatusInfo->fStatusInfo);
    strcat(pStatus, "|");
    strcat(pStatus, pStatusInfo->fErrSeverity);
    strcat(pStatus, "|");
    strcat(pStatus, pStatusInfo->fDiagStatus);
    strcat(pStatus, "|");
    strcat(pStatus, pStatusInfo->fSuppliesStatus);

return (OK);
}
*/
int CopyP7Buffer(pP7BuffIn, pP7BuffOut)
TSIsoInfo *pP7BuffIn;
TSIsoInfo *pP7BuffOut;
{
int     lCpt;
int 	lgField;
char 	tmpField[LG_MAX];


trace_event("Start CopyP7Buffer()", PROCESSING);

    pP7BuffOut->nMsgType = pP7BuffIn->nMsgType;

    for(lCpt = 1; lCpt < 128; lCpt ++)
 		if (GetIsoField( lCpt, pP7BuffIn, tmpField, &lgField) == SUCCES )
			PutIsoField(lCpt, pP7BuffOut, tmpField, lgField);

trace_event("End   CopyP7Buffer(OK)", PROCESSING);
return (OK);
}

