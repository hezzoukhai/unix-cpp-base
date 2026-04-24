#include <define.h>
#define SMART_CARD
#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_jcc.h>
#include <iso_pos.h>
#include <iso_ictag.h>
#include <sms_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <sms_ictag.h>


/***************************************************************************************/
/** Function :   **/
/** Spec     :   **/
/** Input    :   **/
/** Output   :   **/
/** Author   :   **/
/** Date     :   **/
/***************************************************************************************/
int 	IsoToSmsIcc	(
							TSSmsInfo  *SmsInfo,
                         	TSIsoInfo   *IsoInfo,
							char	     Mode
						)
{

	TSTagIso       TagIso;
	char           tmpField[256];
	int            tmpFieldLen;
	int			   ptr_id;
    char           sLine[MAX_LINE_TRC];
    char           sSmsDisData[LG_MAX];
    char           sWork[LG_MAX];
    int			   nBytes;
    int			   nPos,i;

	sprintf(sLine,"Start IsoToSmsIcc(%c)", Mode ) ;
	trace_event(sLine,PROCESSING);

	memset (tmpField,     0, sizeof(tmpField));
        DumpSmsStruct(SmsInfo);


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
			/* PutSmsField (SMS_TERM_CAPBLTY_PROFILE, SmsInfo, "E0F0C0", 6); */
		 PutSmsField (SMS_TERM_CAPBLTY_PROFILE, SmsInfo, tmpField, tmpFieldLen); 

      /* (131) */
      if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
     {
			sprintf(sLine,"SMS TVR is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
			PutSmsField (SMS_TERM_VERIF_RESULTS, SmsInfo, tmpField, tmpFieldLen);
     }

      /* (132) */
      if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			PutSmsField (SMS_UNPREDICT_NBR, SmsInfo, tmpField, tmpFieldLen);

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

			PutSmsField (SMS_TERM_SERIAL_NBR, SmsInfo, sWork, 16);
	  }
*/

      /* (134.3) */
/*
	  memset(sSmsDisData, 0, sizeof(sSmsDisData));
      nPos = 0;
      sSmsDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
      sSmsDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
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
         *(sSmsDisData + nPos ) = nBytes;nPos += 1;
         memcpy(sSmsDisData + nPos, sWork, nBytes); nPos += nBytes;
	  }
*/

      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
      {
			sprintf(sLine,"SMS ISS is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
	  PutSmsField (SMS_VISA_DISCRET_DATA, SmsInfo, tmpField + 2, tmpFieldLen - 2);
     }

      /* (135) */
/*
      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_ISSUER_DISCRET_DATA, SmsInfo, tmpField, tmpFieldLen);
*/

      /* (136) */
      if (GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_CRYPTOGRAM, SmsInfo, tmpField, tmpFieldLen);

      /* (137) */
      if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_APP_TRANS_COUNTER, SmsInfo, tmpField, tmpFieldLen);

      /* (138) */
      if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_APP_INTCHG_PROFILE, SmsInfo, tmpField, tmpFieldLen);

      /* (144) */
      if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_CRYPTO_TRANS_TYPE, SmsInfo, tmpField, tmpFieldLen);

      /* (145) */
      if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_TERM_COUNTRY_CODE, SmsInfo, tmpField, tmpFieldLen);

      /* (146) */
      if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_TERM_TRANS_DATE, SmsInfo, tmpField, tmpFieldLen);

      /* (147) */
      if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_CRYPTO_AMOUNT, SmsInfo, tmpField, tmpFieldLen);

      /* (148) */
      if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_CRYPTO_CRNCY_CODE, SmsInfo, tmpField, tmpFieldLen);
   
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
/* SMS CERTIF
      if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_TERM_VERIF_RESULTS, SmsInfo, tmpField, tmpFieldLen);
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

			PutSmsField (SMS_TERM_SERIAL_NBR, SmsInfo, sWork, 16);
	  }
*/

      /* (134.3) */
/*
	  memset(sSmsDisData, 0, sizeof(sSmsDisData));
      nPos = 0;
      sSmsDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
      sSmsDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
	  tmpFieldLen = 0;
      if (GetIsoIcTag (ISO_TAG_CVM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
	  {
		 memset(sWork, 0, sizeof(sWork));
		 nBytes = AsciiToBcd( tmpField, sWork, tmpFieldLen);
         *(sSmsDisData + nPos ) = nBytes;nPos += 1;
         memcpy(sSmsDisData + nPos, sWork, nBytes); nPos += nBytes;
	  }


	  PutSmsField (SMS_VISA_DISCRET_DATA, SmsInfo, sSmsDisData, nPos);
*/
/* SMS CERTIF
      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
      {
			sprintf(sLine,"SMS ISS is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
	  PutSmsField (SMS_VISA_DISCRET_DATA, SmsInfo, tmpField + 2, tmpFieldLen - 2);
     }
*/
      /* (137) */
      if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutSmsField (SMS_APP_TRANS_COUNTER, SmsInfo, tmpField, tmpFieldLen);

   }

}


	trace_event("End   IsoToSmsIcc(OK)", PROCESSING ) ;
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
int 	SmsToIsoIcc	(
							TSSmsInfo  *p_SmsInfo,
                         	TSIsoInfo   *p_IsoInfo
						)
{
	TSTagIso       v_TagIso;
	char           tmpField[256];
	char           sTmp[256];
	char           sArc[256];
	int            tmpFieldLen;
	char           sIccBuffer[LG_MAX];
	char           sWork[LG_MAX];
	int			   nBytes;
    char           sLine[MAX_LINE_TRC];

	trace_event("Start SmsToIsoIcc()", PROCESSING ) ;

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
			if( GetSmsField (SMS_TERM_CAPBLTY_PROFILE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TERM_CAP, &v_TagIso, tmpField, tmpFieldLen);


   /* (131) */
			if( GetSmsField (SMS_TERM_VERIF_RESULTS, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TVR, &v_TagIso, tmpField, tmpFieldLen);

   /* (132) */
			if( GetSmsField (SMS_UNPREDICT_NBR, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_UNPRED_NUMBER, &v_TagIso, tmpField, tmpFieldLen);

   /* (133) */
			if( GetSmsField (SMS_TERM_SERIAL_NBR, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &v_TagIso, tmpField, tmpFieldLen);

   /* (134.3) */
			if( GetSmsField (SMS_VISA_DISCRET_DATA, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
		            memset(sWork, 0, sizeof(sWork));
					memcpy(sWork, tmpField + 4, tmpFieldLen - 4);
					sprintf(sLine," SMS_VISA_DISCRET_DATA : %s|%d", sWork,tmpFieldLen - 4);
					trace_event(sLine,TRACE);

				  /* 
				/ *
                    nBytes = BcdToAscii( tmpField + 3, sWork, (tmpFieldLen - 3) * 2);
					nBytes *= 2;
					sprintf(sLine," SMS_VISA_DISCRET_DATA : %s|%d", sWork,nBytes);
					trace_event(sLine,TRACE);
			 * /
					memcpy(sWork, "A0B800", 6); nBytes = 6;
					*/
					PutIsoIcTag (ISO_TAG_CVM, &v_TagIso, sWork, tmpFieldLen - 4);
			}

   /* (136) */
			if( GetSmsField (SMS_CRYPTOGRAM, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField, tmpFieldLen);

   /* (137) */
			if( GetSmsField (SMS_APP_TRANS_COUNTER, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);

   /* (138) */
			if( GetSmsField (SMS_APP_INTCHG_PROFILE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

   /* (139) */
			if( GetSmsField (SMS_ARPC_RESP_CODE , p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
					sprintf(sLine,"SMS_ARPC_RESP_CODE : %s|%d", tmpField, tmpFieldLen);
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
			if( GetSmsField (SMS_ISSUER_SCRIPT, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField, tmpFieldLen);
   /* (144) */
			if( GetSmsField (SMS_CRYPTO_TRANS_TYPE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_TYPE, &v_TagIso, tmpField, tmpFieldLen);

   /* (145) */
			if( GetSmsField (SMS_TERM_COUNTRY_CODE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &v_TagIso, tmpField, tmpFieldLen);

   /* (146) */
			if( GetSmsField (SMS_TERM_TRANS_DATE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_DATE, &v_TagIso, tmpField, tmpFieldLen);

   /* (147) */
			if( GetSmsField (SMS_CRYPTO_AMOUNT, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

   /* (148) */
			if( GetSmsField (SMS_CRYPTO_CRNCY_CODE, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &v_TagIso, tmpField, tmpFieldLen);
   

   /* ISO DF 55 */
	        memset(sIccBuffer, 0, sizeof(sIccBuffer));
			tmpFieldLen = IsoBuildIcFld (sIccBuffer, &v_TagIso);
	        IsoDumpIccFields(sIccBuffer ,tmpFieldLen);
        if ( tmpFieldLen > 0 )
			PutIsoField( ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);


	trace_event("End   SmsToIsoIcc(OK)", PROCESSING ) ;
	return(OK);
}

int     SmsToIso55Icc  (
	TSSmsInfo  *p_SmsInfo,
	TSIsoInfo   *p_IsoInfo
	)
{

	char           sLine[MAX_LINE_TRC];
	char           tmpField[BUF_LG_MAX];
	char           IsotmpField[BUF_LG_MAX];
	char           sIccBuffer[BUF_LG_MAX];
	char           sSmsRespCode [ 2 + 1 ];
	int            tmpFieldLen;
	TSTagSms      SMSIccInfo;
	TSTagIso       v_TagIso;
	int				nRespCodeExists = 0;


	trace_event("Start SmsToIso55Icc()", PROCESSING ) ;


	memset(sLine,         0, sizeof(sLine));
	memset (tmpField,     0, sizeof(tmpField));
	memset (IsotmpField,     0, sizeof(IsotmpField));
	memset(sIccBuffer,    0, sizeof(sIccBuffer));
	memset(sSmsRespCode, 0, sizeof(sSmsRespCode));

	InitIsoIcTag (&v_TagIso);

	if (GetIsoField (ISO_ICC_DATA, p_IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsoDumpIccFields(tmpField,tmpFieldLen);
		AnalyseIsoIc (tmpField, tmpFieldLen, &v_TagIso);
	}
	else
	{
		trace_event("End   SmsToIso55Icc(OK. NO ICC DATA)", PROCESSING ) ;
		return(OK);
	}


	if (GetSmsField (SMS_RESPONSE_CODE, p_SmsInfo, sSmsRespCode, &tmpFieldLen) == SUCCES )
	{
		sprintf(sLine, "sSmsRespCode(%s)", sSmsRespCode);
		trace_event(sLine, PROCESSING);
		nRespCodeExists = 1;
	}

	if (GetSmsField (SMS_ICC_DATA, p_SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		SmsDumpIccFields(tmpField,tmpFieldLen);

		AnalyseSmsIc  ( tmpField, tmpFieldLen , &SMSIccInfo );

		trace_event ("Processing 55 field for response to POS", PROCESSING);

		trace_event ("tags sont recuperable si ils existent de l'iso interne", PROCESSING);

		if(GetSmsIcTag  (SMS_TAG_ISS_AUTHEN_DATA , &SMSIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetSmsIcTag  (SMS_TAG_ISS_SCRIPT1 , &SMSIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT1, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetSmsIcTag  (SMS_TAG_ISS_SCRIPT2 , &SMSIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField,  tmpFieldLen );

		if(GetSmsIcTag  (SMS_TAG_ISS_SCRIPT_CMD , &SMSIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			PutIsoIcTag (ISO_TAG_ISS_SCRIPT_CMD, &v_TagIso, tmpField,  tmpFieldLen );

		if (nRespCodeExists == 1 )
		{
			sprintf(sSmsRespCode,"%02X%02X",sSmsRespCode[0],sSmsRespCode[1]);
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sSmsRespCode,  4);
		}
		
		tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
		if ( tmpFieldLen > 0 )
			PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
	} 
	else
	{

		if (  (GetIsoField (ISO_ICC_DATA, p_IsoInfo, IsotmpField, &tmpFieldLen) == SUCCESS) )
		{
			trace_event ("No DE55 in VISA response", PROCESSING);

		if (nRespCodeExists == 1 )
		{
			sprintf(sSmsRespCode,"%02X%02X",sSmsRespCode[0],sSmsRespCode[1]);
			PutIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso,  sSmsRespCode,  4);
		}
			tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);

			if ( tmpFieldLen > 0 )
				PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
		}
	}

	
	memset (tmpField,     0, sizeof(tmpField));
	memset (IsotmpField,     0, sizeof(IsotmpField));
	memset(sIccBuffer,    0, sizeof(sIccBuffer));
	InitIsoIcTag (&v_TagIso);
	InitSmsIcTag(&SMSIccInfo);

	trace_event("End   SmsToIso55Icc(OK)", PROCESSING ) ;
	return(OK);
}
