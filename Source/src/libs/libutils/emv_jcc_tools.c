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

	    memset(sIccBuffer, 0, sizeof(sIccBuffer));

     	tmpFieldLen = JccBuildIcFld ( sIccBuffer, &v_TagJcc);
        if ( tmpFieldLen > 0 )
     			AddJccField (JCC_ICC_DATA, p_JccInfo, sIccBuffer, tmpFieldLen);
   }

	trace_event("End   IsoToHidIcc()", PROCESSING);
	return(OK);

}
