#include <define.h>
#include <iso_hps.h>
#include <iso_visa.h>
#include <iso_euro.h>
#include <iso_jcc.h>
#include <iso_pos.h>
#include <iso_ictag.h>
#include <euro_ictag.h>
#include <jcc_ictag.h>
#include <pos_ictag.h>
#include <visa_param.h>
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
int 	IsoToVisaIcc	(
							TSVisaInfo  *VisaInfo,
                         	TSIsoInfo   *IsoInfo,
							char         Type /*YK XXX*/
						)
{

	TSTagIso       TagIso;
	char           tmpField[256];
	int            tmpFieldLen;
	int			   ptr_id;

	trace_event("Start IsoToVisaIcc()", PROCESSING ) ;

	memset (tmpField,     0, sizeof(tmpField));

   /** Retrieve ICC Data : ISO field 55 **/
   if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
      InitIsoIcTag (&TagIso);
      AnalyseIsoIc (tmpField, tmpFieldLen, &TagIso);

      if( Type == 'N') 
	  {
	  /* (130) */
      if (GetIsoIcTag (ISO_TAG_TERM_CAP, &TagIso,tmpField,&tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_TERM_CAPBLTY_PROFILE, VisaInfo, tmpField, tmpFieldLen);

      /* (131) */
      if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddVisaField (VISA_TERM_VERIF_RESULTS, VisaInfo, tmpField, tmpFieldLen);

      /* (132) */
      if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddVisaField (VISA_UNPREDICT_NBR, VisaInfo, tmpField, tmpFieldLen);

      /* (133) */
      if (GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_TERM_SERIAL_NBR, VisaInfo, tmpField, tmpFieldLen);

      /* (134.3) */
      if (GetIsoIcTag (ISO_TAG_CVM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_VISA_DISCRET_DATA, VisaInfo, tmpField, tmpFieldLen);

      /* (136) */
      if (GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_CRYPTOGRAM, VisaInfo, tmpField, tmpFieldLen);
      }
      /* (137) */
      if (GetIsoIcTag (ISO_TAG_ATC, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_APP_TRANS_COUNTER, VisaInfo, tmpField, tmpFieldLen);

      if( Type == 'N')
	  {
	  /* (138) */
      if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_APP_INTCHG_PROFILE, VisaInfo, tmpField, tmpFieldLen);

      /* (144) */
      if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_CRYPTO_TRANS_TYPE, VisaInfo, tmpField, tmpFieldLen);

      /* (145) */
      if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_TERM_COUNTRY_CODE, VisaInfo, tmpField, tmpFieldLen);

      /* (146) */
      if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_TERM_TRANS_DATE, VisaInfo, tmpField, tmpFieldLen);

      /* (147) */
      if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_CRYPTO_AMOUNT, VisaInfo, tmpField, tmpFieldLen);

      /* (148) */
      if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_CRYPTO_CRNCY_CODE, VisaInfo, tmpField, tmpFieldLen);
   
      /* (149) */
      if (GetIsoIcTag (ISO_TAG_OTHER_AMOUNT, &TagIso, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField (VISA_CRYPTO_CBACK_AMOUNT, VisaInfo, tmpField, tmpFieldLen);
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
	int            tmpFieldLen;
	char           sIccBuffer[LG_MAX];

	trace_event("Start VisaToIsoIcc()", PROCESSING ) ;

	memset (tmpField,     0, sizeof(tmpField));

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

   /* (133) */
			if( GetVisaField (VISA_TERM_SERIAL_NBR, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_IFD_SERIAL_NUM, &v_TagIso, tmpField, tmpFieldLen);

   /* (134.3) */
			if( GetVisaField (VISA_VISA_DISCRET_DATA, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_CVM, &v_TagIso, tmpField, tmpFieldLen);

   /* (136) */
			if( GetVisaField (VISA_CRYPTOGRAM, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField, tmpFieldLen);

   /* (137) */
			if( GetVisaField (VISA_APP_TRANS_COUNTER, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);

   /* (138) */
			if( GetVisaField (VISA_APP_INTCHG_PROFILE, p_VisaInfo, tmpField, &tmpFieldLen) == SUCCESS )
					PutIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

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
        if ( tmpFieldLen > 0 )
			AddIsoField( ISO_ORIG_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);

	trace_event("End   VisaToIsoIcc(OK)", PROCESSING ) ;
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
int 	EuropayToIsoIcc	(
							TSEuroInfo  *p_EuroInfo,
							TSIsoInfo   *p_IsoInfo
						)
{

	char          tmpField[256];
	int           tmpFieldLen;
	TSTagIso      v_TagIso;
	TSTagEuro     v_TagEuro;
	char          sIccBuffer[LG_MAX];

	trace_event("Start EuropayToIsoIcc", PROCESSING ) ;

	memset(tmpField, 0, sizeof(tmpField));

/*-------------------------------------------------------------------------*/
   /* ICC Data: to be read from EUROPAY field 55 */
/*-------------------------------------------------------------------------*/
   if (GetEuroField (EURO_ICC_DATA, p_EuroInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
       	InitEuroIcTag ( &v_TagEuro );
       	AnalyseEuroIc ( tmpField, tmpFieldLen , &v_TagEuro );
		InitIsoIcTag  ( &v_TagIso );

/** ISO_TAG_APP_CRYPTOGRAM **/
       	if(GetEuroIcTag (EURO_TAG_APP_CRYPTOGRAM,&v_TagEuro,tmpField, &tmpFieldLen ) == SUCCESS )
        	AddIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_ISS_APP_DATA **/
     	if ( GetEuroIcTag (EURO_TAG_ISS_APP_DATA , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag(ISO_TAG_ISS_APP_DATA , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_UNPRED_NUMBER **/
     	if ( GetEuroIcTag (EURO_TAG_UNPRED_NUMBER , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_UNPRED_NUMBER , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_ATC **/
     	if ( GetEuroIcTag (EURO_TAG_ATC , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_ATC , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TVR **/
     	if ( GetEuroIcTag (EURO_TAG_TVR , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TVR , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_DATE **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_DATE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_DATE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_TYPE **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_TYPE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_AMOUNT **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_AMOUNT , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_CUR_CODE **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_CUR_CODE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_APP_INTER_PROFILE **/
     	if ( GetEuroIcTag (EURO_TAG_APP_INTER_PROFILE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_COUNTRY_CODE **/
     	if ( GetEuroIcTag (EURO_TAG_TERM_COUNTRY_CODE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_OTHER_AMOUNT **/
     	if ( GetEuroIcTag (EURO_TAG_OTHER_AMOUNT , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_OTHER_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_CAP **/
     	if ( GetEuroIcTag (EURO_TAG_TERM_CAP , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_CAP , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_CVM **/
     	if ( GetEuroIcTag (EURO_TAG_CVM , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
         	 AddIsoIcTag (ISO_TAG_CVM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_TYPE **/
     	if ( GetEuroIcTag (EURO_TAG_TERM_TYPE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_IFD_SERIAL_NUM **/
     	if ( GetEuroIcTag (EURO_TAG_IFD_SERIAL_NUM , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_CATEG_CODE **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_CATEG_CODE , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_DED_FILE_NAME **/
     	if ( GetEuroIcTag (EURO_TAG_DED_FILE_NAME , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_DED_FILE_NAME , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_APP_VER_NUM **/
     	if ( GetEuroIcTag (EURO_TAG_TERM_APP_VER_NUM , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_SEQ_NUM **/
     	if ( GetEuroIcTag (EURO_TAG_TRANS_SEQ_NUM , &v_TagEuro, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &v_TagIso, tmpField,  tmpFieldLen );

	    memset(sIccBuffer, 0, sizeof(sIccBuffer));

     	tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
        if ( tmpFieldLen > 0 )
     			AddIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
   }

	trace_event("End   IsoToEuropayIcc(OK)", PROCESSING ) ;
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
int 	IsoToEuropayIcc	(
							TSEuroInfo  *p_EuroInfo,
                         	TSIsoInfo   *p_IsoInfo
						)
{

	char          tmpField[256];
	int           tmpFieldLen;
	TSTagIso      v_TagIso;
	TSTagEuro     v_TagEuro;
	char          sIccBuffer[LG_MAX];

	trace_event("Start IsoToEuropayIcc", PROCESSING ) ;

	memset(tmpField, 0, sizeof(tmpField));

/*-------------------------------------------------------------------------*/
   /* ICC Data: to be read from ISO field 55 */
/*-------------------------------------------------------------------------*/
   if (GetIsoField (ISO_ICC_DATA, p_IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
		InitIsoIcTag  ( &v_TagIso );
       	AnalyseIsoIc  ( tmpField, tmpFieldLen , &v_TagIso );
       	InitEuroIcTag ( &v_TagEuro );

/** EURO_TAG_APP_CRYPTOGRAM **/
       	if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM,&v_TagIso,tmpField, &tmpFieldLen ) == SUCCESS )
        	AddEuroIcTag (EURO_TAG_APP_CRYPTOGRAM, &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_ISS_APP_DATA **/
     	if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag(EURO_TAG_ISS_APP_DATA , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_UNPRED_NUMBER **/
     	if ( GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_UNPRED_NUMBER , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_ATC **/
     	if ( GetIsoIcTag (ISO_TAG_ATC , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_ATC , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TVR **/
     	if ( GetIsoIcTag (ISO_TAG_TVR , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TVR , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_DATE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_DATE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_DATE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_TYPE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_TYPE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_TYPE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_AMOUNT **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_AMOUNT , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_CUR_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_CUR_CODE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_APP_INTER_PROFILE **/
     	if ( GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_APP_INTER_PROFILE , &v_TagEuro, tmpField,  tmpFieldLen );

/**  EURO_TAG_TERM_COUNTRY_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TERM_COUNTRY_CODE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_OTHER_AMOUNT **/
     	if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_OTHER_AMOUNT , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TERM_CAP **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TERM_CAP , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_CVM **/
     	if ( GetIsoIcTag (ISO_TAG_CVM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
         	 AddEuroIcTag (EURO_TAG_CVM , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TERM_TYPE **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_TYPE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TERM_TYPE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_IFD_SERIAL_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_IFD_SERIAL_NUM , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_CATEG_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_CATEG_CODE , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_DED_FILE_NAME **/
     	if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_DED_FILE_NAME , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TERM_APP_VER_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TERM_APP_VER_NUM , &v_TagEuro, tmpField,  tmpFieldLen );

/** EURO_TAG_TRANS_SEQ_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddEuroIcTag (EURO_TAG_TRANS_SEQ_NUM , &v_TagEuro, tmpField,  tmpFieldLen );

	    memset(sIccBuffer, 0, sizeof(sIccBuffer));

     	tmpFieldLen = EuroBuildIcFld ( sIccBuffer, &v_TagEuro);
        if ( tmpFieldLen > 0 )
     			AddEuroField (EURO_ICC_DATA, p_EuroInfo, sIccBuffer, tmpFieldLen);
   }

	trace_event("End   IsoToEuropayIcc(OK)", PROCESSING ) ;
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
int 	HidToIsoIcc 	(
							TSJccInfo   *p_JccInfo,
                         	TSIsoInfo   *p_IsoInfo
						)
{
	char          tmpField[256];
	int           tmpFieldLen;
	TSTagIso      v_TagIso;
	TSTagJcc      v_TagJcc;
	char          sIccBuffer[LG_MAX];

	trace_event("Start HidToIsoIcc()", PROCESSING);


	memset(tmpField, 0, sizeof(tmpField));

/*-------------------------------------------------------------------------*/
   /* ICC Data: to be read from JCC field 55 */
/*-------------------------------------------------------------------------*/
   if (GetJccField (JCC_ICC_DATA, p_JccInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
       	InitJccIcTag ( &v_TagJcc );
       	AnalyseJccIc ( tmpField, tmpFieldLen , &v_TagJcc );
		InitIsoIcTag  ( &v_TagIso );

/** ISO_TAG_APP_CRYPTOGRAM **/
       	if(GetJccIcTag (JCC_TAG_APP_CRYPTOGRAM,&v_TagJcc,tmpField, &tmpFieldLen ) == SUCCESS )
        	AddIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_ISS_APP_DATA **/
     	if ( GetJccIcTag (JCC_TAG_ISS_APP_DATA , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag(ISO_TAG_ISS_APP_DATA , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_UNPRED_NUMBER **/
     	if ( GetJccIcTag (JCC_TAG_UNPRED_NUMBER , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_UNPRED_NUMBER , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_ATC **/
     	if ( GetJccIcTag (JCC_TAG_ATC , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_ATC , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TVR **/
     	if ( GetJccIcTag (JCC_TAG_TVR , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TVR , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_DATE **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_DATE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_DATE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_TYPE **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_TYPE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_AMOUNT **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_AMOUNT , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_CUR_CODE **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_CUR_CODE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_APP_INTER_PROFILE **/
     	if ( GetJccIcTag (JCC_TAG_APP_INTER_PROFILE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_COUNTRY_CODE **/
     	if ( GetJccIcTag (JCC_TAG_TERM_COUNTRY_CODE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_OTHER_AMOUNT **/
     	if ( GetJccIcTag (JCC_TAG_OTHER_AMOUNT , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_OTHER_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_CAP **/
     	if ( GetJccIcTag (JCC_TAG_TERM_CAP , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_CAP , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_CVM **/
     	if ( GetJccIcTag (JCC_TAG_CVM , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
         	 AddIsoIcTag (ISO_TAG_CVM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_TYPE **/
     	if ( GetJccIcTag (JCC_TAG_TERM_TYPE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_IFD_SERIAL_NUM **/
     	if ( GetJccIcTag (JCC_TAG_IFD_SERIAL_NUM , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_CATEG_CODE **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_CATEG_CODE , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_DED_FILE_NAME **/
     	if ( GetJccIcTag (JCC_TAG_DED_FILE_NAME , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_DED_FILE_NAME , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TERM_APP_VER_NUM **/
     	if ( GetJccIcTag (JCC_TAG_TERM_APP_VER_NUM , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_TRANS_SEQ_NUM **/
     	if ( GetJccIcTag (JCC_TAG_TRANS_SEQ_NUM , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
          	AddIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &v_TagIso, tmpField,  tmpFieldLen );

/* ACK PIN CHANGE MARCH2015 START*/
	/*NEW PIN */
	if ( GetJccIcTag (JCC_TAG_SEC_PIN_DATA , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCES )
            AddIsoIcTag (ISO_TAG_SEC_PIN_DATA , &v_TagIso, tmpField,  tmpFieldLen );
       
/* ACK PIN CHANGE MARCH2015 END*/

/** ISO_TAG_FFI **/ /*ADD SWI06032015 ENH15.1*/
    	if ( GetJccIcTag (JCC_TAG_FFI , &v_TagJcc, tmpField,  &tmpFieldLen ) == SUCCESS )
            AddIsoIcTag (ISO_TAG_FFI , &v_TagIso, tmpField,  tmpFieldLen );

	    memset(sIccBuffer, 0, sizeof(sIccBuffer));

     	tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
        if ( tmpFieldLen > 0 )
     			AddIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
   }

	trace_event("End   HidToIsoIcc(OK)", PROCESSING);
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
int 	IsoToHidIcc 	(
							TSJccInfo   *p_JccInfo,
                         	TSIsoInfo   *p_IsoInfo
						)
{
	char          tmpField[256];
	int           tmpFieldLen;
	TSTagIso      v_TagIso;
	TSTagJcc      v_TagJcc;
	char          sIccBuffer[LG_MAX];

	trace_event("Start IsoToHidIcc()", PROCESSING);

memset(tmpField, 0, sizeof(tmpField));

/*-------------------------------------------------------------------------*/
   /* ICC Data: to be read from ISO field 55 */
/*-------------------------------------------------------------------------*/
   if (GetIsoField (ISO_ICC_DATA, p_IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
	InitIsoIcTag  ( &v_TagIso );
       	AnalyseIsoIc  ( tmpField, tmpFieldLen , &v_TagIso );
       	InitJccIcTag ( &v_TagJcc );

/** JCC_TAG_APP_CRYPTOGRAM **/
       	if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM,&v_TagIso,tmpField, &tmpFieldLen ) == SUCCESS )
        		AddJccIcTag (JCC_TAG_APP_CRYPTOGRAM, &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_ISS_APP_DATA **/
     	if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag(JCC_TAG_ISS_APP_DATA , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_UNPRED_NUMBER **/
     	if ( GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_UNPRED_NUMBER , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_ATC **/
     	if ( GetIsoIcTag (ISO_TAG_ATC , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_ATC , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TVR **/
     	if ( GetIsoIcTag (ISO_TAG_TVR , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TVR , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_DATE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_DATE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_DATE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_TYPE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_TYPE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_TYPE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_AMOUNT **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_AMOUNT , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_CUR_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_CUR_CODE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_APP_INTER_PROFILE **/
     	if ( GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_APP_INTER_PROFILE , &v_TagJcc, tmpField,  tmpFieldLen );

/**  JCC_TAG_TERM_COUNTRY_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TERM_COUNTRY_CODE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_OTHER_AMOUNT **/
     	if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_OTHER_AMOUNT , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TERM_CAP **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TERM_CAP , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_CVM **/
     	if ( GetIsoIcTag (ISO_TAG_CVM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
         	 	AddJccIcTag (JCC_TAG_CVM , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TERM_TYPE **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_TYPE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TERM_TYPE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_IFD_SERIAL_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_IFD_SERIAL_NUM , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_CATEG_CODE **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_CATEG_CODE , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_DED_FILE_NAME **/
     	if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_DED_FILE_NAME , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TERM_APP_VER_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TERM_APP_VER_NUM , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_TRANS_SEQ_NUM **/
     	if ( GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
          		AddJccIcTag (JCC_TAG_TRANS_SEQ_NUM , &v_TagJcc, tmpField,  tmpFieldLen );

/** JCC_TAG_FFI **/ /*SWI06032015 ENH15.1*/
    	if ( GetIsoIcTag (ISO_TAG_FFI , &v_TagIso, tmpField,  &tmpFieldLen ) == SUCCESS )
        		AddJccIcTag (JCC_TAG_FFI , &v_TagJcc, tmpField,  tmpFieldLen );

	    memset(sIccBuffer, 0, sizeof(sIccBuffer));

     	tmpFieldLen = JccBuildIcFld ( sIccBuffer, &v_TagJcc);
        if ( tmpFieldLen > 0 )
     			AddJccField (JCC_ICC_DATA, p_JccInfo, sIccBuffer, tmpFieldLen);
   }

	trace_event("End   IsoToHidIcc()", PROCESSING);
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
int 	PosIsoToIsoIcc 	(
							TSPosInfo   *p_PosInfo,
                         	TSIsoInfo   *p_IsoInfo
						)
{
	TSTagIso       v_TagIso;
	TSTagPos       v_TagPos;
	char           tmpField[256];
	int            tmpFieldLen;
	char           sIccBuffer[LG_MAX];

	trace_event("Start PosIsoToIsoIcc()", PROCESSING);

	InitIsoIcTag (&v_TagIso);
	InitPosIcTag (&v_TagPos);

	memset (tmpField,     0, sizeof(tmpField));

   /* ICC Data: to be read from ISO field 55 */
   if (GetPosField (POS_ICC_DATA, p_PosInfo, tmpField, &tmpFieldLen) == SUCCESS)
   {
   AnalysePosIc (tmpField, tmpFieldLen, &v_TagPos);

	/**  1. ISO_TAG_ISS_SCRIPT1  **/
	if (GetPosIcTag (POS_TAG_ISS_SCRIPT1 , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ISS_SCRIPT1, &v_TagIso, tmpField, tmpFieldLen);

	/**  2. ISO_TAG_ISS_SCRIPT2  **/
	if (GetPosIcTag (POS_TAG_ISS_SCRIPT2 , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField, tmpFieldLen);

	/**  3. ISO_TAG_ISS_SCRIPT_CMD **/
	if (GetPosIcTag (POS_TAG_ISS_SCRIPT_CMD , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ISS_SCRIPT_CMD, &v_TagIso, tmpField, tmpFieldLen);

	/**  4. ISO_TAG_APP_INTER_PROFILE **/
	if (GetPosIcTag (POS_TAG_APP_INTER_PROFILE , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_APP_INTER_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

	/**  5. ISO_TAG_DED_FILE_NAME **/
	if (GetPosIcTag (POS_TAG_DED_FILE_NAME , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_DED_FILE_NAME, &v_TagIso, tmpField, tmpFieldLen);

	/**  6. ISO_TAG_ICC_APP_ID **/
	if (GetPosIcTag ( POS_TAG_ICC_APP_ID, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag (ISO_TAG_ICC_APP_ID , &v_TagIso, tmpField, tmpFieldLen);

	/**  7. ISO_TAG_TERM_CAP_PROFILE **/
	if (GetPosIcTag (POS_TAG_TERM_CAP_PROFILE , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TERM_CAP_PROFILE, &v_TagIso, tmpField, tmpFieldLen);

	/**  8. ISO_TAG_AUTHOR_RESP_CODE **/
	if (GetPosIcTag (POS_TAG_AUTHOR_RESP_CODE , &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso, tmpField, tmpFieldLen);

	/**  9. ISO_TAG_ISS_AUTHEN_DATA **/
	if (GetPosIcTag ( POS_TAG_ISS_AUTHEN_DATA, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ISS_AUTHEN_DATA, &v_TagIso, tmpField, tmpFieldLen);

	/** 10. ISO_TAG_TVR **/
	if (GetPosIcTag ( POS_TAG_TVR, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TVR, &v_TagIso, tmpField, tmpFieldLen);

	/** 11. ISO_TAG_TRANS_DATE **/
	if (GetPosIcTag ( POS_TAG_TRANS_DATE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_DATE, &v_TagIso, tmpField, tmpFieldLen);

	/** 12. ISO_TAG_TRANS_TYPE **/
	if (GetPosIcTag ( POS_TAG_TRANS_TYPE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_TYPE, &v_TagIso, tmpField, tmpFieldLen);

	/** 13. ISO_TAG_TRANS_CUR_CODE **/
	if (GetPosIcTag ( POS_TAG_TRANS_CUR_CODE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_CUR_CODE, &v_TagIso, tmpField, tmpFieldLen);

	/** 14. ISO_TAG_TRANS_AMOUNT **/
	if (GetPosIcTag ( POS_TAG_TRANS_AMOUNT, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

	/** 15. ISO_TAG_OTHER_AMOUNT **/
	if (GetPosIcTag ( POS_TAG_OTHER_AMOUNT, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_OTHER_AMOUNT, &v_TagIso, tmpField, tmpFieldLen);

	/** 16. ISO_TAG_TERM_APP_VER_NUM **/
	if (GetPosIcTag ( POS_TAG_TERM_APP_VER_NUM, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TERM_APP_VER_NUM, &v_TagIso, tmpField, tmpFieldLen);

	/** 17. ISO_TAG_ISS_APP_DATA **/
	if (GetPosIcTag ( POS_TAG_ISS_APP_DATA, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ISS_APP_DATA, &v_TagIso, tmpField, tmpFieldLen);

	/** 18. ISO_TAG_TERM_COUNTRY_CODE **/
	if (GetPosIcTag ( POS_TAG_TERM_COUNTRY_CODE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TERM_COUNTRY_CODE, &v_TagIso, tmpField, tmpFieldLen);

	/** 19. ISO_TAG_IFD_SERIAL_NUM **/
	if (GetPosIcTag ( POS_TAG_IFD_SERIAL_NUM, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_IFD_SERIAL_NUM, &v_TagIso, tmpField, tmpFieldLen);

	/** 20. ISO_TAG_APP_CRYPTOGRAM **/
	if (GetPosIcTag ( POS_TAG_APP_CRYPTOGRAM, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField, tmpFieldLen);

	/** 21. ISO_TAG_CRYPTO_INFO_DATA **/
	if (GetPosIcTag ( POS_TAG_CRYPTO_INFO_DATA, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_CRYPTO_INFO_DATA, &v_TagIso, tmpField, tmpFieldLen);

	/** 22. ISO_TAG_TERM_CAP **/
	if (GetPosIcTag ( POS_TAG_TERM_CAP, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TERM_CAP, &v_TagIso, tmpField, tmpFieldLen);

	/** 23. ISO_TAG_CVM **/
	if (GetPosIcTag ( POS_TAG_CVM, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_CVM, &v_TagIso, tmpField, tmpFieldLen);

	/** 24. ISO_TAG_TERM_TYPE **/
	if (GetPosIcTag ( POS_TAG_TERM_TYPE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TERM_TYPE, &v_TagIso, tmpField, tmpFieldLen);

	/** 25. ISO_TAG_ATC **/
	if (GetPosIcTag ( POS_TAG_ATC, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_ATC, &v_TagIso, tmpField, tmpFieldLen);

	/** 26. ISO_TAG_UNPRED_NUMBER **/
	if (GetPosIcTag ( POS_TAG_UNPRED_NUMBER, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_UNPRED_NUMBER, &v_TagIso, tmpField, tmpFieldLen);

	/** 27. ISO_TAG_TRANS_SEQ_NUM **/
	if (GetPosIcTag ( POS_TAG_TRANS_SEQ_NUM, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_SEQ_NUM, &v_TagIso, tmpField, tmpFieldLen);

	/** 28. ISO_TAG_TRANS_CATEG_CODE **/
	if (GetPosIcTag ( POS_TAG_TRANS_CATEG_CODE, &v_TagPos,tmpField,&tmpFieldLen) == SUCCESS)
			PutIsoIcTag ( ISO_TAG_TRANS_CATEG_CODE, &v_TagIso, tmpField, tmpFieldLen);


   /* ISO DF 55 */
	        memset(sIccBuffer, 0, sizeof(sIccBuffer));
			tmpFieldLen = IsoBuildIcFld (sIccBuffer, &v_TagIso);
        if ( tmpFieldLen > 0 )
			AddIsoField( ISO_ORIG_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);

   }
	

	trace_event("End   PosIsoToIsoIcc(OK)", PROCESSING);
	return(OK);
}


