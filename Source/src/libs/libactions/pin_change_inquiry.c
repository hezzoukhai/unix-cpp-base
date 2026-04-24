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
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>

#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>

#include <tlv_privfields.h>
#include <tlv_private.h>

#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>

#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>

#define PIN_METHODE_PVV 	'2'
#define PIN_METHODE_OFFSET  '3'


int PinChange_Inquiry ( int nIndexCtx, char *sTlvBuffer  )
{

	TSTlvAutho      sTlvInfo;
	char    		lNewPinOff 			[ 16 + 1 ];
	char    		lNewPinPvv 			[ 5 ];
	char    		lOldPinPvv 			[ 5 ];
	int     		nLength;
	int     		lRet = NOK;
	char 			pin_method;
	char   			sPan 	    [ 22 + 1];
	char   			sSecurityFlags  [ 32 + 1 ];
	char   			lPvki[1 + 1];
	char   			sLine [ MAX_LINE_TRC ];
	char            tOffset      [ 12 + 1];
	int             tPinLength;
	char			sIccData [1024], sHsmResult[256];
	char			sData[BUF_LG_MAX + 1];
	char			sAppCode[4 + 1];
	char 			sExpiryDate[4+1];
	char 			sCardSeqNbr[3+1];
	char			offline_pin_cap;
	int				nPvki;
	int				nPinChanged;


	trace_event("Start PinChange_Inquiry()", PROCESSING);

	/************************************************************************/
	/* INITIALISATION DES VARIABLES                                         */
	/************************************************************************/

	memset ( sPan,  		0, sizeof ( sPan ));
	memset ( sSecurityFlags   , 0,  sizeof (sSecurityFlags));
	memset ( lPvki   , 0, sizeof ( lPvki ));
	memset ( lNewPinPvv,  		0, sizeof ( lNewPinPvv ));
	memset ( lOldPinPvv,  		0, sizeof ( lOldPinPvv ));
	memset(lNewPinOff, 0, sizeof(lNewPinOff));
	memset(sHsmResult, 0, sizeof(sHsmResult));
	memset(sAppCode, 0, sizeof(sAppCode));
	memset(sExpiryDate, 0, sizeof(sExpiryDate));
	memset(sCardSeqNbr, 0, sizeof(sCardSeqNbr));
	

	InitTlvAutho	(&sTlvInfo);
	AnalyseTlvAutho (sTlvBuffer , &sTlvInfo);

	
	nPinChanged = NOK;

	if ( GetTlvAutho (  AUTO_SECURITY_VERIF_FLAGS  , &sTlvInfo, sSecurityFlags , &nLength ) != SUCCES )
	{
		trace_event("Security Flags Empty.",PROCESSING);
		trace_event("End   PinChange_Inquiry(NOK)", PROCESSING);
		return ( SYSTEM_MALFUNCTION );
	}
	
	pin_method  	= sSecurityFlags[SEC_FLAG_PIN_MTHD];
	

	GetTlvAutho(AUTO_PAN,&sTlvInfo,sPan,&nLength);
	GetTlvAutho(AUTO_CARD_SEC_INFO_DATA,&sTlvInfo,sData,&nLength);

	get_pin_info_data(sData,&nPvki,lOldPinPvv,tOffset,&tPinLength);
	sprintf(lPvki,"%.1d",nPvki);
	

	if (pin_method  	== PIN_METHODE_PVV )
	{
		lRet = ComputePvv(nIndexCtx,&sTlvInfo ,lNewPinPvv,lPvki[0]);
		if (lRet == OK )
		{
			lRet = ComputePinOffset(nIndexCtx,&sTlvInfo,lNewPinOff,lPvki[0],tPinLength);
			if( lRet == OK )
			{
				GetTlvBuffer(AUTO_EXPIRY_DATE, sTlvBuffer, sExpiryDate, &nLength);	/*AMER20191025: PLUTONL-490*/
				GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sCardSeqNbr, &nLength);	/*AMER20191025: PLUTONL-490*/

				nPinChanged = PutPvv(nIndexCtx,sPan, sCardSeqNbr,sExpiryDate,lNewPinPvv,lPvki, lNewPinOff);
			}
		}
	}
	else if (pin_method  	== PIN_METHODE_OFFSET )
	{
		lRet = ComputePinOffset(nIndexCtx,&sTlvInfo,lNewPinOff,lPvki[0],tPinLength);

		if (lRet == OK )
		{
			GetTlvBuffer(AUTO_EXPIRY_DATE, sTlvBuffer, sExpiryDate, &nLength);	/*AMER20191025: PLUTONL-490*/
			GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sCardSeqNbr, &nLength);	/*AMER20191025: PLUTONL-490*/
			
			nPinChanged = PutPinOffset(nIndexCtx,sPan,sCardSeqNbr,sExpiryDate,lNewPinOff);
		}
	}

	sprintf(sLine,"PinChange Result : [%d] ", nPinChanged);
	trace_event(sLine,PROCESSING);

	offline_pin_cap = '0';
	if( GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sData, &nLength) == SUCCESS )
	{
		offline_pin_cap		= sData[13];
		memcpy(sAppCode,sData,4);
	}

	if( nPinChanged == OK && offline_pin_cap != '0')
	{
		nPinChanged = GenPC_IssScript(nIndexCtx,sPan,sAppCode);
		if( nPinChanged != OK )
		{
			GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sCardSeqNbr, &nLength);	/*AMER20191025: PLUTONL-490*/
			lRet = PinInfoReversal(nIndexCtx,sPan,sCardSeqNbr);	
			if( lRet != OK )
			{
				trace_event(">> Unable to reverse PIN value", ERROR);
			}
		}
	}


	memset ( sPan,  		0, sizeof ( sPan ));
	memset ( lNewPinPvv,  		0, sizeof ( lNewPinPvv ));
	memset ( lOldPinPvv,  		0, sizeof ( lOldPinPvv ));

	if( nPinChanged != OK )
	{
		trace_event("End   PinChange_Inquiry(NOK)", PROCESSING);
	}
	
	trace_event("End   PinChange_Inquiry(OK)", PROCESSING);
	return ( nPinChanged );
}


