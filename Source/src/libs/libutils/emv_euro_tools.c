#include <define.h>
#include <iso_hps.h>
#include <iso_euro.h>
#include <iso_ictag.h>
#include <euro_ictag.h>
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
        EuroDumpIccFields(tmpField, tmpFieldLen);

       	InitEuroIcTag ( &v_TagEuro );
       	AnalyseEuroIc ( tmpField, tmpFieldLen , &v_TagEuro );
		InitIsoIcTag  ( &v_TagIso );

/** ISO_TAG_APP_CRYPTOGRAM **/
       	if(GetEuroIcTag (EURO_TAG_APP_CRYPTOGRAM,&v_TagEuro,tmpField, &tmpFieldLen ) == SUCCESS )
        	AddIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField,  tmpFieldLen );

/** ISO_TAG_CRYPTO_INFO_DATA **/
       	if(GetEuroIcTag (EURO_TAG_CRYPTO_INFO_DATA,&v_TagEuro,tmpField, &tmpFieldLen ) == SUCCESS )
        	AddIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA, &v_TagIso, tmpField,  tmpFieldLen );

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

        IsoDumpIccFields(sIccBuffer,tmpFieldLen);
   }
    
	trace_event("End   EuropayToIsoIcc(OK)", PROCESSING ) ;
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

/** EURO_TAG_CRYPTO_INFO_DATA **/
      
        /* 
            if(GetIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA,&v_TagEuro,tmpField, &tmpFieldLen ) == SUCCESS )
            AddEuroIcTag (EURO_TAG_CRYPTO_INFO_DATA, &v_TagIso, tmpField,  tmpFieldLen );
        */

        if(GetIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA,&v_TagIso,tmpField, &tmpFieldLen ) == SUCCESS )
            AddEuroIcTag (EURO_TAG_CRYPTO_INFO_DATA, &v_TagEuro, tmpField,  tmpFieldLen );/*Y.Benjira141204*/

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
