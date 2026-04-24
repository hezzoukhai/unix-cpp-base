#include <define.h>
#include <iso_hps.h>
#include <iso_cis.h>
#include <iso_pos.h>
#include <iso_ictag.h>
#include <cis_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>


/***************************************************************************************/
/** Function :   **/
/** Spec     :   **/
/** Input    :   **/
/** Output   :   **/
/** Author   :   **/
/** Date     :   **/
/***************************************************************************************/
int 	IsoToCisIcc	(
							TSCisInfo  *CisInfo,
                         	TSIsoInfo   *IsoInfo,
							char	     Mode
						)
{

	TSTagIso       TagIso;
	char           tmpField[256];
	int            tmpFieldLen;
	int			   ptr_id;
    char           sLine[MAX_LINE_TRC];
    char           sCisDisData[LG_MAX];
    char           sWork[LG_MAX];
    int			   nBytes;
    int			   nPos,i;

	sprintf(sLine,"Start IsoToCisIcc(%c)", Mode ) ;
	trace_event(sLine,PROCESSING);

	memset (tmpField,     0, sizeof(tmpField));

    DumpCisStruct(CisInfo);


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
			/* PutCisField (CIS_TERM_CAPBLTY_PROFILE, CisInfo, "E0F0C0", 6); */
		 PutCisField (CIS_TERM_CAPBLTY_PROFILE, CisInfo, tmpField, tmpFieldLen); 

      /* (131) */
      if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
     {
			sprintf(sLine,"CIS TVR is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
			PutCisField (CIS_TERM_VERIF_RESULTS, CisInfo, tmpField, tmpFieldLen);
     }

      /* (132) */
      if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			PutCisField (CIS_UNPREDICT_NBR, CisInfo, tmpField, tmpFieldLen);

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

			PutCisField (CIS_TERM_SERIAL_NBR, CisInfo, sWork, 16);
	  }
*/


      /* (134.3) */
/*
	  memset(sCisDisData, 0, sizeof(sCisDisData));
      nPos = 0;
      sCisDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
      sCisDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
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
         *(sCisDisData + nPos ) = nBytes;nPos += 1;
         memcpy(sCisDisData + nPos, sWork, nBytes); nPos += nBytes;
	  }
*/

      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
      {
			sprintf(sLine,"CIS ISS is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
	  PutCisField (CIS_CIS_DISCRET_DATA, CisInfo, tmpField + 2, tmpFieldLen - 2);
     }

      /* (135) */
/*
      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_ISSUER_DISCRET_DATA, CisInfo, tmpField, tmpFieldLen);
*/

      /* (136) */
      if (GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_CRYPTOGRAM, CisInfo, tmpField, tmpFieldLen);

      /* (137) */
      if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_APP_TRANS_COUNTER, CisInfo, tmpField, tmpFieldLen);

      /* (138) */
      if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_APP_INTCHG_PROFILE, CisInfo, tmpField, tmpFieldLen);

      /* (144) */
      if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_CRYPTO_TRANS_TYPE, CisInfo, tmpField, tmpFieldLen);

      /* (145) */
      if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_TERM_COUNTRY_CODE, CisInfo, tmpField, tmpFieldLen);

      /* (146) */
      if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_TERM_TRANS_DATE, CisInfo, tmpField, tmpFieldLen);

      /* (147) */
      if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_CRYPTO_AMOUNT, CisInfo, tmpField, tmpFieldLen);

      /* (148) */
      if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_CRYPTO_CRNCY_CODE, CisInfo, tmpField, tmpFieldLen);
   
      /* (149) */
      if (GetIsoIcTag (ISO_TAG_OTHER_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
	  {
		/*	PutCisField (CIS_OTHER_AMOUNTS, CisInfo, tmpField, tmpFieldLen); */
			PutCisField (CIS_CRYPTO_CBACK_AMOUNT, CisInfo, tmpField, tmpFieldLen);
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
/* CIS CERTIF
      if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_TERM_VERIF_RESULTS, CisInfo, tmpField, tmpFieldLen);
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

			PutCisField (CIS_TERM_SERIAL_NBR, CisInfo, sWork, 16);
	  }
*/

      /* (134.3) */
/*
	  memset(sCisDisData, 0, sizeof(sCisDisData));
      nPos = 0;
      sCisDisData[0] = 0X01; / * Derivation Key * / nPos += 1;
      sCisDisData[1] = 0X0A; / * Cryptogram Version * / nPos += 1;
	  tmpFieldLen = 0;
      if (GetIsoIcTag (ISO_TAG_CVM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
	  {
		 memset(sWork, 0, sizeof(sWork));
		 nBytes = AsciiToBcd( tmpField, sWork, tmpFieldLen);
         *(sCisDisData + nPos ) = nBytes;nPos += 1;
         memcpy(sCisDisData + nPos, sWork, nBytes); nPos += nBytes;
	  }


	  PutCisField (CIS_CIS_DISCRET_DATA, CisInfo, sCisDisData, nPos);
*/
/* CIS CERTIF
      if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
      {
			sprintf(sLine,"CIS ISS is %d|%s",tmpFieldLen,tmpField);
			trace_event(sLine,TRACE);
	  PutCisField (CIS_CIS_DISCRET_DATA, CisInfo, tmpField + 2, tmpFieldLen - 2);
     }
*/
      /* (137) */
      if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			PutCisField (CIS_APP_TRANS_COUNTER, CisInfo, tmpField, tmpFieldLen);

   }

}


	trace_event("End   IsoToCisIcc(OK)", PROCESSING ) ;
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
int 	CisToIsoIcc	(
							TSCisInfo  *p_CisInfo,
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

	trace_event("Start CisToIsoIcc()", PROCESSING ) ;

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
			if( GetCisField (CIS_TERM_CAPBLTY_PROFILE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TERM_CAP, &v_TagIso, tmpField, tmpFieldLen);


   /* (131) */
			if( GetCisField (CIS_TERM_VERIF_RESULTS, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TVR, &v_TagIso, tmpField, tmpFieldLen);

   /* (132) */
			if( GetCisField (CIS_UNPREDICT_NBR, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_UNPRED_NUMBER, &v_TagIso, tmpField, tmpFieldLen);

   /* (133) */
			if( GetCisField (CIS_TERM_SERIAL_NBR, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &v_TagIso, tmpField, tmpFieldLen);

   /* (134.3) */
			if( GetCisField (CIS_CIS_DISCRET_DATA, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
		            memset(sWork, 0, sizeof(sWork));
					memcpy(sWork, tmpField + 4, tmpFieldLen - 4);
					sprintf(sLine," CIS_CIS_DISCRET_DATA : %s|%d", sWork,tmpFieldLen - 4);
					trace_event(sLine,TRACE);

				  /* 
				/ *
                    nBytes = BcdToAscii( tmpField + 3, sWork, (tmpFieldLen - 3) * 2);
					nBytes *= 2;
					sprintf(sLine," CIS_CIS_DISCRET_DATA : %s|%d", sWork,nBytes);
					trace_event(sLine,TRACE);
			 * /
					memcpy(sWork, "A0B800", 6); nBytes = 6;
					*/
					PutIsoIcTag (ISO_TAG_CVM, &v_TagIso, sWork, tmpFieldLen - 4);
			}

   /* (136) */
			if( GetCisField (CIS_CRYPTOGRAM, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField, tmpFieldLen);

   /* (137) */
			if( GetCisField (CIS_APP_TRANS_COUNTER, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);

   /* (138) */
			if( GetCisField (CIS_APP_INTCHG_PROFILE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

   /* (139) */
			if( GetCisField (CIS_ARPC_RESP_CODE , p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
					sprintf(sLine,"CIS_ARPC_RESP_CODE : %s|%d", tmpField, tmpFieldLen);
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
			if( GetCisField (CIS_ISSUER_SCRIPT, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField, tmpFieldLen);
   /* (144) */
			if( GetCisField (CIS_CRYPTO_TRANS_TYPE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_TYPE, &v_TagIso, tmpField, tmpFieldLen);

   /* (145) */
			if( GetCisField (CIS_TERM_COUNTRY_CODE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &v_TagIso, tmpField, tmpFieldLen);

   /* (146) */
			if( GetCisField (CIS_TERM_TRANS_DATE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_DATE, &v_TagIso, tmpField, tmpFieldLen);

   /* (147) */
			if( GetCisField (CIS_CRYPTO_AMOUNT, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

   /* (148) */
			if( GetCisField (CIS_CRYPTO_CRNCY_CODE, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &v_TagIso, tmpField, tmpFieldLen);
   
   /* (149) */
			if( GetCisField (CIS_CRYPTO_CBACK_AMOUNT, p_CisInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_OTHER_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

   /* ISO DF 55 */
	        memset(sIccBuffer, 0, sizeof(sIccBuffer));
			tmpFieldLen = IsoBuildIcFld (sIccBuffer, &v_TagIso);
	        IsoDumpIccFields(sIccBuffer ,tmpFieldLen);
        if ( tmpFieldLen > 0 )
			PutIsoField( ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);


	trace_event("End   CisToIsoIcc(OK)", PROCESSING ) ;
	return(OK);
}
