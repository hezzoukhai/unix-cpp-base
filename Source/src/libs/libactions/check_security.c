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
0007           08-02-2016      Y EL BAKKOUCHI (YEL05022016)     Add a new verirification method API
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <p7_define.h>

#include <tlv_fields.h>
#include <tlv_autho.h>
#include <event_auth.h>


#include <tlv_privfields.h>
#include <tlv_private.h>

#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <p7_sid.h>

#define CVC1_POSITION 0
#define CVC2_POSITION 1
#define PIN_POSITION  2
#define CRPT_POSITION  4
#define ICVV_POSITION  18
#define AUC_POSITION   19        
#define DAC_POSITION   20
#define CVC3_POSITION 24
#define AVV_POSITION 25
#define DYNAMIC_NBR_POSITION  21
#define OFFLINE_DATA_FAILED 1   
#define DDA	3
#define SDA 	2
#define DDA_FAILED	5			
#define SDA_FAILED	2			
#define VISA_NETWORK   "01"
#define MC_NETWORK	"02"       /**End Mehdi Elyajizi 19092006 :  MYJ231007******/ 
/* 0002YO */
#define TRACKI_POSITION  5
#define TRACKII_POSITION  6
#define TRACKIII_POSITION  7
/* 0002YO */
/*YK011003*/

#define SIZE_KEY_TDES 256


/*YK011003*/
/*================================================================================*/
/*================================================================================*/
void FillSecurityResult ( char result , char *sSecurityResults , int position )
{
	switch ( result )
	{
	case 0 : sSecurityResults [ position ]  = '0'; break;
	case 1 : sSecurityResults [ position ]  = '1'; break;
	case 2 : sSecurityResults [ position ]  = '2'; break;
	case 3 : sSecurityResults [ position ]  = '3'; break;
	case 9 : sSecurityResults [ position ]  = '3'; break;
	default: sSecurityResults [ position ]  = '4'; break;
	}
}


void PutHsmRjectEvent ( TSTlvAutho *sTlvInfo , char result , int position )
{
	char sPinNbr	[LG_MAX];
	char sPinMaxNbr	[LG_MAX];
	char sLine		[MAX_LINE_TRC];
	int  nLength;

	if ( result == CRYPTOGRAPHIC_ERROR )
		PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo ,ERROR_SYCHRONISATION_KEY , 3);
	else
		if ( result == HSM_MALFUNCTION     )
			/* 0002YO */
			if (position == TRACKIII_POSITION)
				PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , EVENT_INVALID_TRACK_III , 3);
			else
				/* 0002YO */
				PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo ,MALFUNCTION_EVENT , 3);
		else
			if ( result == HSM_UNAVAILABLE )
				PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , HSM_UNAVAILABLE_EVENT , 3);
			else
				if ( result == VERIFICATION_FAILED )
				{
					switch ( position )
					{
					case ICVV_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_ICVV , 3);
						break;
					case CVC1_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVV1 , 3);
						break;
					case CVC2_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVV2 , 3);
						break;
						/* MJA21092011  CONTACTLESS */
					case CVC3_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CVC3 , 3);
						break;
						/*EMV:issuer*/
						/* MJA20130206 3D-SECURE */
					case AVV_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CAVV , 3);
						break;

					case CRPT_POSITION :
						PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_CRYPTO , 3);
						break;
					case PIN_POSITION :
						memset(sPinNbr, 	0, sizeof(sPinNbr));
						memset(sPinMaxNbr, 	0, sizeof(sPinMaxNbr));
						if ( GetTlvAutho (  AUTO_PIN_NBR  , sTlvInfo, sPinNbr , &nLength ) == SUCCES )
							if ( GetTlvAutho (  AUTO_PIN_RETRY_MAX  , sTlvInfo, sPinMaxNbr , &nLength ) == SUCCES )
							{
								sprintf(sLine," Pin Error %s, Max Retry %s", sPinNbr, sPinMaxNbr);
								trace_event(sLine,TRACE);
								if (atoi(sPinNbr) + 1 >= atoi(sPinMaxNbr) )
								{
									PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , PIN_NB_EXCEEDED  , 3);
									return;
								}
							}
							PutTlvAutho (AUTO_EVENT_CODE, sTlvInfo , ERROR_PIN  , 3);
							break;
					}  
				}  
}
/*================================================================================*/
/*================================================================================*/
int HsmDecline ( TSTlvAutho *sTlvInfo , char *sSecurityResults , 
	char *sHsmResultCode , char position )
{
	char    		sTlvPrivateBuffer [ P7_F048_MAX_LEN + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	char   sLine [ MAX_LINE_TRC ];
	int             nLength;

	InitTlvPrivate(&sTlvPrivateInfo);

	PutTlvAutho ( AUTO_SECURITY_VER_RESULTS  , sTlvInfo,sSecurityResults, 32 ); /*** MYJ231007 : Mehdi Elyajizi 16092006 32 instead of 20*****/
	GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , sTlvInfo , sTlvPrivateBuffer ,&nLength);
	AnalyseTlvPrivate(sTlvPrivateBuffer,nLength,&sTlvPrivateInfo );

	sprintf ( sLine , "HsmResultCode = %s ", sHsmResultCode );
	trace_event ( sLine , PROCESSING );
	PutTlvPrivate(HSM_RESULT_CODE_TAG  ,&sTlvPrivateInfo,sHsmResultCode,4);

	if ( position == CVC1_POSITION ) /* Resultat de la verification du CVC1 */
		PutTlvPrivate(CVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + CVC1_POSITION ,1);
	else if ( position == CVC2_POSITION ) /* Resultat de la verification du CVC2 */
		PutTlvPrivate(CVV2_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + CVC2_POSITION ,1);
	/*START MJA21092011  CONTACTLESS */
	else if ( position == CVC3_POSITION ) /* Resultat de la verification du CVC3 */
		PutTlvPrivate(CVV2_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + CVC3_POSITION ,1);
	/*END MJA21092011  CONTACTLESS */

	/*  Start Mehdi Elyajizi  20092006 : MYJ231007*/

	if ( position == ICVV_POSITION ) /* Resultat de la verification du ICVV */
		PutTlvPrivate(ICVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + ICVV_POSITION ,1);
	/* ELKAM20221221 PLUTONL-5555 START */
	/* if ( position == AUC_POSITION ) /* Resultat de la verification du AUC * /
		PutTlvPrivate(AUC_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + AUC_POSITION ,1);/* Mehdi Elyajizi Not Reliable since the information is contained on DB service CODE * /
	if ( position == DAC_POSITION ) /* Resultat de la verification du DAC * //* Mehdi Elyajizi veification DAC ignor� fonctionnalit� MasterCard et DAC non envoy� syst�matiquement * /
		PutTlvPrivate(DAC_RESULT_CODE_TAG ,&sTlvPrivateInfo,sSecurityResults + DAC_POSITION ,1); /* Mehdi Elyajizi veification DAC ignor� fonctionnalit� MasterCard et DAC non envoy� syst�matiquement * /
	if ( position == DYNAMIC_NBR_POSITION )  /* Resultat de la verification du IDN * /
		PutTlvPrivate( DYNAMIC_NBR_RESULT_CODE_TAG,&sTlvPrivateInfo,sSecurityResults + DYNAMIC_NBR_POSITION ,1);/* Mehdi Elyajizi veification IDN ignor� fonctionnalit� MasterCard et IDN non envoy� syst�matiquement * / */  
 	/* ELKAM20221221 PLUTONL-5555 END */
	/*  End Mehdi Elyajizi 20092006 : MYJ231007*/

	nLength = PrivateBuildTlv( sTlvPrivateBuffer , &sTlvPrivateInfo);
	PutTlvAutho ( AUTO_ADD_DATA_PRIVATE,sTlvInfo,sTlvPrivateBuffer,nLength);
	return ( OK );
}

/* BEG YEL05022016 */
int GetQFromTrack2Data ( char *sTrack2Data ,
					char *sQ)
{
   int i,len;
   len = strlen ( sTrack2Data );
   if( len != 0 )
    {
       for(i=0;(isdigit(sTrack2Data[i]) != 0); i++ );
       memcpy ( sQ           , sTrack2Data + i + 1 + 7 + 5 , 1 );
     }
	return(OK);
}
/* END YEL05022016 */

/*================================================================================*/
/*================================================================================*/
int CheckSecurity ( int nIndiceCtx, char *sTlvBuffer)
{

	TSIsoInfo       isoInfo;
	TSTlvAutho      sTlvInfo;
	char    		sTlvPrivateBuffer [ P7_F048_MAX_LEN + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	char    		sSecurityData       [ LG_MAX + 1 ];
	char    		sIccDataTLV         [ LG_MAX + 1 ];
	char    		sIccDataISO         [ LG_MAX + 1 ];
	int     		nIsoICCLen;
	int     		nLength;
	int					nLen;       /***Mehdi Elyajizi 16092006 : MYJ231007**/    
	int     		sPinLength;

	char cvv1_flag = 'Y';
	char cvv2_flag = 'Y';
	char pin_flag  = 'Y';
	char cvc3_flag = 'Y';   /* MJA21092011  CONTACTLESS */ 
	char pin_method;
	char encryption_mode  ;
	char pin_method_flag = PVV_VERIFICATION_METHOD ;
	char crpt_flag = 'Y';

	/*****Start Mehdi Elyajizi 15 09 2006 : MYJ231007 ******/
	char icvv_flag = 'Y';
	char app_usage_ctrl_flag = 'Y';
	char data_auth_code_flag = 'Y';
	char dynamic_number_flag = 'Y';
	char private_data_1_flag = 'Y';
	char private_data_2_flag = 'Y';    
	char private_data_3_flag = 'Y';
	char private_data_4_flag = 'Y';
	char private_data_5_flag = 'Y';
	/*****End Mehdi Elyajizi 15 09 2006  : MYJ231007*******/

	char cvv1_result = '4';
	char cvv2_result = '4';
	char pin_result  = '4';
	char crpt_result = '4';
	char cvc3_result = '4';   /* MJA21092011  CONTACTLESS */   
	/*****start Mehdi Elyajizi 15 09 2006  : MYJ231007**********/
	char icvv_result = '4';          
	char app_usage_ctrl_result = '4';
	char data_auth_code_result = '4';
	char dynamic_number_result = '4';
	char private_data_1_result = '4';
	char private_data_2_result = '4';
	char private_data_3_result = '4';
	char private_data_4_result = '4';
	char private_data_5_result = '4';
	/*****End Mehdi Elyajizi 15 09 2006  : MYJ231007*******/    

	char   sTrack1Data 	[ 77 + 1];
	char   sTrack2Data 	[ 40 + 1];
	char   sTrack2Data2 [ 40 + 1];
	char   sTrackInfo   [ 77 + 1]; /* MJA21092011  CONTACTLESS  */
	char   sPan 	    [ 22 + 1];
	char   sPinBlock    [ 16 + 1];
	char   sExpiryDate  [  4 + 1];
	char   sExpirySwapped  [  4 + 1];
	char   sServiceCode [  3 + 1];
	char   pvki;
	char   sPvv         [  5 + 1]; 
	char   sCvv1        [  3 + 1];
	char   sCvv2        [  3 + 1];
	/***Start Mehdi Elyajizi 15 09 2006  : MYJ231007*******/
	char   siCvv        [  3 + 1];
	char	 sICC_DATA		[ 512 + 1 ];
	TSTlvPrivate tlvInfo;
	char   tmpPan 	    [ 22 + 1];
	char   tmpExpiryDate  [  4 + 1];
	char   tmpServiceCode [  3 + 1];
	char   tmpPvv         [  5 + 1];
	char   sTvr		  [ 10 + 1 ];
	char   sAtc	          [ 2 + 1 ];
	char   sAip					[ 4 + 1 ];
	char   szWork				[LG_MAX];
	int			result_tvr_cvr;
	/***End Mehdi Elyajizi   15 09 2006 : MYJ231007**/    

	char   sSecurityFlags  [ 32 + 1 ];/*** MYJ231007: Mehdi Elyajizi   15 09 2006 ******/  
	char   sSecurityResults[ 32 + 1 ];/*** MYJ231007: Mehdi Elyajizi   15 09 2006 ******/  
	char   sHsmResultCode  [ 4 + 1 ];
	char   sMsgErr         [ 17 + 1 ];
	char   sOffset         [ 5 + 1 ];

	char   sCVKA  [ SIZE_KEY_TDES + 1];
	char   sCVKB  [ SIZE_KEY_TDES + 1];
	char   sPVKA  [ SIZE_KEY_TDES + 1];
	char   sPVKB  [ SIZE_KEY_TDES + 1];
	char   tvki;       /* holds PVKI read from database,   if available */
	char   sTransKey  [ SIZE_KEY_TDES + 1];
	char   sPIN_KEY   [ SIZE_KEY_TDES + 1];
	char   sSecurityFormat  [ 2 + 1];
	char   sPinBlockFormat  [ 2 + 1];
	char   sFunctionCode    [ 3 + 1];
	char   sTrack3          [ 107 + 1];
	char   sLine [ MAX_LINE_TRC ];
	int    result;
	int    nLenExpDate = 0;

	/* 0002YO */
	char   szPinOff[6];
	/* 0002YO */

	/* 0003AZ */
	char   tPvkiPVV     [  5 + 1];  /* holds PVKi + PVV read from database, if available */
	char   tOffset  [ 12 + 1];  /* holds Offset read from database, if available */
	int    tPinLength;          /* PIN length */
	int    result_pin_info;
	/* 0003AZ */

	/*Start SWI01122008 Enh VISA iCVV */
	char   sPOS_DATA	[ 12 + 1];
	char   sNetworkCode[2+1]; 
	char   sAddDataPriv [ 256 ];
	/*End SWI01122008 Enh VISA iCVV */

	/* Mehdi Elyajizi PCI */
	int   Pvv;
	int   VisaOfset = 0;
	int   nPvki;
	char  sPvki[4]; /*EBE130114*/
	/* Mehdi Elyajizi PCI */
	/*START MJA21092011  CONTACTLESS */
	char   sCvc3        [ 5 + 1];
	char   sUn          [ 10 + 1];
	char   sAtcCvc3     [ 5 + 1];
	char   sPanSeqNo    [ 3 + 1];
	char   sPCvc3Track1 [12 + 1];
	char   sPUnAtcTrack1 [12 + 1];
	char   sNAtcTrack1   [2 + 1];
	int    nNAtcTrack1  ;
	int    offset = 0;
	char   sPCvc3Track2 [4 + 1];
	char   sPUnAtcTrack2 [4 + 1];
	char   sNAtcTrack2   [2 + 1];
	int    nNAtcTrack2 ;
	char   sDiscretionaryData1 [24 + 1];
	char   sDiscretionaryData2 [13 + 1];
	char   sBitmapCvc3Track1 [48 + 1];
	char   sBitmapCvc3Track2 [16 + 1];
	char   sBitmapUnAtcTrack1 [48 + 1];
	char   sBitmapUnAtcTrack2 [16 + 1];
	char   sTemp [1 + 1];
	char   track_flag [ 1 + 1];
	char   sAppControlFlag [1 + 1];
	char   sKcvkc [ 2*SIZE_KEY + 1]; 
	char            sControlFlag[1 + 1];
	char            sIccApplicationIndex[4 + 1];
	char            sIssuingBankCode[6 + 1];
	char            sApplicationControl[24 + 1];
	char			sSeqNum[8];
	/* END  MJA21092011  CONTACTLESS */

	/*START 3D-SECURE MCK 19072012*/
	char avv_flag 		= 'Y';
	char avv_result 	= '4';
	char aav_field    [256];
	int  check_aav_flg = 0;
        char  sCaptureCode   [  6 + 1];
	char sEffectiveDate  [4 + 1];
	
	/* YEL05022016 BEG */
	int 	n_pin_result;
	char   sKV  [ SIZE_KEY_TDES + 1];
	char   sPvvQ[6+1];
	/* YEL05022016 END */


	/************************************************************************/
	/* INITIALISATION DES VARIABLES                                         */
	/************************************************************************/
	trace_event("Start CheckSecurity()",PROCESSING);

	/* BEG YEL05022016 */
	memset ( sKV  , 0, sizeof ( sKV ));
	memset ( sPvvQ   , 0, sizeof ( sPvvQ ));
	/* END YEL05022016 */
	
	memset ( sPvki, 	    0, sizeof ( sPvki ));
	memset ( sTrack2Data2, 	0, sizeof ( sTrack2Data2 ));
	memset ( sEffectiveDate,0, sizeof ( sEffectiveDate ));
	memset ( sCaptureCode, 	0, sizeof ( sCaptureCode ));
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

	/****start Mehdi Elyajizi 15092006:MYJ231007************/
	memset ( siCvv,  		0, sizeof ( siCvv));
	memset ( sICC_DATA, 	0, sizeof ( sICC_DATA ));
	memset ( tmpPan,  		0, sizeof ( tmpPan ));
	memset ( tmpExpiryDate,  	0, sizeof ( tmpExpiryDate));
	memset ( tmpServiceCode,  0, sizeof ( tmpServiceCode));
	memset ( tmpPvv,  		0, sizeof ( tmpPvv ));
	memset ( sAtc,  		0, sizeof ( sAtc));
	memset ( sTvr,  		0, sizeof ( sTvr));
	memset ( sAip,  		0, sizeof ( sAip));
	memset(  szWork       , 0, sizeof(szWork));
	/****end Mehdi Elyajizi 15092006:MYJ231007*************/    

	memset ( sSecurityFlags   , 0,  sizeof (sSecurityFlags));
	memset ( sSecurityResults , 0,  sizeof ( sSecurityResults));
	memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
	memset ( sMsgErr          , 0, sizeof ( sMsgErr ));
	memset ( sOffset          , 0, sizeof ( sOffset ));
	memset ( sSecurityFormat  , 0, sizeof ( sSecurityFormat ));
	memset ( sFunctionCode    , 0, sizeof ( sFunctionCode ));
	memset ( sTrack3    	  , 0, sizeof ( sTrack3 ));

	memset ( sCVKA  , 0, sizeof ( sCVKA ));
	memset ( sCVKB  , 0, sizeof ( sCVKB ));
	memset ( sPVKA  , 0, sizeof ( sPVKA ));
	memset ( sPVKB  , 0, sizeof ( sPVKB ));
	memset ( sTransKey  , 0, sizeof ( sTransKey ));
	memset ( sPIN_KEY   , 0, sizeof ( sPIN_KEY )); 
	memset ( sPOS_DATA  , 0, sizeof ( sPOS_DATA ));  /* Mehdi Elyajizi 15092006 :MYJ231007 */
	/* 0002YO */
	memset ( szPinOff   , 0, sizeof ( szPinOff ));
	/* 0002YO */

	/* 0003AZ */
	memset ( tPvkiPVV,  		0, sizeof ( tPvkiPVV ));
	memset ( tOffset,  		0, sizeof ( tOffset ));
	/* 0003AZ */
	/*Start SWI01122008 Enh Visa iCVV*/
	memset ( siCvv,             0, sizeof ( siCvv)); 
	memset ( sPOS_DATA   , 0, sizeof ( sPOS_DATA ));
	memset ( sICC_DATA   , 0, sizeof ( sICC_DATA ));
	memset ( tmpPan   , 0, sizeof ( tmpPan ));
	memset ( sNetworkCode   , 0, sizeof ( sNetworkCode ));
	memset ( tmpExpiryDate   , 0, sizeof ( tmpExpiryDate ));
	memset ( tmpServiceCode   , 0, sizeof ( tmpServiceCode ));
	memset ( tmpPvv   , 0, sizeof ( tmpPvv ));
	/*End SWI01122008 Enh Visa iCVV*/

	/*START MJA21092011  CONTACTLESS */

	memset ( sPCvc3Track1,              0, sizeof ( sPCvc3Track1));
	memset ( sPUnAtcTrack1,             0, sizeof ( sPUnAtcTrack1));
	memset ( sNAtcTrack1,               0, sizeof ( sNAtcTrack1));
	memset ( sPCvc3Track2,              0, sizeof ( sPCvc3Track2));
	memset ( sPUnAtcTrack2,             0, sizeof ( sPUnAtcTrack2));
	memset ( sNAtcTrack2,               0, sizeof ( sNAtcTrack2));
	memset ( sDiscretionaryData1,               0, sizeof ( sDiscretionaryData1));
	memset ( sDiscretionaryData2,               0, sizeof ( sDiscretionaryData2));
	memset ( sBitmapCvc3Track1,                 0, sizeof ( sBitmapCvc3Track1));
	memset ( sBitmapCvc3Track2,                 0, sizeof ( sBitmapCvc3Track2));
	memset ( sBitmapUnAtcTrack1,                0, sizeof ( sBitmapUnAtcTrack1));
	memset ( sBitmapUnAtcTrack2,                0, sizeof ( sBitmapUnAtcTrack2));
	memset ( sTemp,             0, sizeof ( sTemp));
	memset ( track_flag,                0, sizeof ( track_flag));
	memset ( sAppControlFlag,           0, sizeof ( sAppControlFlag));
	memset ( sKcvkc  , 0, sizeof ( sKcvkc ));
	memset ( sControlFlag, 0, sizeof(sControlFlag));
	memset ( sIccApplicationIndex, 0, sizeof(sIccApplicationIndex));
	memset ( sIssuingBankCode, 0, sizeof(sIssuingBankCode));
	memset ( sApplicationControl, 0, sizeof(sApplicationControl));
	memset ( sCvc3, 0, sizeof(sCvc3));
	memset ( sSeqNum, 0, sizeof(sSeqNum));
	
	/* END  MJA21092011  CONTACTLESS */

	memset ( aav_field, 0, sizeof(aav_field));

	InitTlvAutho	(&sTlvInfo);
	AnalyseTlvAutho (sTlvBuffer , &sTlvInfo);

	memset ( sTlvPrivateBuffer   , 0, sizeof(sTlvPrivateBuffer) );

	/*****************************************************************************/
	/*         extracting the SECURITY_VERIF_FLAG Tag'Value                      */
	/*         If  Daes not Exist take the default values                        */
	/*         means YES for all verif and PVV_METHOD for PIN_VERIF_METHOD       */
	/*****************************************************************************/
	if ( GetTlvAutho (  AUTO_SECURITY_VERIF_FLAGS  , &sTlvInfo, sSecurityFlags   		   , &nLength ) != SUCCES )
	{
		sprintf(sLine,"Security Flags Empty.");
		trace_event(sLine,PROCESSING);
		trace_event("End   CheckSecurity(MALFUNCTION)",PROCESSING);
		return ( SYSTEM_MALFUNCTION );
	}
	else
	{

		trace_event("SECURITY FLAGS RECEIVED",TRACE);
		sprintf(sLine,"________ _____ ________");
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"CVV1 FLAG : %c", sSecurityFlags[0] );
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"CVC2 FLAG : %c", sSecurityFlags[1] );
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"PIN  FLAG : %c", sSecurityFlags[2] );
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"PIN  METD : %c", sSecurityFlags[3] );
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"CRPT FLAG : %c", sSecurityFlags[4] );
		trace_event(sLine,PROCESSING);
		sprintf(sLine,"ICVV FLAG : %c", sSecurityFlags[18] ); /*SWI01122008 Enh Visa iCVV*/
		trace_event(sLine,PROCESSING);
		/*START MJA21092011  CONTACTLESS */
		sprintf(sLine,"CVC3 FLAG : %c", sSecurityFlags[CVC3_POSITION]); 
		trace_event(sLine,PROCESSING);
		/*END MJA21092011  CONTACTLESS */

		sprintf(sLine,"AVV  FLAG : %c", sSecurityFlags[AVV_POSITION]); 
		trace_event(sLine,PROCESSING);

		cvv1_flag 	= sSecurityFlags[0];
		cvv2_flag 	= sSecurityFlags[1];
		pin_flag  	= sSecurityFlags[2];
		pin_method  	= sSecurityFlags[3];
		crpt_flag  	= sSecurityFlags[4];
		icvv_flag        = sSecurityFlags[18];   /* SWI01122008 Enh Visa ICVV*/
		cvc3_flag        = sSecurityFlags[CVC3_POSITION]; /* MJA21092011  CONTACTLESS */
		app_usage_ctrl_flag = sSecurityFlags[19];
		data_auth_code_flag = sSecurityFlags[20];
		dynamic_number_flag = sSecurityFlags[21];      /*** Mehdi Elyajizi 15092006:MYJ231007******/ 
		avv_flag   = sSecurityFlags[AVV_POSITION]; /* 3D-SECURE MCK 19072012*/
	}

	if ( GetTlvAutho (  AUTO_SECURITY_VER_RESULTS , &sTlvInfo,
		sSecurityResults , &nLength ) != SUCCES )
	{
		sprintf(sLine,"Security Results Empty.");
		trace_event(sLine,PROCESSING);
		trace_event("End   CheckSecurity(MALFUNCTION)",PROCESSING);
		return ( SYSTEM_MALFUNCTION );
	}
	/*****************************************************************************/
	/* Read Pos Data                                                           */
	/*****************************************************************************/
	GetTlvAutho ( AUTO_POS_DATA, &sTlvInfo, sPOS_DATA , &nLength); 
	GetTlvAutho ( AUTO_NETWORK_CODE , &sTlvInfo, sNetworkCode,  &nLength );
	GetTlvAutho ( AUTO_CAPTURE_CODE , &sTlvInfo, sCaptureCode , &nLength );
	
	/******************************************************************************************/
	/*---------Recuperation du ICVV du TRACK2 Equivalent from ICC Mehdi Elyajizi 16092006:MYJ231007----------*******/
	/******************************************************************************************/
	/*if  ( icvv_flag == 'Y' )*/ /*EBE121126*/
	{
		if(GetTlvAutho (AUTO_ICC_DATA, &sTlvInfo, sICC_DATA , &nLength)==SUCCES)
		{
			memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
			InitTlvPrivate (&tlvInfo);
			AnalyseTlvPrivate(sICC_DATA ,nLength,&tlvInfo);
			if (GetTlvPrivate (CHIP_T2_EQUIVALENT_DATA , &tlvInfo, sTrack2Data, &nLen) == SUCCES)
			{
				sprintf(sLine,"iCVV sTrack2Data %d",nLen);
				trace_event(sLine,TRACE);
				if (memcmp( sNetworkCode, NET_DINERS, 2) == 0
					&& memcmp( sCaptureCode ,"55", 2) != 0) 
					GetDinersTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,sEffectiveDate,sPvv,siCvv, sTrack2Data2);
				else	
					GetTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,sPvv,siCvv); 

			}
			else
			{
				trace_event("Erreur Recupition TRACK2 EQUIVALENT FROM ICC, Field 35 will be used",PROCESSING);
				memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
				if(GetTlvAutho(AUTO_TRACK2_DATA,&sTlvInfo,sTrack2Data,&nLength)== SUCCES)
				{
					if (memcmp( sNetworkCode, NET_DINERS, 2) == 0
					&& memcmp( sCaptureCode ,"55", 2) != 0)
						GetDinersTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,sEffectiveDate,tmpPvv,siCvv, sTrack2Data2);
					else	
						GetTrack2Data(sTrack2Data,tmpPan,tmpExpiryDate,tmpServiceCode,tmpPvv,siCvv);
				}
			}

		}
	}

	/*****************************************************************************/
	/* Read TRACK Data                                                           */
	/*****************************************************************************/
	if(GetTlvAutho(AUTO_TRACK2_DATA,&sTlvInfo,sTrack2Data,&nLength)== SUCCES)
	{ 
		if (memcmp( sNetworkCode, NET_DINERS, 2) == 0
					&& memcmp( sCaptureCode ,"55", 2) != 0)
			GetDinersTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sEffectiveDate,sPvv,sCvv1, sTrack2Data2);
		else
			GetTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
		/* START MJA21092011  CONTACTLESS */
		offset = strlen(sTrack2Data) - 13 ;
		memcpy (sDiscretionaryData2, sTrack2Data + offset, 13);
		offset = strlen(sPan);
		memcpy (sTrackInfo, sTrack2Data + offset + 1, 7);
		/* END MJA21092011  CONTACTLESS */
		memcpy (track_flag, "2", 1);/* MJA21092011  CONTACTLESS */

	}
	else if(GetTlvAutho(AUTO_TRACK1_DATA,&sTlvInfo,sTrack1Data,&nLength) == SUCCES)
	{
		if (memcmp( sNetworkCode, NET_DINERS, 2) == 0)
			GetDinersTrack1Data(sTrack1Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
		else
			GetTrack1Data(sTrack1Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
			
		/* START MJA21092011  CONTACTLESS */
		offset = strlen(sTrack1Data) - 24 ;
		memcpy (sDiscretionaryData1, sTrack1Data + offset, 24); /* R�cup�rer Discretionary Data*/
		offset = strlen(sTrack1Data) - 31 ;
		memcpy (sTrackInfo, sTrack1Data + offset , 7);
		/* END MJA21092011  CONTACTLESS */
		memcpy (track_flag, "1", 1);/* MJA21092011  CONTACTLESS */

	}
	else
	{
		GetTlvAutho(AUTO_PAN,&sTlvInfo,sPan,&nLength);
		GetTlvAutho(AUTO_EXPIRY_DATE,&sTlvInfo,sExpiryDate,&nLenExpDate); 
	}

	if( strlen(sExpiryDate) )
		nLenExpDate = strlen(sExpiryDate) ;

	if(GetTlvAutho (AUTO_ICC_DATA, &sTlvInfo, sICC_DATA , &nLength)!=SUCCES)
	{
		if ( memcmp(track_flag,"2",1) == 0 )
			memcpy(sCvc3,sDiscretionaryData2 + 5 , 3);
		else if ( memcmp(track_flag,"1",1) == 0 )
			memcpy(sCvc3,sDiscretionaryData1 + 15 , 3);
	}
	/*End SWI01122008 Enh Visa iCVV*/

	/*---------------------------------------------------------------------------*/
	/*        CVV1 VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	sSecurityResults [0]  = cvv1_result;
	sSecurityResults [1]  = cvv2_result;
	sSecurityResults [2]  = pin_result;
	sSecurityResults [4]  = crpt_result;
	/* Mehdi Elyajizi 18092006 :MYJ231007*/
	sSecurityResults [18] = icvv_result;
	sSecurityResults [19] = app_usage_ctrl_result;
	sSecurityResults [20] = data_auth_code_result;
	sSecurityResults [21] = dynamic_number_result;

	sSecurityResults [CVC3_POSITION] = cvc3_result;
	sSecurityResults [AVV_POSITION] = avv_result;
	/* Mehdi Elyajizi 18092006 :MYJ231007*/  
	PutTlvAutho(AUTO_SECURITY_VER_RESULTS,&sTlvInfo, sSecurityResults, 32 );/* Mehdi Elyajizi 18092006:MYJ231007 */
	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);



	/******************************************************************************************/
	/*-------R�cuperation du ATC Application Transaction Counter from ICC Mehdi Elyajizi   MTP EMV CTRL VERIF FLAGS--*******/
	/******************************************************************************************/
	if(GetTlvAutho (AUTO_ICC_DATA, &sTlvInfo, sICC_DATA , &nLength)==SUCCES) {
		InitTlvPrivate (&tlvInfo);
		AnalyseTlvPrivate(sICC_DATA ,nLength,&tlvInfo);	
		memset(szWork, 0, sizeof(szWork));
		if (GetTlvPrivate (CHIP_ATC_TAG , &tlvInfo, szWork, &nLen) == SUCCES) /**Recup�ration du ATC**/ 
		{
			/******Traitement necessaire pour verification du ATC***************/
			sprintf(sLine, " Application Transaction Counter :[%s]", szWork);
			trace_event(sLine, PROCESSING);
			if(GetTlvAutho(AUTO_CARD_SEQUENCE_NUMBER,&sTlvInfo,sSeqNum,&nLength) != SUCCES)
				memcpy(sSeqNum,"000",3);
			sprintf(sLine, " Sequence number :[%s]", sSeqNum);
			trace_event(sLine, PROCESSING);
			result = CheckATC(nIndiceCtx,sPan,sSeqNum,sExpiryDate,szWork);/*Ajout de sExpiryDate  pour traitement des cartes renouvl�es*/
			if ( result != OK )
			{
				memcpy(sHsmResultCode, "-ATC", 4);
				FillSecurityResult ( VERIFICATION_FAILED  , sSecurityResults , CRPT_POSITION );
				PutHsmRjectEvent ( &sTlvInfo , VERIFICATION_FAILED , CRPT_POSITION );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CRPT_POSITION );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckSecurity(NOK ATC)",PROCESSING);
				return ( NOK );
			}
		}
		else
			trace_event("Erreur Recup�ration CHIP_ATC_TAG EQUIVALENT FROM ICC",PROCESSING);
	}

	/*Start SWI01122008 Enh Visa iCVV*/
	/******************************************************************************************/
	/* 	     Add iCVV/CVV1 verification process for VISA cards only	                        ***/
	/******************************************************************************************/


	if (memcmp(sNetworkCode,"01",2) == 0 || memcmp( sNetworkCode, NET_DINERS, 2) ==0 )   /*checking for Visa & Diners */
	{
		sprintf(sLine,"=> CVV1 Flag : %c", cvv1_flag );
		trace_event(sLine,PROCESSING);
		if ( ( cvv1_flag == 'Y' ) && ( strlen ( sCvv1 ) != 0 )  && (strlen(siCvv) == 0) )
		{

			result = GetCVK ( nIndiceCtx , sPan , sCVKA , sCVKB );
			if ( result != OK ) 
			{
				cvv1_result = HSM_MALFUNCTION;
				memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
				memcpy ( sHsmResultCode , "-CVK" , 4 );
				memcpy ( sMsgErr,"INVALID KEYs CVK",16);
			}
			else
			{
				cvv1_result = check_cvv_data (	nIndiceCtx ,
					sPan , 
					sCVKA, sCVKB,
					sExpiryDate, sServiceCode, sCvv1,
					sHsmResultCode );
			}

			if ( cvv1_result != OK ) 
			{
				FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
				PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

				if(memcmp(sHsmResultCode,"-CVK", 4) != 0)
					TextError(sHsmResultCode + 2,sMsgErr);

				InitTlvPrivate(&sTlvPrivateInfo );
				memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
				GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv , &nLength);
				AnalyseTlvPrivate(sAddDataPriv,nLength,&sTlvPrivateInfo );
				PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsgErr,strlen(sMsgErr));
				memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
				nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
				PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sAddDataPriv ,  nLength);
				PrintTlvBuffer(sTlvBuffer);
				return ( NOK );
			}
		}
		/*---------------------------------------------------------------------------*/
		/*        ICVV VERIFICATION                                                  */
		/*---------------------------------------------------------------------------*/
		if( debug_option == 1 )
		{
			sprintf(sLine,"=> iCVV Flag : %c", icvv_flag );
			trace_event(sLine,PROCESSING);
		}
		if ( ( icvv_flag == 'Y' ) && ( (strlen ( siCvv ) != 0) || (sPOS_DATA[6] == '8') ) ) /* 8 : Fallback*/
		{

			result = GetCVK ( nIndiceCtx , sPan , sCVKA , sCVKB );
			if ( result != OK )
			{
				icvv_result = HSM_MALFUNCTION;
				memcpy ( sHsmResultCode , "-CVK" , 4 );
			}
			else
			{
				if (sPOS_DATA[6] == '8' )
				{
					if ( cvv1_result != OK) 
					{
						trace_event("Inside FallBack checking",TRACE);
						cvv1_result = check_cvv_data (  nIndiceCtx ,
							sPan ,
							sCVKA, sCVKB,
							sExpiryDate, sServiceCode, sCvv1,
							sHsmResultCode );
						if ( cvv1_result != OK )
						{
							FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
							PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
							HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );

							AuthoBuildTlv(sTlvBuffer, &sTlvInfo); 
							trace_event("End   CheckSecurity(NOK ICVV / CVV1)",PROCESSING);
							return ( NOK );
						}
					}
				}
				else
				{
					icvv_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, sExpiryDate, "999", siCvv, sHsmResultCode );
					if ( icvv_result != OK )
					{
						cvv1_result = check_cvv_data (  nIndiceCtx ,
							sPan ,
							sCVKA, sCVKB,
							sExpiryDate, sServiceCode, siCvv,
							sHsmResultCode );
						if ( cvv1_result != OK )
						{
							FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
							PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
							HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );

							FillSecurityResult ( icvv_result , sSecurityResults , ICVV_POSITION );
							PutHsmRjectEvent ( &sTlvInfo , icvv_result , ICVV_POSITION );
							HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , ICVV_POSITION );

							AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
							trace_event("End   CheckSecurity(NOK ICVV / CVV1)",PROCESSING);
							return ( NOK );
						}
					}
				}
			}
			FillSecurityResult ( icvv_result , sSecurityResults , ICVV_POSITION);
			FillSecurityResult ( cvv1_result , sSecurityResults ,  CVC1_POSITION);
		}
	}
	else
	{
		/*End SWI01122008 Enh Visa iCVV*/
		if ( ( cvv1_flag == 'Y' ) && ( strlen ( sCvv1 ) != 0 ) )
		{		
			result = GetCVK ( nIndiceCtx , sPan , sCVKA , sCVKB );
			if ( result != OK )
			{
				cvv1_result = HSM_MALFUNCTION;
				memcpy ( sHsmResultCode , "-CVK" , 4 );
				FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
				PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckSecurity(NOK CVV1)",PROCESSING);
				return ( NOK );
			}
			else 
			{
				sprintf(sLine,"sPOS_DATA 1 %s",sPOS_DATA);
				trace_event(sLine,TRACE);
				if( (sServiceCode[0] == '2') || (sServiceCode[0] == '6'))
				{
					trace_event(" SERVICE CODE TECHNOLOGY ICC", PROCESSING);
					if (GetTlvAutho (AUTO_ICC_DATA, &sTlvInfo, sICC_DATA , &nLength)!=SUCCES)
					{
						trace_event("Start   Checking CVV IN FALLBACK CONDITION",PROCESSING);
						icvv_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, 
							sExpiryDate, "999", sCvv1, sHsmResultCode );
						if ( icvv_result != OK ) 
						{
							cvv1_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, 
								sExpiryDate, sServiceCode, sCvv1, sHsmResultCode );
							if ( cvv1_result != OK )
							{
								FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
								PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
								HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );

								FillSecurityResult ( icvv_result , sSecurityResults , ICVV_POSITION );
								PutHsmRjectEvent ( &sTlvInfo , icvv_result , ICVV_POSITION );
								HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , ICVV_POSITION );

								AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
								trace_event("End   CheckSecurity(NOK ICVV / CVV1)",PROCESSING);
								return ( NOK );
							}
						}
						FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION);
						FillSecurityResult ( icvv_result , sSecurityResults , ICVV_POSITION);
					}
					/* Dans le cas ou le fallback est fait mais le tracck II est celui de piste on fait le check du CVV*/
					if (sPOS_DATA[6] == '8')
					{
						cvv1_result = check_cvv_data (  nIndiceCtx , sPan , sCVKA, sCVKB, 
							sExpiryDate, sServiceCode, sCvv1, sHsmResultCode );
						if ( cvv1_result != OK )
						{
							FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
							PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
							HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );
							AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
							trace_event("End   CheckSecurity(NOK CVV1)",PROCESSING);
							return ( NOK );
						}
						FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION);
					}
				}
				else 
				{
					cvv1_result = check_cvv_data (  nIndiceCtx ,
						sPan ,
						sCVKA, sCVKB,
						sExpiryDate, sServiceCode, sCvv1,
						sHsmResultCode );
					if ( cvv1_result != OK )
					{
						FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION );
						PutHsmRjectEvent ( &sTlvInfo , cvv1_result , CVC1_POSITION );
						HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC1_POSITION );
						AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
						trace_event("End   CheckSecurity(NOK CVV1)",PROCESSING);
						return ( NOK );
					}
					FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION);
				}
			}
		}		
	}/*SWI01122008 Enh Visa iCVV*/
	/* Mehdi Elyajizi MTP FALLBACK251007 */


	/*---------------------------------------------------------------------------*/
	/*        CVC2 VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	if ( GetTlvAutho( AUTO_ADD_DATA_PRIVATE , &sTlvInfo, sTlvPrivateBuffer 	   , &nLength ) == SUCCES )
	{
		AnalyseTlvPrivate(sTlvPrivateBuffer   ,nLength,&sTlvPrivateInfo);
		GetTlvPrivate	(CVC2_DATA_TAG	,&sTlvPrivateInfo , sCvv2 ,&nLength);
	}

	if ( ( cvv2_flag == 'Y' ) && ( strlen ( sCvv2 ) != 0 ) && ( memcmp ( sCvv2 , "XXX" , 3 ) != 0 ) && (nLenExpDate != 0))
	{
		
		if (debug_option == 1)
		{
			sprintf(sLine,"=> CVV2 : [%s]", sCvv2 );
			trace_event(sLine,PROCESSING);
		}

		result = GetCVK ( nIndiceCtx , sPan , sCVKA , sCVKB );
		if ( result != OK ) 
		{
			cvv2_result = HSM_MALFUNCTION;
			memcpy ( sHsmResultCode , "-CVK" , 4 );
		}
		else
		{
			memcpy ( sExpirySwapped + 0 , sExpiryDate + 2 , 2 );
			memcpy ( sExpirySwapped + 2 , sExpiryDate + 0 , 2 );
			cvv2_result = check_cvv_data (  nIndiceCtx ,
				sPan , 
				sCVKA, 
				sCVKB,
				sExpirySwapped ,
				"000" ,
				sCvv2,
				sHsmResultCode );
		}

		if ( cvv2_result != OK ) 
		{
			FillSecurityResult ( cvv2_result , sSecurityResults , CVC2_POSITION );
			PutHsmRjectEvent ( &sTlvInfo , cvv2_result , CVC2_POSITION );
			HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC2_POSITION );
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			trace_event("End   CheckSecurity(NOK CVV2)",PROCESSING);
			return ( NOK );
		}
		FillSecurityResult ( cvv2_result , sSecurityResults , CVC2_POSITION );
	}
	else
	{
		if ( ( cvv2_flag == 'Y' ) && ( strlen ( sCvv2 ) != 0 ) && ( memcmp ( sCvv2 , "XXX" , 3 ) != 0 ) && (nLenExpDate == 0))
		{
			trace_event("CVC2 PRESENT | EXPIRY DATE NOT PRESENT ",TRACE);
			trace_event("NO CVC2 VERIFICATION                   ",TRACE);
			FillSecurityResult ( 4 , sSecurityResults , CVC2_POSITION );
		}
	}

	/*---------------------------------------------------------------------------*/
	/*        CVC3 VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	/* START MJA21092011  CONTACTLESS */

	/*if ( (memcmp(sPOS_DATA+6,"D",1)==0) || (memcmp(sPOS_DATA+6,"C",1)==0) ) *//* C :Contactless MagStripe,D:Contactless Mchip */
	if ( (memcmp(sPOS_DATA+6,"3",1)==0) || (memcmp(sPOS_DATA+6,"4",1)==0) ) /* EBE140305: looks like above is wrong. C :Contactless MagStripe,D:Contactless Mchip */
		memcpy(sControlFlag , "Y",1);
	else
		memcpy(sControlFlag , "N",1);

	if (  (cvc3_flag == 'Y' ) && (sControlFlag[0] == 'Y') && ( strlen ( sCvc3 ) != 0 ) )
	{


		if (memcmp(sNetworkCode,"01",2) == 0) /*VISA*/
			result = Get_Dcvv_Key ( nIndiceCtx, sPan, sKcvkc );
		else
			result = Get_Cvc3_Key ( nIndiceCtx, sPan, sKcvkc );

		if ( result != OK )
		{
			memcpy ( sHsmResultCode , "-CVK" , 4 );
			return (HSM_MALFUNCTION);
		}

		if( debug_option == 1 )
		{
			sprintf(sLine, "DCVV / CVV3 KEY        : %s", sKcvkc);
			trace_event(sLine, PROCESSING);
		}


		if (memcmp(sNetworkCode,"01",2) == 0)
		{

			memset (sCvc3 , 0,sizeof(sCvc3));
			memset (sAtcCvc3 , '0',sizeof(sAtcCvc3));

			if (memcmp(track_flag,"2",1) == 0) /*TRACK2*/
			{
				offset = strlen(sPan);
				memcpy (sAtcCvc3 + 2 , sTrack2Data + offset+1+4+3+1+4+3 , 4);
				memcpy (sCvc3 , sTrack2Data + offset+1+4+3+1+4 ,3);
			}
			else /*TRACK1*/
			{
				offset = strlen(sTrack1Data) - 11;
				memcpy (sAtcCvc3 + 2 , sTrack1Data + offset , 2);
				memcpy (sAtcCvc3 + 4 , sTrack1Data + offset + 5 , 2);
				memcpy (sCvc3 , sTrack1Data + offset + 2 ,3);
			}

			cvc3_result = check_dcvv_data (  nIndiceCtx ,
				sKcvkc,
				sPan ,
				sExpiryDate,
				sServiceCode,
				sAtcCvc3,
				sCvc3,
				sHsmResultCode
				);

		}
		else
		{
			cvc3_result = check_cvc3_data (  nIndiceCtx ,
				sKcvkc,
				sPan ,
				"000",
				"",
				"",
				"000000",
				"",
				"",
				strlen(sTrackInfo),
				sTrackInfo,
				"0000000000",
				sCvc3,
				sHsmResultCode
				);
		}

		if ( cvc3_result != OK )
		{
			FillSecurityResult ( cvc3_result , sSecurityResults , CVC3_POSITION );
			PutHsmRjectEvent ( &sTlvInfo , cvc3_result , CVC3_POSITION );
			HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CVC3_POSITION );
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			trace_event("End   CheckSecurity(NOK CVC3)",PROCESSING);
			return ( NOK );
		}

		FillSecurityResult ( cvc3_result , sSecurityResults , CVC3_POSITION );
	}
	else
	{
		trace_event("NO CVC3 VERIFICATION ",TRACE);
		FillSecurityResult ( 4 , sSecurityResults , CVC3_POSITION );
	}
	/* END MJA21092011  CONTACTLESS */

	/* START 3D-SECURE MCK 24072012*/
	/*---------------------------------------------------------------------------*/
	/*        CAVV AVV AEVV  VERIFICATION                                        */
	/*---------------------------------------------------------------------------*/
	InitTlvPrivate(&sTlvPrivateInfo );
	if ( GetTlvAutho( AUTO_ADD_DATA_PRIVATE , &sTlvInfo, sTlvPrivateBuffer , &nLength ) == SUCCES )
	{
		memset(aav_field 		, 0 , sizeof(aav_field		));
		AnalyseTlvPrivate(sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	}

	if (GetTlvPrivate	(AAV_TAG	,&sTlvPrivateInfo , aav_field ,&nLength)== SUCCES )
	{
		sprintf(sLine ,"AAV_TAG == %s",aav_field);	
		trace_event(sLine ,FATAL);
		check_aav_flg = 1;
	}
	else if (GetTlvPrivate	(TRANS_STAIN_TAG	,&sTlvPrivateInfo , aav_field ,&nLength)== SUCCES)
	{
		sprintf(sLine ,"AVV FROM PG == %s",aav_field);	
		trace_event(sLine ,FATAL);
		check_aav_flg = 1;
	}

	if (avv_flag == 'Y' && check_aav_flg == 1 )
	{
		if( memcmp(sNetworkCode,"01",2) == 0 )
		{
			if ((aav_field[0] != '5') && (aav_field[0] != '9'))
			{
				result = check_cavv_data(	nIndiceCtx,
					sPan,
					aav_field,
					sHsmResultCode
					);
				if ( result != OK )
				{
					FillSecurityResult ( result , sSecurityResults , AVV_POSITION );
					PutHsmRjectEvent ( &sTlvInfo , result , AVV_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , AVV_POSITION );
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);          
					if (aav_field[0] != '7')
						PutTlvPrivate(AAV_RESULT ,&sTlvPrivateInfo,"1",1);
					else
						PutTlvPrivate(AAV_RESULT ,&sTlvPrivateInfo,"4",1);
				}
				else
				{
					FillSecurityResult ( result , sSecurityResults , AVV_POSITION );
					if (aav_field[0] != '7')
						PutTlvPrivate(AAV_RESULT ,&sTlvPrivateInfo,"2",1);
					else
						PutTlvPrivate(AAV_RESULT ,&sTlvPrivateInfo,"3",1);
				}	       
			}
			else
				PutTlvPrivate(AAV_RESULT ,&sTlvPrivateInfo,"0",1);

			memset(sAddDataPriv, 0, sizeof(sAddDataPriv));          
			nLength = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
			PutTlvAutho (AUTO_ADD_DATA_PRIVATE, &sTlvInfo , sAddDataPriv, strlen(sAddDataPriv));
		}
		else if( memcmp(sNetworkCode,"02",2) == 0 )
		{

			result = check_aav_data(		nIndiceCtx,
				sPan,
				aav_field,
				sHsmResultCode
				);

			if ( result != OK )
			{
				FillSecurityResult ( result , sSecurityResults , AVV_POSITION );
				PutHsmRjectEvent ( &sTlvInfo , result , AVV_POSITION );
				HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , AVV_POSITION );
				AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
				trace_event("End   CheckSecurity(NOK AAV)",PROCESSING);
				return ( NOK );
			}
		}
	}

	/*---------------------------------------------------------------------------*/
	/*        PIN  VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/

	if( GetTlvAutho ( AUTO_PIN_DATA , &sTlvInfo, sPinBlock, &nLength ) == SUCCES)
	{
		if ( ( pin_flag == 'Y' ) && ( nLength != 0 )/* ( strlen ( sPinBlock ) != 0 ) */)
		{
			GetTlvPrivate   ( ENCR_PIN_BLOCK_KEY_TAG,&sTlvPrivateInfo,sTransKey,&nLength);
			GetTlvAutho     ( AUTO_SECURITY_DATA,&sTlvInfo,sSecurityData,&nLength);
			GetTlvAutho     ( AUTO_FUNCTION_CODE ,&sTlvInfo,sFunctionCode,&nLength);
			GetTlvAutho     ( AUTO_TRACK3_DATA ,&sTlvInfo,sTrack3,&nLength);
			GetSecurityData ( SECURITY_FORMAT,sSecurityData,sSecurityFormat,&nLength);

			if ( memcmp(sSecurityFormat,"01",2) == 0)  /* TPK ENCRYPTION */
				encryption_mode = TERMINAL_MODE;
			else if ( memcmp(sSecurityFormat,"02",2) == 0)  /* ZPK ENCRYPTION */
				encryption_mode = SCHEME_MODE;

			GetSecurityData(PIN_BLOCK_FORMAT,sSecurityData,sPinBlockFormat,&nLength);

			
			result_pin_info = get_pin_info ( nIndiceCtx, sPan, &nPvki,&VisaOfset, tOffset, &tPinLength);/*MCK140408 */

			sprintf (sLine, "PIN Length = %d", tPinLength);
			trace_event (sLine, PROCESSING);

			if (debug_option == 1)
			{
				sprintf (sLine, "sPvv value (%s), sPinBlockFormat(%s)", sPvv,sPinBlockFormat);
				trace_event (sLine, PROCESSING);
			} /** HM - 09022010 - PA_DSS **/

			if ( pin_method ==  PVV_VERIFICATION_METHOD )
			{
				if ( result_pin_info == OK )
				{
					sprintf(sPvki,"%01d",nPvki);            
					pvki = sPvki[0];
					Pvv =  VisaOfset;
					sprintf(sPvv,"%01d%04d",nPvki,Pvv);
				}
				else 
				{
					pvki = sPvv[0];
				}

				result = GetPVK ( nIndiceCtx, sPan  , pvki , sPVKA  , sPVKB );
				if ( result != OK ) 
				{
					pin_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-PVK" , 4 );
				}
				else
				{
					pin_result = check_pvv_data ( 	nIndiceCtx, sPan,
						sPVKA, sPVKB,
						sPinBlock,sPinBlockFormat,sTransKey,
						sPvv , encryption_mode ,
						sHsmResultCode );
				}
				if ( pin_result != OK ) 
				{
					FillSecurityResult ( pin_result  , sSecurityResults , PIN_POSITION);
					PutHsmRjectEvent ( &sTlvInfo , pin_result , PIN_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION);
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
					trace_event("End   CheckSecurity(NOK PIN)",PROCESSING);
					return ( NOK );
				}
			}
			else if ( pin_method ==  PIN_VERIFICATION_METHOD )
			{
				GetTrack3Data(sTrack3, szPinOff);
				if (strlen(sTrack3) == 0 || strlen(szPinOff) == 0)
				{
					pin_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-TCK" , 4 );
					sprintf(sLine, "==> Invalid TrackIII Data"); trace_event(sLine, TRACE);
					/*MKB060309 PCI Norme Integration */
					if (debug_option == 1)
					{
						sprintf(sLine, "= sTrack3       : %.40s", sTrack3); trace_event(sLine, TRACE);
						sprintf(sLine, "= szPinOff      : %.6s", szPinOff); trace_event(sLine, TRACE);
					}
					sprintf(sLine, "==> Invalid TrackIII Data"); trace_event(sLine, TRACE);

					FillSecurityResult ( pin_result  , sSecurityResults , TRACKIII_POSITION );
					PutHsmRjectEvent ( &sTlvInfo , pin_result , TRACKIII_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , TRACKIII_POSITION );
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
					trace_event("End   CheckSecurity(NOK PIN)",PROCESSING);
					return ( NOK );
				}

				result = GetPinKey ( nIndiceCtx ,sPan, sPIN_KEY );
				if ( result != OK ) 
				{
					pin_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-PVK" , 4 );
				}
				else
				{
					pin_result = check_pin_data (	nIndiceCtx,    sPan,
						sPinBlock, sPinBlockFormat,
						sPIN_KEY, sTransKey,
						sHsmResultCode );
				}
				if ( pin_result != OK ) 
				{
					FillSecurityResult ( pin_result  , sSecurityResults , PIN_POSITION );
					PutHsmRjectEvent ( &sTlvInfo , pin_result , PIN_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION );
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
					trace_event("End   CheckSecurity(NOK PIN)",PROCESSING);
					return ( NOK );
				}
			}
			else if ( pin_method == OFFSET_VERIFICATION_METHOD )
			{
				if ( result_pin_info != OK )  /* offset was not found in database */
				{
					pin_result = HSM_MALFUNCTION;
					memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
					memcpy ( sHsmResultCode , "-PVK" , 4 );
					memcpy ( sMsgErr,"OFFSET READ ERROR",17);
				}
				else
				{
					result = GetPVKPinKey ( nIndiceCtx, sPan  , sPVKA  , sPVKB ); /** MAW20030515 **/
					if ( result != OK ) 
					{
						pin_result = HSM_MALFUNCTION;
						memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
						memcpy ( sHsmResultCode , "-PVK" , 4 );
					}
					else
					{
						pin_result = check_offset_data ( 	nIndiceCtx, sPan,
							sPVKA, sPinBlock,
							sPinBlockFormat, sTransKey,
							tOffset , encryption_mode ,
							sHsmResultCode , tPinLength );
					}
				}

				if ( pin_result != OK ) 
				{
					FillSecurityResult ( pin_result  , sSecurityResults , PIN_POSITION);
					PutHsmRjectEvent ( &sTlvInfo , pin_result , PIN_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION);
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
					trace_event("End   CheckSecurity(PIN NOK)",PROCESSING);
					return ( NOK );
				}
			}
			/* BEG YEL05022016 */ 
			else if ( pin_method ==  API_VERIFICATION_METHOD )
			{
				/* Getting the KV key */	
				result = GetKV(nIndiceCtx,sPan,sKV);
				if(result != OK)
				{
					memcpy ( sHsmResultCode , "-KV", 3 );
					trace_event("INVALID KEY KV ", ERROR);
					trace_event("END   GET_KV())",PROCESSING);
					return(HSM_MALFUNCTION);
				}
				
				if ( strlen(sKV) == 0 ) 
				{
					n_pin_result = HSM_MALFUNCTION;
					memcpy ( sHsmResultCode , "-KV" , 3 );
				}
				else
				{
					memcpy ( sPvvQ           , sPvv , 5 );
					GetQFromTrack2Data ( sTrack2Data , sPvvQ);
					
					
					n_pin_result = check_pvv_data ( 	nIndiceCtx, sPan,
														sKV, sKV,
														sPinBlock,sPinBlockFormat,sTransKey,
														sPvvQ , encryption_mode ,
														sHsmResultCode );
				}
				if ( n_pin_result != OK ) 
				{
					FillSecurityResult ( n_pin_result  , sSecurityResults , PIN_POSITION);
					PutHsmRjectEvent ( &sTlvInfo , n_pin_result , PIN_POSITION );
					HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION);
					AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
						trace_event("End  CheckSecurity(NOK PIN)",PROCESSING);
					return ( NOK );
				}
			}
			/* END YEL05022016 */
			
			FillSecurityResult ( pin_result  , sSecurityResults , PIN_POSITION);
		}
	}
	/*---------------------------------------------------------------------------*/
	/*        CRYPTOGRAMME  VERIFICATION                                                  */
	/*---------------------------------------------------------------------------*/
	/** START MAW20050916 **/
	if ( GetTlvAutho ( AUTO_ICC_DATA , &sTlvInfo, sIccDataTLV, &nLength ) == SUCCES 
		&& 'Y' == crpt_flag )
	{
		crpt_result = check_crpt_data ( 
			nIndiceCtx,
			sTlvBuffer,
			sHsmResultCode 
			);

		/** Adding Issuer Authen Data On Iso from Tlv **/
		if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccDataTLV ,&nLength) == SUCCESS )
			PutTlvAutho (AUTO_ICC_DATA, &sTlvInfo , sIccDataTLV, nLength);
		/** ***************************************** **/

		if ( crpt_result != OK )
		{
			FillSecurityResult ( crpt_result  , sSecurityResults , CRPT_POSITION );
			PutHsmRjectEvent ( &sTlvInfo , crpt_result , CRPT_POSITION );
			HsmDecline ( &sTlvInfo , sSecurityResults , sHsmResultCode , CRPT_POSITION );
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			trace_event("End   CheckSecurity(NOK CRPT)",PROCESSING);
			return ( NOK );
		}
	}

	

	FillSecurityResult ( cvv1_result , sSecurityResults , CVC1_POSITION);
	FillSecurityResult ( cvv2_result , sSecurityResults , CVC2_POSITION);
	FillSecurityResult ( pin_result  , sSecurityResults , PIN_POSITION );
	FillSecurityResult ( crpt_result  , sSecurityResults , CRPT_POSITION );
	FillSecurityResult ( icvv_result  , sSecurityResults , ICVV_POSITION );


	PutTlvAutho(AUTO_SECURITY_VER_RESULTS,&sTlvInfo,sSecurityResults, 32 );
	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
	trace_event("End   CheckSecurity(OK)",PROCESSING);

	return ( OK );
}

/**********************************************************************************************/
int BuildTagOffsetPvv ( TSTlvAutho *sTlvInfo , char *sOffset )
{
	char    	    sTlvPrivateBuffer [ P7_F048_MAX_LEN + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	int             nLength;

	InitTlvPrivate(&sTlvPrivateInfo);

	GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , sTlvInfo , sTlvPrivateBuffer ,&nLength);
	AnalyseTlvPrivate(sTlvPrivateBuffer,nLength,&sTlvPrivateInfo );
	PutTlvPrivate (PVV_OFFSET_TAG , &sTlvPrivateInfo, sOffset, 4);
	nLength = PrivateBuildTlv( sTlvPrivateBuffer , &sTlvPrivateInfo);
	PutTlvAutho ( AUTO_ADD_DATA_PRIVATE,sTlvInfo,sTlvPrivateBuffer,nLength);
	return ( OK );
}
/* ----------------------------------------------------------------------------------*/ 
/*       										 								     */ 
/* ----------------------------------------------------------------------------------*/ 
int SetPvvOffsetData(int nIndexCtx,TSTlvAutho *sTlvInfo) 
{
	char    		sTlvPrivateBuffer 	[ P7_F048_MAX_LEN + 1 ];
	char   			sTrack3Data 		[ 77 + 1];
	char   			sTrack2Data 		[ 40 + 1];
	char   			sPan 	    		[ 22 + 1];
	char   			sExpiryDate  		[  4 + 1];
	char   			sServiceCode 		[  3 + 1];
	char   			sPvv         		[  5 + 1];
	char   			sPvki         		[  1 + 1];
	char   			sCvv1        		[  5 + 1];
	char   			sOffset      		[  5 + 1];
	char   			sNewPinOff    		[  5 + 1];
	char   			sLine 				[ MAX_LINE_TRC ];
	int 			Return_Status = OK;
	int 			lRet 		  = OK;
	int 			nLength = 0;

	trace_event("Start SetPvvOffsetData() ",PROCESSING);

	memset ( sTrack3Data, 	0, sizeof ( sTrack3Data ));
	memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
	memset ( sPan,  		0, sizeof ( sPan ));
	memset ( sExpiryDate,  	0, sizeof ( sExpiryDate));
	memset ( sServiceCode,  0, sizeof ( sServiceCode));
	memset ( sPvv,  		0, sizeof ( sPvv ));
	memset ( sCvv1,  		0, sizeof ( sCvv1));
	memset ( sOffset,  		0, sizeof ( sOffset));

	if(GetTlvAutho(AUTO_TRACK2_DATA,sTlvInfo,sTrack2Data,&nLength) == SUCCES) 
		GetTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
	if ((memcmp (sPvv,"0000",4 ) == 0 ) || (trim(sPvv,4) == 0 )) {
		if(GetTlvAutho(AUTO_TRACK3_DATA,sTlvInfo,sTrack3Data,&nLength) == SUCCES) { 
			if ( UpdateTrack3(nIndexCtx,sTlvInfo) != OK ) {
				trace_event(" End SetPvvOffsetData (NOK) ",PROCESSING);
				return(NOK);
			}
		}
		else{
			if ( get_pin_info(nIndexCtx,sPan,sPvv,sOffset,sPvki) == OK ) {
				lRet = ComputePinOffset(nIndexCtx,sTlvInfo,sNewPinOff);
				if (lRet == OK )
					lRet = PutPinOffset(nIndexCtx,sPan,sNewPinOff);
			}
			else{
				trace_event(" End SetPvvOffsetData (NOK) ",PROCESSING);
				return(NOK);
			}
		}
	}
	else{
		lRet = ComputePvv(nIndexCtx,sTlvInfo,sNewPinOff);
		if (lRet == OK )
			lRet = PutPvv(nIndexCtx,sPan, sNewPinOff);
	}
	sprintf(sLine,"End SetPvvOffsetData(%d) ",lRet);
	return(lRet);
}

GetCvc3FromTrack( char *sDiscretionaryData, char *sCvc3)
{
	int i =0;
	for(i = 0; i< strlen(sDiscretionaryData); i++)
		if ( sDiscretionaryData[i] == 'F' )
		{
			memcpy (sCvc3 + 2, sDiscretionaryData + i - 8, 3);
			break;
		}
}

