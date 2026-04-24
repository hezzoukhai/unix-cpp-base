/*******************************************************************************/
/*  (c) Hightech Payment Systems 2009                                          */
/* Module                                                                      */
/*      bic_emv_tools.c                                                        */
/*                                                                             */
/* Description                                                                 */
/*      Insert Text Tools Management                                           */
/*				        				       */
/* Author								       */
/*      RHA								       */
/*******************************************************************************/

#include <define.h>
#include <iso_hps.h>
#include <iso_bic.h>
#include <iso_ictag.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <bic_fields.h>


int 	IsoToBic55Icc	(
	TSBicInfo  *BicInfo,
	TSIsoInfo   *IsoInfo
	)
{


	char           tmpField[256];	
	char           sIccBuffer[LG_MAX];
	int            tmpFieldLen;
	TSTagIso       TagIso;
	TSTagBic       BicIccInfo;



	trace_event("Start IsoToBic55Icc()",PROCESSING);

	memset (tmpField  ,     0, sizeof(tmpField));
	memset (sIccBuffer,     0, sizeof(sIccBuffer));

	InitIsoIcTag (&TagIso);
	InitBicIcTag (&BicIccInfo );

	DumpStructBic(BicInfo);

	if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsoDumpIccFields(tmpField,tmpFieldLen);
		AnalyseIsoIc (tmpField, tmpFieldLen, &TagIso);

		if ( GetIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_ISS_SCRIPT1 , &BicIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_ISS_SCRIPT2 , &BicIccInfo, tmpField,  tmpFieldLen );

		if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_APP_INTER_PROFILE, &BicIccInfo, tmpField, tmpFieldLen);

		if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_DED_FILE_NAME , &BicIccInfo, tmpField,  tmpFieldLen );

		if (GetIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_AUTHOR_RESP_CODE, &BicIccInfo, tmpField, tmpFieldLen);


		if (GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_ISS_AUTHEN_DATA, &BicIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_TVR, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_TVR, &BicIccInfo, tmpField, tmpFieldLen);


		if (GetIsoIcTag (ISO_TAG_TRANS_DATE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_TRANS_DATE, &BicIccInfo, tmpField, tmpFieldLen);

		if (GetIsoIcTag (ISO_TAG_TRANS_TYPE, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_TRANS_TYPE, &BicIccInfo, tmpField, tmpFieldLen);


		if ( GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TRANS_CUR_CODE , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TRANS_AMOUNT , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES ) 
			AddBicIcTag (BIC_TAG_OTHER_AMOUNT , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TERM_APP_VER_NUM , &BicIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag(BIC_TAG_ISS_APP_DATA , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TERM_COUNTRY_CODE , &BicIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_IFD_SERIAL_NUM , &BicIccInfo, tmpField,  tmpFieldLen );



		if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM,&TagIso,tmpField, &tmpFieldLen ) == SUCCES ) 
			AddBicIcTag (BIC_TAG_APP_CRYPTOGRAM, &BicIccInfo, tmpField,  tmpFieldLen );



		if (GetIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA, &TagIso, tmpField, &tmpFieldLen) == SUCCESS) 
			AddBicIcTag (BIC_TAG_CRYPTO_INFO_DATA, &BicIccInfo, tmpField, tmpFieldLen);


		if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TERM_CAP , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_CVM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_CVM , &BicIccInfo, tmpField,  tmpFieldLen );

		if ( GetIsoIcTag (ISO_TAG_TERM_TYPE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TERM_TYPE , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_ATC , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_ATC , &BicIccInfo, tmpField,  tmpFieldLen );


		if ( GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_UNPRED_NUMBER , &BicIccInfo, tmpField,  tmpFieldLen );



		if ( GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TRANS_SEQ_NUM , &BicIccInfo, tmpField,  tmpFieldLen ); 


		if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &TagIso, tmpField,  &tmpFieldLen ) == SUCCES )
			AddBicIcTag (BIC_TAG_TRANS_CATEG_CODE , &BicIccInfo, tmpField,  tmpFieldLen );

		tmpFieldLen = 0;
		tmpFieldLen = BicBuildIcFld ( sIccBuffer, &BicIccInfo);

		if ( tmpFieldLen > 0 )
			PutBicField (BIC_ICC_DATA, BicInfo, sIccBuffer, tmpFieldLen);
	}
	else
	{
		trace_event("----->NO ISO_ICC_DATA<-----", PROCESSING ) ;
	}


	trace_event("End   IsoToBic55Icc(OK)", PROCESSING ) ;
	return(OK);

}


int 	BicToIso55Icc	(
	TSBicInfo  *p_BicInfo,
	TSIsoInfo   *p_IsoInfo
	)
{

	char           tmpField[LG_MAX];
	char           sIccBuffer[LG_MAX];    

	int            tmpFieldLen;
	TSTagBic      BicIccInfo;
	TSTagIso       v_TagIso;


	trace_event("Start BicToIso55Icc()", PROCESSING ) ;


	memset (tmpField,     0, sizeof(tmpField));	
	memset(sIccBuffer,    0, sizeof(sIccBuffer));

	InitBicIcTag  ( &BicIccInfo );
	InitIsoIcTag ( &v_TagIso );    



	if (GetBicField (BIC_ICC_DATA, p_BicInfo, tmpField, &tmpFieldLen) == SUCCES)
	{ 

		BicDumpIccFields(tmpField,tmpFieldLen);

		AnalyseBicIc  ( tmpField, tmpFieldLen , &BicIccInfo );

		if ( GetBicIcTag (BIC_TAG_DED_FILE_NAME , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_DED_FILE_NAME , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_APP_INTER_PROFILE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &v_TagIso, tmpField,  tmpFieldLen ); 

		if(GetBicIcTag  (BIC_TAG_AUTHOR_RESP_CODE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &v_TagIso, tmpField,  tmpFieldLen );  

		if(GetBicIcTag  (BIC_TAG_ISS_AUTHEN_DATA , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &v_TagIso, tmpField,  tmpFieldLen ); 

		if(GetBicIcTag  (BIC_TAG_ISS_SCRIPT1 , &BicIccInfo, tmpField,  &tmpFieldLen )  
			== SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_SCRIPT1, &v_TagIso, tmpField,  tmpFieldLen ); 

		if(GetBicIcTag  (BIC_TAG_ISS_SCRIPT2 , &BicIccInfo, tmpField,  &tmpFieldLen )  
			== SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_SCRIPT2, &v_TagIso, tmpField,  tmpFieldLen ); 

		if(GetBicIcTag  (BIC_TAG_ISS_SCRIPT_CMD , &BicIccInfo, tmpField,  &tmpFieldLen ) 
			== SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_SCRIPT_CMD, &v_TagIso, tmpField,  tmpFieldLen ); 


		if(GetBicIcTag  (BIC_TAG_APP_CRYPTOGRAM , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &v_TagIso, tmpField,  tmpFieldLen );

		if (GetBicIcTag (BIC_TAG_CRYPTO_INFO_DATA, &BicIccInfo, tmpField, &tmpFieldLen) == SUCCESS) 
			AddIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA, &v_TagIso, tmpField,  tmpFieldLen );	   

		if ( GetBicIcTag (BIC_TAG_ISS_APP_DATA , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag(ISO_TAG_ISS_APP_DATA , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_UNPRED_NUMBER , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_UNPRED_NUMBER , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_ATC , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_ATC , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TVR , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TVR , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_DATE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_DATE , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_TYPE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_AMOUNT , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_CUR_CODE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &v_TagIso, tmpField,  tmpFieldLen );


		if ( GetBicIcTag (BIC_TAG_TERM_COUNTRY_CODE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_OTHER_AMOUNT , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_OTHER_AMOUNT , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TERM_CAP , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TERM_CAP , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_CVM , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_CVM , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TERM_TYPE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TERM_TYPE , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_IFD_SERIAL_NUM , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_CATEG_CODE , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &v_TagIso, tmpField,  tmpFieldLen );


		if ( GetBicIcTag (BIC_TAG_TERM_APP_VER_NUM , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &v_TagIso, tmpField,  tmpFieldLen );

		if ( GetBicIcTag (BIC_TAG_TRANS_SEQ_NUM , &BicIccInfo, tmpField,  &tmpFieldLen )
			== SUCCES )
			AddIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &v_TagIso, tmpField,  tmpFieldLen );



		tmpFieldLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
		if ( tmpFieldLen > 0 )
		{
			PutIsoField (ISO_ICC_DATA, p_IsoInfo, sIccBuffer, tmpFieldLen);
			IsoDumpIccFields(sIccBuffer,tmpFieldLen);
		}
	}
	else
	{
		trace_event("----->NO BIC_ICC_DATA<------", PROCESSING ) ;
	}


	trace_event("End   BicToIso55Icc(OK)", PROCESSING ) ;
	return(OK);
}


