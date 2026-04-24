#include <stdio.h>
#include <string.h>

#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <tlv_private.h>
#include <security_data.h>
#include <hps_iso_8583.h>
#include <tlv_fields_len.h>/* YDA20180103 */
#include <p7_sid.h>

void IsoIccToTlv (char *szBufferIn,  int nLengthIn, char *szBufferOut, int *nLengthOut)
{
	TSTagIso		icInfo;
	TSTlvPrivate	tlvInfo;
	int				icLen;
	char			icTmp[BUF_LG_MAX];

	char			sLine[MAX_LINE_TRC];

	trace_event("Start IsoIccToTlv()",PROCESSING);

	memset(icTmp, 0, sizeof(icTmp));
	InitIsoIcTag (&icInfo);
	InitTlvPrivate (&tlvInfo);

	AnalyseIsoIc(szBufferIn, nLengthIn, &icInfo);
	
	if (GetIsoIcTag (ISO_TAG_TRACK2,&icInfo, icTmp, &icLen) == SUCCES)
	{
		PutTlvPrivate (CHIP_T2_EQUIVALENT_DATA, &tlvInfo, icTmp, icLen);
	}
	
	if (GetIsoIcTag (ISO_TAG_SEC_PIN_DATA,&icInfo, icTmp, &icLen) == SUCCES)
	{
		if ( PutTlvPrivate (CHIP_SEC_PIN_DATA, &tlvInfo, icTmp, icLen) != SUCCES)
			trace_event("ERROR PutTlvPrivate <CHIP_SEC_PIN_DATA> ",FATAL); 	
	}

	if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_AIP_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_ICC_APP_ID      , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_APPLI_ID_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TERM_CAP_PROFILE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TERM_CAPABILITY_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, icTmp, icLen);

	trace_event(" PRSP: ---- 9100 ---- ",PROCESSING);

	if (GetIsoIcTag (ISO_TAG_TVR             , &icInfo, icTmp, &icLen) == SUCCES)
	{	
		PutTlvPrivate (CHIP_TVR_TAG, &tlvInfo, icTmp, icLen);
	}

	if (GetIsoIcTag (ISO_TAG_TRANS_DATE      , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TRANS_DATE_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TRANS_TYPE      , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TRANS_TYPE_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE  , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TRANS_CUR_CODE_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT    , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TRANS_AMOUNT_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_OTHER_AMOUNT    , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_OTHER_AMOUNT_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA    , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TERM_COUNTRY_CODE_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM  , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_IFD_SERIAL_NUM_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM  , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_CRYPTO_INFO_DATA_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_PAN_SEQ_NBR, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_PAN_SEQ_NBR, &tlvInfo, icTmp, icLen);

	/* START MTR - Enh4.12 */
        if (GetIsoIcTag (ISO_TAG_APPLI_EXPIRY_DATE,&icInfo, icTmp, &icLen) == SUCCES)
                PutTlvPrivate (CHIP_APPL_EXP_DATE, &tlvInfo, icTmp, icLen);
	/* End MTR - Enh4.12 */


	trace_event(" PRSP: ---- 5F34 ---- ",PROCESSING);

	if (GetIsoIcTag (ISO_TAG_CVM            , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_CVM_RESULTS_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TERM_TYPE      , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TERM_TYPE_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_ATC            , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ATC_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER  , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_UNPRED_NUMBER_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TRX_SEQ_COUNT_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_CATEGORY_CODE_TAG , &tlvInfo, icTmp, icLen);
	/* 139 */
	if (GetIsoIcTag (ISO_TAG_ARPC ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, icTmp, icLen);

	/* Issuer script TAGs Mehdi ELyajizi */
	if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT1 ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_SCRIPT_1, &tlvInfo, icTmp, icLen);

	trace_event(" PRSP: ---- 7100 ---- ",PROCESSING);

	if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT2 ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_SCRIPT_2, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, icTmp, icLen);


	/* START MTR - Enh4.11 */
	if (GetIsoIcTag (ISO_TAG_APP_RGSTD_DATA ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (EMV_TAG_APP_RGSTD_DAT, &tlvInfo, icTmp, icLen);
	/* End MTR - Enh4.11 */
	if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT_RESULT ,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_ISS_SCRIPT_RESULT_TAG, &tlvInfo, icTmp, icLen);
	/* Issuer script TAGs Mehdi ELyajizi */

	/*********Start LHI *********************************/
	/***Extract des TAG 9F06 57 5A 9F12 8C00 5000  *****/  
	if (GetIsoIcTag (ISO_TAG_APP_SELECTED, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_APP_SELECTED_TAG , &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate",FATAL);

	if (GetIsoIcTag (ISO_TAG_TRACK2, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_TRACK2_TAG , &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate",FATAL);

	if (GetIsoIcTag (ISO_TAG_APP_PAN, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_PAN_TAG , &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate",FATAL);

	if (GetIsoIcTag (ISO_TAG_APP_NAME, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_APP_NAME_TAG , &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate",FATAL);

	if (GetIsoIcTag (ISO_TAG_ICC, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_SECUR_ATT_TAG , &tlvInfo, icTmp, icLen)!=SUCCES)	/*AMER 201060719: CHIP_APP_LABEL_TAG turned into CHIP_SECUR_ATT_TAG*/
			trace_event("Erreur PutTlvPrivate",FATAL);

	if (GetIsoIcTag (ISO_TAG_APP_LABEL, &icInfo, icTmp, &icLen) == SUCCES)
		if(PutTlvPrivate (CHIP_APP_LABEL_TAG , &tlvInfo, icTmp, icLen)!=SUCCES) /*AMER 201060719: CHIP_SECUR_ATT_TAG turned into CHIP_APP_LABEL_TAG*/
			trace_event("Erreur PutTlvPrivate",FATAL);

	/***End LHI ***************************************/

	if (GetIsoIcTag (ISO_TAG_KERNEL_ID, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_KERNEL_ID , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_ICC_PROCESSING_RESULT, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_ICC_PROCESSING_RESULT , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_APPL_TYPE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_APPL_TYPE , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_RTT_RESULT, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_RTT_RESULT , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_CONTACTLESS_DEVICE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_CONTACTLESS_DEVICE , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_EMV_ACQ_MODE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_EMV_ACQ_MODE , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_CARD_AUTH_RES_CODE, &icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_TAG_CARD_AUTH_RES_CODE , &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_DED_FILE_NAME,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_DED_FILE_NAME_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_APP_USAGE_CTRL,&icInfo, icTmp, &icLen) == SUCCES)
		PutTlvPrivate (CHIP_APP_USE_CONTROL_TAG, &tlvInfo, icTmp, icLen);

	if (GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_APPLI_VER_TAG, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_APPLI_VER_TAG",FATAL);

	if (GetIsoIcTag (ISO_TAG_TRACK_1_DATA,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_TAG_TRACK_1_DATA, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_TAG_TRACK_1_DATA",FATAL);

	memset(icTmp, 0, sizeof(icTmp));
	if (GetIsoIcTag (ISO_TAG_FFI  , &icInfo, icTmp, &icLen) == SUCCES)
	{
		PutTlvPrivate (CHIP_FFI_TAG, &tlvInfo, icTmp, icLen);
	}

	if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT_CMD,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_ISS_SCRIPT_CMD, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_ISS_SCRIPT_CMD",FATAL);

	if (GetIsoIcTag (ISO_TAG_TERM_CAP,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_TERM_CAP, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_TAG_TERM_CAP",FATAL);

	if (GetIsoIcTag (ISO_TAG_APP_SEQ_NUMBER,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_APP_SEQ_NUMBER, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_APP_SEQ_NUMBER",FATAL);

	if (GetIsoIcTag (ISO_TAG_CVM_LIST,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_CVM_LIST, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_CVM_LIST",FATAL);

	if (GetIsoIcTag (ISO_TAG_APPL_EFF_DATE,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_APPL_EFF_DATE, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_APPL_EFF_DATE",FATAL);

	if (GetIsoIcTag (ISO_TAG_APPL_EXP_DATE,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_APPL_EXP_DATE, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_APPL_EXP_DATE",FATAL);

	if (GetIsoIcTag (ISO_TAG_IAC_DEFAULT,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_IAC_DEFAULT, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_IAC_DEFAULT",FATAL);

	if (GetIsoIcTag (ISO_TAG_IAC_DENIAL,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_IAC_DENIAL, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_IAC_DENIAL",FATAL);

	if (GetIsoIcTag (ISO_TAG_IAC_ONLINE,&icInfo, icTmp, &icLen) == SUCCES) 
		if(PutTlvPrivate (CHIP_IAC_ONLINE, &tlvInfo, icTmp, icLen)!=SUCCES)
			trace_event("Erreur PutTlvPrivate CHIP_IAC_ONLINE",FATAL);

	*nLengthOut = PrivateBuildTlv (szBufferOut, &tlvInfo);
	memset(icTmp, 0, sizeof(icTmp));
	InitIsoIcTag (&icInfo);
	InitTlvPrivate (&tlvInfo);
	trace_event("End   IsoIccToTlv(OK)",PROCESSING);
}



int IsoToTlv (int nIndiceCtx , TSIsoInfo * isoInfo, TSTlvAutho *tlvInfo)
{
	char  sLine     [250];
	char  tmpField  [  999];
	char  tmpData   [  999];
	char  tagName   [   4 ];
	char  sAmount   [ 18+1];
	char  sCurrency [  3+1];
	char  sOrigAmounts [ 12 + 1 ];
	char  sTlvBuffer  [ LG_MAX ];
	char  sMti [4+1];
	int	 nRetCode;
	int   tmpFldLen, nLength;
	int   i,j,k;
	int   BlockSize ;
	char  bBlockSize [3];
	char  cBlockSize [5];
	char  bTraceAudit [4];
	char  cTraceAudit [7];
	char	tmp110   [  LG_MAX];
	char	sOrigMessage[4 + 1];
	char  sCBAmount [ 12 + 1 ];
	TSTlvPrivate	PrivateTlvInfo;

	InitTlvPrivate(&PrivateTlvInfo);


	trace_event ( "Start IsoToTlv ()", PROCESSING );

	memset ( sLine  , 0 , sizeof ( sLine));
	memset ( tmpField  , 0 , sizeof ( tmpField));
	memset ( tmpData  , 0 , sizeof ( tmpData));
	memset (tagName, 0, sizeof(tagName));
	memset ( sOrigAmounts , 0 , sizeof ( sOrigAmounts ));
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset ( sAmount   , 0,  sizeof (  sAmount));
	memset ( sCurrency , 0,  sizeof (sCurrency));
	memset (   sMti , 0 , sizeof ( sMti ));
	memset (tagName, 0, sizeof(tagName));
	memset (sOrigMessage, 0, sizeof(sOrigMessage));
	memset ( sCBAmount , 0 , sizeof ( sCBAmount ));

	sprintf ( sMti , "%04d", isoInfo->nMsgType );

	/*EBE150908: don't let the repeat indicator goes into the switch*/
	if( sMti[MSG_TYPE_TRX_ORIGIN] == MSG_TYPE_P4_ACQ_REP || 
		sMti[MSG_TYPE_TRX_ORIGIN] == MSG_TYPE_P4_ISS_REP ||
		sMti[MSG_TYPE_TRX_ORIGIN] == MSG_TYPE_P4_OTHER_REP)
	{

		memcpy(sOrigMessage,sMti,4);
		switch(sMti[MSG_TYPE_TRX_ORIGIN])
		{
		case MSG_TYPE_P4_ACQ_REP: sMti[MSG_TYPE_TRX_ORIGIN] = MSG_TYPE_P4_ACQ;break;
		case MSG_TYPE_P4_ISS_REP: sMti[MSG_TYPE_TRX_ORIGIN] = MSG_TYPE_P4_ISS;break;
		case MSG_TYPE_P4_OTHER_REP: sMti[MSG_TYPE_TRX_ORIGIN] = MSG_TYPE_P4_OTHER;break;
		}
		PutTlvAutho ( AUTO_ORIG_MSG_TYPE, tlvInfo, sOrigMessage, 4);

	}


	PutTlvAutho ( AUTO_MSG_TYPE, tlvInfo, sMti, 4);

	for ( i= 1; i< 128; i++)
	{
		memset (tmpField, 0, sizeof(tmpField));
		memset (tmpData , 0, sizeof(tmpData));


		switch (i)
		{

		case ISO_PROC_CODE :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES)
			{
				sprintf(tmpData,    "%.3s002%.2s", PROC_TRANS_TYPE, tmpField ); 
				sprintf(tmpData+8,  "%.3s002%.2s", PROC_ACCOUNT_FROM, tmpField+2); 
				sprintf(tmpData+16, "%.3s002%.2s", PROC_ACCOUNT_TO, tmpField+4); 
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpData, 24);
			}
			break;

		case ISO_ICC_DATA	 :
			trace_event ( "--IsoToTlv () ISO_ICC_DATA", PROCESSING );
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES)
			{
				IsoIccToTlv(tmpField, tmpFldLen, tmpData, &nLength);
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpData, nLength);
			}
			break;

		case ISO_TRANS_AMOUNT :
			sprintf (tagName, "%.3d", i+1);
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, sAmount, &tmpFldLen) == SUCCES)
			{
				if (GetIsoField ( i + 45 , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sAmount , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_TRANS_AMOUNT)", ERROR);
						return ( NOK );
					}
					PutTlvAutho (tagName, tlvInfo, tmpData, 18);
				}
			}
			break;

		case ISO_STLMNT_AMOUNT :
			sprintf (tagName, "%.3d", i+1);
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, sAmount, &tmpFldLen) == SUCCES)
			{
				if (GetIsoField ( i + 45 , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sAmount , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_STLMNT_AMOUNT)", ERROR);
						return ( NOK );
					}
					PutTlvAutho (tagName, tlvInfo, tmpData, 18);
				}
			}
			break;

		case ISO_BILLING_AMOUNT :
			sprintf (tagName, "%.3d", i+1);
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, sAmount, &tmpFldLen) == SUCCES)
			{
				if (GetIsoField ( i + 45 , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sAmount , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_BILLING_AMOUNT)", ERROR);
						return ( NOK );
					}
					PutTlvAutho (tagName, tlvInfo, tmpData, 18);
				}
			}
			break;

			/*EBE141202: Adjust fees amount too */
		case ISO_FEES_AMOUNT :
			sprintf (tagName, "%.3d", i+1);
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( ISO_FEES_AMOUNT, isoInfo, sAmount, &tmpFldLen) == SUCCES)
			{
				if (GetIsoField ( ISO_TRANS_CRNCY , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sAmount , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_FEES_AMOUNT)", ERROR);
						return ( NOK );
					}
					PutTlvAutho (tagName, tlvInfo, tmpData, 18);
				}
			}
			break;
		case ISO_ORIGINAL_AMOUNTS :
			memset ( sOrigAmounts , 0 , sizeof ( sOrigAmounts ));
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if ( GetIsoField ( ISO_ORIGINAL_AMOUNTS ,isoInfo , tmpField , &tmpFldLen ) == SUCCES )
			{
				memcpy ( sOrigAmounts , tmpField , 12 );
				if (GetIsoField ( ISO_TRANS_CRNCY , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sOrigAmounts , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_ORIGINAL_AMOUNTS)", ERROR);
						return ( NOK );
					}
					PutTlvAutho   ( AUTO_REPLACEMENT_AMOUNT , tlvInfo, tmpData , 18);
				}

				memcpy ( sOrigAmounts , tmpField + 12 , 12 );
				if (GetIsoField ( ISO_BILLING_CRNCY , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sOrigAmounts , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_ORIGINAL_AMOUNTS-ISO_BILLING_CRNCY)", ERROR);
						return ( NOK );
					}
					PutTlvAutho   ( AUTO_REPLACEMENT_BILLING , tlvInfo, tmpData , 18);
				}
				
				/*Start AMER20210414 PLUTONL-2991*/
				memcpy ( sOrigAmounts , tmpField + 24 , 12 );
				
				if (memcmp(sOrigAmounts, "000000000000", 12) != 0
				    && GetIsoField ( ISO_STLMNT_CRNCY , isoInfo, sCurrency, &tmpFldLen) == SUCCES)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , sOrigAmounts , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_ORIGINAL_AMOUNTS-ISO_STLMNT_AMOUNT)", ERROR);
						return ( NOK );
					}
					PutTlvAutho   ( AUTO_REPLACEMENT_SETTL , tlvInfo, tmpData , 18);
				}
				/*End AMER20210414 PLUTONL-2991*/
			}
			break;
			/*Start EBE131125*/
		case ISO_ADTNL_AMOUNTS :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES) 
			{
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpField, tmpFldLen);
				nLength = tmpFldLen;
				for( j=0; j<nLength; j+=20)
				{
					if( memcmp(tmpField + j + P7_F054_ACCT_TYPE_LEN, "40",P7_F054_AMNT_TYPE_LEN) == 0 )
					{
						/*Cashback amount*/
						memcpy ( sCurrency , tmpField + P7_F054_ACCT_TYPE_LEN + P7_F054_AMNT_TYPE_LEN + j , P7_F054_CCY_LEN );
						memcpy ( sCBAmount , tmpField  + 8 + j , P7_F054_AMNT_LEN );
						nRetCode = AdjustTlvAmount (  nIndiceCtx , sCBAmount, sCurrency , tmpData );
						if ( nRetCode != OK )
						{
							trace_event("Error AdjustTlvAmount(AUTO_CASH_BACK_AMOUNT)", ERROR);
							return ( NOK );
						}
						PutTlvAutho (AUTO_CASH_BACK_AMOUNT, tlvInfo, tmpData, 18);
					}
				}
			}
			break;
		case ISO_ADTNL_DATA_PRIV :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES) 
			{
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpField, tmpFldLen);
				AnalyseTlvPrivate(tmpField,tmpFldLen,&PrivateTlvInfo);
				if( GetTlvPrivate(TRANSACTION_IDENTIFIER,&PrivateTlvInfo,tmpField,&tmpFldLen) == SUCCESS )
					PutTlvAutho (AUTO_TRANSACTION_IDENTIFIER, tlvInfo, tmpField, tmpFldLen);
			}
			break;
			/*End EBE131125*/
			/* START Add Filed 110 HKETTANI 19/02/2004 */
		case ISO_RESERVED_ISO_2:       
			memset ( tmpField , 0 , sizeof ( tmpField ));
			memset ( tmp110   , 0 , sizeof ( tmp110 ));

			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES)
			{  
				j = 0; /* Counter for tmpFiled original 110 */
				k = 0; /* Counter for tmp110 resulting 110 */

				while (j < tmpFldLen){

					trace_event ( "Inside the while", PROCESSING );

					memset ( cBlockSize   , 0 , sizeof ( cBlockSize ));
					memset ( bBlockSize   , 0 , sizeof ( bBlockSize ));
					memset ( bBlockSize   , 0 , sizeof ( bBlockSize ));

					memset ( bTraceAudit  , 0 , sizeof ( bTraceAudit));
					memset ( cTraceAudit  , 0 , sizeof ( cTraceAudit));

					memcpy(bBlockSize,tmpField+j,2); /* compy the block size */
					memcpy(bTraceAudit,tmpField+j+2,3); /* compy the trace audit number*/


					sprintf(sLine,"--bBlockSize %s",bBlockSize);
					trace_event(sLine,TRACE);

					sprintf(sLine,"--bTraceAudit %s",bTraceAudit);
					trace_event(sLine,TRACE);

					HexToAscii(bBlockSize,cBlockSize,4);
					HexToAscii(bTraceAudit,cTraceAudit,6);


					sprintf(sLine,"--cBlockSize %s",cBlockSize);
					trace_event(sLine,TRACE);

					sprintf(sLine,"--cTraceAudit %s",cTraceAudit);
					trace_event(sLine,TRACE);

					BlockSize = atoi(cBlockSize) - 3 ; /* remove 3 posistions of trace Audit number */

					sprintf(sLine,"--BlockSize %d",BlockSize);
					trace_event(sLine,TRACE);

					IsoIccToTlv(tmpField+j+5, BlockSize, tmpData, &nLength);


					/* copy the first portion into field 110 tarce_audit_number */
					memcpy(tmp110+k,cTraceAudit,6); 


					/* copy the second portion */
					memcpy(tmp110+6+k,tmpData,nLength); 

					/* Insert a Field Separator*/
					memcpy(tmp110+6+k+nLength,"?",1); 

					/* PROD00055108: sLine is not big enough to hold tmp110 which may be more than 250 bytes
					sprintf(sLine,"--tmp110+6+k= %s",tmp110+6+k);
					trace_event(sLine,TRACE);
					*/

					k = k + 7+ nLength; 
					j = j  + BlockSize + 5;  


					sprintf(sLine,"--k= %d",k);
					trace_event(sLine,TRACE);
					sprintf(sLine,"--j= %d",j);
					trace_event(sLine,TRACE);
				}

				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmp110, k);
			}
			break;
			/* END Add Filed 110 HKETTANI 19/02/2004 */
			/* MKB271106: in PIN change messages ISO_RESERVED_PRIVE_USAGE_4 contains the new PIN block */
		case ISO_RESERVED_PRIVE_USAGE_4 :
			if (GetIsoField ( ISO_RESERVED_PRIVE_USAGE_4 ,isoInfo , tmpField , &tmpFldLen ) == SUCCES)
				PutTlvAutho   ( AUTO_RESERVED_PRIVATE_4 , tlvInfo, tmpField , 16);
			break;
			/* MKB271106 */
			/* MMJ 25052009 : Used for chek deposit DEPOSIT_TYPE and DEPOSIT_NUMBER  */
		case ISO_RESERVED_ISO_3 :
			if (GetIsoField (ISO_RESERVED_ISO_3 ,isoInfo , tmpField , &tmpFldLen ) == SUCCES)
				PutTlvAutho   ( AUTO_RESERVED_ISO_3 , tlvInfo, tmpField , tmpFldLen);
			break;
			/* MMJ25052009 */
		case ISO_ORIG_DATA:
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES) 
			{
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpField, tmpFldLen);
			}

			break;

		default :

			memset ( tmpField , 0 , sizeof ( tmpField ));
			if (GetIsoField ( i, isoInfo, tmpField, &tmpFldLen) == SUCCES) 
			{
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpField, tmpFldLen);
				if ( i == ISO_SECURITY_DATA )
				{
					GetSecurityData(USER,tmpField,tmpData,&tmpFldLen);
					PutTlvAutho ( AUTO_USER, tlvInfo, tmpData , USER_LEN);
				}
			}

			break;

		}/* Switch */
	} /* For */

	/*EBE130925:SWMON*/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	sprintf(tmpField,"%.010lu",isoInfo->msgId);
	if (memcmp(tmpField, "0000000000", 10) !=0) /* HBO18062014 SWITCH MONITORING */
		PutTlvAutho ( AUTO_MSG_FLOW_MAPPING_ID, tlvInfo, tmpField, /*MSG_ID_LEN*/strlen(tmpField) /*ZKO 18032022 PLUTONL-4130*/);/* YDA20180103 */

	AuthoBuildTlv ( sTlvBuffer, tlvInfo);

	PrintTlvBuffer(sTlvBuffer);

	nRetCode = ValidateTlvData(sTlvBuffer);

	memset (tmpData , 0, sizeof(tmpData));
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));/*PADSS NAK*/
	InitTlvPrivate(&PrivateTlvInfo);
	if (nRetCode != OK)
	{
		trace_event("Error ValidateTlvData()", ERROR);
		trace_event("End IsoToTlv(NOK)", ERROR);
		return ( NOK );
	}
	trace_event ( "End IsoToTlv ()", PROCESSING );
	return ( OK );
}

