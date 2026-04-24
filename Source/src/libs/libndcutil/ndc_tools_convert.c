/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      ndc_tools_convert.c                                                    */
/*                                                                             */
/* Description                                                                 */
/*      Convert Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
SWI03022012   		03-02-2012   		Said El Waggagui   		MTP-V226_DEV-02022012-MKB-NDC_CURRENCY	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include <define.h>
#include <ndcmsgcst.h>
#include <account.h>
#include <utility.h>
#include <billing.h>
#include <atmopcode.h>
#include <ndcopcode.h>
#include <currency.h>
#include <langue.h>
#include <ndcconvert.h>
#include <ndcstatus.h>
#include <iso_hps.h>
#include <donation.h>
#include <p7_sid.h>
#include <ndc_param.h>


static SNdcConvert		sNdcConvert[]={
	{OR_ONUS_WITHDRAWAL,	"010000",	"200"},
	{OR_NAT_WITHDRAWAL,	"010000",	"200"},
	{OR_INT_WITHDRAWAL,		"010000",	"200"},
	{OR_ONUS_ACCOUNT_LIST,		"180000",	"691"},
	{OR_RECONC_DATA_REQ,		"180000",	"692"},
	{OR_ONUS_AUTHENTICATION,	"300000",	"693"},
	{OR_ONUS_CASH_DPT,		"180000",	"694"},
	{OR_ONUS_CHEQ_DPT,		"180000",	"694"},
	{OR_ACCOUNT_ONLINE_CASH,	"180000",	"694"}, /**MKB060508**/
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
	{OR_ONUS_PIN_CHANGE,		"910000",	"304"},
	/*{OR_ONUS_LOAN_INQUIRY,		"910000",	"305"},*/
	{OR_ONUS_HOST_ACCOUNTS,		"400000",	"200"},
	/*{OR_ONUS_PRE_TFR_TO_CR,		"420000",	"284"},*//* PRE FT To a Credit Card (Credit card payement) RJE 16032014 */
	/*{OR_ONUS_PRE_TFR_TO_CR_BY_CC,   "420000",       "284"},*/ /* Credit payment service by Account for Credit card in ATM  RJE15062014 */
	/*{OR_ONUS_CARDLESS_CASH_D,		"210000",	"691"},*/ /*RJE25042014 */
	/*{OR_ACCOUNT_LIST_ONL_CASH,		"210000",	"691"},*/ /*RJE25042014 */
	/*{OR_ACC_ONLINE_CASH_CR,		    "210000",	"691"},*/ /*RJE25042014 */
	/*{OR_ACCOUNT_ONLINE_CASH_WN,		"210000",	"691"},*/ /*RJE25042014 */
	/*{OR_ACCOUNT_LIST_ONL_CASH_WN,	"210000",	"691"},*/ /*RJE25042014 */
	/*{OR_ONUS_CARDLESS_CASH_C,		"210090",	"691"},*/ /*RJE23052014 */
	/*{OR_BILL_PAYMENT_RECHAG,               "490000",       "200"},*/ /*RJE21052014 */
	{OR_ONUS_PRE_DONATION,  "800000",       "200"},
	{OR_ONUS_DONATION,      "800000",       "200"},
	{OR_NAT_DONATION,       "800000",       "200"},
	{OR_INT_DONATION,       "800000",       "200"},
    {OR_ONUS_CASHSEND,              "580000",       "200"} ,/*CASHSEND MANAGE*/
    {OR_ONUS_CASHOUT,               "010000",       "201"},	
	{OR_ONUS_PIN_UNBLOCK,           "980000",       "200"},	
	{OR_NAT_PIN_UNBLOCK,           "980000",       "200"},	
	{OR_INT_PIN_UNBLOCK,           "980000",       "200"},	

	/*{OR_ONUS_TOPUP,       "950000",       P7_F024_TOPUP_FROM_CH_ACC},*/ /* CFE_TOREV */

	{-1,				      "", 	   ""}
};

/*N.G 180703 Ajout libel francais */
static STransName	sTransName[]={
	{OR_ONUS_WITHDRAWAL,		"^>OZ &;C",		"WITHDRAWAL","Retrait"},
	{OR_NAT_WITHDRAWAL,		"^>OZ &;C",		"WITHDRAWAL","Retrait"},
	{OR_INT_WITHDRAWAL,		"^>OZ &;C",		"WITHDRAWAL","Retrait"},
	{OR_ONUS_ACCOUNT_LIST,			"(#&#C;U\" \\;%V",		"ACCOUNT LIST REQ",		"LIST DESC COMPTES"},
	{OR_RECONC_DATA_REQ,			"",						"RECONCILIATION",		"RECONCILIATION"},
	{OR_ONUS_AUTHENTICATION,		"N_O;(",				"AUTHENTICATION",		"AUTHENTIFICATION"},
	{OR_ONUS_CASH_DPT,				"^>OZ I\">_\"",			"ENVELOP CASH DEPOSIT",	"DEPOT ESPECE ENVELOP"},
	{OR_ONUS_CHEQ_DPT,				"(#Q_DU\" I\">_\"",		"ENVELOP CHEQUE DEPOSIT","DEPOT CHEQUE ENVELOP"},
	{OR_ONUS_DOC_DPT,				"(\">Z(CXU\" I\">_\"",	"DOCUMENT DEPOSIT",		"DEPOT DOCUMENT"},
	{OR_ONUS_BALANCE,		">_E@U\" YJ WVJ(C\"","BALANCE","SOLDE"},
	{OR_NAT_BALANCE,		">_E@U\" YJ WVJ(C\"","BALANCE","SOLDE"},
	{OR_INT_BALANCE,		">_E@U\" YJ WVJ(C\"","BALANCE","SOLDE"},
	{OR_ONUS_SHORT_STAT,		"@E(=X &#C; MDQ","SHORT STATEMENT","RELEVE"},
	{OR_ONUS_CHEQ_BOOK,		"(#Q_DU\" @(M> &UG","CHEQUE BOOK REQ",""},
	{OR_ONUS_STAT_REQ,		"(#_\"XJU\" Y#_& &UG","STATEMENT REQ","RELEVE"},
	{OR_ONUS_INT_TFR,				"T_[;(",				"TRANSFER REQ",			"TRANSFERT"},
	{OR_ONUS_EXT_TFR,				"T_[;(",				"TRANSFER REQ",			"TRANSFERT"},
	{OR_ONUS_PIN_CHANGE,			"^@CU\" WO@U\" @__L(",	"PIN CHANGE",			"CHANGEMENT DE PIN"},
	{OR_ONUS_HOST_ACCOUNTS ,		"",	"TIERS VIEREMENT","VIREMENT TIERS"},
	/*
	{OR_ONUS_PRE_TFR_TO_CR,		"Y#X(%V\" IM>","CREDIT PAYMENT",""}, / * PRE FT To a Credit Card (Credit card payement) RJE 16032014 * /
	{OR_ONUS_EXT_TFR_TO_CR,		"Y#X(%V\" IM>","CREDIT PAYMENT",""}, / * PRE FT To a Credit Card (Credit card payement) RJE 16032014 * /
	{OR_ONUS_LOAN_INQUIRY,		"!F@OU\" Y#_&","LOAN INQUIRY",""},
	{OR_BILL_PAYMENT,		"@_(\"[MU\" IM>",	"BILL PAYMENT","Paiement Fact"}, / *RJE 12032014* /
	{OR_BILL_PAYMENT_RECHAG,               "@_(\"[MU\" IM>",     "BILL PAYMENT","Paiement Fact"}, / *RJE 12032014* /
	{OR_BILL_PAYMENT_OTHER,               "@_(\"[MU\" IM>",     "BILL PAYMENT","Paiement Fact"}, / *RJE 12032014* /
	*/
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
	char    lLanguageRequested;
	char	sLine[ MAX_LINE_TRC ];

	if (debug_option == 1)
		sprintf(sLine, "Start GetLanguageRequested(%.8s,%.37s)", pOCBuffer, pTrackii);
	else
		sprintf(sLine, "Start GetLanguageRequested((%.8s)",pOCBuffer);
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
		if( strlen(pTrackii) >= 2 )
		{
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
		else
		{
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
int GetOperationRequested(char *OCBuffer)
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
	/*case 'C' :
		operationRequested = OR_LOAD_BILLING;
		break;*/
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
int GetSelectedAccount(	char *pBBuffer, 
	account_s pAccountList[], 
	int pAccountListSize, 
	char *pAccount, 
	char *pAccountCur, 
	char *pAccountType)
{
	char	sLine[ MAX_LINE_TRC ];
	int		nChoiceIndex;

	sprintf(sLine, "Start GetSelectedAccount(%.32s)", pBBuffer);
	trace_event(sLine, PROCESSING);

	nChoiceIndex = -1;
	switch(pBBuffer[0])
	{
	case OC_ACCOUNT_1 : nChoiceIndex = 0; break;
	case OC_ACCOUNT_2 : nChoiceIndex = 1; break;
	case OC_ACCOUNT_3 : nChoiceIndex = 2; break;
	case OC_ACCOUNT_4 : nChoiceIndex = 3; break;
	case OC_ACCOUNT_5 : nChoiceIndex = 4; break;
	case OC_ACCOUNT_6 : nChoiceIndex = 5; break;
	case OC_ACCOUNT_7 : nChoiceIndex = 6; break;
	default : nChoiceIndex = -1; break;
	}

	pAccount[NDC_ACC_NBR_LG_MAX]='\0';
	pAccountCur[NDC_ACC_CCY_LG_MAX]='\0';
	pAccountType[NDC_ACC_TYPE_LG_MAX]='\0';
	if( nChoiceIndex >= 0 )
	{
		strncpy(pAccount, pAccountList[nChoiceIndex].accountNumber, NDC_ACC_NBR_LG_MAX);
		strncpy(pAccountCur, pAccountList[nChoiceIndex].accountCurrency, NDC_ACC_CCY_LG_MAX);
		strncpy(pAccountType, pAccountList[nChoiceIndex].accountType, NDC_ACC_TYPE_LG_MAX);
		pAccountList[nChoiceIndex].accountAvailable = NO;
	}
	else
	{
		memset(pAccount,'0',NDC_ACC_NBR_LG_MAX);
		memset(pAccountCur,'0',NDC_ACC_CCY_LG_MAX);
	}

	sprintf(sLine, "End   GetSelectedAccount(%.24s,%.3s,%.2s)", pAccount, pAccountCur, pAccountType);
	trace_event(sLine, PROCESSING);
	return (OK);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* RJE 13022014 */


/***********************************************************************
**  nom   : GetSelectedService 									**
**  desc   : charge le compte selectionne de l'operation      **
**  entree   : pBBuffer   -> BBuffer code operation      **
**  sortie   : pAccount   -> compte selectionne         **
**  retour   : OK   -> operation OK               **
***********************************************************************/
int GetSelectedService (char 		*pBBuffer, 
	utility_s 	pUtilityList[], 
	int 		pUtilityListSize, 
	char 		*pUtility)
{
	char  sLine[ MAX_LINE_TRC ];


	sprintf(sLine, "Start GetSelectedService (%.32s)", pBBuffer);
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

	sprintf(sLine, "End   GetSelectedService(%.24s)", pUtility);
	trace_event(sLine, PROCESSING);
	return (OK);
}

/*RJE 02132014*/



/***********************************************************************
**  nom   : GetSelectedUtility 									**
**  desc   : charge le compte selectionne de l'operation      **
**  entree   : pBBuffer   -> BBuffer code operation      **
**  sortie   : pAccount   -> compte selectionne         **
**  retour   : OK   -> operation OK               **
***********************************************************************/
int GetSelectedUtility (
	char 		*pBBuffer, 
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
int GetEntredAccount(char *BBuffer, char *account, char *accountCur, char *accountType)
{
	char  sLine[ MAX_LINE_TRC ];
	

	sprintf(sLine, "Start GetEntredAccount(%.32s)", BBuffer);
	trace_event(sLine, PROCESSING);

	
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
/*EBE: not used
int GetPaymentDestination(char *OCBuffer, char *name)
{
char  sLine[ MAX_LINE_TRC ];
memset(sLine,                0,      sizeof(sLine));

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
*/
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
int GetTransCurrency(int nIndexCtx,char *pTermNbr,char *pAcqBank, char *pOCBuff, char *pTransCurr)
{
	char  sLine[ MAX_LINE_TRC ];
	
	int  plRetour;
	
	char            plProfileCode   [    3+1];  /*Start SWI03022012 NDC CURRENCY*/
	char            p_default_currency [4]; 
	char            p_first_currency [4];
	char            p_second_currency [4];
	char            p_third_currency [4];


	sprintf(sLine, "Start GetTransCurrency(%.8s)---%c", pOCBuff,pOCBuff[OC_CURRENCY_CODE_POS]);
	trace_event(sLine, PROCESSING);

	
	memset(plProfileCode,        0,      sizeof(plProfileCode));
	memset(p_default_currency,   0,      sizeof(p_default_currency));
	memset(p_first_currency,     0,      sizeof(p_first_currency));
	memset(p_second_currency,    0,      sizeof(p_second_currency));
	memset(p_third_currency,     0,      sizeof(p_third_currency));



	plRetour = GetTerminalProfile(nIndexCtx, pTermNbr, pAcqBank, plProfileCode);
	if(plRetour != OK)
	{
		trace_event("End   GetTransCurrency(NOK)", PROCESSING);
		return (NOK);
	}

	plRetour = GetProfileCurencies(		nIndexCtx,
										plProfileCode,
										pAcqBank,
										p_default_currency,
										p_first_currency,
										p_second_currency,
										p_third_currency ); 
	if(plRetour != OK)
	{
		trace_event("End   GetTransCurrency(NOK)", PROCESSING);
		return (NOK);
	}

	/*End SWI03022012 NDC CURRENCY*/
	switch(pOCBuff[OC_CURRENCY_CODE_POS])
	{
	case OC_FIRST_CURRENCY :
		if (strlen(p_first_currency) != 0)
			memcpy(pTransCurr, p_first_currency, 3);
		else
		{
			trace_event("End GetTransCurrency(NOK): first currency is null", PROCESSING);
			return (NOK);
		}
		break;
	case OC_SECOND_CURRENCY :
		if (strlen(p_second_currency) != 0)
			memcpy(pTransCurr, p_second_currency, 3);
		else
		{
			trace_event("End GetTransCurrency(NOK): second currency is null", PROCESSING);
			return (NOK);
		}
		break;
	case OC_THIRD_CURRENCY :
		if (strlen(p_third_currency) != 0)
			memcpy(pTransCurr, p_third_currency, 3);
		else
		{
			trace_event("End GetTransCurrency(NOK): third currency is null", PROCESSING);
			return (NOK);
		}
		break;
	default :
		if (strlen(p_default_currency) != 0)
			memcpy(pTransCurr, p_default_currency, 3);
		else
		{
			trace_event("End GetTransCurrency(NOK): default currency is null", PROCESSING);
			return (NOK);
		}
	}

	sprintf(sLine, "End   GetTransCurrency(%.3s)", pTransCurr);
	trace_event(sLine, PROCESSING);
	return (OK);
}

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* EBE: Not used
int   GetStartingDate(int nIndexCtx,char *OCBuffer, char *startingDate)
{
char      month      [ 7];
int      choice;
int      retour;
char  sLine[ MAX_LINE_TRC ];


memset(startingDate,   0, 7);
memset(month,      0, sizeof(month));
memset(sLine,      0, sizeof(sLine));

sprintf(sLine, "Start GetStartingDate(%.9s)", OCBuffer);
trace_event(sLine, PROCESSING);



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
*/

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/*EBE Not used
int   GetEndDate(int nIndexCtx,char *OCBuffer, char *endDate)
{
char      month      [ 7];
int      choice;
int      retour;
char  sLine[ MAX_LINE_TRC ];


memset(endDate,      0, 7);
memset(month,      0, sizeof(month));
memset(sLine,      0, sizeof(sLine));


sprintf(sLine, "Start GetEndDate(%.9s)", OCBuffer);
trace_event(sLine, PROCESSING);



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
*/

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/*
int   GetDeliveryMode(char OCBuffer[], char deliveryMode[])
{
char  sLine[ MAX_LINE_TRC ];

memset(deliveryMode,   0, 2);
memset(sLine,   0, sizeof(sLine));

sprintf(sLine, "Start GetDeliveryMode(%.9s)", OCBuffer);
trace_event(sLine, PROCESSING);



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
*/

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/*
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

return(0);
}
*/

/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   LoadTransCode(int pOpReq, char pProcessingCode[], char pFunctionCode[])
{
	int   lCpt;
	char  sLine[ MAX_LINE_TRC ];


	sprintf(sLine, "Start LoadTransCode(OpCode: [%d])", pOpReq);
	trace_event(sLine, PROCESSING);
	

	lCpt = 0;
	while(1)
	{
		if ( sNdcConvert[lCpt].fOpReq == -1 )
			break;

		if ( sNdcConvert[lCpt].fOpReq == pOpReq )
		{
			memcpy(pProcessingCode, sNdcConvert[lCpt].fProcessingCode,P7_F003_LEN);pProcessingCode[P7_F003_LEN] = '\0';
			memcpy(pFunctionCode, sNdcConvert[lCpt].fFunctionCode,P7_F024_LEN);pFunctionCode[P7_F024_LEN] = '\0';
			sprintf(sLine, "End   LoadTransCode(ProcCode[%s],FuncCode[%s])", pProcessingCode, pFunctionCode);
			trace_event(sLine, PROCESSING);
			return (OK);
		}
		lCpt ++;
	}


	sprintf(sLine, "End   LoadTransCode(NOK): Unkown operation code (%d)",pOpReq);
	trace_event(sLine, PROCESSING);
	return (NOK);


}
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
int   GetPrintFlag(char   pOCBuff[])
{
	return ((pOCBuff[OC_PRINT_FLAG_POS] == OC_PRINT ? OK : NOK));
}


/*********************************************************************/
/*********************************************************************/
int LoadNdcField(char pNdcBuff[], int pNdcBuffLen, char pSep,  int pFldNbr, char pFld[])
{
	char tmp_buf[MAX_FIELD_LENGTH];
	char  sLine[ MAX_LINE_TRC ];

	int   lCpt1 = 0;
	int   lCpt2 = 0;
	int   lNbrSep = 0;

	memset(tmp_buf,0,sizeof(tmp_buf));
	

	sprintf(sLine, "Start LoadNdcField(%d)", pFldNbr);
	trace_event(sLine, PROCESSING);


	while ((lNbrSep < pFldNbr) && (lCpt1 < pNdcBuffLen-1))
	{
		if (pNdcBuff[lCpt1] == pSep) lNbrSep += 1;
		lCpt1 += 1;
	}

	if ((lNbrSep < pFldNbr) || (lCpt1 == pNdcBuffLen))
	{
		trace_event("End   LoadNdcField(NOK)", PROCESSING);
		return(NOK);
	}

	lCpt2 = lCpt1;
	while ((pNdcBuff[lCpt2] != pSep)  &&  (lCpt2 < pNdcBuffLen))
		lCpt2+=1;

	memcpy(pFld, pNdcBuff+lCpt1, lCpt2 - lCpt1);

	sprintf(sLine, "End   LoadNdcField(%.30s)", pFld);
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

	for(lCpt = 0; ; lCpt++)
	{
		if (sTransName[lCpt].fOpReq == -1)
			break;

		if (sTransName[lCpt].fOpReq == pOpReq)
		{
			switch (pLang)
			{
				case ARABIC      :
					memcpy(pTransName,sTransName[lCpt].fAraName,strlen(sTransName[lCpt].fAraName));
					break;
				case ENGLISH   :
					memcpy(pTransName,sTransName[lCpt].fEngName,strlen(sTransName[lCpt].fEngName));
					break;
				case FRENSH      :
					memcpy(pTransName,sTransName[lCpt].fFreName,strlen(sTransName[lCpt].fFreName));
					break;
				default      :
					memcpy(pTransName,sTransName[lCpt].fEngName,strlen(sTransName[lCpt].fEngName));
					break;
			}
			break;
		}
	}

	sprintf(sLine, "End   GetTransName(%.20s)", pTransName);
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
int GetStatus( SStatusInfo *pStatusInfo, char        *pStatus)
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

/*EBE
int CopyP7Buffer(TSIsoInfo *pP7BuffIn,TSIsoInfo *pP7BuffOut)
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
*/

int CopyP7Buffer(TSIsoInfo *pP7BuffIn,TSIsoInfo *pP7BuffOut)
{

	trace_event("Start CopyP7Buffer()", PROCESSING);
	memcpy((char*)pP7BuffOut,(char*)pP7BuffIn,sizeof(TSIsoInfo));
	trace_event("End   CopyP7Buffer(OK)", PROCESSING);
	return (OK);
}

/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* To a Credit Card (Credit card payement) RJE 16032014 */

/***********************************************************************
**  nom   : GetSelectedAccountCard               **
**  desc   : charge le compte selectionne de l'operation      **
**  entree   : pBBuffer   -> BBuffer code operation      **
**  sortie   : pAccount   -> compte selectionne         **
**           : pToCard    -> Carte  selectionne         **
**  retour   : OK   -> operation OK               **
***********************************************************************/
int GetSelectedAccountCard(char *pBBuffer, account_s pAccountList[], 
	int pAccountListSize, 
	char *pAccount, char *pAccountCur, char *pAccountType, char *pToCard)
{
	char  sLine[ MAX_LINE_TRC ];
	memset(sLine,       0, sizeof(sLine));

	sprintf(sLine, "Start GetSelectedAccountCard(%s)", pBBuffer);
	trace_event(sLine, PROCESSING);




	switch(pBBuffer[0])
	{
	case OC_ACCOUNT_1 :
		memcpy(pAccount,     pAccountList[0].accountNumber,    strlen(pAccountList[0].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[0].accountCurrency,  strlen(pAccountList[0].accountCurrency));
		memcpy(pAccountType, pAccountList[0].accountType,      strlen(pAccountList[0].accountType    ));

		if (strlen(pAccountList[0].accountWording) >24)
			memcpy(pToCard,      pAccountList[0].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[0].accountWording,   strlen(pAccountList[0].accountWording ));

		pAccountList[0].accountAvailable = NO;
		break;
	case OC_ACCOUNT_2 :
		memcpy(pAccount,     pAccountList[1].accountNumber,    strlen(pAccountList[1].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[1].accountCurrency,  strlen(pAccountList[1].accountCurrency));
		memcpy(pAccountType, pAccountList[1].accountType,      strlen(pAccountList[1].accountType    ));

		if (strlen(pAccountList[1].accountWording) >24)
			memcpy(pToCard,      pAccountList[1].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[1].accountWording,   strlen(pAccountList[1].accountWording ));

		pAccountList[1].accountAvailable = NO;                                    
		break;
	case OC_ACCOUNT_3 :
		memcpy(pAccount,     pAccountList[2].accountNumber,    strlen(pAccountList[2].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[2].accountCurrency,  strlen(pAccountList[2].accountCurrency));
		memcpy(pAccountType, pAccountList[2].accountType,      strlen(pAccountList[2].accountType    ));

		if (strlen(pAccountList[2].accountWording) >24)
			memcpy(pToCard,      pAccountList[2].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[2].accountWording,   strlen(pAccountList[2].accountWording ));

		pAccountList[2].accountAvailable = NO;
		break;
	case OC_ACCOUNT_4 :
		memcpy(pAccount,     pAccountList[3].accountNumber,    strlen(pAccountList[3].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[3].accountCurrency,  strlen(pAccountList[3].accountCurrency));
		memcpy(pAccountType, pAccountList[3].accountType,      strlen(pAccountList[3].accountType    ));
		if (strlen(pAccountList[3].accountWording) >24)
			memcpy(pToCard,      pAccountList[3].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[3].accountWording,   strlen(pAccountList[3].accountWording ));
		pAccountList[3].accountAvailable = NO;
		break;
	case OC_ACCOUNT_5 :
		memcpy(pAccount,     pAccountList[4].accountNumber,    strlen(pAccountList[4].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[4].accountCurrency,  strlen(pAccountList[4].accountCurrency));
		memcpy(pAccountType, pAccountList[4].accountType,      strlen(pAccountList[4].accountType    ));

		if (strlen(pAccountList[4].accountWording) >24)
			memcpy(pToCard,      pAccountList[4].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[4].accountWording,   strlen(pAccountList[4].accountWording ));

		pAccountList[4].accountAvailable = NO;
		break;
	case OC_ACCOUNT_6 :
		memcpy(pAccount,     pAccountList[5].accountNumber,    strlen(pAccountList[5].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[5].accountCurrency,  strlen(pAccountList[5].accountCurrency));
		memcpy(pAccountType, pAccountList[5].accountType,      strlen(pAccountList[5].accountType    ));

		if (strlen(pAccountList[5].accountWording) >24)
			memcpy(pToCard,      pAccountList[5].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[5].accountWording,   strlen(pAccountList[5].accountWording ));

		pAccountList[5].accountAvailable = NO;
		break;
	case OC_ACCOUNT_7 :
		memcpy(pAccount,     pAccountList[6].accountNumber,    strlen(pAccountList[6].accountNumber  ));
		memcpy(pAccountCur,  pAccountList[6].accountCurrency,  strlen(pAccountList[6].accountCurrency));
		memcpy(pAccountType, pAccountList[6].accountType,      strlen(pAccountList[6].accountType    ));

		if (strlen(pAccountList[6].accountWording) >24)
			memcpy(pToCard,      pAccountList[6].accountWording,   24);
		else
			memcpy(pToCard,      pAccountList[6].accountWording,   strlen(pAccountList[6].accountWording ));

		pAccountList[6].accountAvailable = NO;
		break;
	default :
		memset(pAccount, '0', 24);
		memset(pAccountCur, '0',  3);
		trace_event("End   GetSelectedAccountCard (NOK)", PROCESSING);
		return (NOK);
	}

	if (debug_option == 1)
		sprintf(sLine, "End   GetSelectedAccountCard(%.24s,%.3s,%.2s, %.24s)", pAccount, pAccountCur, pAccountType, pToCard);
	else
		sprintf(sLine, "End   GetSelectedAccountCard(%.24s,%.3s,%.2s)", pAccount, pAccountCur, pAccountType);
	trace_event(sLine, PROCESSING);
	return (OK);
}
/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/* END FT To a Credit Card (Credit card payement) RJE 16032014 */



/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
**  nom   : GetSelectedDonation               **
**  desc   : charge le compte selectionne de l'operation      **
**  entree   : pBBuffer   -> BBuffer code operation      **
**  sortie   : pdonation   -> compte selectionne         **
**  retour   : OK   -> operation OK               **
**  auteur   :                                            **
***********************************************************************/
int GetSelectedDonation(char *pBBuffer, donation_s pDonationList[], 
	int pDonationListSize, 
	donation_s pdonation[])
{
	char  sLine[ MAX_LINE_TRC ];

	sprintf(sLine, "Start GetSelectedDonation(%.32s)", pBBuffer);
	trace_event(sLine, PROCESSING);

	memset(pdonation[0].donationNumber,        0, 25);
	memset(pdonation[0].donationAraWording,    0,  31);
	memset(pdonation[0].donationEngWording,    0,  31);


	switch(pBBuffer[0])
	{
	case OC_DONATION_1 :
		memcpy(pdonation[0].donationNumber,     pDonationList[0].donationNumber, strlen(pDonationList[0].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[0].donationAraWording, strlen(pDonationList[0].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[0].donationEngWording, strlen(pDonationList[0].donationEngWording));
		pDonationList[0].donationAvailable = NO;
		break;
	case OC_DONATION_2 :
		memcpy(pdonation[0].donationNumber,     pDonationList[1].donationNumber, strlen(pDonationList[1].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[1].donationAraWording, strlen(pDonationList[1].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[1].donationEngWording, strlen(pDonationList[1].donationEngWording));
		pDonationList[1].donationAvailable = NO;
		break;
	case OC_DONATION_3 :
		memcpy(pdonation[0].donationNumber,     pDonationList[2].donationNumber, strlen(pDonationList[2].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[2].donationAraWording, strlen(pDonationList[2].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[2].donationEngWording, strlen(pDonationList[2].donationEngWording));
		pDonationList[2].donationAvailable = NO;
		break;
	case OC_DONATION_4 :
		memcpy(pdonation[0].donationNumber,     pDonationList[3].donationNumber, strlen(pDonationList[3].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[3].donationAraWording, strlen(pDonationList[3].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[3].donationEngWording, strlen(pDonationList[3].donationEngWording));
		pDonationList[3].donationAvailable = NO;
		break;
	case OC_DONATION_5 :
		memcpy(pdonation[0].donationNumber,     pDonationList[4].donationNumber, strlen(pDonationList[4].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[4].donationAraWording, strlen(pDonationList[4].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[4].donationEngWording, strlen(pDonationList[4].donationEngWording));
		pDonationList[4].donationAvailable = NO;
		break;
	case OC_DONATION_6 :
		memcpy(pdonation[0].donationNumber,     pDonationList[5].donationNumber, strlen(pDonationList[5].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[5].donationAraWording, strlen(pDonationList[5].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[5].donationEngWording, strlen(pDonationList[5].donationEngWording));
		pDonationList[5].donationAvailable = NO;
		break;
	case OC_DONATION_7 :
		memcpy(pdonation[0].donationNumber,     pDonationList[6].donationNumber, strlen(pDonationList[6].donationNumber));
		memcpy(pdonation[0].donationAraWording, pDonationList[6].donationAraWording, strlen(pDonationList[6].donationAraWording));
		memcpy(pdonation[0].donationEngWording, pDonationList[6].donationEngWording, strlen(pDonationList[6].donationEngWording));
		pDonationList[6].donationAvailable = NO;
		break;
	default :
		memset(pdonation[0].donationNumber,        '0', 24);
		memset(pdonation[0].donationAraWording,    '0',  3);
		memset(pdonation[0].donationEngWording,    '0',  3);
	}

	sprintf(sLine, "End   GetSelectedDonation(%.24s,%.20s,)", pdonation[0].donationNumber, pdonation[0].donationEngWording);
	trace_event(sLine, PROCESSING);
	return (OK);
}
/*-=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- -=- */
/***********************************************************************
**  nom   : GetSelectedSubDonation               **
**  desc   : charge le compte selectionne de l'operation      **
**  entree   : pBBuffer   -> BBuffer code operation      **
**  sortie   : pSubDonationNumber   -> compte selectionne         **
**  retour   : OK   -> operation OK               **
**  auteur   :                                            **
***********************************************************************/
int GetSelectedSubDonation(char *pBBuffer, sub_donation_s pSubDonationList[], 
	int pSubDonationListSize, 
	sub_donation_s pSubDonation[])
{
	char  sLine[ MAX_LINE_TRC ];

	sprintf(sLine, "Start GetSelectedSubDonation(%.32s)", pBBuffer);
	trace_event(sLine, PROCESSING);

	memset(pSubDonation[0].donationNumber,   	   0, 25);
	memset(pSubDonation[0].subDonationNumber,       0, 25);
	memset(pSubDonation[0].subDonationAraWording,   0, 31);
	memset(pSubDonation[0].subDonationEngWording,   0, 31);
	memset(pSubDonation[0].subDonationAccount,      0, 25);


	switch(pBBuffer[0])
	{
	case OC_DONATION_1 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[0].donationNumber,         strlen(pSubDonationList[0].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[0].subDonationNumber,      strlen(pSubDonationList[0].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[0].subDonationAraWording,  strlen(pSubDonationList[0].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[0].subDonationEngWording,  strlen(pSubDonationList[0].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[0].subDonationAccount,     strlen(pSubDonationList[0].subDonationAccount));
		pSubDonationList[0].subDonationAvailable = NO;
		break;

	case OC_DONATION_2 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[1].donationNumber,         strlen(pSubDonationList[1].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[1].subDonationNumber,      strlen(pSubDonationList[1].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[1].subDonationAraWording,  strlen(pSubDonationList[1].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[1].subDonationEngWording,  strlen(pSubDonationList[1].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[1].subDonationAccount,     strlen(pSubDonationList[1].subDonationAccount));
		pSubDonationList[1].subDonationAvailable = NO;
		break;

	case OC_DONATION_3 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[2].donationNumber,         strlen(pSubDonationList[2].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[2].subDonationNumber,      strlen(pSubDonationList[2].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[2].subDonationAraWording,  strlen(pSubDonationList[2].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[2].subDonationEngWording,  strlen(pSubDonationList[2].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[2].subDonationAccount,     strlen(pSubDonationList[2].subDonationAccount));
		pSubDonationList[2].subDonationAvailable = NO;                                                             
		break;

	case OC_DONATION_4 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[3].donationNumber,         strlen(pSubDonationList[3].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[3].subDonationNumber,      strlen(pSubDonationList[3].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[3].subDonationAraWording,  strlen(pSubDonationList[3].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[3].subDonationEngWording,  strlen(pSubDonationList[3].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[3].subDonationAccount,     strlen(pSubDonationList[3].subDonationAccount));
		pSubDonationList[3].subDonationAvailable = NO;                                                             
		break;

	case OC_DONATION_5 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[4].donationNumber,         strlen(pSubDonationList[4].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[4].subDonationNumber,      strlen(pSubDonationList[4].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[4].subDonationAraWording,  strlen(pSubDonationList[4].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[4].subDonationEngWording,  strlen(pSubDonationList[4].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[4].subDonationAccount,     strlen(pSubDonationList[4].subDonationAccount));
		pSubDonationList[4].subDonationAvailable = NO;                                                             
		break;

	case OC_DONATION_6 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[5].donationNumber,         strlen(pSubDonationList[5].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[5].subDonationNumber,      strlen(pSubDonationList[5].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[5].subDonationAraWording,  strlen(pSubDonationList[5].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[5].subDonationEngWording,  strlen(pSubDonationList[5].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[5].subDonationAccount,     strlen(pSubDonationList[5].subDonationAccount));
		pSubDonationList[5].subDonationAvailable = NO;                                                             
		break;

	case OC_DONATION_7 :
		memcpy(pSubDonation[0].donationNumber,        pSubDonationList[6].donationNumber,         strlen(pSubDonationList[6].donationNumber));
		memcpy(pSubDonation[0].subDonationNumber,     pSubDonationList[6].subDonationNumber,      strlen(pSubDonationList[6].subDonationNumber));
		memcpy(pSubDonation[0].subDonationAraWording, pSubDonationList[6].subDonationAraWording,  strlen(pSubDonationList[6].subDonationAraWording));
		memcpy(pSubDonation[0].subDonationEngWording, pSubDonationList[6].subDonationEngWording,  strlen(pSubDonationList[6].subDonationEngWording));
		memcpy(pSubDonation[0].subDonationAccount,    pSubDonationList[6].subDonationAccount,     strlen(pSubDonationList[6].subDonationAccount));
		pSubDonationList[6].subDonationAvailable = NO;                                                             
		break;

	default :
		memset(pSubDonation[0].donationNumber, 	     '0', 24);
		memset(pSubDonation[0].subDonationNumber,      '0', 24);
		memset(pSubDonation[0].subDonationAraWording,  '0',  3);
		memset(pSubDonation[0].subDonationEngWording,  '0',  3);
		memset(pSubDonation[0].subDonationAccount, 	 '0', 24);
	}

	sprintf(sLine, "End   GetSelectedSubDonation(%.24s,%.20s,)", pSubDonation[0].subDonationNumber, pSubDonation[0].subDonationEngWording);
	trace_event(sLine, PROCESSING);
	return (OK);
}
