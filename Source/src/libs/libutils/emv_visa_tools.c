#include <define.h>
#include <iso_hps.h>
#include <iso8583.h>
#include <iso_visa.h>
#include <iso_jcc.h>
#include <iso_pos.h>
#include <iso_ictag.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <visa_ictag.h> /**MKB221208*/


/***************************************************************************************/
/** Function :   **/
/** Spec     :   **/
/** Input    :   **/
/** Output   :   **/
/** Author   :   **/
/** Date     :   **/
/***************************************************************************************/
int 	IsoToVisaIcc	(
	TSVisaInfo  *VisaInfo,
	TSIsoInfo   *IsoInfo,
	char	     Mode
	)
{

	TSTagIso       TagIso;
	char           tmpField[256];
	int            tmpFieldLen;
	int			   ptr_id;
	char           sLine[MAX_LINE_TRC];
	/*char           sVisaDisData[LG_MAX];
	/*char           sWork[LG_MAX];
	int			   nBytes;
	int			   nPos,i;*/

	sprintf(sLine,"--Start IsoToVisaIcc(%c)--", Mode ) ;
	trace_event(sLine,PROCESSING);

	memset (tmpField,     0, sizeof(tmpField));
	DumpVisaStruct(VisaInfo);


	if (Mode != 'R') /* Normal Request */
	{
		/** Retrieve ICC Data : ISO field 55 **/
		if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			InitIsoIcTag (&TagIso);
			IsoDumpIccFields(tmpField,tmpFieldLen);
			AnalyseIsoIc (tmpField, tmpFieldLen, &TagIso);

			/* (130) */
			if (GetIsoIcTag (ISO_TAG_TERM_CAP, &TagIso,tmpField,&tmpFieldLen) == SUCCESS)
				/* PutVisaField (VISA_TERM_CAPBLTY_PROFILE, VisaInfo, "E0F0C0", 6); */
				PutVisaField (VISA_TERM_CAPBLTY_PROFILE, VisaInfo, tmpField, tmpFieldLen); 

			/* (131) */
			if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
				sprintf(sLine,"VISA TVR is %d|%s",tmpFieldLen,tmpField);
				trace_event(sLine,TRACE);
				PutVisaField (VISA_TERM_VERIF_RESULTS, VisaInfo, tmpField, tmpFieldLen);
			}

			/* (132) */
			if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
				PutVisaField (VISA_UNPREDICT_NBR, VisaInfo, tmpField, tmpFieldLen);

			/* (133) */
			/*
			if (GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
			memset(sWork, 0, sizeof(sWork));
			nBytes = HexToAscii( tmpField,
			sWork,
			16  );
			sprintf(sLine,"ISO_TAG_IFD_SERIAL_NUM : %s|%d",sWork,nBytes);
			trace_event(sLine,TRACE);

			PutVisaField (VISA_TERM_SERIAL_NBR, VisaInfo, sWork, 16);
			}
			*/

			/* (134.3) */
			/*
			memset(sVisaDisData, 0, sizeof(sVisaDisData));
			nPos = 0;
			sVisaDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
			sVisaDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
			tmpFieldLen = 0;
			if (GetIsoIcTag (ISO_TAG_CVM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
			memset(sWork, 0, sizeof(sWork));
			nBytes = AsciiToBcd( tmpField, sWork, tmpFieldLen);
			/ *
			sWork[0] = 0XA0;
			sWork[1] = 0XB8;
			sWork[2] = 0X00;
			* /
			*(sVisaDisData + nPos ) = nBytes;nPos += 1;
			memcpy(sVisaDisData + nPos, sWork, nBytes); nPos += nBytes;
			}
			*/

			if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
				sprintf(sLine,"VISA ISS is %d|%s",tmpFieldLen,tmpField);
				trace_event(sLine,TRACE);
				PutVisaField (VISA_VISA_DISCRET_DATA, VisaInfo, tmpField + 2, tmpFieldLen - 2);
			}

			/* (135) */
			/*
			if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutVisaField (VISA_ISSUER_DISCRET_DATA, VisaInfo, tmpField, tmpFieldLen);
			*/

			/* (136) */
			if (GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_CRYPTOGRAM, VisaInfo, tmpField, tmpFieldLen);

			/* (137) */
			if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_APP_TRANS_COUNTER, VisaInfo, tmpField, tmpFieldLen);

			/* (138) */
			if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_APP_INTCHG_PROFILE, VisaInfo, tmpField, tmpFieldLen);

			/* (144) */
			if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_CRYPTO_TRANS_TYPE, VisaInfo, tmpField, tmpFieldLen);

			/* (145) */
			if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_TERM_COUNTRY_CODE, VisaInfo, tmpField, tmpFieldLen);

			/* (146) */
			if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_TERM_TRANS_DATE, VisaInfo, tmpField, tmpFieldLen);

			/* (147) */
			if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_CRYPTO_AMOUNT, VisaInfo, tmpField, tmpFieldLen);

			/* (148) */
			if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_CRYPTO_CRNCY_CODE, VisaInfo, tmpField, tmpFieldLen);

			/* (149) */
			if (GetIsoIcTag (ISO_TAG_OTHER_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
				/*	PutVisaField (VISA_OTHER_AMOUNTS, VisaInfo, tmpField, tmpFieldLen); */
				PutVisaField (VISA_CRYPTO_CBACK_AMOUNT, VisaInfo, tmpField, tmpFieldLen);
			}
		}
	}
	else /* Reversal */
	{
		if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			InitIsoIcTag (&TagIso);
			IsoDumpIccFields(tmpField,tmpFieldLen);
			AnalyseIsoIc (tmpField, tmpFieldLen, &TagIso);

			/* (131) */
			/* VISA CERTIF
			if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutVisaField (VISA_TERM_VERIF_RESULTS, VisaInfo, tmpField, tmpFieldLen);
			*/

			/* (133) */
			/*
			if (GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
			memset(sWork, 0, sizeof(sWork));
			nBytes = HexToAscii( tmpField,
			sWork,
			16  );
			sprintf(sLine,"ISO_TAG_IFD_SERIAL_NUM : %s|%d",sWork,nBytes);
			trace_event(sLine,TRACE);

			PutVisaField (VISA_TERM_SERIAL_NBR, VisaInfo, sWork, 16);
			}
			*/

			/* (134.3) */
			/*
			memset(sVisaDisData, 0, sizeof(sVisaDisData));
			nPos = 0;
			sVisaDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
			sVisaDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
			tmpFieldLen = 0;
			if (GetIsoIcTag (ISO_TAG_CVM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
			memset(sWork, 0, sizeof(sWork));
			nBytes = AsciiToBcd( tmpField, sWork, tmpFieldLen);
			*(sVisaDisData + nPos ) = nBytes;nPos += 1;
			memcpy(sVisaDisData + nPos, sWork, nBytes); nPos += nBytes;
			}


			PutVisaField (VISA_VISA_DISCRET_DATA, VisaInfo, sVisaDisData, nPos);
			*/
			/* VISA CERTIF
			if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			{
			sprintf(sLine,"VISA ISS is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
			PutVisaField (VISA_VISA_DISCRET_DATA, VisaInfo, tmpField + 2, tmpFieldLen - 2);
			}
			*/
			/* (137) */
			if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
				PutVisaField (VISA_APP_TRANS_COUNTER, VisaInfo, tmpField, tmpFieldLen);

		}

	}


	trace_event("End   IsoToVisaIcc(OK)", PROCESSING ) ;
	return(OK);

}

/***************************************************************************************/
/** Function :   **/
/** Spec     :   **/
/** Input    :   **/
/** Output   :   **/
/** Author   :   **/
/** Date     :   **/
/***************************************************************************************/
int 	VisaToIsoIcc	(
	TSVisaInfo  *p_VisaInfo,
	TSIsoInfo   *p_IsoInfo
	)
{
	TSTagIso       v_TagIso;
	char           tmpField[256];
	char           sTmp[256];
	char           sArc[256];
	int            tmpFieldLen;
	char           sIccBuffer[BUF_LG_MAX];
	char           sWork[BUF_LG_MAX];
	int			   nBytes;
	char           sLine[MAX_LINE_TRC];

	trace_event("Start VisaToIsoIcc()", PROCESSING ) ;

	memset (tmpField,     0, sizeof(tmpField));
	/** Retrieve ICC Data : ISO field 55 If Already Exist **/
	if (GetIsoField (ISO_ICC_DATA, p_IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitIsoIcTag (&v_TagIso);
		IsoDumpIccFields(tmpField,tmpFieldLen);
		AnalyseIsoIc (tmpField, tmpFieldLen, &v_TagIso);
	}
	else
		InitIsoIcTag (&v_TagIso);

	/* (130) */
	if( GetVisaField (VISA_TERM_CAPBLTY_PROFILE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TERM_CAP, &v_TagIso, tmpField, tmpFieldLen);


	/* (131) */
	if( GetVisaField (VISA_TERM_VERIF_RESULTS, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TVR, &v_TagIso, tmpField, tmpFieldLen);

	/* (132) */
	if( GetVisaField (VISA_UNPREDICT_NBR, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_UNPRED_NUMBER, &v_TagIso, tmpField, tmpFieldLen);

	/* (133) */		/*SEW280208 ENH April 2008
					if( GetVisaField (VISA_TERM_SERIAL_NBR, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &v_TagIso, tmpField, tmpFieldLen);*/

	/* (134.3) */
	if( GetVisaField (VISA_VISA_DISCRET_DATA, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
	{
		memset(sWork, 0, sizeof(sWork));
		memcpy(sWork, tmpField + 4, tmpFieldLen - 4);
		sprintf(sLine," VISA_VISA_DISCRET_DATA : %s|%d", sWork,tmpFieldLen - 4);
		trace_event(sLine,TRACE);

		/* 
		/ *
		nBytes = BcdToAscii( tmpField + 3, sWork, (tmpFieldLen - 3) * 2);
		nBytes *= 2;
		sprintf(sLine," VISA_VISA_DISCRET_DATA : %s|%d", sWork,nBytes);
		trace_event(sLine,TRACE);
		* /
		memcpy(sWork, "A0B800", 6); nBytes = 6;
		*/
		PutIsoIcTag (ISO_TAG_CVM, &v_TagIso, sWork, tmpFieldLen - 4);
	}

	/* (136) */
	if( GetVisaField (VISA_CRYPTOGRAM, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField, tmpFieldLen);

	/* (137) */
	if( GetVisaField (VISA_APP_TRANS_COUNTER, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);

	/* (138) */
	if( GetVisaField (VISA_APP_INTCHG_PROFILE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

	/* (139) */
	if( GetVisaField (VISA_ARPC_RESP_CODE , p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
	{
		sprintf(sLine,"VISA_ARPC_RESP_CODE : %s|%d", tmpField, tmpFieldLen);
		trace_event(sLine,TRACE);
		/* PutIsoIcTag (ISO_TAG_ARPC , &v_TagIso, tmpField, tmpFieldLen); */
		PutIsoIcTag (ISO_TAG_ARPC , &v_TagIso, tmpField, tmpFieldLen);
		PutIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &v_TagIso, tmpField, 16); 
		memset(sWork, 0, sizeof(sWork));
		AsciiToHex(tmpField + 16, sWork, 4);
		sprintf(sLine,"SWORK %02X|%02X",sWork[0], sWork[1]);
		trace_event(sLine,TRACE);
		memset(sTmp, 0, sizeof(sTmp));
		EbcdicToAscii(sWork, sTmp, 2);
		sprintf(sLine," STMP %s",sTmp);
		trace_event(sLine,TRACE);
		sprintf(sArc,"%02X%02X",sTmp[0],sTmp[1]);
		sprintf(sLine," SARC %s",sArc);
		trace_event(sLine,TRACE);
		PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE , &v_TagIso, sArc, 4); 
		/* PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE , &v_TagIso, tmpField + 16, 4); */
		/* PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE , &v_TagIso, "3030", 4);  */
	}
	/* (142) */
	if( GetVisaField (VISA_ISSUER_SCRIPT, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField, tmpFieldLen);
	/* (144) */
	if( GetVisaField (VISA_CRYPTO_TRANS_TYPE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TRANS_TYPE, &v_TagIso, tmpField, tmpFieldLen);

	/* (145) */
	if( GetVisaField (VISA_TERM_COUNTRY_CODE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &v_TagIso, tmpField, tmpFieldLen);

	/* (146) */
	if( GetVisaField (VISA_TERM_TRANS_DATE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TRANS_DATE, &v_TagIso, tmpField, tmpFieldLen);

	/* (147) */
	if( GetVisaField (VISA_CRYPTO_AMOUNT, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TRANS_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

	/* (148) */
	if( GetVisaField (VISA_CRYPTO_CRNCY_CODE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &v_TagIso, tmpField, tmpFieldLen);

	/* (149) */
	if( GetVisaField (VISA_CRYPTO_CBACK_AMOUNT, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
		PutIsoIcTag (ISO_TAG_OTHER_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

	/* ISO DF 55 */
	memset(sIccBuffer, 0, sizeof(sIccBuffer));
	tmpFieldLen = IsoBuildIcFld (sIccBuffer, &v_TagIso);
	IsoDumpIccFields(sIccBuffer ,tmpFieldLen);
	if ( tmpFieldLen > 0 )
		PutIsoField( ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);


	trace_event("End   VisaToIsoIcc(OK)", PROCESSING ) ;
	return(OK);
}


int 	ReversalIsoIcc 	(
	TSIsoInfo   *p_PosInfo,
	TSIsoInfo   *p_IsoInfo
	)
{
	TSTagIso       v_TagIso;
	TSTagIso       v_TagPos;
	char           tmpField[256];
	int            tmpFieldLen;
	char           sIccBuffer[BUF_LG_MAX];

	trace_event("Start PosIsoToIsoIcc()", PROCESSING);

	InitIsoIcTag (&v_TagIso);
	InitIsoIcTag (&v_TagPos);

	memset (tmpField,     0, sizeof(tmpField));

	/* ICC Data: to be read from ISO field 55 */
	if (GetIsoField (ISO_ICC_DATA, p_PosInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseIsoIc (tmpField, tmpFieldLen, &v_TagPos);

		/** 25. ISO_TAG_ATC **/
		if (GetIsoIcTag ( ISO_TAG_ATC, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);


		memset(sIccBuffer, 0, sizeof(sIccBuffer));
		tmpFieldLen = IsoBuildIcFld (sIccBuffer, &v_TagIso);
		IsoDumpIccFields(sIccBuffer ,tmpFieldLen);
		if ( tmpFieldLen > 0 )
			PutIsoField( ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);



	}


	trace_event("End   PosIsoToIsoIcc(OK)", PROCESSING);
	return(OK);
}

/**Start MKB221208***/
int     IsoToVisa55Icc  (
	TSVisaInfo  *VisaInfo,
	TSIsoInfo   *IsoInfo
	)
{
	char           tmpField[256];
	char           sLine[MAX_LINE_TRC];
	char           sIccBuffer[BUF_LG_MAX];
	int            tmpFieldLen;
	TSTagIso       TagIso;
	TSTagVisa       VisaIccInfo;

	memset (sLine,     0, sizeof(sLine));
	sprintf(sLine,"Start IsoToVisa55Icc()") ;
	trace_event(sLine,PROCESSING);

	memset (tmpField  ,     0, sizeof(tmpField));
	memset (sIccBuffer,     0, sizeof(sIccBuffer));
	DumpVisaStruct(VisaInfo);


	/** Retrieve ICC Data : ISO field 55 **/
	if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitIsoIcTag (&TagIso);
		IsoDumpIccFields(tmpField,tmpFieldLen);
		AnalyseIsoIc (tmpField, tmpFieldLen, &TagIso);

		InitVisaIcTag ( &VisaIccInfo );

		if ( GetIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_ISS_SCRIPT1 , &VisaIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_ISS_SCRIPT2 , &VisaIccInfo, tmpField,  tmpFieldLen );

		if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaIcTag (VISA_TAG_APP_INTER_PROFILE, &VisaIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaIcTag (VISA_TAG_AUTHOR_RESP_CODE, &VisaIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaIcTag (VISA_TAG_ISS_AUTHEN_DATA, &VisaIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaIcTag (VISA_TAG_TVR, &VisaIccInfo, tmpField, tmpFieldLen);


		if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaIcTag (VISA_TAG_TRANS_DATE, &VisaIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
		{
			if (tmpField[0] == '0' && tmpField[1] == '1') /* If cach advance */
				AddVisaIcTag (VISA_TAG_TRANS_TYPE, &VisaIccInfo, "17", 2);
			else
				AddVisaIcTag (VISA_TAG_TRANS_TYPE, &VisaIccInfo, tmpField, tmpFieldLen);
		}

		if ( GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_CUR_CODE , &VisaIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_AMOUNT , &VisaIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_OTHER_AMOUNT , &VisaIccInfo, tmpField,  tmpFieldLen );


		/*if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
		AddVisaIcTag (VISA_TAG_TERM_APP_VER_NUM , &VisaIccInfo, tmpField,  tmpFieldLen ); */

		if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag(VISA_TAG_ISS_APP_DATA , &VisaIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TERM_COUNTRY_CODE , &VisaIccInfo, tmpField,  tmpFieldLen );

		/*SEW280208 ENH April 2008
		if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
		AddVisaIcTag (VISA_TAG_IFD_SERIAL_NUM , &VisaIccInfo, tmpField,  tmpFieldLen );*/



		if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM,&TagIso,tmpField, &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_APP_CRYPTOGRAM, &VisaIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TERM_CAP , &VisaIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_CVM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_CVM , &VisaIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_TERM_TYPE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TERM_TYPE , &VisaIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_ATC , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_ATC , &VisaIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_UNPRED_NUMBER , &VisaIccInfo, tmpField,  tmpFieldLen );


		/*if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_CATEG_CODE , &VisaIccInfo, tmpField,  tmpFieldLen );*/

		tmpFieldLen = 0;
		tmpFieldLen = VisaBuildIcFld ( sIccBuffer, &VisaIccInfo);

		if ( tmpFieldLen > 0 )
			PutVisaField (VISA_ICC_DATA, VisaInfo, sIccBuffer, tmpFieldLen);


	}
	trace_event("End   IsoToVisa55Icc(OK)", PROCESSING ) ;
	return(OK);

}

int     VisaToIso55Icc  (
	TSVisaInfo  *p_VisaInfo,
	TSIsoInfo   *p_IsoInfo
	)
{

	char           sLine[MAX_LINE_TRC];
	char           tmpField[BUF_LG_MAX];
	char           IsotmpField[BUF_LG_MAX];
	char           sIccBuffer[BUF_LG_MAX];
	char           sVisaRespCode [ 2 + 1 ];
	int            tmpFieldLen;
	TSTagVisa      VISAIccInfo;
	TSTagIso       v_TagIso;


	trace_event("Start VisaToIso55Icc()", PROCESSING ) ;


	memset(sLine,         0, sizeof(sLine));
	memset (tmpField,     0, sizeof(tmpField));
	memset (IsotmpField,     0, sizeof(IsotmpField));
	memset(sIccBuffer,    0, sizeof(sIccBuffer));
	memset(sVisaRespCode, 0, sizeof(sVisaRespCode));

	InitIsoIcTag (&v_TagIso);

	if (GetIsoField (ISO_ICC_DATA, p_IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsoDumpIccFields(tmpField,tmpFieldLen);
		AnalyseIsoIc (tmpField, tmpFieldLen, &v_TagIso);
	}
	else
	{
		trace_event("End   VisaToIso55Icc(OK. NO ICC DATA)", PROCESSING ) ;
		return(OK);
	}


	if (GetVisaField (VISA_RESPONSE_CODE, p_VisaInfo, sVisaRespCode, &tmpFieldLen) == SUCCES )
	{
		sprintf(sLine, "sVisaRespCode(%s)", sVisaRespCode);
		trace_event(sLine, PROCESSING);
	}

	if (GetVisaField (VISA_ICC_DATA, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		VisaDumpIccFields(tmpField,tmpFieldLen);

		AnalyseVisaIc  ( tmpField, tmpFieldLen , &VISAIccInfo );

		trace_event ("Processing 55 field for response to POS", PROCESSING);

		trace_event ("tags sont recuperable si ils existent de l'iso interne", PROCESSING);
		

		/*if(GetVisaIcTag  (VISA_TAG_AUTHOR_RESP_CODE , &VISAIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso, tmpField,  tmpFieldLen );*//*PLUTONL-3230*/

		if(GetVisaIcTag  (VISA_TAG_ISS_AUTHEN_DATA , &VISAIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetVisaIcTag  (VISA_TAG_ISS_SCRIPT1 , &VISAIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT1, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetVisaIcTag  (VISA_TAG_ISS_SCRIPT2 , &VISAIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetVisaIcTag  (VISA_TAG_ISS_SCRIPT_CMD , &VISAIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT_CMD, &v_TagIso, tmpField,  tmpFieldLen );

        /*PLUTONL-3230*/
		/* When the trasaction is apporved Add the TAG 8A in the response */

		/*if ((sVisaRespCode[0] == '0') && (sVisaRespCode[1] == '0'))
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso, "00",  2);
		else
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sVisaRespCode,  2);

		tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
		if ( tmpFieldLen > 0 )
			PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);*/
		
		
		sprintf(sVisaRespCode,"%02X%02X",sVisaRespCode[0],sVisaRespCode[1]);
		PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sVisaRespCode,  4);
		/*PLUTONL-3230*/
		
		tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
		if ( tmpFieldLen > 0 )
			PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
	} 
	else
	{

		if (  (GetIsoField (ISO_ICC_DATA, p_IsoInfo, IsotmpField, &tmpFieldLen) == SUCCESS) )
		{
			trace_event ("No DE55 in VISA response", PROCESSING);


			/* InitIsoIcTag ( &v_TagIso );*/ /*KS YK 241005 */
			/*PLUTONL-3230*/
			/*if ((sVisaRespCode[0] == '0') && (sVisaRespCode[1] == '0'))
				PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso, "00",  2);
			else
				PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sVisaRespCode,  2);*/
			
			
			sprintf(sVisaRespCode,"%02X%02X",sVisaRespCode[0],sVisaRespCode[1]);
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sVisaRespCode,  4);
			/*PLUTONL-3230*/

			tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);

			if ( tmpFieldLen > 0 )
				PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
		}
	}


	
	memset (tmpField,     0, sizeof(tmpField));
	memset (IsotmpField,     0, sizeof(IsotmpField));
	memset(sIccBuffer,    0, sizeof(sIccBuffer));
	InitIsoIcTag (&v_TagIso);
	InitVisaIcTag(&VISAIccInfo);

	trace_event("End   VisaToIso55Icc(OK)", PROCESSING ) ;
	return(OK);
}


/**End MKB221208***/
