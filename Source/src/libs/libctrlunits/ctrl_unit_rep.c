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
#include <signal.h>
#include <general_macros.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_services.h>


/******************************************************************************\
* Return: 
* AddInfo:
* 
*
*
\******************************************************************************/
int UpdateFraud(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start UpdateFraud()", PROCESSING);

	nResult = FraudUpdate(nIndexCtx, sTlvBuffer);
	

	trace_event("End UpdateFraud(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int SetAuthActionCode(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	char		sActionCode 		[ 3 + 1 ];
	int			nRetCode;


	trace_event("Start SetAuthActionCode()", PROCESSING);

	memset(sActionCode, '\0', sizeof (sActionCode));
	
	
	nRetCode = GetActionCode( nIndexCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE  , sTlvBuffer, MALFUNCTION_ACTION , P7_F039_LEN);
	}
	else
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE  , sTlvBuffer, sActionCode , P7_F039_LEN);
	}
	
	trace_event("End SetAuthActionCode(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GenIssuerScripts(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	char			sPosData[12 + 1];
	char			sServiceCode[3 + 1];
	char			szARC[12];
	char			szOrigARC[12];
	char			sHsmResultCode[24];
	char   			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char			sActionCode 		[ 3 + 1 ];
	int 			n_crpt_result;

	trace_event("Start GenIssuerScripts()", PROCESSING);

	memset(sPosData, '\0', sizeof (sPosData));
	memset(sServiceCode, '\0', sizeof (sServiceCode));
	memset(szARC, '\0', sizeof (szARC));
	memset(szOrigARC, '\0', sizeof (szOrigARC));
	memset(sHsmResultCode, '\0', sizeof (sHsmResultCode));
	memset(sSecurityResults, '\0', sizeof (sSecurityResults));
	memset(sActionCode, '\0', sizeof (sActionCode));
	
	
	GetTlvBuffer ( AUTO_POS_DATA, sTlvBuffer, sPosData ,  &nLength);
	GetTlvBuffer ( AUTO_SERVICE_CODE, sTlvBuffer, sServiceCode ,  &nLength);
	

	if( ( sServiceCode[0] != '2' && sServiceCode[0] != '6' ) || ( sPosData[6] !='5' ))
	{
		trace_event("End   GenIssuerScripts(OK)", PROCESSING);
		return OK;
	}

	GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults ,  &nLength);
	
	nResult = gen_iss_script_data(nIndexCtx, sTlvBuffer, sHsmResultCode);

	if (nResult == OK  && sSecurityResults [SEC_FLAG_CHK_ARQC]	== '0'  )
	{
		/*TODO: should be F055 and not sTlvBuffer*/
		/*if ((GetTlvBuffer(CHIP_AUTHOR_RESP_CODE, sTlvBuffer, szARC, &nLength) == SUCCESS)*/ /*ARC Feature ICH20191010*/
		if ( (GetTlvBuffer(CHIP_CALC_ARC, sTlvBuffer, szARC, &nLength) == SUCCESS)
			&& (GetTlvBuffer(CHIP_ORIG_ARC, sTlvBuffer, szOrigARC, &nLength) == SUCCESS)
			&& memcmp(szARC,szOrigARC,nLength ) != 0)
		{
			n_crpt_result = check_crpt_data (
				nIndexCtx,
				sTlvBuffer,
				sHsmResultCode);

			if ( n_crpt_result != OK )
			{
				switch(n_crpt_result)
				{
				case CRYPTOGRAPHIC_ERROR:PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,ERROR_SYCHRONISATION_KEY , 3);break;
				case HSM_MALFUNCTION: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,MALFUNCTION_EVENT , 3);break;
				case HSM_UNAVAILABLE: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , HSM_UNAVAILABLE_EVENT , 3); break;
				case VERIFICATION_FAILED: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , ERROR_CRYPTO , AUTH_EVENT_CODE_LEN);break;
				}

				FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
				PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS  , sTlvBuffer,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );

				nResult = GetActionCode( nIndexCtx , sTlvBuffer , sActionCode );                                                            
				if ( nResult != OK )
				{
					memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
				}
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,P7_F039_LEN);
				return NOK;

			}
		}

	}
	
	trace_event("End   GenIssuerScripts(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int UpdatePanActivity(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	char		sActionFlag[4];
	char		sActionCode[3 + 1];
	int			nResult;
	int			nLength;


	trace_event("Start UpdatePanActivity()", PROCESSING);

	memset(sActionFlag, '\0', sizeof (sActionFlag));
	memset(sActionCode, '\0', sizeof (sActionCode));
	
	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);
	GetTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode ,  &nLength);

	if ( sActionFlag[0] != ACTION_FLAG_APPROVAL )
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) == 0 )
			nResult = UpdateCardActivity( nIndexCtx , PIN_ERROR , sTlvBuffer );
		else
			nResult = UpdateCardActivity( nIndexCtx , DECLINED_AUTH , sTlvBuffer );
	}
	else
	{
		nResult = UpdateCardActivity  ( nIndexCtx , APPROVED_AUTH , sTlvBuffer );
	}
	

	trace_event("End   UpdatePanActivity(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int UpdateAccBalance(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	
	int			nResult;
	int			nLength;

	trace_event("Start UpdateAccBalance()", PROCESSING);

	nResult = UpdateBalance  ( nIndexCtx , APPROVED_AUTH , sTlvBuffer );
	
	trace_event("End   UpdateAccBalance(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int UpdateNwSettlement(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	char		sActionFlag[4];
	
	int			nResult;
	int			nLength;


	trace_event("Start UpdateNwSettlement()", PROCESSING);

	memset(sActionFlag, '\0', sizeof (sActionFlag));
	
	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);

	if ( sActionFlag[0] != ACTION_FLAG_APPROVAL )
	{
		trace_event("End   UpdateNwSettlement(OK)", PROCESSING);
		return OK;		
	}

	nResult = UpdateSettlement(nIndexCtx, sTlvBuffer);
	

	trace_event("End   UpdateNwSettlement(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int AuthorSafToIssuer(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	
	char		sRoutingCode[RESOURCE_ID_LEN + 1];
	char		sCaptureCode[RESOURCE_ID_LEN + 1];
	int			nResult;
	int			nLength;


	trace_event("Start AuthorSafToIssuer()", PROCESSING);

	memset(sRoutingCode, '\0', sizeof (sRoutingCode));
	memset(sCaptureCode, '\0', sizeof (sCaptureCode));
	
	
	GetTlvBuffer ( AUTO_CAPTURE_CODE, sTlvBuffer, sCaptureCode ,  &nLength);
	GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sRoutingCode, &nLength);

	if( memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{
		nResult = AutSafProcessing(nIndexCtx, sTlvBuffer, sCaptureCode, sRoutingCode);
	}
	

	trace_event("End   AuthorSafToIssuer(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int UpdateStopList(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int			nResult;
	int			nLength;


	trace_event("Start UpdateStopList()", PROCESSING);

	nResult = StopListReplyProcessing (nIndexCtx,sTlvBuffer);

	trace_event("End   UpdateStopList(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GenAuthoReversal(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	char		sRoutingCode[RESOURCE_ID_LEN + 1];
	char		sCaptureCode[RESOURCE_ID_LEN + 1];
	char  		TableIndicator [       3 + 1];
	int			nResult;
	int			nLength;


	trace_event("Start GenAuthoReversal()", PROCESSING);
	
	memset(sRoutingCode, '\0', sizeof (sRoutingCode));
	memset(sCaptureCode, '\0', sizeof (sCaptureCode));
	memset(TableIndicator, '\0', sizeof (TableIndicator));
	
	
	GetTlvBuffer ( AUTO_CAPTURE_CODE, sTlvBuffer, sCaptureCode ,  &nLength);
	GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sRoutingCode, &nLength);
	GetTlvBuffer ( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, TableIndicator , &nLength );

	nResult = AutomaticReversalProc   (		nIndexCtx,    
											CAPTURE_MODE ,
											sCaptureCode, 
											sRoutingCode, 
											TableIndicator,  
											sTlvBuffer, 
											P7_F025_REV_MALFUNCTION );

	trace_event("End   GenAuthoReversal(OK)", PROCESSING);
	return nResult;
}


/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int PostLocalAuthorDtl(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	
	int			nResult;
	int			nLength;


	trace_event("Start PostLocalAuthorDtl()", PROCESSING);

	
	
	nResult = PostAuthorization ( nIndexCtx , 'I', sTlvBuffer );

	trace_event("End   PostLocalAuthorDtl(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int PostReplyAuthorDtl(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	
	int			nResult;
	int			nLength;


	trace_event("Start PostReplyAuthorDtl()", PROCESSING);

	nResult = PostAuthorization ( nIndexCtx , 'U', sTlvBuffer );

	trace_event("End   PostReplyAuthorDtl(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int InsertAuthorDetails(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	
	int			nRetCode;


	trace_event("Start InsertAuthorDetails()", PROCESSING);

	nRetCode = InsertionAuthoActivity ( nIndexCtx , sTlvBuffer );
	
	trace_event("End   InsertAuthorDetails(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int UpdateAuthorDetails(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	
	int			nRetCode;


	trace_event("Start UpdateAuthorDetails()", PROCESSING);

	nRetCode = UpdateAuthoActivity ( nIndexCtx , sTlvBuffer );
	
	trace_event("End   UpdateAuthorDetails(OK)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int SetLocAuthorResponseDtl(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int		nRetCode;

	trace_event("Start SetLocAuthorResponseDtl()", PROCESSING);

	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);

	nRetCode = SetAuthActionCode( nIndexCtx , sTlvBuffer , sAddInfo );                                                            
	if ( nRetCode != OK )
	{
		trace_event("Error SetAuthActionCode", ERROR);
	}

	trace_event("End   SetLocAuthorResponseDtl(OK)", PROCESSING);
	return nRetCode;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int SetAuthorResponseDtl(int nIndexCtx, char*	sTlvBuffer, TSIsoInfo *IsoInfo)
{

	char				tmpField        [  BUF_LG_MAX];
	char 				sActionCode	    [  3 + 1];
	char				tmpField2		[  BUF_LG_MAX];
	char				sCurrency		[  3 + 1];
	char  				sCaptureCode   [       6 + 1];
	char				vAuthorSrc     [      2 + 1 ];
	double				amount;
	int					nRetCode;
	int					nLength;
	int					nProcResult;
	TSTlvPrivate        s_sTlvPrivateInfo;
	TSTlvPrivate		sTlvPrivateInfo;
	TSTlvPrivate		tlvInfo;
	TSTagIso			icInfo;



	trace_event("Start SetAuthorResponseDtl()", PROCESSING);

	nProcResult = OK;
	InitTlvPrivate (&s_sTlvPrivateInfo);
	InitTlvPrivate (&sTlvPrivateInfo);


	/* PROD00051860: fees are generated when host declines an auth. we should set the received action code before calling GenAuthoFees */
	if( GetIsoField ( ISO_AUTHOR_ID , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_AUTH_NUMBER       , sTlvBuffer, tmpField    , nLength ); 
	}

	if( GetIsoField ( ISO_RESPONSE_CODE , IsoInfo , sActionCode , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE       , sTlvBuffer, sActionCode    , nLength );
	}


	if ( GetIsoField(ISO_NETWORK_DATA, IsoInfo,tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_NETWORK_DATA , sTlvBuffer, tmpField, nLength);
	}

	if ( GetIsoField(ISO_ACQR_ID, IsoInfo,tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_ACQ_INST_IDENT_CODE , sTlvBuffer, tmpField, nLength);
	}
	if ( GetIsoField(ISO_FORWD_ID, IsoInfo,tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_FOR_INST_IDENT_CODE , sTlvBuffer, tmpField, nLength);
	}
	if ( GetIsoField(ISO_OUTLET_NBR, IsoInfo,tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_CARD_ACC_ID , sTlvBuffer, tmpField, nLength);
	}
	

	if ( GetIsoField(ISO_STLMNT_AMOUNT, IsoInfo, tmpField, &nLength) == SUCCESS &&
		GetIsoField ( ISO_STLMNT_CRNCY , IsoInfo, sCurrency, &nLength) == SUCCESS)
	{
		nRetCode = AdjustTlvAmount (  nIndexCtx , tmpField , sCurrency , tmpField2 );
		if ( nRetCode != OK )
		{
			trace_event("Error AdjustTlvAmount(ISO_STLMNT_AMOUNT)", ERROR);
			nProcResult = NOK;
		}
		else
		{
			PutTlvBuffer ( AUTO_ACQ_STLMNT_AMOUNT , sTlvBuffer, tmpField2    , 18 );
			PutTlvBuffer ( AUTO_ACQ_STLMNT_CURRENCY , sTlvBuffer, sCurrency    , 3 );

			if (GetIsoField ( ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, &nLength) == SUCCESS)
			{
				PutTlvBuffer ( AUTO_ACQ_CNV_RATE_STLMNT, sTlvBuffer,tmpField, nLength );
			}


			if (GetIsoField ( ISO_STLMNT_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
			{
				GetTlvBuffer ( AUTO_ACQ_STLMNT_DATE, sTlvBuffer,tmpField2, &nLength );
				memcpy(tmpField2 + 2,tmpField,6);
				PutTlvBuffer ( AUTO_ACQ_STLMNT_DATE, sTlvBuffer,tmpField2, nLength );
			}
			if (GetIsoField ( ISO_CNVRSN_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
			{
				PutTlvBuffer ( AUTO_CONVERSION_DATE, sTlvBuffer,tmpField, nLength );
			}
		}
	}


	/*TODO: standin is called called after this process, we should not have partial approval on standin...*/
	if (GetIsoField ( ISO_ADTNL_DATA_PRIV , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		AnalyseTlvPrivate( tmpField ,nLength,&s_sTlvPrivateInfo);
	}

	if (memcmp(sActionCode,P7_F039_PARTIAL_APPROVAL,P7_F039_LEN)==0 &&
		GetTlvPrivate( PARTIAL_AMOUNT_TYPE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		
		if ( tmpField[0] == P7_F048_K02_BILL_AMNT_TYPE)
		{
			if(GetIsoField ( ISO_BILLING_AMOUNT , IsoInfo , tmpField , &nLength ) == SUCCESS )
			{
				if (GetIsoField ( ISO_BILLING_CRNCY , IsoInfo, sCurrency, &nLength) == SUCCESS)
				{
					nRetCode = AdjustTlvAmount (  nIndexCtx , tmpField , sCurrency , tmpField2 );
					if ( nRetCode != OK ) 															             
					{ 				
						trace_event("Error AdjustTlvAmount(ISO_BILLING_AMOUNT)", ERROR); 										     
						nProcResult = NOK;
					}
					else
					{
						amount = (double)(atoi(tmpField2) / 1000);
						ReplaceBillingAmnt(nIndexCtx,amount,sTlvBuffer);
						PrintTlvBuffer(sTlvBuffer);
					}

				}
			}
		}
		else
		{					
			if(GetIsoField ( ISO_TRANS_AMOUNT , IsoInfo , tmpField , &nLength ) == SUCCESS )
			{
				if (GetIsoField ( ISO_TRANS_CRNCY , IsoInfo, sCurrency, &nLength) == SUCCESS)
				{
					nRetCode = AdjustTlvAmount (  nIndexCtx , tmpField , sCurrency , tmpField2 );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_TRANS_AMOUNT)", ERROR);
						nProcResult = NOK;
					}
					else
					{
						amount = (double)(atoi(tmpField2) / 1000);
						ReplaceTransactionAmnt(nIndexCtx,amount,sTlvBuffer);
						PrintTlvBuffer(sTlvBuffer);
					}
				}
			}
		}

		/*AMER20170310 End: PROD00039737*/
	}	

	if ( GetIsoField (ISO_NET_POS_DATA, IsoInfo, tmpField, &nLength ) == SUCCES )
	{
		PutTlvBuffer ( AUTO_NETWORK_POS_DATA , sTlvBuffer, tmpField ,  nLength); 
	}

	
	if ( GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &nLength) == SUCCESS)
	{
		AnalyseIsoIc(tmpField, nLength, &icInfo);
		
		InitTlvPrivate (&tlvInfo);

		if ( GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, tmpField,&nLength) == SUCCESS )
			AnalyseTlvPrivate (tmpField ,nLength , &tlvInfo);

	
		if (GetIsoIcTag (ISO_TAG_ARPC ,&icInfo, tmpField, &nLength) == SUCCES)
			PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, tmpField, nLength);


		if (GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &icInfo, tmpField, &nLength ) == SUCCES)
			PutTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, tmpField, nLength);

		if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT1 ,&icInfo, tmpField, &nLength) == SUCCES)
			PutTlvPrivate (CHIP_ISS_SCRIPT_1, &tlvInfo, tmpField, nLength);


		if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT2 ,&icInfo, tmpField, &nLength) == SUCCES)
			PutTlvPrivate (CHIP_ISS_SCRIPT_2, &tlvInfo, tmpField, nLength);

		if (GetIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE ,&icInfo, tmpField, &nLength) == SUCCES)
			PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, tmpField, nLength);

		
	
		nLength = PrivateBuildTlv (tmpField, &tlvInfo);
		if ( nLength > 0)
			PutTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, tmpField,nLength);

	}
	
	
	if(GetIsoField ( ISO_CPS_FIELDS , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_CPS_DATA , sTlvBuffer, tmpField ,  nLength);
	}	

	

	if( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , tmpField ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate( tmpField ,nLength,&sTlvPrivateInfo);
	}

	
	if (GetTlvPrivate(NET_POS_ENTRY_MODE, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (NET_POS_ENTRY_MODE, &sTlvPrivateInfo, tmpField, nLength);
	}
	

	if (GetTlvPrivate(TRANSACTION_IDENTIFIER, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (TRANSACTION_IDENTIFIER, &sTlvPrivateInfo, tmpField, nLength);
	}	

	if (GetTlvPrivate(AMEX_TRX_ID, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (AMEX_TRX_ID, &sTlvPrivateInfo, tmpField, nLength);
	}

	if (GetTlvPrivate(DINERS_NET_REF_ID, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (DINERS_NET_REF_ID, &sTlvPrivateInfo, tmpField, nLength);
	}

	if(GetTlvPrivate( EXT_SECURITY_VER_RESULTS_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (EXT_SECURITY_VER_RESULTS_TAG , &sTlvPrivateInfo, tmpField, nLength);
	}
	
	if(GetTlvPrivate( SECURITY_LEVEL_IND_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (SECURITY_LEVEL_IND_TAG , &sTlvPrivateInfo, tmpField, nLength);
	}

	if(GetTlvPrivate( ECOM_AUTH_INFO ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (ECOM_AUTH_INFO , &sTlvPrivateInfo, tmpField, nLength);
	}
	
	if(GetTlvPrivate( AAV_RESULT ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (AAV_RESULT , &sTlvPrivateInfo, tmpField, nLength);
	}
	
	if(GetTlvPrivate( ADDRESS_VERIF_REP_TYPE_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (ADDRESS_VERIF_REP_TYPE_TAG, &sTlvPrivateInfo, tmpField, nLength);			
	}

	if(GetTlvPrivate( CVV2_RESULT_CODE_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (CVV2_RESULT_CODE_TAG , &sTlvPrivateInfo, tmpField, nLength);
	}

	if(GetTlvPrivate( MCI_TRACEID ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (MCI_TRACEID, &sTlvPrivateInfo, tmpField, nLength);
	}
	
	if(GetTlvPrivate( PARTIAL_AMOUNT_TYPE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (PARTIAL_AMOUNT_TYPE, &sTlvPrivateInfo, tmpField, nLength);
	}
	
	if(GetTlvPrivate( PAYMENT_ACC_REFERENCE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (PAYMENT_ACC_REFERENCE, &sTlvPrivateInfo, tmpField, nLength);
	}

	nLength = PrivateBuildTlv (tmpField, &sTlvPrivateInfo);
	if ( nLength > 0 )
	{
		PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, tmpField ,  nLength);
	}

	if ( GetIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer ( AUTO_ADD_AMOUNTS  , sTlvBuffer,tmpField, nLength );
	}


	PutTlvBuffer ( AUTO_AUTHO_FLAG         , sTlvBuffer, "P" ,  1);

	
	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	if ( memcmp(sCaptureCode, DSCREEN_RES_CODE, 2) == 0 )
		memcpy( vAuthorSrc, "P0" , 2);
	else
		memcpy( vAuthorSrc, "E0" , 2);

	PutTlvBuffer ( AUTO_AUTHOR_SRC         , sTlvBuffer , vAuthorSrc ,  2);


	trace_event("End   SetAuthorResponseDtl(OK)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CheckDynamicStandin(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	char				sLine[MAX_LINE_TRC];
	char				sTlvPrivateBuffer [ 1024];
	char  				sIssActionCode	[ 256 + 1];
	char				sActionCode		[ 3 + 1];
	char 				sSecResource    [ RESOURCE_ID_LEN + 1 ];
	char				sRoutingCode    [ RESOURCE_ID_LEN + 1 ];
	char  				sCaptureCode   [  RESOURCE_ID_LEN + 1];
	char				vAuthorSrc     [      2 + 1 ];
	int					nRetCode;
	int					nLength;
	TSTlvPrivate        sTlvPrivateInfo;
	TSTlvPrivate        sTlvNetPOSData;
	int					nProcResult;


	trace_event("Start CheckDynamicStandin()", PROCESSING);

	memset(sTlvPrivateBuffer, '\0', sizeof (sTlvPrivateBuffer));
	memset(sIssActionCode, '\0', sizeof (sIssActionCode));
	memset(sActionCode, '\0', sizeof (sActionCode));
	memset(sSecResource, '\0', sizeof (sSecResource));
	memset(sRoutingCode, '\0', sizeof (sRoutingCode));
	memset(sCaptureCode, '\0', sizeof (sCaptureCode));
	memset(vAuthorSrc, '\0', sizeof (vAuthorSrc));
	
	
	InitTlvPrivate (&sTlvPrivateInfo);
	InitTlvPrivate (&sTlvNetPOSData);

	nProcResult = OK;

	if( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , sTlvPrivateBuffer ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	}

	if( GetTlvBuffer ( AUTO_NETWORK_POS_DATA , sTlvBuffer , sTlvPrivateBuffer ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate ( sTlvPrivateBuffer , nLength , &sTlvNetPOSData);
	}


	

	GetTlvPrivate (RESPONSE_CODE_TAG, &sTlvNetPOSData, sIssActionCode , &nLength);
	GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength);
	
	
	/* PROD00051860: we should process the standin before calling GenAuthoFees in order to have the approved action code instead of malfunction*/
	if( memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, strlen(ISS_MALFUNCTION_ACTION)) != 0 &&
		memcmp(sIssActionCode, ISSUER_UNAVAILABLE_ACTION, strlen(ISSUER_UNAVAILABLE_ACTION)) != 0)
	{
		trace_event("End   CheckDynamicStandin(OK): F039 NA", PROCESSING);
		return OK;
	}

	if( memcmp(sSecResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN ) != 0 )
	{
		trace_event("End   CheckDynamicStandin(OK): SecRes NA", PROCESSING);
		return OK;
	}

	GetTlvBuffer ( AUTO_ROUTING_CODE        , sTlvBuffer, sRoutingCode   , &nLength );

	nRetCode = CheckDynamicAdvice(nIndexCtx, sRoutingCode);
	if( nRetCode != OK )
	{
		trace_event("End   CheckDynamicStandin(OK): DynStandin NA", PROCESSING);
		return OK;
	}



	if(memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, strlen(ISS_MALFUNCTION_ACTION)) == 0)
	{
		nRetCode = StandinProc(nIndexCtx , sTlvBuffer , P7_F025_MF_STANDIN );
	}
	else if(memcmp(sIssActionCode, ISSUER_UNAVAILABLE_ACTION, strlen(ISSUER_UNAVAILABLE_ACTION)) == 0)
	{
		nRetCode = StandinProc(nIndexCtx , sTlvBuffer , P7_F025_NA_STANDIN );

	}


		
	if( nRetCode != OK )
	{
		nProcResult = NOK;
	}
	else
	{
		nRetCode = GetActionCode( nIndexCtx , sTlvBuffer , sActionCode );                                                            
		if ( nRetCode != OK )                                                                                                         
		{
			nProcResult = NOK;
			trace_event(">>> Error on standin process GetActionCode()",ERROR);
		}
		else
		{
			
			PutTlvBuffer ( AUTO_ORIG_ACTION_CODE       , sTlvBuffer, sIssActionCode    , 3 ); 
		}
		
		PutTlvPrivate( MALFUNCTION_STIP_TAG , &sTlvPrivateInfo , "Y" , 1 );

		
		GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
		if ( memcmp(sCaptureCode, DSCREEN_RES_CODE, 2) == 0 )
			memcpy( vAuthorSrc, "P2" , 2);
		else
			memcpy( vAuthorSrc, "E2" , 2);

		PutTlvBuffer ( AUTO_AUTHOR_SRC         , sTlvBuffer , vAuthorSrc ,  2);
		

	}
		
	trace_event(">>> EndOf Standin",PROCESSING);
	
	if( nProcResult == NOK )
	{
		PutTlvBuffer ( AUTO_MALFUNCTION_EVENT         , sTlvBuffer , "Y" ,  1);
	}

	
	trace_event("End   CheckDynamicStandin(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int ReplyMalfunctionProcessingNR(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	char				sActionCode[3 + 1];
	char				sMalfunctionFlag[4 + 1];
	int					nRetCode;
	int					nLength;


	trace_event("Start ReplyMalfunctionProcessingNR()", PROCESSING);


	memset(sActionCode, '\0', sizeof (sActionCode));
	memset(sMalfunctionFlag, '\0', sizeof (sMalfunctionFlag));

	GetTlvBuffer ( AUTO_MALFUNCTION_EVENT        , sTlvBuffer, sMalfunctionFlag   , &nLength );

	if( sMalfunctionFlag[0] != 'Y' )
	{
		trace_event("End   ReplyMalfunctionProcessingNR(OK): No malfunction", PROCESSING);
		return OK;
	}

	trace_event("************** Processing Error **************",ERROR);

	
	
	
	DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
	
	PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
	nRetCode = GetActionCode( nIndexCtx , sTlvBuffer , sActionCode );
	if ( nRetCode != OK )
	{
		memcpy(sActionCode,MALFUNCTION_ACTION,3);
	}

	PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
	

	nRetCode = UpdateAuthoActivity ( nIndexCtx , sTlvBuffer );

	trace_event("End   ReplyMalfunctionProcessingNR(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int ReplyMalfunctionProcessing(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	
	int					nRetCode;
	int					nLength;


	trace_event("Start ReplyMalfunctionProcessing()", PROCESSING);

	
	nRetCode= ReplyMalfunctionProcessingNR(nIndexCtx,sTlvBuffer,sAddInfo);
	

	nRetCode = GenAuthoReversal(nIndexCtx,sTlvBuffer,sAddInfo);


	trace_event("End   ReplyMalfunctionProcessing(OK)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int PostAuthorReversal(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	
	int			nResult;
	int			nLength;
	char		szCaptureCode[  6 + 1 ];
	char		sRoutingCode[  6 + 1 ];

	trace_event("Start PostAuthorReversal()", PROCESSING);

	memset(szCaptureCode, '\0', sizeof (szCaptureCode));
	memset(sRoutingCode, '\0', sizeof (sRoutingCode));
	
	
	GetTlvBuffer(AUTO_CAPTURE_CODE,sTlvBuffer,szCaptureCode,&nLength);

	nResult = ReversalProcedure(  	nIndexCtx , 
									NORMAL_MODE , 
									"Y",
									sTlvBuffer , 
									szCaptureCode , 
									sRoutingCode );
	if( nResult == OK )
	{
		trace_event("End   PostAuthorReversal(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   PostAuthorReversal(NOK)", PROCESSING);
	}


	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int PostPreAuthorCompl(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	
	int			nResult;
	int			nLength;
	char		szCaptureCode[  6 + 1 ];
	

	trace_event("Start PostPreAuthorCompl()", PROCESSING);

	memset(szCaptureCode, '\0', sizeof (szCaptureCode));
	
	GetTlvBuffer(AUTO_CAPTURE_CODE,sTlvBuffer,szCaptureCode,&nLength);

	nResult = AuthPreauthComplProc( nIndexCtx, sTlvBuffer);

	if( nResult == OK )
	{
		trace_event("End   PostPreAuthorCompl(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   PostPreAuthorCompl(NOK)", PROCESSING);
	}

	
	return nResult;
}
