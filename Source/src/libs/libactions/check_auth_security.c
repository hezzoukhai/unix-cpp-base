/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*                                                                             */
/*                                                                             */
/* Description                                                                 */
/*                                                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    							Initial Version	
0002YO     		03-09-2002   		Younes OUADI    								Gestion de piste III non valide
0003AZ         29-10-2002      Abdelouahab ZAAZAA              Ajout gestion verification PIN par methode Offset
0004MAW        25-05-2003      M.A.WAHBI                       * Take the PVK pair referenced to by the Pin Key 
of the Card Range while Pin verification by Offset                                                                               method
0005MAW        16-09-2005      M.A.WAHBI (MAW20050916)         * Cryptogram Checking
0006						19-09-2006			Mehdi Elyajizi 									* Verification du IDN,DAC,ATC,AUC
0006SWI        01-12-2008      S.EL WAGGAGUI (SWI01122008)     * Visa Enhancement iCVV Mandate
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>



#include <tlv_fields.h>
#include <tlv_autho.h>
#include <event_auth.h>

#include <tlv_privfields.h>
#include <tlv_private.h>

#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <iso8583.h>
/*================================================================================*/
/*================================================================================*/
/*static*/ void FillSecurityResult ( int result , char *sSecurityResults , int position )
{
	switch ( result )
	{
	case 0 : sSecurityResults [ position ]  = P7_CTRL_CHECK_OK;		break;
	case 1 : sSecurityResults [ position ]  = P7_CTRL_CHECK_INV;	break;
	case 2 : sSecurityResults [ position ]  = P7_CTRL_CHECK_ERROR;	break;
	case 3 : sSecurityResults [ position ]  = P7_CTRL_CHECK_MALFUNCTION; break;
	case 9 : sSecurityResults [ position ]  = P7_CTRL_CHECK_MALFUNCTION; break;
	default: sSecurityResults [ position ]  = P7_CTRL_CHECK_NOPROC; break;
	}
}


static void PutHsmRjectEvent ( TSTlvAutho *sTlvInfo , int result , int position )
{
	char sPinNbr	[4];
	char sPinMaxNbr	[4];
	char sLine		[MAX_LINE_TRC];
	int  nLength;

	switch(result)
	{
	case CRYPTOGRAPHIC_ERROR: 
		PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo ,ERROR_SYCHRONISATION_KEY , 3);
		break;
	case HSM_MALFUNCTION:
		/*if (position == TRACKIII_POSITION)
		PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , EVENT_INVALID_TRACK_III , 3);
		else*/
		PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo ,MALFUNCTION_EVENT , 3);
		break;
	case HSM_UNAVAILABLE:
		PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , HSM_UNAVAILABLE_EVENT , 3);
		break;
	case VERIFICATION_FAILED:
		switch ( position )
		{
		case SEC_FLAG_CHK_ICVV :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_ICVV , AUTH_EVENT_CODE_LEN);
			break;
		case SEC_FLAG_CHK_CVV1 :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVV1 , AUTH_EVENT_CODE_LEN);
			break;
		case SEC_FLAG_CHK_CVV2 :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVV2 , AUTH_EVENT_CODE_LEN);
			break;
		case SEC_FLAG_CHK_CVC3 :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVC3 , AUTH_EVENT_CODE_LEN);
			break;
		case SEC_FLAG_CHK_AAV :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CAVV , AUTH_EVENT_CODE_LEN);
			break;
		case SEC_FLAG_CHK_ARQC :
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CRYPTO , AUTH_EVENT_CODE_LEN);
			break;
		case PIN_POSITION :
			memset(sPinNbr, 	0, sizeof(sPinNbr));
			memset(sPinMaxNbr, 	0, sizeof(sPinMaxNbr));
			if ( GetTlvAutho (  AUTO_PIN_NBR  , sTlvInfo, sPinNbr , &nLength ) == SUCCES &&
				GetTlvAutho (  AUTO_PIN_RETRY_MAX  , sTlvInfo, sPinMaxNbr , &nLength ) == SUCCES
				)
			{
				sprintf(sLine," Pin Error %s, Max Retry %s", sPinNbr, sPinMaxNbr);
				trace_event(sLine,TRACE);
				if (atoi(sPinNbr) + 1 >= atoi(sPinMaxNbr) )
				{
					PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , PIN_NB_EXCEEDED  , AUTH_EVENT_CODE_LEN);
					return;
				}
			}
			PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_PIN  , AUTH_EVENT_CODE_LEN);
			break;
		}  
		break;
	}

}
/*================================================================================*/
/*================================================================================*/
static int HsmDecline ( TSTlvAutho *sTlvInfo , 
	char		*sSecurityResults , 
	char		*sHsmResultCode , 
	char		position )
{
	char    		sTlvPrivateBuffer [ P7_F048_MAX_LEN + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	char			sLine [ MAX_LINE_TRC ];
	int             nLength;

	InitTlvPrivate(&sTlvPrivateInfo);

	PutTlvAutho ( AUTO_SECURITY_VER_RESULTS  , sTlvInfo,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
	GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , sTlvInfo , sTlvPrivateBuffer ,&nLength);
	AnalyseTlvPrivate(sTlvPrivateBuffer,nLength,&sTlvPrivateInfo );

	sprintf ( sLine , "HsmResultCode = %s ", sHsmResultCode );
	trace_event ( sLine , PROCESSING );
	PutTlvPrivate(HSM_RESULT_CODE_TAG  ,&sTlvPrivateInfo,sHsmResultCode,4);

	switch(position)
	{
	case SEC_FLAG_CHK_CVV1:
		PutTlvPrivate(CVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_CVV1 ,1);
		break;
	case SEC_FLAG_CHK_CVV2:
		PutTlvPrivate(CVV2_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_CVV2 ,1);
		break;
	case SEC_FLAG_CHK_CVC3:
		PutTlvPrivate(CVV2_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_CVC3 ,1);
		break;
	case SEC_FLAG_CHK_ICVV:
		PutTlvPrivate(ICVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_ICVV ,1);
		break;
	/* ELKAM20221221 PLUTONL-5555 START */
	/* case SEC_FLAG_CHK_APP_USAGE_CTRL:
		PutTlvPrivate(AUC_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_APP_USAGE_CTRL ,1);
		break;
	case SEC_FLAG_CHK_DATA_AUTH_CD:
		PutTlvPrivate(DAC_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_DATA_AUTH_CD ,1);
		break;
	case SEC_FLAG_CHK_DYN_NBR:
		PutTlvPrivate( DYNAMIC_NBR_RESULT_CODE_TAG,&sTlvPrivateInfo,sSecurityResults + SEC_FLAG_CHK_DYN_NBR ,1);
		break; */ /* ELKAM20221221 PLUTONL-5555 */
	/* ELKAM20221221 PLUTONL-5555 END */
	}

	nLength = PrivateBuildTlv( sTlvPrivateBuffer , &sTlvPrivateInfo);
	PutTlvAutho ( AUTO_ADD_DATA_PRIVATE,sTlvInfo,sTlvPrivateBuffer,nLength);
	return ( OK );
}
/*================================================================================*/
/*================================================================================*/
/******************************************************************************/
#define DISC_DATA_MAX_LEN				13
#define MCI_CVC3_T1_BITMAP				"AAAUUCCCCUUOOO00"
#define MCI_CVC3_T2_BITMAP				"AAAUCCCCUUUOOO00"
/******************************************************************************/
int CheckAuthSecurity ( int nIndiceCtx, char *sTlvBuffer)
{


	TSTlvAutho      sTlvInfo;
	TSTlvPrivate    sTlvPrivateInfo;
	TSTlvPrivate    sTlvIccInfo;
	char    		sSecurityData       [ P7_F053_MAX_LEN + 1 ];
	char    		sIccDataTLV         [ BUF_LG_MAX + 1 ];
	char			sFieldData[512];
	int     		nIsoICCLen;
	int     		nLength;
	int				nLen;
	int     		sPinLength;
	int				nIsChipCard;
	char			pin_method;
	char			encryption_mode;

	char cvv1_flag = 'Y';
	char cvv2_flag = 'Y';
	char pin_flag  = 'Y';
	char cvc3_flag = 'Y';
	char crpt_flag = 'Y';
	char icvv_flag = 'Y';
	char app_usage_ctrl_flag = 'Y';
	char data_auth_code_flag = 'Y';
	char dynamic_number_flag = 'Y';
	char avv_flag = 'Y';
	char cavv_decision_matrix_flag = 'Y';	/*AEA20210906 PLUTONL-2595*/
	char app_usage_ctrl_result = '4';
	char data_auth_code_result = '4';
	char dynamic_number_result = '4';
	char cEcI[1+1];/*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

	int n_cvv1_result = 4;
	int n_cvv2_result = 4;
	int n_pin_result  = 4;
	int n_crpt_result = 4;
	int n_cvc3_result = 4; 
	int n_icvv_result = 4;
	int n_avv_result  = 4;

	int n_dpin_result  = 4;

	char c_avv_result;

	char   sTrack1Data 		[ 77 + 1];
	char   sTrack2Data 		[ 40 + 1];
	char   sTrackInfo		[ 77 + 1];
	char   sPan 			[ 22 + 1];
	char   sPinBlock		[ 16 + 1];
	char   sExpiryDate		[  4 + 1];
	char   sExpirySwapped	[  4 + 1];
	char   sServiceCode		[  3 + 1];
	char   pvki;
	char   sPvv         	[  5 + 1]; 
	char   sCvv1        	[  3 + 1];
	char   sCvv2        	[  3 + 1];
	char   siCvv        	[  3 + 1];
	char   sReelPvv			[5];
	char   tmpPan 			[ 22 + 1];
	char   tmpExpiryDate	[  4 + 1];
	char   tmpServiceCode	[  3 + 1];
	char   tmpPvv			[  5 + 1];


	int    nFlag=AMX_ALGO_V2;


	char   sSecurityFlags  [ SEC_FLAG_LEN + 1 ];
	char   sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char   sHsmResultCode  [ 23 + 1 ];/*IBO20220803 PLUTONL-5013 PLUTONL-5014*/
	char   sMsgErr         [ 17 + 1 ];
	char   sOffset         [ 5 + 1 ];

	char   sCVKA  [ SIZE_KEY_TDES + 1];
	char   sCVKB  [ SIZE_KEY_TDES + 1];
	char   sPVKA  [ SIZE_KEY_TDES + 1];
	char   sPVKB  [ SIZE_KEY_TDES + 1];
	char   sTransKey  [ SIZE_KEY_TDES + 1];
	char   sPIN_KEY   [ SIZE_KEY_TDES + 1];
	char   sSecurityFormat  [ 2 + 1];
	char   sPinBlockFormat  [ 2 + 1];
	
	
	char   sLine [ MAX_LINE_TRC ];
	int    result;
	int    nResult;
	int    nLenExpDate = 0;

	
	
	char   tOffset  [ 12 + 1];
	int    tPinLength;
	int    result_pin_info;
	char   sPOS_DATA	[ 12 + 1];
	char   sProcCode	[ 24 + 1];
	char   sNetworkCode[2+1]; 
	char   sAddDataPriv [ P7_F048_MAX_LEN +1 ];
	int   			nPvki;
	char   			sCvc3        [ 5 + 1];
	int    			offset = 0;	
	char   			track_ind;
	char   			sKcvkc [ SIZE_KEY_TDES + 1]; 
	char			dcvv_flag;
	
	char			sCardEffDate[4 + 1];

	char 		aav_field    [256];
	char 		sCSC5[5 + 1];
	char 		siCSC5[5 + 1];
	int  		aav_present_flg = 0;	/*AEA20210906 PLUTONL-2595*/
	int 		do_eval_matrix = 0;		/*AEA20210906 PLUTONL-2595*/
	int 		icvvCheck =0;
	char		cIsChipAuth;
	char		cvv1_as_icvv_opt;

	/* MCI CVC3 */
	int		i;
	int		nDiscDataLen;
	int		nIndexCVC3;
	int		nIndexATC;
	int		nIndexUN;
	int		nIndexStaticDiscData;
	char	sDiscDataBitmap[DISC_DATA_MAX_LEN + 1];
	char	sDiscData[DISC_DATA_MAX_LEN + 1];
	char	sCVC3ATC[12];
	char	sCVC3UN[12];
	char	sTrackInfoBCD[128];
	char	sTrackData[128];
	char	sAtcTrack[12];
	char	sUnTrack[12];
	char	sStaticDiscData[DISC_DATA_MAX_LEN + 1];

	int		nPanLen;
	int		nTrackInfoLen;
	int		nOffsetTrackInfo;
	int		nDiscDataOff;
	/* MCI CVC3 */
	/*ZKORACHI START 04122019  SPA2 MCI */
	char			sDSTransactionID[P7_F048_K06_LEN + 1 ];
	char			sTxCurr [3 + 1];
	char			sTxAmount[18 + 1];
	char			sMerchantName[P7_F043_MAX_LEN + 1];
	/*char			sCardSecInfoData[512];*/ /*ASO202000401*/
	/*ZKORACHI START 04122019*/

	int    			result_matrix_decision = 0; /*ACH30112019 PLUTONL-606*/ 
	/*int   			nCvki;/*ICH20200310* /
	char			sCvki[1+1];/*ASO202000401*/
	char   			sExtSecResults[ SEC_FLAG_LEN + 1 ]; /*ACH05042020 PLUTONL-1476*/ 
	TlvInfo    		KIsoEcomAuthenTlv;   
	int  		    estimated_amnt_pres = 0;
	int  		    estimated_mer_pres  = 0;

	char			sAuthAddInfo[512];/*ASO16122021 - PLUTONL-4046 - MDES reject - SLI = 242 with no UCAF */
	char            sEcomPosInfo[P7_F048_P66_LEN + 1];


	/* MJI 13/10/2020 DYNAMIC AUTHENTICATION START */
	char			sDynAuthentication[1+1];
	char   			sCvcServiceCode[ 3 + 1];
	char   			sDpvv[  5 + 1];
	char   			tDoffset  [ 12 + 1];
	char   			sReelDpvv [5];
	/* MJI 13/10/2020 DYNAMIC AUTHENTICATION END */

	/************************************************************************/
	/* INITIALISATION DES VARIABLES                                         */
	/************************************************************************/
	trace_event("Start CheckAuthSecurity()",PROCESSING);




	memset ( sTrack1Data, 	0, sizeof ( sTrack1Data ));
	memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
	memset ( sPan,  		0, sizeof ( sPan ));
	memset ( sPinBlock,     0, sizeof ( sPinBlock ));
	memset ( sSecurityData, 0, sizeof ( sSecurityData ));
	memset ( sExpiryDate,  	0, sizeof ( sExpiryDate));
	memset ( sExpirySwapped,  	0, sizeof ( sExpirySwapped));
	memset ( sServiceCode,  0, sizeof ( sServiceCode));
	memset ( sPvv,  		0, sizeof ( sPvv ));
	memset ( sCvv1,  		0, sizeof ( sCvv1));
	memset ( sCvv2,  		0, sizeof ( sCvv2));
	memset ( siCvv,  		0, sizeof ( siCvv));
	memset ( tmpPan,  		0, sizeof ( tmpPan ));
	memset ( tmpExpiryDate,  	0, sizeof ( tmpExpiryDate));
	memset ( tmpServiceCode,  0, sizeof ( tmpServiceCode));
	memset ( tmpPvv,  		0, sizeof ( tmpPvv ));

	memset ( sSecurityFlags   , 0,  sizeof (sSecurityFlags));
	memset ( sSecurityResults , 0,  sizeof ( sSecurityResults));

	memset ( siCSC5 , '\0',  sizeof (siCSC5 )); 
	memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
	memset ( sMsgErr          , 0, sizeof ( sMsgErr ));
	memset ( sOffset          , 0, sizeof ( sOffset ));
	memset ( sSecurityFormat  , 0, sizeof ( sSecurityFormat ));
	
	
	memset ( sReelPvv  , 0 , sizeof (sReelPvv)) ;
	memset ( sCVKA  , 0, sizeof ( sCVKA ));
	memset ( sCVKB  , 0, sizeof ( sCVKB ));
	memset ( sPVKA  , 0, sizeof ( sPVKA ));
	memset ( sPVKB  , 0, sizeof ( sPVKB ));
	memset ( sTransKey  , 0, sizeof ( sTransKey ));
	memset ( sPIN_KEY   , 0, sizeof ( sPIN_KEY )); 
	memset ( sPOS_DATA  , 0, sizeof ( sPOS_DATA ));
	
	
	memset ( tOffset,  		0, sizeof ( tOffset ));
	memset ( siCvv,             0, sizeof ( siCvv)); 
	memset ( tmpPan   , 0, sizeof ( tmpPan ));
	memset ( sNetworkCode   , 0, sizeof ( sNetworkCode ));
	memset ( tmpExpiryDate   , 0, sizeof ( tmpExpiryDate ));
	memset ( tmpServiceCode   , 0, sizeof ( tmpServiceCode ));
	memset ( tmpPvv   , 0, sizeof ( tmpPvv ));

	memset ( sKcvkc  , 0, sizeof ( sKcvkc ));
	memset ( sCvc3, 0, sizeof(sCvc3));
	memset ( sCSC5, 0, sizeof(sCSC5));

	memset ( aav_field, 0, sizeof(aav_field));
	memset ( sCardEffDate   , 0, sizeof(sCardEffDate) );
	memset(sTransKey,'\0',sizeof(sTransKey));
	memset(sProcCode,'\0',sizeof(sProcCode));
	
	/*ZKORACHI START 04122019  SPA2 MCI */
	memset ( sDSTransactionID, 	0, sizeof ( sDSTransactionID ));
	memset ( sTxCurr, 	0, sizeof ( sTxCurr ));
	memset ( sTxAmount, 	0, sizeof ( sTxAmount ));
	memset ( sMerchantName, 	0, sizeof ( sMerchantName ));
	/*memset ( sCardSecInfoData, 	0, sizeof ( sCardSecInfoData ));*/
	/*ZKORACHI START 04122019*/
	memset ( sExtSecResults, 	0, sizeof ( sExtSecResults )); /*ACH05042020 PLUTONL-1476*/ 
	/* MJI 13/10/2020 DYNAMIC AUTHENTICATION START */
	memset ( sDynAuthentication,  0, sizeof ( sDynAuthentication));
	memset ( sCvcServiceCode,  0, sizeof ( sCvcServiceCode));
	memset ( sDpvv,  		0, sizeof ( sDpvv ));
	memset ( tDoffset,  		0, sizeof ( tDoffset ));
	memset ( sReelDpvv  , 0 , sizeof (sReelDpvv)) ;
	/* MJI 13/10/2020 DYNAMIC AUTHENTICATION END */
	memset ( sAuthAddInfo, 	0, sizeof ( sAuthAddInfo )); /*ASO16122021 - PLUTONL-4046 - MDES reject - SLI = 242 with no UCAF */
	memset(sEcomPosInfo,'\0',sizeof(sEcomPosInfo));
	memset(cEcI,   	'\0', sizeof(cEcI));/*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

	InitTlvPrivate (&sTlvPrivateInfo);
	InitTlvPrivate (&sTlvIccInfo);

	InitTlvAutho	(&sTlvInfo);

	track_ind = 'X';
	dcvv_flag = 'N';
	cIsChipAuth = 'N';
	cvv1_as_icvv_opt = 'N'; /*Should check a CVV1 as an iCVV if CVV1 check fails? */
	nIsChipCard = NOK;


	result = GetSecurityChecksParams(nIndiceCtx,sTlvBuffer);
	if( result != OK )
	{
		PrintTlvBuffer(sTlvBuffer);
		trace_event("End   GetSecurityChecksParams(MALFUNCTION): Security params fault",ERROR);
		return ( result );
	}

	AnalyseTlvAutho (sTlvBuffer , &sTlvInfo);


	if ( GetTlvAutho (  AUTO_SECURITY_VERIF_FLAGS  , &sTlvInfo, sSecurityFlags , &nLength ) != SUCCES )
	{
		trace_event("End   CheckAuthSecurity(MALFUNCTION): Security Flags Empty.",ERROR);
		return ( SYSTEM_MALFUNCTION );
	}

	if ( GetTlvAutho (  AUTO_SECURITY_VER_RESULTS , &sTlvInfo, sSecurityResults , &nLength ) != SUCCES )
	{
		sprintf(sLine,"Security Results Empty"); trace_event(sLine,ERROR);
		trace_event("End   CheckAuthSecurity(MALFUNCTION)",ERROR);
		return ( SYSTEM_MALFUNCTION );
	}


	if(GetTlvAutho (AUTO_ICC_DATA, &sTlvInfo, sIccDataTLV , &nLength)==SUCCES)
	{
		AnalyseTlvPrivate(sIccDataTLV ,nLength,&sTlvIccInfo);
		cIsChipAuth = 'Y';
	}

	if(GetTlvAutho (AUTO_ADD_DATA_PRIVATE, &sTlvInfo, sAddDataPriv , &nLength)==SUCCES)
	{
		AnalyseTlvPrivate(sAddDataPriv,nLength,&sTlvPrivateInfo );
	}

	trace_event("SECURITY FLAGS RECEIVED",TRACE);
	sprintf(sLine,"________ _____ ________");
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CVV1 FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_CVV1] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CVC2 FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_CVV2] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"PIN  FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_PIN] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"PIN  METD : %c", sSecurityFlags[SEC_FLAG_PIN_MTHD] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CRPT FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_ARQC] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"ICVV FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_ICVV] );
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"CVC3 FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_CVC3]); 
	trace_event(sLine,PROCESSING);
	sprintf(sLine,"AVV  FLAG : %c", sSecurityFlags[SEC_FLAG_CHK_AAV]); 
	trace_event(sLine,PROCESSING);


	cvv1_flag 			= sSecurityFlags[SEC_FLAG_CHK_CVV1];
	cvv2_flag 			= sSecurityFlags[SEC_FLAG_CHK_CVV2];
	pin_flag  			= sSecurityFlags[SEC_FLAG_CHK_PIN];
	pin_method			= sSecurityFlags[SEC_FLAG_PIN_MTHD];
	crpt_flag  			= sSecurityFlags[SEC_FLAG_CHK_ARQC];
	icvv_flag			= sSecurityFlags[SEC_FLAG_CHK_ICVV];
	cvc3_flag			= sSecurityFlags[SEC_FLAG_CHK_CVC3];
	app_usage_ctrl_flag = sSecurityFlags[SEC_FLAG_CHK_APP_USAGE_CTRL];
	data_auth_code_flag = sSecurityFlags[SEC_FLAG_CHK_DATA_AUTH_CD];
	dynamic_number_flag = sSecurityFlags[SEC_FLAG_CHK_DYN_NBR];
	avv_flag			= sSecurityFlags[SEC_FLAG_CHK_AAV];
	cavv_decision_matrix_flag = sSecurityFlags[SEC_FLAG_CHK_CAVV_DECISION_MATRIX];	/*AEA20210906 PLUTONL-2595*/

	
	if(sSecurityResults[SEC_FLAG_CHK_AAV]=='4')
	{
		sSecurityResults [SEC_FLAG_CHK_AAV]		= 'R';
	}

	PutTlvAutho(AUTO_SECURITY_VER_RESULTS,&sTlvInfo, sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );


	/*****************************************************************************/
	/* Read Pos Data                                                           */
	/*****************************************************************************/
	GetTlvAutho (AUTO_POS_DATA, &sTlvInfo, sPOS_DATA , &nLength);

	/******************************************************************************************/
	/* Recuperation du ICVV du TRACK2 Equivalent from ICC										*/
	/******************************************************************************************/


	memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
	if (GetTlvPrivate (CHIP_T2_EQUIVALENT_DATA , &sTlvIccInfo, sTrack2Data, &nLen) == SUCCES)
	{
		GetTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,sPvv,siCvv);

	}

	
	if(GetTlvAutho(AUTO_TRACK2_DATA,&sTlvInfo,sTrack2Data,&nLength)== SUCCES)
	{
		GetTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,tmpPvv,sCvv1);
		GetTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
		track_ind = '2';
	}
	else if(GetTlvAutho(AUTO_TRACK1_DATA,&sTlvInfo,sTrack1Data,&nLength) == SUCCES)
	{
		GetTrack1Data(sTrack1Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
		track_ind = '1';
	}
	else
	{
		GetTlvAutho(AUTO_PAN,&sTlvInfo,sPan,&nLength);
		GetTlvAutho(AUTO_EXPIRY_DATE,&sTlvInfo,sExpiryDate,&nLenExpDate); 
		
	}

	GetTlvAutho( AUTO_NETWORK_CODE , &sTlvInfo, sNetworkCode, &nLength );

	if( memcmp(sNetworkCode,NET_AMEX,NET_LEN) == 0 ) 		
	{
		GetAmxTrack2Data(sTrack2Data,ISO_7813_T2_FMT,tmpPan,sExpiryDate,sServiceCode,sCardEffDate,sCSC5);

		if (GetTlvPrivate (CHIP_T2_EQUIVALENT_DATA , &sTlvIccInfo, sTrack2Data, &nLen) == SUCCES)
		{
			GetAmxTrack2Data(sTrack2Data,ISO_7813_T2_FMT,tmpPan,sExpiryDate,sServiceCode,sCardEffDate,siCSC5);   
		}  

		if( memcmp(sServiceCode, "201", 3) == 0  || memcmp(sServiceCode, "702", 3) == 0 )
			nIsChipCard = OK;

	}
	else
	{
		if( (sServiceCode[0] == '2') || (sServiceCode[0] == '6'))
			nIsChipCard = OK;
	}


	if ( sPOS_DATA[6] == P7_F022_P7_PAN_AUTO_ENTRY_CL_MAG )
	{
		dcvv_flag = 'Y';
		cvv1_flag = 'N';
		icvv_flag = 'N';
	}
	
	/*PRODXX: Do not check a CVV1 as an iCVV is the CVV1 fails on a non chip auth*/
	if( (sPOS_DATA[6] == P7_F022_P7_ICC ) || (sPOS_DATA[6] == P7_F022_P7_PAN_AUTO_ENTRY_CL_ICC)) /*NAB16032020  PLUTONL-1372*/
	{
		cvv1_as_icvv_opt = 'Y';
	}
	else
	{
		cvv1_as_icvv_opt = 'N';
	}


	nLenExpDate = strlen(sExpiryDate) ;

	GetTlvPrivate( ECOM_AUTH_INFO, &sTlvPrivateInfo, sEcomPosInfo, &nLength); /*IBO20221025 PLUTONL-5272*/


	/*---------------------------------------------------------------------------*/
	/*        CRYPTOGRAMME  VERIFICATION                                         */
	/*---------------------------------------------------------------------------*/
	/*PRODXX: Moved at the begining to skip cvv check if ARQC is OK*/
	if ( 'Y' == crpt_flag && 'Y' == cIsChipAuth )
	{
		GetTlvAutho ( AUTO_PROCESSING_CODE , &sTlvInfo, sProcCode, &nLength);
		
		if((memcmp(sNetworkCode, NET_VISA, NET_LEN) != 0 ) ||  	/*AMER20190611 PROD00069534*/
			(memcmp(sProcCode+6, P7_F003_REFUND, P7_F003_P1_LEN) != 0 ) 
		)
		{
			n_crpt_result = check_crpt_data ( 	
				nIndiceCtx,
				sTlvBuffer,
				sHsmResultCode 
				);

			/** Adding Issuer Authen Data On Iso from Tlv **/
			if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccDataTLV ,&nLength) == SUCCESS )
				PutTlvAutho (AUTO_ICC_DATA, &sTlvInfo , sIccDataTLV, nLength);
			/** ***************************************** **/

			if ( n_crpt_result != OK )
			{
				FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
				PutHsmRjectEvent ( &sTlvInfo , n_crpt_result , SEC_FLAG_CHK_ARQC );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_ARQC );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckAuthSecurity(NOK CRPT)",PROCESSING);
				return ( NOK );
			}
			FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC ); /*AMER 20161102: We fill crpt_result if the check is successful*/
			/*PRODXX: ARQC is OK, no need to check icvv/cvv1 */
			cvv1_flag = 'N';
			icvv_flag = 'N';
		}
	}



	

	result = GetTlvAutho( AUTO_CVK_KEY , &sTlvInfo, sTransKey, &nLength );
	if(strlen(sTransKey) <= 32 + 1 )
	{
		memcpy(sCVKA, sTransKey, SIZE_KEY_DES);
		memcpy(sCVKB, sTransKey + SIZE_KEY_DES, SIZE_KEY_DES);
	}
	else
	{
		memcpy(sCVKA, sTransKey, strlen(sTransKey));
		memcpy(sCVKB, sTransKey, strlen(sTransKey));
	}

	
	if( memcmp(sNetworkCode,NET_AMEX,NET_LEN) != 0 )
	{
		if ( ( cvv1_flag == 'Y' ) && ( strlen ( sCvv1 ) != 0 )  )
		{
			n_cvv1_result = HSM_MALFUNCTION;

			if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0 ) 
			{

				strcpy ( sHsmResultCode , "-CVK");
				strcpy ( sMsgErr,"INVALID KEYs CVK");
			}
			else
			{


				n_cvv1_result = check_cvv_data (	nIndiceCtx ,
					sPan , 
					sCVKA, sCVKB,
					sExpiryDate, sServiceCode, sCvv1,
					sHsmResultCode );
				/* Check if the T2 is actually the T2 equivalent data from Chip */
				/*PRODXX: only if auth is icc */
				if( n_cvv1_result != OK && nIsChipCard == OK  && cvv1_as_icvv_opt == 'Y' )
				{
					n_icvv_result = check_cvv_data (	nIndiceCtx ,
						sPan , 
						sCVKA, sCVKB,
						sExpiryDate, "999", sCvv1,
						sHsmResultCode );	
					n_cvv1_result = n_icvv_result;
					FillSecurityResult ( n_icvv_result , sSecurityResults , SEC_FLAG_CHK_ICVV );
				}
				else
				{
					FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );
				}
			}


			if ( n_cvv1_result != OK ) 
			{
				FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );/*ICH20170108*/

				PutHsmRjectEvent ( &sTlvInfo , n_cvv1_result , SEC_FLAG_CHK_CVV1 );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_CVV1 );

				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

				if(memcmp(sHsmResultCode,"-CVK", 4) != 0)
					TextError(sHsmResultCode + 2,sMsgErr);

				PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
				memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
				nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
				PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv ,  nLength);
				PrintTlvBuffer(sTlvBuffer);

				return ( NOK );
			}
		}
	}
	else
	{
		if ( ( cvv1_flag == 'Y' ) && ( strlen ( sCSC5 ) != 0 )  )
		{
			n_cvv1_result = HSM_MALFUNCTION;

			if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0 ) 
			{

				strcpy ( sHsmResultCode , "-CVK");
				strcpy ( sMsgErr,"INVALID KEYs CVK");
			}
			else
			{
				/* if service code = 201 or 702(contact/contacless cards then 5CSC is calculated using SVC 201
				* else(magstrip card), 5CSC is calculated by the service code encoded on the card*/
				if( memcmp(sServiceCode,"201",3) == 0 || memcmp(sServiceCode,"702",3) == 0 )
				{
					n_cvv1_result = check_amx_csc_data (  nIndiceCtx ,
						sPan , 
						sCVKA, 
						sCVKB,
						sExpiryDate ,
						"201",
						AMX_CSC5 ,
						sCSC5,
						nFlag,
						sHsmResultCode );
				}
				else
				{
					n_cvv1_result = check_amx_csc_data (  nIndiceCtx ,
						sPan , 
						sCVKA, 
						sCVKB,
						sExpiryDate ,
						sServiceCode,
						AMX_CSC5 ,
						sCSC5,
						nFlag,
						sHsmResultCode );
				}
				/* Check if the T2 is actually the T2 equivalent data from Chip */
				if( n_cvv1_result != OK && nIsChipCard == OK  && cvv1_as_icvv_opt == 'Y' )
				{
					/*contact chip cards(identified by SVC 201) have iCSC calculated using SVC 999, contactless cards have 5iCSC calculated usng SVC 702*/
					if( memcmp(sServiceCode,"702",3) == 0 )
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"702",
							AMX_CSC5 ,
							sCSC5,
							nFlag,
							sHsmResultCode );
					}
					else
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"999",
							AMX_CSC5 ,
							sCSC5,
							nFlag,
							sHsmResultCode );
					}

					n_cvv1_result = n_icvv_result;
					FillSecurityResult ( n_icvv_result , sSecurityResults , SEC_FLAG_CHK_ICVV );
				}
				else
				{
					FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );
				}
			}


			if ( n_cvv1_result != OK ) 
			{
				FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );/*ICH20170108*/

				PutHsmRjectEvent ( &sTlvInfo , n_cvv1_result , SEC_FLAG_CHK_CVV1 );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_CVV1 );

				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

				if(memcmp(sHsmResultCode,"-CVK", 4) != 0)
					TextError(sHsmResultCode + 2,sMsgErr);

				PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
				memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
				nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
				PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv ,  nLength);
				PrintTlvBuffer(sTlvBuffer);

				return ( NOK );
			}
		}

	}


	/*---------------------------------------------------------------------------*/
	/*        ICVV VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/

	if(memcmp(sNetworkCode,NET_AMEX,NET_LEN) != 0 )
	{
		if( (icvv_flag == 'Y' && n_icvv_result != OK ) &&
			(strlen(sCvv1) > 0 || strlen(siCvv) > 0 ) )
		{

			if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0)
			{
				strcpy ( sHsmResultCode , "-CVK");
				strcpy ( sMsgErr,"INVALID KEYs CVK");
			}
			else 
			{	
				if( strlen(siCvv) > 0 )
				{
					n_icvv_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, sExpiryDate, "999", siCvv, sHsmResultCode );
				}

				if( n_icvv_result != OK && sSecurityResults[SEC_FLAG_CHK_CVV1] == '4' && strlen(sCvv1) > 0 )
				{
					n_icvv_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, sExpiryDate, "999", sCvv1, sHsmResultCode );
					if ( n_icvv_result != OK )
					{
						n_icvv_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, sExpiryDate, sServiceCode, sCvv1, sHsmResultCode );
						if( n_icvv_result == OK )
						{
							n_cvv1_result = n_icvv_result;
							n_icvv_result = 4;
							FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );

						}
					}
				}

				if ( n_icvv_result != OK && n_icvv_result != 4 ) 
				{
					PutHsmRjectEvent ( &sTlvInfo , n_icvv_result , SEC_FLAG_CHK_ICVV );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_ICVV );

					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

					if(memcmp(sHsmResultCode,"-CVK", 4) != 0)
						TextError(sHsmResultCode + 2,sMsgErr);

					PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
					memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
					nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
					PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv ,  nLength);
					PrintTlvBuffer(sTlvBuffer);

					return ( NOK );
				}
			}
		}
	}
	else
	{
		if( (icvv_flag == 'Y' && n_icvv_result != OK )&&
			(strlen(sCSC5) > 0 || strlen(siCSC5) > 0 ) )
		{

			if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0)
			{
				strcpy ( sHsmResultCode , "-CVK");
				strcpy ( sMsgErr,"INVALID KEYs CVK");
			}
			else 
			{	
				if( strlen(siCSC5) > 0 )
				{
					if( memcmp(sServiceCode,"702",3) == 0 )
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"702",
							AMX_CSC5 ,
							siCSC5,
							nFlag,
							sHsmResultCode );
					}
					else
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"999",
							AMX_CSC5 ,
							siCSC5,
							nFlag,
							sHsmResultCode );
					}

				}

				if( n_icvv_result != OK && sSecurityResults[SEC_FLAG_CHK_CVV1] == '4' && strlen(sCSC5) > 0 )
				{
					if( memcmp(sServiceCode,"702",3) == 0 )
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"702",
							AMX_CSC5 ,
							sCSC5,
							nFlag,
							sHsmResultCode );
					}
					else
					{
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"999",
							AMX_CSC5 ,
							sCSC5,
							nFlag,
							sHsmResultCode );
					}

					if ( n_icvv_result != OK )
					{
						/*iCSC checks have failed, this may be a magstrip iCSC, both contact and contactless cards use SVC 201*/
						n_icvv_result = check_amx_csc_data (  nIndiceCtx ,
							sPan , 
							sCVKA, 
							sCVKB,
							sExpiryDate ,
							"201",
							AMX_CSC5 ,
							sCSC5,
							nFlag,
							sHsmResultCode );

						if( n_icvv_result == OK )
						{
							n_cvv1_result = n_icvv_result;
							n_icvv_result = 4;
							FillSecurityResult ( n_cvv1_result , sSecurityResults , SEC_FLAG_CHK_CVV1 );

						}
					}
				}

				if ( n_icvv_result != OK && n_icvv_result != 4 ) 
				{
					PutHsmRjectEvent ( &sTlvInfo , n_icvv_result , SEC_FLAG_CHK_ICVV );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_ICVV );

					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

					if(memcmp(sHsmResultCode,"-CVK", 4) != 0)
						TextError(sHsmResultCode + 2,sMsgErr);

					PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
					memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
					nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
					PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv ,  nLength);
					PrintTlvBuffer(sTlvBuffer);

					return ( NOK );
				}
			}
		}	
	}

	

	/*---------------------------------------------------------------------------*/
	/*        CVC2 VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	memset(sCvv2,'\0',sizeof(sCvv2));
	GetTlvPrivate	(CVC2_DATA_TAG	,&sTlvPrivateInfo , sCvv2 ,&nLength);

	/*MJI 13/10/2020 DYNAMIC AUTHENTICATION START*/
	GetTlvPrivate	(DYNAMIC_AUTHENTICATION	,&sTlvPrivateInfo , sDynAuthentication ,&nLength);
	if(sDynAuthentication[0] == 'C')
	{
		GetTlvAutho (  DYNAMIC_AUTHENTICATION_INFO , &sTlvInfo, sCvcServiceCode , &nLength );
	}
	else
	{
		strcpy ( sCvcServiceCode , "000");
	}
	/*MJI 13/10/2020 DYNAMIC AUTHENTICATION END*/

	if( memcmp(sNetworkCode,NET_AMEX,NET_LEN) != 0 )
	{
		if ( ( cvv2_flag == 'Y' ) && ( strlen ( sCvv2 ) != 0 ) && ( memcmp ( sCvv2 , "XXX" , 3 ) != 0 ) /*&& (nLenExpDate != 0)*/)
		{
			if (nLenExpDate != 0) /* CFE_KSA PLUTONL-3489*/
			{
				if (debug_option == 1)
				{
					sprintf(sLine,"=> CVV2 : [%s]", sCvv2 );
					trace_event(sLine,PROCESSING);
				}

				if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0) 
				{
					n_cvv2_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-CVK" , 4 );
				}
				else
				{
					memcpy ( sExpirySwapped + 0 , sExpiryDate + 2 , 2 );
					memcpy ( sExpirySwapped + 2 , sExpiryDate + 0 , 2 );
					n_cvv2_result = check_cvv_data (  nIndiceCtx ,
						sPan , 
						sCVKA, 
						sCVKB,
						sExpirySwapped ,
						"000" ,
						sCvv2,
						sHsmResultCode );

					if (n_cvv2_result != OK)
						n_cvv2_result = check_cvv_data (  nIndiceCtx ,
						sPan , 
						sCVKA, 
						sCVKB,
						sExpiryDate ,
						"000" ,
						sCvv2,
						sHsmResultCode );
				}
			}
			else
			{
				n_cvv2_result = VERIFICATION_FAILED;
			}

			FillSecurityResult ( n_cvv2_result , sSecurityResults , SEC_FLAG_CHK_CVV2 );

			if ( n_cvv2_result != OK ) 
			{
				PutHsmRjectEvent ( &sTlvInfo , n_cvv2_result , SEC_FLAG_CHK_CVV2 );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_CVV2 );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckAuthSecurity(NOK CVV2)",PROCESSING);
				return ( NOK );
			}

		}
	}
	else
	{			
		if ( ( cvv2_flag == 'Y' ) && ( strlen ( sCvv2 ) != 0 ) && ( memcmp ( sCvv2 , "XXX" , 3 ) != 0 ))
		{
			if (nLenExpDate != 0) /* CFE_KSA PLUTONL-3489*/
			{
				if (debug_option == 1)
				{
					sprintf(sLine,"=> CVV2 : [%s]", sCvv2 );
					trace_event(sLine,PROCESSING);
				}

				if ( strlen(sCVKA) == 0 || strlen(sCVKB) == 0) 
				{
					n_cvv2_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-CVK" , 4 );
				}
				else
				{
					n_cvv2_result = check_amx_csc_data (  nIndiceCtx ,
						sPan , 
						sCVKA, 
						sCVKB,
						sExpiryDate ,
						"000",
						strlen(sCvv2) ,
						sCvv2,
						nFlag,
						sHsmResultCode );
				}
			}
			else
			{
				n_cvv2_result = VERIFICATION_FAILED;
			}

			FillSecurityResult ( n_cvv2_result , sSecurityResults , SEC_FLAG_CHK_CVV2 );

			if ( n_cvv2_result != OK ) 
			{
				PutHsmRjectEvent ( &sTlvInfo , n_cvv2_result , SEC_FLAG_CHK_CVV2 );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_CVV2 );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckAuthSecurity(NOK CVV2)",PROCESSING);
				return ( NOK );
			}

		}

	}

	/*---------------------------------------------------------------------------*/
	/*        CVC3 VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	memset(sKcvkc,'\0',sizeof(sKcvkc));
	

	result = GetTlvAutho( AUTO_DCVV_KEY , &sTlvInfo, sKcvkc, &nLength );

	

	if (  (cvc3_flag == 'Y' ) && (dcvv_flag == 'Y' ) && (track_ind != 'X') )
	{

		if ( strlen(sKcvkc) == 0)
		{
			memcpy ( sHsmResultCode , "-CVK" , 4 );
			return (HSM_MALFUNCTION);
		}

		if( debug_option == 1 )
		{
			sprintf(sLine, "DCVV / CVV3 KEY        : %s", sKcvkc);
			trace_event(sLine, PROCESSING);
		}


		if (memcmp(sNetworkCode,NET_VISA,NET_LEN) == 0)
		{
			memset(sAtcTrack,'\0',sizeof(sAtcTrack));
			memset (sCvc3 , 0,sizeof(sCvc3));
			memset(sAtcTrack,'0',6);

			if( track_ind == '2' )
			{
				offset = strlen(sPan);
				memcpy (sCvc3 , 		sTrack2Data + offset+1+4+3+1+4 ,	3);
				memcpy (sAtcTrack + 2 , sTrack2Data + offset+1+4+3+1+4+3 ,  4);
				
			}
			else /*TRACK1*/
			{
				offset = strlen(sTrack1Data) - 11;
				memcpy (sAtcTrack + 2 , sTrack1Data + offset , 2);
				memcpy (sAtcTrack + 4 , sTrack1Data + offset + 5 , 2);
				memcpy (sCvc3 , sTrack1Data + offset + 2 ,3);
			}

			n_cvc3_result = check_dcvv_data (  nIndiceCtx ,
				sKcvkc,
				sPan ,
				sExpiryDate,
				sServiceCode,
				sAtcTrack,
				sCvc3,
				sHsmResultCode
				);

		}
		else if (memcmp(sNetworkCode,NET_MASTERCARD,NET_LEN) == 0)
		{
		
			

			nDiscDataLen = 0;
			nPanLen = 0;
			nTrackInfoLen = 0;
			nIndexStaticDiscData = 0;

			memset(sDiscDataBitmap,'\0',sizeof(sDiscDataBitmap));
			memset(sDiscData,'\0',sizeof(sDiscData));
			memset(sCVC3ATC,'\0',sizeof(sCVC3ATC));
			memset(sCVC3UN,'\0',sizeof(sCVC3UN));
			memset(sTrackInfoBCD,'\0',sizeof(sTrackInfoBCD));
			memset(sTrackData,'\0',sizeof(sTrackData));
			memset(sAtcTrack,'\0',sizeof(sAtcTrack));
			memset(sUnTrack,'\0',sizeof(sUnTrack));
			memset(sStaticDiscData,'\0',sizeof(sStaticDiscData));

			if( track_ind == '2' )
			{
				strncpy(sTrackData,sTrack2Data,sizeof(sTrackData) - 1);
				strncpy(sDiscDataBitmap,MCI_CVC3_T2_BITMAP,sizeof(sDiscDataBitmap)-1);

				nLength = strlen(sTrackData);
				nPanLen = strlen(sPan);
				nDiscDataOff = nPanLen + 1 + 7;

				if( nLength <= nDiscDataOff )
				{
					memcpy ( sHsmResultCode , "-CDD" , 4 );
					return (HSM_MALFUNCTION);
				}
			}
			else if( track_ind == '1' )
			{
				strncpy(sTrackData,sTrack1Data,strlen(sTrack1Data));
				strncpy(sDiscDataBitmap,MCI_CVC3_T1_BITMAP,sizeof(sDiscDataBitmap)-1);

				nLength = strlen(sTrackData);
				nPanLen = strlen(sPan);

				nDiscDataOff = strlen(sTrackData) - 13; /* we may also look for it by parsing trackdata and looking for the 2snd ^ and add 7*/
			}

		
			nDiscDataLen = nLength - nDiscDataOff;
			memcpy (sDiscData, sTrackData + nDiscDataOff, nDiscDataLen);

			
			
			nIndexCVC3 = 0;
			nIndexATC = 0;
			nIndexUN = 0;
			nIndexStaticDiscData = 0;
			for(i=0; i < strlen(sDiscDataBitmap), i < nDiscDataLen; i++ )
			{
				if( sDiscDataBitmap[i] == 'C' )
				{
					sFieldData[nIndexCVC3++] = sDiscData[i];
					sStaticDiscData[nIndexStaticDiscData++]='0';
				}
				else if( sDiscDataBitmap[i] == 'A' )
				{
					sCVC3ATC[nIndexATC++] = sDiscData[i];
					sStaticDiscData[nIndexStaticDiscData++]='0';
				}
				else if( sDiscDataBitmap[i] == 'U' )
				{
					sCVC3UN[nIndexUN++] = sDiscData[i];
					sStaticDiscData[nIndexStaticDiscData++]='0';
				}
				else if( sDiscDataBitmap[i] == 'O' )
				{
					sStaticDiscData[nIndexStaticDiscData++]='0';
				}
				else
				{
					sStaticDiscData[nIndexStaticDiscData++]=sDiscData[i];
				}
			}
			memset(sCvc3,'X',5);
			if( nIndexCVC3 < 5 )
			{
				memcpy(sCvc3 + (5-nIndexCVC3), sFieldData, nIndexCVC3);
				nIndexCVC3 = 5;
			}
		
			sCvc3[nIndexCVC3]='\0';
			sCVC3ATC[nIndexATC]='\0';		/* decimal number */
			sCVC3UN[nIndexUN]='\0';
			sStaticDiscData[nIndexStaticDiscData]='\0';



			if( track_ind == '2' )
			{
				nOffsetTrackInfo = 0;
				memcpy (sTrackInfo, sTrackData, nPanLen); nOffsetTrackInfo+=nPanLen;
				sTrackInfo[nOffsetTrackInfo++]='D';
				memcpy (sTrackInfo + nOffsetTrackInfo, sTrackData + nPanLen + 1, 7);nOffsetTrackInfo+=7;

				memcpy(sTrackInfo + nOffsetTrackInfo,sStaticDiscData,nIndexStaticDiscData); nOffsetTrackInfo+=nIndexStaticDiscData;
				if( nIndexStaticDiscData%2 != 0 )
				{
					sTrackInfo[nOffsetTrackInfo++]='F';
				}

				nTrackInfoLen = AsciiToHex(sTrackInfo,sTrackInfoBCD,nOffsetTrackInfo);
			}
			else
			{
				nTrackInfoLen = strlen(sTrackData);
				memcpy(sTrackInfo,sTrackData,nTrackInfoLen);
				memcpy(sTrackInfo + nDiscDataOff ,sStaticDiscData,nIndexStaticDiscData);
				memcpy(sTrackInfoBCD,sTrackInfo,nTrackInfoLen);
			}


			sprintf ( sAtcTrack, "%05d", atoi(sCVC3ATC));
			sprintf ( sUnTrack, "%08d", atoi(sCVC3UN));

			n_cvc3_result = check_cvc3_data (			nIndiceCtx ,
														sKcvkc,
														sPan ,
														"000",			/* PSN	*/
														"",				/*Expiry date*/
														"",				/*Service code*/
														sAtcTrack,		/* ATC		*/
														"",				/* dCVV		*/
														"",				/* sIvcvc3	*/
														nTrackInfoLen,
														sTrackInfoBCD,
														sUnTrack,
														sCvc3,
														sHsmResultCode );
		}
		else
		{
			/*Amex??*/
			trace_event(">>>>: Non supported network",ERROR);
			n_cvc3_result = NOK;
		}

		FillSecurityResult ( n_cvc3_result , sSecurityResults , SEC_FLAG_CHK_CVC3 );

		if ( n_cvc3_result != OK )
		{
			PutHsmRjectEvent ( &sTlvInfo , n_cvc3_result , SEC_FLAG_CHK_CVC3 );
			HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_CVC3 );
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			trace_event("End   CheckAuthSecurity(NOK CVC3)",PROCESSING);
			return ( NOK );
		}

		
	}
	else
	{
		trace_event("NO CVC3 VERIFICATION ",TRACE);
		FillSecurityResult ( 4 , sSecurityResults , SEC_FLAG_CHK_CVC3 );
	}

	/* END MJA21092011  CONTACTLESS */

	/* START 3D-SECURE MCK 24072012*/
	/*---------------------------------------------------------------------------*/
	/*        CAVV AVV AEVV  VERIFICATION                                        */
	/*---------------------------------------------------------------------------*/
	/*

	- If CAVV not present
		- If F022.P7 IN (S,T) 			-> P54 = P7_F048_P54_CAVV_INVALID(0)						-> Eval matrix
		- Else, If F022.P7 IN (U,V,9,F) 
			- If CVV2 present 			-> P54 = P7_F048_P54_CAVV_NOT_VERF_OTHER_SEC_METHD(S)		-> Eval matrix
			- Else(CVV2 not present) 	-> P54 = P7_F048_P54_CAVV_VAL_NOT_PERF(V)					-> Eval matrix
		- Else(not an ecom author)																	-> continue
	- Else(CAVV present)
		- If CAVV ctrl chk = N 			-> P54 = P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART(P)			-> Eval matrix
		- Else(CAVV ctrl chk = Y)
			- Verify CAVV
			- If CAVV verified OK			-> continue
			- If CAVV verified NOK/Error	-> reject
			- If CAVV not verified
				- If External CAVV result present	-> 	P54 = P7_F048_P54_CAVV_VALID_OK_VISA(B)		-> No Decision matrix evaluation
				- Else(CAVV result not present)
					- Si F022.P7 = S -> P54 = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE(O)				-> Eval matrix
					- Si F022.P7 = T -> P54 = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH(N)				-> Eval matrix
					- Sinon P54 = P7_F048_P54_CAVV_VAL_NOT_PERF(V)									-> Eval matrix


	*/
	
	memset(aav_field 		, 0 , sizeof(aav_field		));
	aav_present_flg = 0;	/*AEA20210906 PLUTONL-2595*/
	do_eval_matrix = 0;	/*AEA20210906 PLUTONL-2595*/
	if (GetTlvPrivate	(AAV_TAG	,&sTlvPrivateInfo , aav_field ,&nLength)== SUCCES )
	{
		sprintf(sLine ,"AAV_TAG == %s",aav_field);	
		trace_event(sLine ,TRACE);
		aav_present_flg = 1;
	}
	else if (GetTlvPrivate	(UCAF_TAG	,&sTlvPrivateInfo , aav_field ,&nLength)== SUCCES) /*CORRECTION AAV CIS NAK*/
	{
		sprintf(sLine ,"CAAV FROM UCAF_TAG == %s",aav_field);	
		trace_event(sLine ,TRACE);
		aav_present_flg = 1;
	}
	/*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	if (sPOS_DATA[6] 	== P7_F022_P7_ECOM_NON_AUTH_ATTEMPT			|| 
		sPOS_DATA[6] 	== P7_F022_P7_ECOM_AUTHENTICATED 			|| 
		sPOS_DATA[6] 	== P7_F022_P7_ECOM_UNSECURE 			    || 
		sPOS_DATA[6] 	== P7_F022_P7_ECOM_SEC_CHAN )
	{

		cEcI[0]=sPOS_DATA[6];
	}
	else if(sPOS_DATA[6] == P7_F022_P7_CRED_ON_FILE)
	{
		cEcI[0]=sEcomPosInfo[0];
	}
	
	
	if( aav_present_flg == 0 )
	{
		/*Start AMER20180322 PROD00054205 Enh18.Q2 VISA Art 1.1/ MC AN1085*/
		if (sPOS_DATA[6] == P7_F022_P7_ECOM_NON_AUTH_ATTEMPT || sPOS_DATA[6] == P7_F022_P7_ECOM_AUTHENTICATED
			|| sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT || sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED /*IBO20221025 PLUTONL-5272*/)
		{
			/*ZKO 05042022 PLUTONL-4548 Art VISA3.10 Start*/
			GetTlvPrivate	(AUTH_ADD_INFO	,&sTlvPrivateInfo , sAuthAddInfo ,&nLength);
			if( memcmp(sNetworkCode,NET_VISA,NET_LEN) == 0 
			&& sAuthAddInfo[P7_F048_P64_TOKEN_AUTH_OSFT] == '1')
			{
				sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VERF_OTHER_SEC_METHD;
			}
			else
			{
				sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_INVALID;
				do_eval_matrix = 1;	/*AEA20210906 PLUTONL-2595*/
			}
			/*ZKO 05042022 PLUTONL-4548 Art VISA3.10 End*/
		}
		else if(sPOS_DATA[6] == P7_F022_P7_ECOM_UNSECURE || sPOS_DATA[6] == P7_F022_P7_ECOM ||
			sPOS_DATA[6] == P7_F022_P7_ECOM_SEC_CHAN || sPOS_DATA[6] == P7_F022_P7_CRED_ON_FILE)
		{
			if( strlen ( sCvv2 ) != 0 )
			{
				sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VERF_OTHER_SEC_METHD;
				do_eval_matrix = 1;
			}
			else
			{
				/*START ASO16122021 - PLUTONL-4046 - MDES reject - SLI = 242 with no UCAF */
				GetTlvPrivate(AUTH_ADD_INFO	,&sTlvPrivateInfo , sAuthAddInfo ,&nLength);
				GetTlvPrivate(EXT_SECURITY_VER_RESULTS_TAG	,&sTlvPrivateInfo , sExtSecResults ,&nLength);
				GetTlvPrivate(SECURITY_LEVEL_IND_TAG	,&sTlvPrivateInfo , sFieldData ,&nLength);

				if(sAuthAddInfo[P7_F048_P64_TOKEN_AUTH_OSFT] == '1' && sExtSecResults[4] == '0' &&
					( memcmp(sFieldData + 4, "242", 3 ) == 0 ||
					  memcmp(sFieldData + 4, "246", 3 ) == 0 ||
					  memcmp(sFieldData + 4, "247", 3 ) == 0 ||
					  memcmp(sFieldData + 4, "210", 3 ) == 0 ))
				{
					sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VERF_OTHER_SEC_METHD;
				}
				else if(sAuthAddInfo[P7_F048_P64_INITIATING_PARTY] != P7_F048_P64_P4_MER_INIT)
				{
					sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_VAL_NOT_PERF;
					do_eval_matrix = 1;
				}
				/*END ASO16122021 - PLUTONL-4046 - MDES reject - SLI = 242 with no UCAF */
			}
			
		}
		else
		{
			do_eval_matrix = 0;
		}
		
	}
	else
	{
		/*Start AEA20210906 PLUTONL-2595*/
		if ( avv_flag == 'N' )
		{
			sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART;
			do_eval_matrix = 1;
		}
		else
		{
			
			result = 3;
			if( memcmp(sNetworkCode,NET_VISA,NET_LEN) == 0 )
			{
				result = check_cavv_data(	nIndiceCtx,
					sPan,
					aav_field,
					sHsmResultCode
					);
			}
			else if (memcmp(sNetworkCode,NET_CUP,NET_LEN) == 0)
			{
				memset(sExpiryDate,'\0',sizeof(sExpiryDate));
				GetTlvPrivate	(CUP_DS_EXPIRY_DATE	,&sTlvPrivateInfo , sExpiryDate ,&nLength);

				result = check_cup_av_data(	nIndiceCtx,
				sPan,
				sExpiryDate,
				aav_field,
				sHsmResultCode
				);
			}
			else if( memcmp(sNetworkCode,NET_MASTERCARD,NET_LEN) == 0 )
			{
			
				/*Zkorachi start 04122019*/
				GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , &sTlvInfo, sTxCurr, &nLength );
				if (GetTlvPrivate(ECOM_AUTHENTIF_DATA , &sTlvPrivateInfo,sFieldData, &nLength) == SUCCES) 
				{
					InitEcomAuthentificationInfoDataTlv(&KIsoEcomAuthenTlv); 	 
					AnalyseTlvInfo(sFieldData, nLength, &KIsoEcomAuthenTlv);

					if ( GetTlvInfoField(P7_F048S26_ECOM_AUT_DATA_ESTIMATED_AMOUNT , &KIsoEcomAuthenTlv, sFieldData, &nLength) == SUCCES)
					{
						result = AdjustTlvAmount (  nIndiceCtx , sFieldData , sTxCurr , sTxAmount );
						if ( result != OK )
						{
							trace_event("Error AdjustTlvAmount(ECOM ESTIMATED AMOUNT)", ERROR);
							return (NOK);
						}
						estimated_amnt_pres = 1;
					}
					if ( GetTlvInfoField(P7_F048S26_ECOM_AUT_DATA_MERCHANT_ACRONYM , &KIsoEcomAuthenTlv, sMerchantName, &nLength) == SUCCES)
					{
						estimated_mer_pres = 1;
					}
				} 
				if (estimated_amnt_pres == 0)
				{
					GetTlvAutho( AUTO_AMOUNT_TRANSACTION , &sTlvInfo, sTxAmount, &nLength );
				}
				if (estimated_mer_pres == 0)
				{
					GetTlvAutho( AUTO_CARD_ACC_LOCATION , &sTlvInfo, sMerchantName, &nLength );
				}
				GetTlvPrivate( DIR_SERV_TRANS_ID ,&sTlvPrivateInfo , sDSTransactionID ,&nLength);
				/*GetTlvAutho     ( AUTO_CARD_SEC_INFO_DATA,&sTlvInfo,sCardSecInfoData,&nLength);*//*ICH20200310*/																						   
				GetTlvPrivate( ECOM_AUTH_INFO, &sTlvPrivateInfo, sFieldData, &nLength);
				
				result = check_aav_data(		nIndiceCtx,
					sPan,
					/*nCvki,*//*ICH20200310*/						 
					sDSTransactionID,
					sTxCurr,
					sTxAmount,
					sMerchantName,
					sFieldData[1],
					aav_field,
					sHsmResultCode
					);
			/*Zkorachi End*/
			}
			else if( memcmp(sNetworkCode,NET_AMEX,NET_LEN) == 0 )
			{
				nFlag  = AMX_AEVV;  /*AEVV  WITH ALGO VERSION 2 AEVV*/
				
				memset ( sCVKA  , 0, sizeof ( sCVKA ));
				memset ( sCVKB  , 0, sizeof ( sCVKB ));
	
				result = check_amx_csc_data (  nIndiceCtx ,
					sPan , 
					sCVKA, 
					sCVKB,
					"",
					"",
					AMX_AEVV ,
					aav_field,
					nFlag,
					sHsmResultCode );		
			}
			/*Start AMER20180322 PROD00054205 Enh18.Q2 VISA Art 1.1/ MC AN1085*/
			if( result == OK )
			{
				/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
				switch ( sPOS_DATA[6] )
				{
					case P7_F022_P7_ECOM_AUTHENTICATED  : c_avv_result = P7_F048_P54_CAVV_AUTHEN_OK; break;
					case P7_F022_P7_ECOM_NON_AUTH_ATTEMPT : c_avv_result = P7_F048_P54_CAVV_ATTEMP_OK; break;
				}
				if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED || sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT) /*IBO20221025 PLUTONL-5272*/
				{
					if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED)
						c_avv_result = P7_F048_P54_CAVV_AUTHEN_OK;
					else
						c_avv_result = P7_F048_P54_CAVV_ATTEMP_OK;
				}

				if(cEcI[0]!=P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT && cEcI[0]!=P7_F048_P66_POS1_ECOM_AUTHENTICATED)/* Issuers should respond to a Visa Data Only*/
				{
					c_avv_result =P7_F048_P54_CAVV_VALID_OK_VISA;
					do_eval_matrix = 1;
				}
				/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

				sSecurityResults[SEC_FLAG_CHK_AAV] = c_avv_result;
			}
			else if(result != NOK)	/*Nothing to be done if result = NOK (CAVV generated with Network keys; the interface already mapped CAVV result to ISS F48.P54*/
			{
				if  (result == VERIFICATION_FAILED)
				{
					switch ( sPOS_DATA[6] )
					{
						case P7_F022_P7_ECOM_AUTHENTICATED   : c_avv_result  = P7_F048_P54_CAVV_AUTHEN_NOK  ; break;
						case P7_F022_P7_ECOM_NON_AUTH_ATTEMPT : c_avv_result  = P7_F048_P54_CAVV_ATTEMP_NOK; break;
					}
					if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED || sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT) /*IBO20221025 PLUTONL-5272*/
					{
						if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED)
							c_avv_result  = P7_F048_P54_CAVV_AUTHEN_NOK;
						else
							c_avv_result  = P7_F048_P54_CAVV_ATTEMP_NOK;
					}
					if(cEcI[0]==P7_F022_P7_ECOM_SEC_CHAN) /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
					{
							c_avv_result = P7_F048_P54_CAVV_AUTHEN_NOK;
					}    
				}
				else	/*Error on HSM*/
				{
					switch ( sPOS_DATA[6] )
					{
						case P7_F022_P7_ECOM_AUTHENTICATED   : c_avv_result  = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH  ; break;
						case P7_F022_P7_ECOM_NON_AUTH_ATTEMPT : c_avv_result  = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE; break;
					}
					if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED || sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT) /*IBO20221025 PLUTONL-5272*/
					{
						if(sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED)
							c_avv_result  = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH;
						else
							c_avv_result  = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE;
					}
				}

				sSecurityResults[SEC_FLAG_CHK_AAV] = c_avv_result;
				PutHsmRjectEvent ( &sTlvInfo , result , SEC_FLAG_CHK_AAV );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_AAV );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckAuthSecurity(NOK AAV)",PROCESSING);
				return ( NOK );

			}

			else
			{
				/* At this stage result = NOK, CAVV have not been checked by the HSM*/
				/*Start AEA20210906 PLUTONL-2595*/
				if (GetTlvPrivate(AAV_RESULT, &sTlvPrivateInfo, sFieldData, &nLength) == SUCCES &&
					sFieldData[0] != ' ') /*ASO16122021 - PLUTONL-4046 - MDES reject - SLI = 242 with no UCAF */
				{
					sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_VALID_OK_VISA;
					do_eval_matrix = 0;
				}
				else
				{
					if (sPOS_DATA[6] == P7_F022_P7_ECOM_NON_AUTH_ATTEMPT)
					{
						sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE;
					}
					else if (sPOS_DATA[6] == P7_F022_P7_ECOM_AUTHENTICATED)
					{
						sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH;
					}
					else if (sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT)/*IBO20221025 PLUTONL-5272*/
					{
						sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE;
					}
					else if (sEcomPosInfo[0] == P7_F048_P66_POS1_ECOM_AUTHENTICATED) /*IBO20221025 PLUTONL-5272*/
					{
						sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH;
					}
					else
					{
						sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_VAL_NOT_PERF;
					}

					do_eval_matrix = 1;
				}
			}
			/*End AEA20210906 PLUTONL-2595*/
		}
	}
	if( do_eval_matrix == 1 && cavv_decision_matrix_flag == 'Y')
	{
		PutTlvAutho(AUTO_SECURITY_VER_RESULTS, &sTlvInfo, sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN);
		AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

		result_matrix_decision = EvaluateDecisionMatrix(nIndiceCtx,
														SEC_FLAG_CHK_AAV + 1,
														sTlvBuffer);

		if (result_matrix_decision != OK)
		{
			trace_event("End   CheckAuthSecurity(NOK AAV)", PROCESSING);
			return (NOK);
		}
		GetTlvBuffer(AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults, &nLength);
		
	}

	/*AEA20210906 PLUTONL-2595 Comment the case of cardholder not present*/
	/*else if(sPOS_DATA[4] == P7_F022_P5_E_ORDER )
	{
		if (avv_flag == 'N') 
		{
			sSecurityResults[SEC_FLAG_CHK_AAV] = P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART;
		}
		else if (sSecurityResults[SEC_FLAG_CHK_AAV] != P7_F048_P54_CAVV_NOT_YET_VERIFIED)
		{
			PutTlvAutho(AUTO_SECURITY_VER_RESULTS,&sTlvInfo,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			result_matrix_decision = EvaluateDecisionMatrix(		nIndiceCtx,
																	SEC_FLAG_CHK_AAV +1, 
																	sTlvBuffer);

			if ( result_matrix_decision != OK )
			{
				trace_event("End   CheckAuthSecurity(NOK AAV)",PROCESSING);
				return ( NOK );
			}			
		}
	}*/

	/*Start AEA20210906 PLUTONL-2595*/
	memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
	nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
	PutTlvAutho ( AUTO_ADD_DATA_PRIVATE , &sTlvInfo, sAddDataPriv ,  nLength);
	/*End AEA20210906 PLUTONL-2595*/

	/*End ACH30112019 PLUTONL-606*/

	/*---------------------------------------------------------------------------*/
	/*        PIN  VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/

	memset(sTransKey,'\0',sizeof(sTransKey));

	result = GetTlvAutho( AUTO_PVK_KEY , &sTlvInfo, sTransKey, &nLength );

	/*memcpy(sPVKA, sTransKey, SIZE_KEY_DES);
	memcpy(sPVKB, sTransKey + SIZE_KEY_DES, SIZE_KEY_DES);*/

	if(strlen(sTransKey) <= 32 + 1 )
	{
		memcpy(sPVKA, sTransKey, SIZE_KEY_DES);
		memcpy(sPVKB, sTransKey + SIZE_KEY_DES, SIZE_KEY_DES);
	}
	else
	{
		memcpy(sPVKA, sTransKey, strlen(sTransKey));
		memcpy(sPVKB, sTransKey, strlen(sTransKey));
	}


	if( GetTlvAutho ( AUTO_PIN_DATA , &sTlvInfo, sPinBlock, &nLength ) == SUCCES)
	{
		if ( ( pin_flag == 'Y' ) && ( nLength != 0 ) )
		{
			GetTlvPrivate   ( ENCR_PIN_BLOCK_KEY_TAG,&sTlvPrivateInfo,sTransKey,&nLength);
			GetTlvAutho     ( AUTO_SECURITY_DATA,&sTlvInfo,sSecurityData,&nLength);
			
			
			GetSecurityData ( SECURITY_FORMAT,sSecurityData,sSecurityFormat,&nLength);

			if ( memcmp(sSecurityFormat,"01",2) == 0)  /* TPK ENCRYPTION */
				encryption_mode = TERMINAL_MODE;
			else if ( memcmp(sSecurityFormat,"02",2) == 0)  /* ZPK ENCRYPTION */
				encryption_mode = SCHEME_MODE;

			GetSecurityData(PIN_BLOCK_FORMAT,sSecurityData,sPinBlockFormat,&nLength);

			/*result_pin_info = get_pin_info ( nIndiceCtx, sPan, &nPvki,&VisaOfset, tOffset, &tPinLength); AHA20150609*/
			/*EBE150809
			result_pin_info = get_pin_info ( nIndiceCtx, sPan, &nPvki,&VisaOfset, tOffset,sReelPvv, &tPinLength);
			*/
			GetTlvAutho     ( AUTO_CARD_SEC_INFO_DATA,&sTlvInfo,sFieldData,&nLength);


			result_pin_info = get_pin_info_data(sFieldData,&nPvki,sReelPvv,tOffset,&tPinLength);
			/* MJI 13/10/2020 DYNAMIC AUTHENTICATION START */
			if(sDynAuthentication[0] == 'P')
			{
				if ( pin_method ==  PVV_VERIFICATION_METHOD &&
					 GetTlvAutho (  DYNAMIC_AUTHENTICATION_INFO , &sTlvInfo, sReelDpvv , &nLength ) == SUCCES)
				{
					if ( strlen(sPVKA) == 0 || strlen(sPVKB) == 0)
					{
						n_dpin_result = HSM_MALFUNCTION;
						memcpy ( sHsmResultCode , "-PVK" , 4 );
					}
					else
					{
						sprintf(sDpvv,"%01d%s",nPvki,sReelDpvv);

						n_dpin_result = check_pvv_data ( 	nIndiceCtx, sPan,
							sPVKA, sPVKB,
							sPinBlock,sPinBlockFormat,sTransKey,
							sDpvv , encryption_mode ,
							sHsmResultCode );
					}
				}
				else if ( pin_method == OFFSET_VERIFICATION_METHOD &&
						  GetTlvAutho (  DYNAMIC_AUTHENTICATION_INFO , &sTlvInfo, tDoffset , &nLength ) == SUCCES)
				{
					/*if(sPVKA[0]=='S')
					{
					solution temporaire pour HSM Atalla � changer*/
						result = GetTlvAutho( AUTO_PVK_V1 , &sTlvInfo, sPVKA, &nLength );
					/*}*/

					if ( strlen(sPVKA) == 0 || strlen(sPVKB) == 0 )
					{
						n_dpin_result = HSM_MALFUNCTION;
						memcpy ( sHsmResultCode , "-PVK" , 4 );
					}
					else
					{
						n_dpin_result = check_offset_data ( 	nIndiceCtx, sPan,
							sPVKA, sPinBlock,
							sPinBlockFormat, sTransKey,
							tDoffset , encryption_mode ,
							sHsmResultCode , tPinLength );
					}
				}
				if ( n_dpin_result == OK )
				{
					FillSecurityResult ( n_dpin_result  , sSecurityResults , SEC_FLAG_CHK_PIN);
				}
			}
			if( sDynAuthentication[0] == '\0' ||
				(sDynAuthentication[0] == 'P' && n_dpin_result != OK ))
			{
			/* MJI 13/10/2020 DYNAMIC AUTHENTICATION END */
				/*EBE160227: if PVV has not been found in DB, use T2 data*/
				if( sReelPvv[0] !=' ' )
				{
					sprintf (sLine, "PIN Length = %d", tPinLength);
					trace_event (sLine, TRACE);
					sprintf(sPvv,"%01d%s",nPvki,sReelPvv);
				}
				if (debug_option == 1)
				{
					sprintf (sLine, "sPvv value (%s), sPinBlockFormat(%s)", sPvv,sPinBlockFormat);
					trace_event (sLine, PROCESSING);
				}

				if ( pin_method ==  PVV_VERIFICATION_METHOD )
				{
					if ( strlen(sPVKA) == 0 || strlen(sPVKB) == 0)
					{
						n_pin_result = HSM_MALFUNCTION;
						memcpy ( sHsmResultCode , "-PVK" , 4 );
					}
					else
					{
						n_pin_result = check_pvv_data ( 	nIndiceCtx, sPan,
							sPVKA, sPVKB,
							sPinBlock,sPinBlockFormat,sTransKey,
							sPvv , encryption_mode ,
							sHsmResultCode );
					}
					if ( n_pin_result != OK )
					{
						FillSecurityResult ( n_pin_result  , sSecurityResults , PIN_POSITION);
						PutHsmRjectEvent ( &sTlvInfo , n_pin_result , PIN_POSITION );
						HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION);
						AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
						trace_event("End  CheckAuthSecurity(NOK PIN)",PROCESSING);
						return ( NOK );
					}
				}
				else if ( pin_method == OFFSET_VERIFICATION_METHOD )
				{
					if ( result_pin_info != OK )  /* offset was not found in database */
					{
						n_pin_result = HSM_MALFUNCTION;
						memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
						memcpy ( sHsmResultCode , "-PVK" , 4 );
						memcpy ( sMsgErr,"GET OFFSET FROM DB ERROR",24);
						PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
					}
					else
					{
						/*if(sPVKA[0]=='S')/*ZKO26032019
						{solution temporaire pour HSM Atalla � changer*/
							result = GetTlvAutho( AUTO_PVK_V1 , &sTlvInfo, sPVKA, &nLength );
						/*}*/

						if ( strlen(sPVKA) == 0 || strlen(sPVKB) == 0 )
						{
							n_pin_result = HSM_MALFUNCTION;
							memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
							memcpy ( sHsmResultCode , "-PVK" , 4 );
							memcpy ( sMsgErr,"ERR GETTING PVKA/PVKB",21);
							PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
						}
						else
						{
							n_pin_result = check_offset_data ( 	nIndiceCtx, sPan,
								sPVKA, sPinBlock,
								sPinBlockFormat, sTransKey,
								tOffset , encryption_mode ,
								sHsmResultCode , tPinLength );
						}
					}

					if ( n_pin_result != OK )
					{
						FillSecurityResult ( n_pin_result  , sSecurityResults , SEC_FLAG_CHK_PIN);
						PutHsmRjectEvent ( &sTlvInfo , n_pin_result , SEC_FLAG_CHK_PIN );
						HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , SEC_FLAG_CHK_PIN);
						AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
						trace_event("End   CheckAuthSecurity(PIN NOK)",PROCESSING);
						return ( NOK );
					}
				}
				FillSecurityResult ( n_pin_result  , sSecurityResults , SEC_FLAG_CHK_PIN);

				if (n_pin_result == OK && sDynAuthentication[0] == 'P')
				{
					/*Start ZKO30122021 PLUTONL-4052*/
					PutTlvPrivate(DYNAMIC_AUTHENTICATION ,&sTlvPrivateInfo,"N", 1);
					memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
					nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
					PutTlvAutho ( AUTO_ADD_DATA_PRIVATE , &sTlvInfo, sAddDataPriv ,  nLength);
					/*End ZKO30122021 PLUTONL-4052*/
				}
			}
		}
	}





	PutTlvAutho(AUTO_SECURITY_VER_RESULTS,&sTlvInfo,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);


	memset ( sTrack1Data, 	0, sizeof ( sTrack1Data ));
	memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
	memset ( sPan,  		0, sizeof ( sPan ));
	memset ( sPinBlock,     0, sizeof ( sPinBlock ));
	memset ( sSecurityData, 0, sizeof ( sSecurityData ));
	memset ( sExpiryDate,  	0, sizeof ( sExpiryDate));
	memset ( sExpirySwapped,  	0, sizeof ( sExpirySwapped));
	memset ( sServiceCode,  0, sizeof ( sServiceCode));
	memset ( sPvv,  		0, sizeof ( sPvv ));
	memset ( sCvv1,  		0, sizeof ( sCvv1));
	memset ( sCvv2,  		0, sizeof ( sCvv2));

	memset ( sSecurityResults , 0,  sizeof ( sSecurityResults));
	memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
	memset ( sSecurityFormat  , 0, sizeof ( sSecurityFormat ));
	
	
	memset ( sReelPvv  , 0 , sizeof (sReelPvv)) ;
	memset ( sCVKA  , 0, sizeof ( sCVKA ));
	memset ( sCVKB  , 0, sizeof ( sCVKB ));
	memset ( sPVKA  , 0, sizeof ( sPVKA ));
	memset ( sPVKB  , 0, sizeof ( sPVKB ));
	memset ( sTransKey  , 0, sizeof ( sTransKey ));
	memset ( sPIN_KEY   , 0, sizeof ( sPIN_KEY )); 
	memset ( sPOS_DATA  , 0, sizeof ( sPOS_DATA ));
	
	memset ( tmpPan   , 0, sizeof ( tmpPan ));
	memset ( tmpPvv   , 0, sizeof ( tmpPvv ));
	memset ( aav_field, 0, sizeof(aav_field));

	memset(sAtcTrack,'\0',sizeof(sAtcTrack));
	memset (sCvc3 , 0,sizeof(sCvc3));

	InitTlvPrivate(&sTlvPrivateInfo);
	InitTlvPrivate(&sTlvIccInfo);
	InitTlvAutho(&sTlvInfo);

	trace_event("End   CheckAuthSecurity(OK)",PROCESSING);
	return ( OK );
}

/**********************************************************************************************/

