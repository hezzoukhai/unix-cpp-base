
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      tlv_to_iso.c                                                           */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Function to convert TLV data to ISO PWC data                        */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   17/04/2002 -         V 2.0.1                              */
/*        Update On ISO PWC filed (16) ISO_CNVRSN_DATE                         */
/*        See Start  HY V2.01 170402 1,2 and 3)                                */
/*        H.YOUSFI   10/05/2002 -         V 2.0.2                              */
/*        Retreive the new sttklement date for reversal messages               */
/*        See Start  HY V2.01 100502                                           */
/*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <general_macros.h>
#include <msg_flow_mapping.h>
#include <hps_iso_8583.h>
#include <p7_sid.h> /*HAL13102020 PLUTONL-2352 */
#include <p7_define.h>


void IccTlvToIso(char *szBufferIn,  int nLengthIn,
				 char *szBufferOut, int *nLengthOut)
{
	TSTagIso      v_TagIso;
	char          sIccBuffer[LG_MAX];
	char          sWork[LG_MAX];
	char          sLine[LG_MAX];
	int			  nLen;
	TSTlvPrivate tlvInfo;

	trace_event("Start IccTlvToIso()",PROCESSING);

	InitIsoIcTag  ( &v_TagIso );
	memset(sIccBuffer, 0, sizeof(sIccBuffer));
	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));
	InitTlvPrivate (&tlvInfo);
	AnalyseTlvPrivate(szBufferIn ,nLengthIn,&tlvInfo);


	if (GetTlvPrivate (CHIP_AIP_TAG, &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_APP_INTER_PROFILE,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_APPLI_ID_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ICC_APP_ID,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TERM_CAPABILITY_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TERM_CAP_PROFILE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TVR_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TVR ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TRANS_DATE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_DATE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TRANS_TYPE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_TYPE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TRANS_CUR_CODE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_CUR_CODE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TRANS_AMOUNT_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_AMOUNT ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_OTHER_AMOUNT_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_OTHER_AMOUNT ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_ISS_APPLI_DATA_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ISS_APP_DATA ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TERM_COUNTRY_CODE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_IFD_SERIAL_NUM_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_IFD_SERIAL_NUM ,&v_TagIso , sWork, nLen) ;

	/* START MTR - Enh4.12 */
	if (GetTlvPrivate (CHIP_APPL_EXP_DATE, &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag(ISO_TAG_APPLI_EXPIRY_DATE,&v_TagIso , sWork, nLen) ;
	/* End MTR - Enh4.12 */

	/* START MTR - Enh4.11 */
 
        if (GetTlvPrivate (EMV_TAG_APP_RGSTD_DAT , &tlvInfo, sWork, &nLen) == SUCCESS)
                AddIsoIcTag (ISO_TAG_APP_RGSTD_DATA ,&v_TagIso , sWork, nLen) ;
        
	/* End MTR - Enh4.11 */

	if (GetTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_APP_CRYPTOGRAM ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_CRYPTO_INFO_DATA_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_CVM_RESULTS_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_CVM ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TERM_TYPE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TERM_TYPE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_ATC_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ATC ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_UNPRED_NUMBER_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_UNPRED_NUMBER ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_TRX_SEQ_COUNT_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_SEQ_NUM ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_CATEGORY_CODE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_TRANS_CATEG_CODE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ARPC ,&v_TagIso , sWork, nLen) ;

	/* Issuer script TAGs Mehdi ELyajizi */
	if (GetTlvPrivate (CHIP_ISS_SCRIPT_RESULT_TAG , &tlvInfo, sWork, &nLen) == SUCCESS) 
		AddIsoIcTag (ISO_TAG_ISS_SCRIPT_RESULT ,&v_TagIso , sWork, nLen) ;	

	if (GetTlvPrivate (CHIP_ISS_SCRIPT_1 , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_ISS_SCRIPT_2 , &tlvInfo, sWork, &nLen) == SUCCESS)
		AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 ,&v_TagIso , sWork, nLen) ;

        if (GetTlvPrivate (CHIP_AUTHOR_RESP_CODE , &tlvInfo, sWork, &nLen) == SUCCESS)
                AddIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE ,&v_TagIso , sWork, nLen) ;

	if (GetTlvPrivate (CHIP_APP_SELECTED_TAG  , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APP_SELECTED ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APP_SELECTED",FATAL); 

	if (GetTlvPrivate (CHIP_TRACK2_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_TRACK2 ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_TRACK2",FATAL); 

	if (GetTlvPrivate (CHIP_PAN_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APP_PAN ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APP_PAN",FATAL); 

	if (GetTlvPrivate (CHIP_APP_NAME_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APP_NAME ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APP_NAME",FATAL); 

	if (GetTlvPrivate (CHIP_APP_LABEL_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APP_LABEL ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APP_LABEL",FATAL); 

	if (GetTlvPrivate (CHIP_SECUR_ATT_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_ICC ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_ICC",FATAL); 
	/**END LHI*****/
	if (GetTlvPrivate (CHIP_TAG_KERNEL_ID , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_KERNEL_ID ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_KERNEL_ID",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_ICC_PROCESSING_RESULT , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_ICC_PROCESSING_RESULT ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_ICC_PROCESSING_RESULT",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_APPL_TYPE , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APPL_TYPE ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APPL_TYPE",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_RTT_RESULT , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_RTT_RESULT ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_RTT_RESULT",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_CONTACTLESS_DEVICE , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_CONTACTLESS_DEVICE ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_CONTACTLESS_DEVICE",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_EMV_ACQ_MODE , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_EMV_ACQ_MODE ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_EMV_ACQ_MODE",FATAL); 

	if (GetTlvPrivate (CHIP_TAG_CARD_AUTH_RES_CODE , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_CARD_AUTH_RES_CODE ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_CARD_AUTH_RES_CODE",FATAL); 

	if (GetTlvPrivate (CHIP_DED_FILE_NAME_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_DED_FILE_NAME ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_DED_FILE_NAME",FATAL); 
			
	if (GetTlvPrivate (CHIP_APP_USE_CONTROL_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_APP_USAGE_CTRL ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_APP_USAGE_CTRL",FATAL); 
	
	if (GetTlvPrivate (CHIP_APPLI_VER_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_TERM_APP_VER_NUM ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_TERM_APP_VER_NUM",FATAL);
			
	if (GetTlvPrivate (CHIP_TAG_TRACK_1_DATA , &tlvInfo, sWork, &nLen) == SUCCESS)
		if(AddIsoIcTag (ISO_TAG_TRACK_1_DATA ,&v_TagIso , sWork, nLen)  !=SUCCES)
			trace_event("Erreur Put TAG: ISO_TAG_TRACK_1_DATA",FATAL);

        if (GetTlvPrivate (CHIP_FFI_TAG , &tlvInfo, sWork, &nLen) == SUCCESS)
                AddIsoIcTag (ISO_TAG_FFI ,&v_TagIso , sWork, nLen);
         		
        if (GetTlvPrivate (CHIP_PAN_SEQ_NBR , &tlvInfo, sWork, &nLen) == SUCCESS)
            AddIsoIcTag ( ISO_TAG_PAN_SEQ_NBR,&v_TagIso , sWork, nLen);
 
        


	nLen = IsoBuildIcFld ( sIccBuffer, &v_TagIso);
	IsoDumpIccFields(sIccBuffer,nLen);
	memcpy(szBufferOut, sIccBuffer, nLen);
	*nLengthOut = nLen;
	
	/* PCI */
	InitIsoIcTag  ( &v_TagIso );
	memset(sIccBuffer, 0, sizeof(sIccBuffer));
	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));
	InitTlvPrivate (&tlvInfo);
	trace_event("End   IccTlvToIso(void)",PROCESSING);
	return;
}

int TlvToIso ( int nIndiceCtx , TSTlvAutho *tlvInfo, TSIsoInfo * isoInfo , char sens )
{

	char			tmpField  [LG_MAX];
	char			secResult [32+1];
	char			sLine  [MAX_LINE_TRC];
	char			sTlvPrivateBuffer [ 2048 +1 ];
	char			sResponse [ 3 + 1 ];
	char			sAmount   [18+1];
	char			sCurrency [3+1];
	char			sOrigAmounts [ P7_F030_LEN + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	TSTlvPrivate    sTlvPrivateInfo1;
	int				nRetCode;
	int				field_48_presence = 0;
	int				nResponseLength;
	int				nLength;
	char			sWork [LG_MAX];
	char			sMti[4 + 1];
	char			sOrigMti[4 + 1];
	int				nLen;
	char            sNetSetlmntInfo [NET_SETL_INFO_L +1];

	memset ( tmpField , 0 , sizeof ( tmpField ));
	memset ( secResult , 0 , sizeof ( secResult ));
	memset ( sLine , 0 , sizeof ( sLine ));
	memset ( sTlvPrivateBuffer , 0 , sizeof ( sTlvPrivateBuffer ));
	memset ( sResponse , 0 , sizeof ( sResponse ));
	memset ( sAmount , 0 , sizeof ( sAmount ));
	memset ( sCurrency , 0 , sizeof ( sCurrency ));
	memset ( sOrigAmounts , 0 , sizeof ( sOrigAmounts ));
	memset ( sWork             , 0 , sizeof ( sWork ));
	memset(sNetSetlmntInfo, 0, sizeof(sNetSetlmntInfo)); 	

	trace_event ( "TlvToIso( start ) ", PROCESSING );
	
	InitTlvPrivate (&sTlvPrivateInfo1);

	if ( GetTlvAutho(AUTO_ADD_DATA_PRIVATE ,tlvInfo,sTlvPrivateBuffer ,&nLength)==SUCCES )
	{
		field_48_presence = 1;
		InitTlvPrivate (&sTlvPrivateInfo);
		AnalyseTlvPrivate(sTlvPrivateBuffer,nLength,&sTlvPrivateInfo );
	}
	else 
	{
		field_48_presence = 0;
		InitTlvPrivate ( &sTlvPrivateInfo );
	}

	if ( sens == REPEAT )
	{
		/*******************************************************************************/
		/*  Transmission date time       */
		/*******************************************************************************/
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho( AUTO_TRANS_DATE_TIME , tlvInfo,tmpField,&nLength) ==SUCCES )
			PutIsoField ( ISO_XMIT_TIME , isoInfo , tmpField , nLength );
		/*******************************************************************************/
		/*  Trace Audit Number           */
		/*******************************************************************************/
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho( AUTO_INTERNAL_STAN , tlvInfo,tmpField,&nLength) ==SUCCES )
			PutIsoField ( ISO_AUDIT_NBR , isoInfo , tmpField , nLength );
		/*******************************************************************************/
		/*  Reference Number             */
		/*******************************************************************************/
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho( AUTO_RRN , tlvInfo,tmpField,&nLength) ==SUCCES )
			PutIsoField ( ISO_REFERENCE_NBR , isoInfo , tmpField , nLength );

		/*******************************************************************************/
		/*  POS DATA                     */
		/*******************************************************************************/
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho( AUTO_POS_DATA , tlvInfo,tmpField,&nLength) ==SUCCES )
			PutIsoField ( ISO_POS_DATA , isoInfo , tmpField , nLength );
	}

	/*******************************************************************************/
	/*  Transaction amount      */
	/*******************************************************************************/
	memset ( sAmount , 0 , sizeof ( sAmount ));
	if ( GetTlvAutho ( AUTO_AMOUNT_TRANSACTION , tlvInfo, sAmount , &nLength) == SUCCES )
	{
		memset ( sCurrency , 0 , sizeof ( sCurrency ));
		if ( GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
		{
			memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
			nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
			if ( nRetCode != OK ) return ( NOK );
			PutIsoField ( ISO_TRANS_AMOUNT , isoInfo , tmpField , 12 );
			/*PutIsoField ( ISO_STLMNT_CRNCY , isoInfo , sCurrency ,  3 );*/
			PutIsoField ( ISO_TRANS_CRNCY , isoInfo , sCurrency ,  3 );
		}
	}
	/*******************************************************************************/
	/*  Settlement Data Informations */
	/*******************************************************************************/
	if ( sens == REQUEST )
	{
		memset ( sAmount , 0 , sizeof ( sAmount ));
		if ( GetTlvAutho ( AUTO_ISS_STLMNT_AMOUNT , tlvInfo, sAmount , &nLength) == SUCCES )
		{
			memset ( sCurrency , 0 , sizeof ( sCurrency ));
			if ( GetTlvAutho( AUTO_ISS_STLMNT_CURRENCY , tlvInfo, sCurrency , &nLength) == SUCCES )
			{
				memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/  
				nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
				if ( nRetCode != OK ) return ( NOK );
				PutIsoField ( ISO_STLMNT_AMOUNT , isoInfo , tmpField , 12 );
				PutIsoField ( ISO_STLMNT_CRNCY , isoInfo , sCurrency ,  3 );
			}
		}
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho(AUTO_ISS_CNV_RATE_STLMNT ,tlvInfo,tmpField,&nLength) == SUCCES )
			PutIsoField ( ISO_STLMNT_CNVRSN_RATE , isoInfo , tmpField , 8 );
		memset ( tmpField , 0 , sizeof ( tmpField ));
		/* Start 1 HY V2.01 170402 */
		/* if ( GetTlvAutho(AUTO_ISS_CNV_RAT_STL_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) */
		if ( GetTlvAutho(AUTO_ISS_STLMNT_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) 
		{
			sprintf(sLine,"Settlement Date IS %s ",tmpField);
			trace_event(sLine,TRACE);
			PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField + 2 , 6 ); 
			/* PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField , 4 ); */
		}
		memset ( tmpField , 0 , sizeof ( tmpField ));
		/* if ( GetTlvAutho(AUTO_ISS_CNV_RAT_STL_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) */
		if ( GetTlvAutho(AUTO_CONVERSION_DATE ,tlvInfo,tmpField,&nLength) == SUCCES )
			PutIsoField ( ISO_CNVRSN_DATE , isoInfo , tmpField , 4 );
		/* End  1 HY V2.01 170402 */
		
		/*ICH20180108 recuperer la nouvelle valeur de laod_org_data au lieu de celle recue dans le msg*/
		if ( GetTlvAutho(AUTO_ORIGINAL_DATA ,tlvInfo,tmpField,&nLength) == SUCCES )
			PutIsoField ( ISO_ORIG_DATA , isoInfo , tmpField , nLength );
		
		/*ZKO 21072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
		if ( GetTlvAutho(AUTO_PAN_EXTENDED_COUNTRY ,tlvInfo,tmpField,&nLength) == SUCCES )
			PutIsoField (ISO_PAN_COUNTRY , isoInfo , tmpField , nLength );
		/*ZKO 21072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

		/*Start IBO20221121 PLUTONL-5416*/
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho(AUTO_ORIGIN_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
		{
			PutTlvPrivate(ORIGIN_CODE ,&sTlvPrivateInfo,tmpField,nLength);
			field_48_presence = 1;

			if (tmpField[0] == '0' || tmpField[0] == '1' || tmpField[0] == '2')
			{
				memset ( tmpField , 0 , sizeof ( tmpField ));
				if ( GetTlvAutho(AUTO_CR_FIRST_DUE_DATE ,tlvInfo,tmpField ,&nLength)==SUCCES )
				{
					PutTlvPrivate(BANK_XMIT_TIME_TAG ,&sTlvPrivateInfo,tmpField,nLength);
				}
			}
			else
			{
				memset ( tmpField , 0 , sizeof ( tmpField ));
				if ( GetTlvAutho(AUTO_ORIG_TRANS_DATE_TIME ,tlvInfo,tmpField ,&nLength)==SUCCES )
				{
					PutTlvPrivate(BANK_XMIT_TIME_TAG ,&sTlvPrivateInfo,tmpField,nLength);
				}
			}
		}
		/*End IBO20221121 PLUTONL-5416*/
	}
	else if ( sens == RESPONSE )
	{
		memset ( sAmount , 0 , sizeof ( sAmount ));
		memset ( tmpField , 0 , sizeof ( tmpField )); 
		memset ( sCurrency , 0 , sizeof ( sCurrency ));

		if (GetTlvPrivate (NET_ISS_SETTL_INFO, &sTlvPrivateInfo, sNetSetlmntInfo, &nLength) == SUCCESS)
		{
			memcpy(sCurrency, sNetSetlmntInfo, NET_SETL_CRNCY_L);
			memcpy(sAmount, sNetSetlmntInfo + NET_SETL_AMOUNT_O, NET_SETL_AMOUNT_L);

			nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
			if ( nRetCode != OK ) return ( NOK );
			PutIsoField ( ISO_STLMNT_AMOUNT , isoInfo , tmpField , 12 );
			PutIsoField ( ISO_STLMNT_CRNCY , isoInfo , sCurrency ,  3 );

			memcpy(tmpField, sNetSetlmntInfo + NET_SETL_CONV_RATE_O, NET_SETL_CONV_RATE_L);
			PutIsoField ( ISO_STLMNT_CNVRSN_RATE , isoInfo , tmpField , 8 );

			memset ( tmpField , 0 , sizeof ( tmpField ));
			memcpy(tmpField, sNetSetlmntInfo + NET_SETL_DATE_O, NET_SETL_DATE_L);
			PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField + 2 , 6 );
		}
		else if ( GetTlvAutho ( AUTO_ACQ_STLMNT_AMOUNT , tlvInfo, sAmount , &nLength) == SUCCES )
		{
			if ( GetTlvAutho( AUTO_ACQ_STLMNT_CURRENCY , tlvInfo, sCurrency , &nLength) == SUCCES )
			{
				nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
				if ( nRetCode != OK ) return ( NOK );
				PutIsoField ( ISO_STLMNT_AMOUNT , isoInfo , tmpField , 12 );
				PutIsoField ( ISO_STLMNT_CRNCY , isoInfo , sCurrency ,  3 );
			}
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if ( GetTlvAutho(AUTO_ACQ_CNV_RATE_STLMNT ,tlvInfo,tmpField,&nLength) == SUCCES )
				PutIsoField ( ISO_STLMNT_CNVRSN_RATE , isoInfo , tmpField , 8 );
			memset ( tmpField , 0 , sizeof ( tmpField ));
			if ( GetTlvAutho(AUTO_ACQ_STLMNT_DATE ,tlvInfo,tmpField,&nLength) == SUCCES )
				PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField + 2 , 6 );
		}
		
		memset ( tmpField , 0 , sizeof ( tmpField ));
		/* Start 2 HY V2.01 170402 */
		/* if ( GetTlvAutho(AUTO_ACQ_CNV_RAT_STL_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) */
		if ( GetTlvAutho(AUTO_CONVERSION_DATE ,tlvInfo,tmpField,&nLength) == SUCCES )
			/* End   2 HY V2.01 170402 */
			PutIsoField ( ISO_CNVRSN_DATE , isoInfo , tmpField , 4 );

		/*Start EBE150908: if we removed the repeat indicator on iso to tlv, put it back here*/
		GetTlvAutho(AUTO_MSG_TYPE ,tlvInfo,sMti,&nLength);
		if( (sMti[MSG_TYPE_CLASS] == MSG_TYPE_P2_AUT || sMti[MSG_TYPE_CLASS] == MSG_TYPE_P2_FIN) &&
			sMti[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_REQ)
		{
			if( GetTlvAutho(AUTO_ORIGINAL_DATA ,tlvInfo,tmpField,&nLength) == SUCCESS )
			{
				if( tmpField[MSG_TYPE_TRX_ORIGIN] == MSG_TYPE_P4_ACQ_REP )
				{
					tmpField[MSG_TYPE_TRX_ORIGIN] = MSG_TYPE_P4_ACQ;
					tmpField[4]='\0';
					isoInfo->nMsgType = atoi(tmpField);
				}
			}
		}
	}

	/* Start  HY V2.01 100502 */
	if ( sens == REVERSAL )
	{
		memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 	
		if ( GetTlvAutho(AUTO_SETTLEMENT_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) 
		{
			sprintf(sLine,"Settlement Date IS %s ",tmpField);
			trace_event(sLine,TRACE);
			PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField , 6 );
		}
	}
	/* End   HY V2.01 170502 */

	if ( sens == RESPONSE )
	{
		memset ( sResponse , 0 , sizeof ( sResponse ));
		if ( GetTlvAutho(AUTO_ORIG_ACTION_CODE ,tlvInfo,sResponse ,&nResponseLength) == SUCCES )
		{     
			memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
			if ( GetIsoField (ISO_NET_POS_DATA, isoInfo, tmpField, &nLength ) == SUCCES )
				AnalyseTlvPrivate ( tmpField , nLength , &sTlvPrivateInfo1);
			else
				InitTlvPrivate (&sTlvPrivateInfo1);

			PutTlvPrivate (RESPONSE_CODE_TAG, &sTlvPrivateInfo1, sResponse , nResponseLength);
			memset ( tmpField , 0 , sizeof ( tmpField ));
			nLength = PrivateBuildTlv (tmpField, &sTlvPrivateInfo1);
			PutIsoField (ISO_NET_POS_DATA, isoInfo, tmpField, nLength);
		}
	}

	/*******************************************************************************/
	/*  Billing    Data Informations */
	/*******************************************************************************/
	memset ( sAmount , 0 , sizeof ( sAmount ));
	if ( GetTlvAutho(AUTO_AMOUNT_CARD_BILLING , tlvInfo, sAmount , &nLength) == SUCCES )
	{
		memset ( sCurrency , 0 , sizeof ( sCurrency ));
		if ( GetTlvAutho( AUTO_BILL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
		{
			memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/  
			nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
			if ( nRetCode != OK ) return ( NOK );
			PutIsoField ( ISO_BILLING_AMOUNT , isoInfo , tmpField , 12 );
			PutIsoField ( ISO_BILLING_CRNCY , isoInfo , sCurrency ,  3 );
		}
		memset ( tmpField , 0 , sizeof ( tmpField ));
		if ( GetTlvAutho(AUTO_CONV_RATE_CARD_BILL ,tlvInfo,tmpField,&nLength) == SUCCES )
			PutIsoField ( ISO_BILLING_CNVRSN_RATE , isoInfo , tmpField , 8 );
	}

	/*******************************************************************************/
	/*  Function Code                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_FUNCTION_CODE , tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_FUNCTION_CODE , isoInfo , tmpField , nLength ); 

	/*******************************************************************************/
	/*  Replacement Amount Infos     */
	/*******************************************************************************/
	memset ( sAmount , 0 , sizeof ( sAmount ));
	memset ( sOrigAmounts , 0       , sizeof ( sOrigAmounts));
	if ( GetTlvAutho(AUTO_REPLACEMENT_BILLING , tlvInfo, sAmount , &nLength) == SUCCES )
	{
		if ( GetTlvAutho( AUTO_BILL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
		{   
			memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
			nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
			if ( nRetCode != OK ) return ( NOK );
			memcpy ( sOrigAmounts + 12 , tmpField , 12 );

			memset ( sAmount   , 0 , sizeof ( sAmount ));
			memset ( sCurrency , 0 , sizeof ( sCurrency ));
			GetTlvAutho( AUTO_REPLACEMENT_AMOUNT , tlvInfo, sAmount  , &nLength);
			GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , tlvInfo, sCurrency , &nLength); 
			memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
			nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
			if ( nRetCode != OK ) return ( NOK );
			memcpy ( sOrigAmounts , tmpField , 12 );
			
			if (GetTlvAutho(AUTO_SETL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES 
			     && GetTlvAutho( AUTO_REPLACEMENT_SETTL , tlvInfo,  sAmount, &nLength) == SUCCES)
			{
				memset ( tmpField , 0 , sizeof ( tmpField ));
				nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
				if ( nRetCode != OK ) 
					return ( NOK );
			
				memcpy ( sOrigAmounts + 24 , tmpField , 12 );
			}
			
			PutIsoField (ISO_ORIGINAL_AMOUNTS , isoInfo, sOrigAmounts , P7_F030_LEN);
		}
	}

	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_ACQ_INST_IDENT_CODE,tlvInfo,tmpField,&nLength) == SUCCES )
		PutIsoField ( ISO_ACQR_ID , isoInfo , tmpField , nLength );


	/*******************************************************************************/
	/* TRACKIII from tlv to iso*/
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_TRACK3_DATA,tlvInfo,tmpField,&nLength) == SUCCES )
		PutIsoField ( ISO_T3_DATA , isoInfo , tmpField , nLength );

	/*******************************************************************************/
	/*  Authorisation Number         */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_AUTH_NUMBER,tlvInfo,tmpField,&nLength) == SUCCES )
		PutIsoField ( ISO_AUTHOR_ID , isoInfo , tmpField , nLength );

	/*******************************************************************************/
	/*  Response Code                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_RESPONSE_CODE ,tlvInfo,tmpField,&nLength) == SUCCES )
		InsertIsoField ( ISO_RESPONSE_CODE , isoInfo , tmpField , nLength );

	/*******************************************************************************/
	/*  Cvv1/Cvc2 Result Code Flags  */
	/*******************************************************************************/
	/*Start SNO070416 Enh4.10*/	/*AMER20190409 PROD00067907: CBAE Enh5.00: We should not overwite EVENT_ACTION
	by EVENT_ACTION, because this field is used by CBAE to send F39 for advice and reversal responses*/
	/*if ( GetTlvAutho(AUTO_EVENT_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
	{
		PutTlvPrivate(EVENT_ACTION ,&sTlvPrivateInfo,tmpField,3); 
		trace_message(TRACE,"====== YES [%s] ======",tmpField);
		/*ICH20171207 need to put action code is moved to GET_ACTIOn_CODE* /
	}*/
	/*Start SNO070416 */

	/*Start EBE140117*/
	/*
	if ( GetTlvAutho(AUTO_CAPTURE_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
	{
		PutTlvPrivate(CAPTURE_CODE ,&sTlvPrivateInfo,tmpField,6);
		field_48_presence = 1;
	}*/
	if ( GetTlvAutho(AUTO_NETWORK_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
	{
		PutTlvPrivate(NETWORK_CODE ,&sTlvPrivateInfo,tmpField,2);
		field_48_presence = 1;
	}
	/*End EBE140117*/

	/*EBE150401*/
	if( GetTlvPrivate(ACQUIRER_BANK_TAG ,&sTlvPrivateInfo,tmpField,&nLength) !=SUCCES &&
		GetTlvAutho(AUTO_ACQUIRER_BANK ,tlvInfo,tmpField ,&nLength)==SUCCES )
	{
		PutTlvPrivate(ACQUIRER_BANK_TAG ,&sTlvPrivateInfo,tmpField,nLength);
		field_48_presence = 1;
	}
    /*Start ICH24032016: PROD00030831*/
    if( GetTlvAutho(AUTO_ISSUING_BANK ,tlvInfo,tmpField ,&nLength)==SUCCES )
    {
            PutTlvPrivate(ISSUER_BANK ,&sTlvPrivateInfo,tmpField,nLength);
            field_48_presence = 1;
    }
    /*End ICH24032016*/

	/*--Start AMER20170727: PROD00042216*/
	
	if(GetTlvPrivate( EXT_SECURITY_VER_RESULTS_TAG ,&sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (EXT_SECURITY_VER_RESULTS_TAG , &sTlvPrivateInfo, tmpField, nLength);
		field_48_presence = 1;
	}
	/*--End AMER20170727*/ 
	
	/*EBE160213*/
	if ( GetTlvAutho(AUTO_TERM_INFO_DATA ,tlvInfo,tmpField ,&nLength)==SUCCES )
	{
		PutTlvPrivate(TERM_INFO_DATA ,&sTlvPrivateInfo,tmpField,nLength);
		field_48_presence = 1;
	}

	/*Start YDA20171113 (PROD00048309)*/
	if ( GetTlvAutho(AUTO_MESSAGE_REASON_CODE ,tlvInfo,tmpField ,&nLength) == SUCCES )
	{
		PutIsoField ( ISO_MSG_REASON , isoInfo , tmpField , nLength );
	}
	if ( sens == RESPONSE || sens == REVERSAL ){
		/*******************************************************************************/
		/*  Routing Code                */
		/*******************************************************************************/
		if ( GetTlvAutho(AUTO_ROUTING_CODE ,tlvInfo,tmpField ,&nLength) == SUCCES )
		{
			PutTlvPrivate(ROUTING_CODE ,&sTlvPrivateInfo,tmpField,nLength);
			field_48_presence = 1;
		}
	}
	/*End YDA20171113*/

	memset ( secResult , 0 , sizeof ( secResult));
	if ( GetTlvAutho( AUTO_SECURITY_VER_RESULTS ,tlvInfo, secResult ,&nLength)==SUCCES )
	{
		memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 0 ];
		PutTlvPrivate(CVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 1 ];
		PutTlvPrivate(CVV2_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		/*EBE121214*/
		memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 2 ];
		PutTlvPrivate(PIN_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		/*  Start Mehdi Elyajizi Emv Ctrl verif 20092006: MYJ231007 ****/
		memset ( tmpField , 0, sizeof ( tmpField ));
		/*tmpField [ 0 ] =   secResult  [ 17 ];*/ /* ICVV */
		tmpField [ 0 ] =   secResult  [ 18 ]; /* ICVV */ /*EBE121123*/
		PutTlvPrivate(ICVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		/* ELKAM20221221 PLUTONL-5555 START*/
		
		/*memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 18 ]; /* AUC * / /*EBE121123: Should be 19* /
		PutTlvPrivate(AUC_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 19 ]; /* DAC MasterCard * //*EBE121123: Should be 20* /
		PutTlvPrivate(DAC_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);

		memset ( tmpField , 0, sizeof ( tmpField ));	
		tmpField [ 0 ] =   secResult  [ 20 ]; /* IDN MasterCard * / /*EBE121123: Should be 21* /
		PutTlvPrivate(DYNAMIC_NBR_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);*/
		  
		/* ELKAM20221221 PLUTONL-5555 END*/

		/*Start AMER20171123 PROD00049633: Fill in CAVV result tag*/
		/*NAB2006 Certif SG si le network a verifie le CAVV on n'ecrase pas sa valeur que si le secResut!='R'*/
		memset ( tmpField , 0, sizeof ( tmpField ));
		
		/*if (memcmp( secResult [ 25 ] , 'R', 1) != 0 )*/
		if ( secResult [ 25 ] != 'R')
		{   
			tmpField [ 0 ] =   secResult  [ 25 ];
			PutTlvPrivate(CAVV_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);
		}
		/*End AMER20171123 PROD00049633: Fill in CAVV result tag*/

		/*  END Mehdi Elyajizi Emv Ctrl verif 20092006: MYJ231007   ****/
		/*EBE121123*/
		memset ( tmpField , 0, sizeof ( tmpField ));
		tmpField [ 0 ] =   secResult  [ 4]; 
		PutTlvPrivate(CRYPT_RESULT_CODE_TAG ,&sTlvPrivateInfo,tmpField,1);
		field_48_presence = 1;
			
		PutTlvPrivate(SECURITY_VER_RESULTS_TAG,&sTlvPrivateInfo,secResult,nLength);
			field_48_presence = 1;
	}

	if ( field_48_presence == 1 )
	{
		memset  ( tmpField , 0, sizeof ( tmpField ));
		nLength = PrivateBuildTlv (tmpField , &sTlvPrivateInfo );
		PutIsoField ( ISO_ADTNL_DATA_PRIV , isoInfo , tmpField , nLength ); 
	}

	/*******************************************************************************/
	/*  Security Data                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_SECURITY_DATA,tlvInfo,tmpField,&nLength)==SUCCES )
		PutIsoField ( ISO_SECURITY_DATA , isoInfo , tmpField , nLength );

	/*******************************************************************************/
	/*  Network Data                 */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_NETWORK_DATA ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_NETWORK_DATA , isoInfo , tmpField , nLength );
	/*******************************************************************************/
	/*  Record   Data                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_UF_FILE_RECORD ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_RECORD_DATA , isoInfo , tmpField , nLength );
	/*******************************************************************************/
	/*  Action Date                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_UF_ACTION_DATE ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_ACTION_DATE , isoInfo , tmpField , nLength );


	/*******************************************************************************/
	/*  Additional Amounts           */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_ADD_AMOUNTS ,tlvInfo,tmpField,&nLength) == SUCCES )
		PutIsoField ( ISO_ADTNL_AMOUNTS , isoInfo , tmpField , nLength );


	/*******************************************************************************/
	/*  Key Manage Data                */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_KEY_MANAGE_DATA ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_KEY_MNG_DATA , isoInfo , tmpField , nLength );

	/******MK 120606***/
	/*******************************************************************************/
	/*  ACCOUNT NUMBER                 */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_ACCOUNT_ID_1 ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_ACCOUNT_ID1 , isoInfo , tmpField , nLength );
	/*******************************************************************************/
	/*  AUTO_CPS_DATA                 */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho( AUTO_CPS_DATA ,tlvInfo,tmpField,&nLength) ==SUCCES )
		PutIsoField ( ISO_CPS_FIELDS , isoInfo , tmpField , nLength );

	/** START MAW20050916 **/
	/*******************************************************************************/
	/*  AUTO_ICC_DATA                 */
	/*******************************************************************************/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	if ( GetTlvAutho(AUTO_ICC_DATA ,tlvInfo,tmpField,&nLength) == SUCCES )
	{
		memset(sWork, 0, sizeof(sWork));
		nLen = 0;
		IccTlvToIso(tmpField, nLength,sWork,&nLen);
		if ( nLen > 0)
			PutIsoField ( ISO_ICC_DATA , isoInfo , sWork ,  nLen );
	}
	/** END   MAW20050916 **/
/** START MUW For Gen UBP Output**/
/*******************************************************************************/
/*  ISO_RESERVED_ISO_3                */
/*******************************************************************************/
   memset ( tmpField , 0 , sizeof ( tmpField ));
   if ( GetTlvAutho( AUTO_RESERVED_ISO_3 ,tlvInfo,tmpField,&nLength) ==SUCCES )
        PutIsoField ( ISO_RESERVED_ISO_3 , isoInfo , tmpField , nLength );


		
	/*EBE130925:SWMON*/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	GetTlvAutho(AUTO_MSG_FLOW_MAPPING_ID ,tlvInfo,tmpField,&nLength);
	isoInfo->msgId = (msg_id_t)atol(tmpField);

	dump_p7_struct ( isoInfo );

	/*PA DSS****/
	memset ( tmpField , 0 , sizeof ( tmpField )); /** PA DSS*/
	memset ( sTlvPrivateBuffer , 0 , sizeof ( sTlvPrivateBuffer )); /**PA DSS*/
	InitTlvPrivate (&sTlvPrivateInfo); /* PA DSS*/
	memset(sWork, 0, sizeof(sWork));

	trace_event ( "TlvToIso( End ) ", PROCESSING );
	return ( OK );
}


int SafTlvToIso ( int nIndiceCtx , TSTlvAutho *tlvInfo, TSIsoInfo * isoInfo)
{
	char sMti [ 4 + 1 ];
	char Tag [ 3 + 1 ];
	char tmpField[ LG_MAX ];
	char tmpData[ LG_MAX ];
	char sWork[ LG_MAX ];
	int  nLen;
	char sTermNbr[ 8 + 1 ];
	char sIsoRate [ 8 + 1 ];
	char sAmount   [ 18+1];
	char sCurrency [  3+1];
	char sOrigAmounts [ 24 + 1 ];
	int  i,nRetCode,nLength;
	TSTlvPrivate	TlvPrivateInfo;
	char szFieldDataAdd[20 + 1];  /*HAL13102020 PLUTONL-2352 */
	char szProcCode[24 + 1];	  /*HAL13102020 PLUTONL-2352 */
	char sTrxCurrency [3 + 1];	  /*HAL13102020 PLUTONL-2352 */

	trace_event("Start SafTlvToIso()",PROCESSING);

	memset (  sMti , 0 ,  sizeof ( sMti )  );
	memset (  Tag  , 0 ,  sizeof (  Tag )  );
	memset (  tmpField      , 0 ,  sizeof (  tmpField )  );
	memset (  tmpData       , 0 ,  sizeof (  tmpData )  );
	memset (  sWork         , 0 ,  sizeof (  sWork )  );
	memset (  sTermNbr      , 0 ,  sizeof (  sTermNbr )  );
	memset (  sIsoRate      , 0 ,  sizeof (  sIsoRate )  );
	memset (  sAmount       , 0 ,  sizeof (  sAmount )  );
	memset (  sCurrency     , 0 ,  sizeof (  sCurrency )  );
	memset (  sOrigAmounts  , 0 ,  sizeof (  sOrigAmounts )  );
	memset (  szFieldDataAdd,'\0', sizeof (	 szFieldDataAdd));	/*HAL13102020 PLUTONL-2352 */
	memset (  szProcCode	,'\0', sizeof (	 szProcCode));		/*HAL13102020 PLUTONL-2352 */
	memset (  sTrxCurrency  , 0 ,  sizeof (  sTrxCurrency )  );	/*HAL13102020 PLUTONL-2352 */

	InitTlvPrivate(&TlvPrivateInfo);

	for ( i = 0 ; i < MAX_ISO_FIELDS ; i ++ )
	{
		memset(tmpField ,    0 , sizeof ( tmpField));

		switch (i) 
		{ 

		case ISO_PROC_CODE :
			sprintf (Tag, "%.3d", i+1);
			memset(tmpData,    0 , sizeof ( tmpData ));
			if ( GetTlvAutho(Tag , tlvInfo, tmpData , &nLength) == SUCCES )
			{
				memset(tmpField ,    0 , sizeof ( tmpField));
				memcpy(tmpField + 0  , tmpData + 6 ,    2 );
				memcpy(tmpField + 2  , tmpData + 14 ,    2 );
				memcpy(tmpField + 4  , tmpData + 22 ,    2 );
				PutIsoField ( i , isoInfo , tmpField , 6 );
			}
			break;


		case ISO_TRANS_AMOUNT :
			sprintf (Tag, "%.3d", i+1);
			if ( GetTlvAutho(Tag , tlvInfo, sAmount , &nLength) == SUCCES )
			{
				if ( GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
				{
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 	
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					PutIsoField ( ISO_TRANS_AMOUNT , isoInfo , tmpField , 12 );
				}
			}
			break;

		case ISO_STLMNT_AMOUNT :
			memset ( sAmount , 0 , sizeof ( sAmount ));
			if ( GetTlvAutho ( AUTO_ISS_STLMNT_AMOUNT , tlvInfo, sAmount , &nLength) == SUCCES )
			{
				memset ( sCurrency , 0 , sizeof ( sCurrency ));
				if ( GetTlvAutho( AUTO_ISS_STLMNT_CURRENCY , tlvInfo, sCurrency , &nLength) == SUCCES )
				{
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/  
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					PutIsoField ( ISO_STLMNT_AMOUNT , isoInfo , tmpField , 12 );
				}
			}
			break;

		case ISO_BILLING_AMOUNT :
			sprintf (Tag, "%.3d", i+1);
			if ( GetTlvAutho(Tag , tlvInfo, sAmount , &nLength) == SUCCES )
			{
				if ( GetTlvAutho( AUTO_BILL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
				{
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 	
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					PutIsoField ( ISO_BILLING_AMOUNT , isoInfo , tmpField , 12 );
				}
			}
			break;


		case ISO_STLMNT_CNVRSN_RATE :
			if ( GetTlvAutho(AUTO_ISS_CNV_RATE_STLMNT ,tlvInfo,tmpField,&nLength) == SUCCES )
				PutIsoField ( ISO_STLMNT_CNVRSN_RATE , isoInfo , tmpField , 8 );
			break;

		case ISO_BILLING_CNVRSN_RATE :
			if ( GetTlvAutho(AUTO_CONV_RATE_CARD_BILL ,tlvInfo,tmpField,&nLength) == SUCCES )
				/* PutIsoField ( ISO_BILLING_CNVRSN_RATE , isoInfo , tmpField , nLength ); */
				PutIsoField ( ISO_BILLING_CNVRSN_RATE , isoInfo , tmpField , 8 );
			break;

		case ISO_STLMNT_DATE :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			/* if ( GetTlvAutho(AUTO_ISS_STLMNT_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) */
			if ( GetTlvAutho(AUTO_SETTLEMENT_DATE ,tlvInfo,tmpField,&nLength) == SUCCES )
				PutIsoField ( ISO_STLMNT_DATE , isoInfo , tmpField , 6 );
			break;

		case ISO_CNVRSN_DATE :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			/* Start 3 HY V2.01 170402 */
			/* if ( GetTlvAutho(AUTO_ISS_CNV_RAT_STL_DATE ,tlvInfo,tmpField,&nLength) == SUCCES ) */
			if ( GetTlvAutho(AUTO_CONVERSION_DATE ,tlvInfo,tmpField,&nLength) == SUCCES )
				/* End   3 HY V2.01 170402 */
				PutIsoField ( ISO_CNVRSN_DATE , isoInfo , tmpField , 4 );
			break;

		/* AHA20161216 PROD00036000 */
		case ISO_FEES_AMOUNT :
			memset ( sAmount , 0 , sizeof ( sAmount ));
			if ( GetTlvAutho(AUTO_AMOUNT_FEE , tlvInfo, sAmount , &nLength) == SUCCES )
			{
				if ( GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
				{
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 	
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					PutIsoField ( ISO_FEES_AMOUNT , isoInfo , tmpField , 12 );
				}
			}
			break;
		
		case ISO_STLMNT_CRNCY :
			if ( GetTlvAutho( AUTO_SETL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
				PutIsoField ( ISO_STLMNT_CRNCY , isoInfo , sCurrency ,  3 );
			break;

		case ISO_TERMINAL_NBR :
			memset(sTermNbr, 0, sizeof(sTermNbr));
			if ( GetTlvAutho( AUTO_CARD_ACC_TERMINAL , tlvInfo, tmpField , &nLength) == SUCCES )
			{
				if (nLength > 8)
				{
					memset(sTermNbr, '0', 8);
					memcpy(sTermNbr + 6, tmpField + (nLength - 2), 2);
					nLength = 8;
				}
				else if (nLength == 8)/*SNO261115*/
					memcpy(sTermNbr, tmpField, nLength);
                                /*Start SNO261115 */
				else
				{
					memset(sTermNbr, ' ', 8);
					memcpy(sTermNbr, tmpField, nLength);
					nLength = 8;
				} /*End SNO261115 */

				PutIsoField ( ISO_TERMINAL_NBR , isoInfo , sTermNbr ,  nLength );	
			}
			break;
		/* Start EBE140117*/
		case ISO_ADTNL_DATA_PRIV:
			if ( GetTlvAutho(AUTO_ADD_DATA_PRIVATE ,tlvInfo,tmpField,&nLength) == SUCCES )
			{
				AnalyseTlvPrivate(tmpField,nLength,&TlvPrivateInfo);
				if ( GetTlvAutho(AUTO_NETWORK_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
				{
					PutTlvPrivate(NETWORK_CODE ,&TlvPrivateInfo,tmpField,2);
				}
				/*EBE160213*/
				if ( GetTlvAutho(AUTO_TERM_INFO_DATA ,tlvInfo,tmpField ,&nLength)==SUCCES )
				{
					PutTlvPrivate(TERM_INFO_DATA ,&TlvPrivateInfo,tmpField,nLength);
				}
                                /*AAB13062016*/
                                if ( GetTlvAutho(AUTO_ACQUIRER_BANK ,tlvInfo,tmpField ,&nLength)==SUCCES )
                                {
                                        PutTlvPrivate(ACQUIRER_BANK_TAG ,&TlvPrivateInfo,tmpField,nLength);
                                }
                                if ( GetTlvAutho(AUTO_ISSUING_BANK ,tlvInfo,tmpField ,&nLength)==SUCCES )
                                {
                                        PutTlvPrivate(ISSUER_BANK ,&TlvPrivateInfo,tmpField,nLength);
                                }
                                /*AAB13062016*/

				/*YDA20171205*/
				if ( GetTlvAutho(AUTO_ROUTING_CODE ,tlvInfo,tmpField ,&nLength)==SUCCES )
				{
					PutTlvPrivate(ROUTING_CODE ,&TlvPrivateInfo,tmpField,nLength);
				}
				/*YDA20171205*/

				nLength = PrivateBuildTlv(tmpField,&TlvPrivateInfo);
				PutIsoField (ISO_ADTNL_DATA_PRIV, isoInfo, tmpField, nLength);
			}
			break;

		/*Start HAL13102020 PLUTONL-2352 */
		case ISO_ADTNL_AMOUNTS  :
			memset ( tmpField , 0 , sizeof ( tmpField ));
			memset(tmpData,    0 , sizeof ( tmpData ));
			if ( GetTlvAutho(AUTO_CASH_BACK_AMOUNT ,tlvInfo,tmpField,&nLength) == SUCCES )
			{	
				GetTlvAutho( AUTO_PROCESSING_CODE , tlvInfo, szProcCode , &nLength);
				memcpy(szFieldDataAdd ,szProcCode + 14,P7_F054_ACCT_TYPE_LEN);

				memcpy(szFieldDataAdd + 2,P7_F054_AT_CB_AMNT,P7_F054_AMNT_TYPE_LEN);
				szFieldDataAdd[7]='D';
				
				if ( GetTlvAutho( AUTO_BILL_CURRENCY_CODE , tlvInfo, sTrxCurrency , &nLength) == SUCCES )
				{	
					nRetCode = AdjustIsoAmount (  nIndiceCtx , tmpField , sTrxCurrency , tmpData );
					if ( nRetCode != OK ) return ( NOK );
					memcpy(szFieldDataAdd + 8,tmpData,P7_F054_AMNT_LEN);	
				}
				memcpy(szFieldDataAdd + 4,sTrxCurrency,P7_F054_CCY_LEN);

				PutIsoField ( ISO_ADTNL_AMOUNTS , isoInfo , szFieldDataAdd , 20 );		
			}
			/* ELKAM20230217 PLUTONL-5914 START */
			else if ( GetTlvAutho(AUTO_ADD_AMOUNTS ,tlvInfo,tmpField,&nLength) == SUCCES )
			{
				PutIsoField ( ISO_ADTNL_AMOUNTS , isoInfo , tmpField , nLength );
			}
			/* ELKAM20230217 PLUTONL-5914 END */
			break;
			/*End HAL13102020 PLUTONL-2352 */

		/* End EBE140117*/
		case ISO_ICC_DATA  :
			if ( GetTlvAutho(AUTO_ICC_DATA ,tlvInfo,tmpField,&nLength) == SUCCES )
			{
				memset(sWork, 0, sizeof(sWork));
				nLen = 0;
				IccTlvToIso(tmpField, nLength,sWork,&nLen);
				if ( nLen > 0)
					PutIsoField ( ISO_ICC_DATA , isoInfo , sWork ,  nLen );	
			}
			break;

		case  ISO_ORIGINAL_AMOUNTS :
			memset ( sAmount , 0 , sizeof ( sAmount ));
			memset ( sOrigAmounts , 0       , sizeof ( sOrigAmounts));
			if ( GetTlvAutho(AUTO_REPLACEMENT_BILLING , tlvInfo, sAmount , &nLength) == SUCCES )
			{
				if ( GetTlvAutho( AUTO_BILL_CURRENCY_CODE , tlvInfo, sCurrency , &nLength) == SUCCES )
				{
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					memcpy ( sOrigAmounts + 12 , tmpField , 12 );

					memset ( sAmount   , 0 , sizeof ( sAmount ));
					memset ( sCurrency , 0 , sizeof ( sCurrency ));
					GetTlvAutho( AUTO_REPLACEMENT_AMOUNT , tlvInfo, sAmount  , &nLength);
					GetTlvAutho( AUTO_TRAN_CURRENCY_CODE , tlvInfo, sCurrency , &nLength); 
					memset ( tmpField , 0 , sizeof ( tmpField ));/*YK*/ 
					nRetCode = AdjustIsoAmount (  nIndiceCtx , sAmount , sCurrency , tmpField );
					if ( nRetCode != OK ) return ( NOK );
					memcpy ( sOrigAmounts , tmpField , 12 );
					PutIsoField (ISO_ORIGINAL_AMOUNTS , isoInfo, sOrigAmounts , 24 );
				}
			}
			break;

		default : 
			sprintf ( Tag , "%03d" , i + 1 );
			if ( GetTlvAutho(Tag ,tlvInfo,tmpField,&nLength) == SUCCES )
				PutIsoField ( i , isoInfo , tmpField , nLength );
			break;
		}
	}


	/*EBE130925:SWMON*/
	memset ( tmpField , 0 , sizeof ( tmpField ));
	nLength = 0;
	GetTlvAutho(AUTO_MSG_FLOW_MAPPING_ID ,tlvInfo,tmpField,&nLength);

	if (nLength > 0)
	isoInfo->msgId = (msg_id_t)atol(tmpField);
	
	nLength = 0;
	GetTlvAutho(AUTO_MSG_TYPE ,tlvInfo,sMti,&nLength);

	if (nLength > 0)
	isoInfo->nMsgType = atoi ( sMti );

	/* PCI */
	memset (  Tag           , 0 ,  sizeof (  Tag )  );
	memset (  tmpField      , 0 ,  sizeof (  tmpField )  );
	memset (  tmpData       , 0 ,  sizeof (  tmpData )  );
	memset (  sWork         , 0 ,  sizeof (  sWork )  );
	InitTlvPrivate(&TlvPrivateInfo);
	
	trace_event("End   SafTlvToIso()",PROCESSING);
	return ( OK );
}


