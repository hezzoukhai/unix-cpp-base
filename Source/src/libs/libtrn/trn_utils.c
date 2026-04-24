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
#include <tlv_fields_len.h>

/*Versioning */
#ifndef GIT_TAG 

#define GIT_TAG				"Undefined"
#define GIT_BRANCH			"Undefined"
#define GIT_COMMIT_HASH		"Undefined"

#endif

int PrintLibtrnVersion()
{
	char sLine[MAX_LINE_TRC];

	sprintf(sLine, ">>>>> LIBMODULE BUILT [%s][%s]", __DATE__, __TIME__); trace_event(sLine, FATAL);
	sprintf(sLine, ">>>>> VERSION TAG:[%s] BRANCH:[%s] HASH[%s]", GIT_TAG,GIT_BRANCH,GIT_COMMIT_HASH); trace_event(sLine, FATAL);
	return OK;
}

/******************************************************************************/
int  AuthLocal(int nIndiceCtx, char *sTlvBuffer)
{

	char		sActionCode 		[ 3 + 1 ];
	char		sLine 				[ MAX_LINE_TRC ];
	char		tmpField 			[  1024 ];
	int			nRetCode;
	int			nLength;
	char		szWork[BUF_LG_MAX];
	char		sProcCode[32];
	char		sProcCodeTlv[32];
	char		sActionFlag[4];
	char		sNetworkCode[2+1];/*PLUTONL-5279 AHA20221021*/
	char		sNewActionFlag[4];
	char		slTransAmount[32];
	char		sFunctionCode[P7_F024_LEN + 4];
	char		sRoutingCode[RESOURCE_ID_LEN + 1];
	char		sCaptureCode[RESOURCE_ID_LEN + 1];
	char  		TableIndicator [       3 + 1];
	int			nResult;
	/*Start  SNO060116 */
	char		sServiceCode[3 + 1];
	char		sPosData[12 + 1];
	char   		sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char		sSecurityFlags[ SEC_FLAG_LEN + 1 ];
	int 		n_crpt_result = 4;
	char		sCaptureResInfo			[RES_INFO_LEN + 1];
	char		sCaptureResType			[4];
	char		sHsmResultCode[12];
	int			nProcResult;
	int			nChkARQC;
	int         nIsChipCard; /*PLUTONL-5279 AHA20221021*/
	/*Start ICH20200131*/
	TSTlvPrivate    iccTlvInfo;
	char            sIccData[512];
	char            sCalcArc[16+1];
	char            sOrigArc[16+1];
	int             nIccLen		= 0;
	/*End ICH20200131 */
	 /*START MHA20200320: PLUTONL-1405 */
	char            sEmvCardInfoData[128];
	char			offline_pin_cap;
	 /*END MHA20200320: PLUTONL-1405 */
	char sMsgID [MSG_ID_LEN+1];
	msg_id_t   nMsgId;
	
	trace_event("Start AuthLocal()",PROCESSING);

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset ( TableIndicator , 	0, sizeof(TableIndicator) ); 
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode ));
	memset ( sProcCodeTlv  , 0, sizeof ( sProcCodeTlv ));
	memset ( sActionFlag  , 0, sizeof ( sActionFlag ));
	memset ( sNewActionFlag  , 0, sizeof ( sNewActionFlag ));
	memset ( slTransAmount  , 0, sizeof ( slTransAmount ));
	memset ( sFunctionCode  , 0, sizeof ( sFunctionCode ));
	memset ( sRoutingCode  , 0, sizeof ( sRoutingCode ));
	memset ( sCaptureCode  , 0, sizeof ( sCaptureCode ));
	memset ( sServiceCode  , 0, sizeof ( sServiceCode ));
	memset ( sPosData  , 0, sizeof ( sPosData ));
	memset ( sSecurityResults  , 0, sizeof ( sSecurityResults ));
	memset ( sSecurityFlags  , 0, sizeof ( sSecurityFlags ));
	/*Start ICH20200131*/
	memset(sIccData, 0, sizeof(sIccData)); 	
	memset(sCalcArc, 0, sizeof(sCalcArc)); 	
	memset(sOrigArc, 0, sizeof(sOrigArc)); 	
	/*End ICH20200131*/
	memset(sEmvCardInfoData, 0, sizeof(sEmvCardInfoData)); /*MHA20200320: PLUTONL-1405*/
	memset ( sMsgID  , 0, sizeof ( sMsgID ));

	nProcResult = OK;
	nChkARQC = NOK;
	nIsChipCard=NOK; /*PLUTONL-5279 AHA20221021*/

	nResult = PrintTlvBuffer ( sTlvBuffer );

	GetTlvBuffer ( AUTO_PROCESSING_CODE, sTlvBuffer, sProcCodeTlv ,  &nLength);
	GetTlvBuffer ( AUTO_FUNCTION_CODE, sTlvBuffer, sFunctionCode ,  &nLength);
	GetTlvBuffer ( AUTO_CAPTURE_CODE, sTlvBuffer, sCaptureCode ,  &nLength);
	GetTlvBuffer ( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, TableIndicator , &nLength );

	memcpy(sProcCode, sProcCodeTlv + 6,2);
	memcpy(sProcCode + 2, sProcCodeTlv + 8 + 6,2);
	memcpy(sProcCode + 4, sProcCodeTlv + 16 + 6,2);

	if (GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sRoutingCode, &nLength) != SUCCESS)
	{
		PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
		/*YDA20171127 : Use memcpy instead of memcmp*/
		memcpy(sRoutingCode,STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	}

	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);

	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )
	{
		memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
	}
	else
	{
		if ( memcmp(sActionCode, APPROVED_ACTION, 3) == 0 )
		{
			if( memcmp(sFunctionCode,P7_F024_ACC_VERIF,P7_F024_LEN) == 0 )
			{
				memcpy(sActionCode,NO_REASON_TO_DECLINE,P7_F039_LEN);
			}
		}
	}

	PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,P7_F039_LEN);

	/*Start ICH11102016*/
	nRetCode = GenAuthoFees(nIndiceCtx , sTlvBuffer);
	if( nRetCode != OK )
	{
		trace_event("Error GenAuthoFees", ERROR);
		nProcResult = NOK;
	}
	/*End ICH11102016*/

	nRetCode = ControlEvents(nIndiceCtx , sTlvBuffer);
	if( nRetCode != OK )
	{
		trace_event("Error ControlEvents", ERROR);
		nProcResult = NOK;
	}

	
	GetTlvBuffer ( AUTO_POS_DATA, sTlvBuffer, sPosData ,  &nLength);
	GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults ,  &nLength);
	GetTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, sSecurityFlags ,  &nLength);
	GetTlvBuffer ( AUTO_SERVICE_CODE, sTlvBuffer, sServiceCode ,  &nLength);
	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);/*NAB20022020  PLUTONL-1068*/
	GetTlvBuffer ( AUTO_NETWORK_CODE, sTlvBuffer, sNetworkCode ,  &nLength);/*PLUTONL-5279 AHA20221021*/
	/*
        if(GetTlvBuffer(AUTO_TRACK2_DATA,sTlvBuffer,szWork,&nLength)== SUCCES)
	{
		GetTrack2Data(szWork,tmpField,tmpField,sServiceCode,tmpField,tmpField);
	}
	else if(GetTlvBuffer(AUTO_TRACK1_DATA,sTlvBuffer,szWork,&nLength) == SUCCES)
	{
		GetTrack1Data(szWork,tmpField,tmpField,sServiceCode,tmpField,tmpField);
	}
    */
   /*START PLUTONL-5279 AHA20221021*/
   	if(memcmp(sNetworkCode, "04",2)!=0)
	{
		if((sServiceCode[0] == '2') || (sServiceCode[0] == '6')  )
		{
			nIsChipCard=OK;
		}
	}
	else 
	{
		if( memcmp(sServiceCode, "201",3)==0 || memcmp(sServiceCode, "702",3) ==0  )
		{
			nIsChipCard=OK;
		}
	}
	/*END PLUTONL-5279 AHA20221021*/
	if(nIsChipCard==OK &&  sPosData[6] =='5' ) /*PLUTONL-5279 AHA20221021*/
	{
		/*Start ICH20200131*/
		if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nIccLen) == SUCCESS )
		{
			nRetCode = gen_iss_script_data(nIndiceCtx, sTlvBuffer, szWork);
			if (nRetCode != ERROR  && sSecurityResults [SEC_FLAG_CHK_ARQC]	== '0'  )
			{
				InitTlvPrivate(&iccTlvInfo);
				nRetCode = AnalyseTlvPrivate(sIccData, nIccLen, &iccTlvInfo);
				/*if ((GetTlvBuffer(CHIP_AUTHOR_RESP_CODE, sTlvBuffer, szWork, &nLength) == SUCCESS)*/ /*ARC Feature ICH20191010*/
				if ((GetTlvPrivate(CHIP_CALC_ARC, &iccTlvInfo, sCalcArc, &nLength) == SUCCESS)
					&& (GetTlvPrivate(CHIP_ORIG_ARC, &iccTlvInfo, sOrigArc, &nLength) == SUCCESS)
					&& memcmp(sCalcArc,sOrigArc,nLength ) != 0)
				{
					sprintf(sLine,"calc ARC(%s) vs orig ARC(%s)",sCalcArc,sOrigArc);
					trace_event(sLine,PROCESSING);
					nChkARQC = OK;
				}				
			}
			/*IF pin change or pin unblcok and no issuer script to send then we should decline not approve*//*NAB20022020 PLUTONL-1068*/
			/*MHA20200320: PLUTONL-1405*/
			offline_pin_cap = '0';
			if (nRetCode != OK &&( memcmp(sProcCode, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 
			                    || memcmp(sProcCode, P7_F003_PIN_UNBLOCK,P7_F003_P1_LEN) == 0    )
								&& sActionFlag[0] == ACTION_FLAG_APPROVAL)
			{	
				if( GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sEmvCardInfoData, &nLength) == SUCCESS )
				{
					offline_pin_cap		= sEmvCardInfoData[13];
					if ( offline_pin_cap == '1' ) /* if it supports offline pin only*/
					{
				 		nProcResult = NOK;
					}
					
				}
			}

			if( sSecurityFlags[SEC_FLAG_CHK_ARQC] == 'Y' && sSecurityResults[SEC_FLAG_CHK_ARQC]	== '4')
			{
				nChkARQC = OK;
			}
		}

		if( nChkARQC == OK )
		{
			n_crpt_result = check_crpt_data (nIndiceCtx,
											sTlvBuffer,
											sHsmResultCode);
			/*if ( n_crpt_result != OK )
			{*//*START NAB19012021 PLUTONL-2658 We fill crpt_result if the check is successful also*/
			FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
			PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS  , sTlvBuffer,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
				/* PRDXX: we already have an action code
				switch(n_crpt_result)
				{
				case CRYPTOGRAPHIC_ERROR:PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,ERROR_SYCHRONISATION_KEY , 3);break;
				case HSM_MALFUNCTION: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,MALFUNCTION_EVENT , 3);break;
				case HSM_UNAVAILABLE: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , HSM_UNAVAILABLE_EVENT , 3); break;
				case VERIFICATION_FAILED: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , ERROR_CRYPTO , AUTH_EVENT_CODE_LEN);break;
				}
				FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
				PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS  , sTlvBuffer,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
				nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
				if ( nRetCode != OK )
				{
					memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
				}
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,P7_F039_LEN);
				*/
			/*}*//*END NAB19012021*/
		}

	}
	/*End  SNO060116 */


	/* AHA20160921  start fonction added to log authorisation in fraud table of not logged yet */
	nResult=FraudUpdate( nIndiceCtx , sTlvBuffer );
	if ( nResult != OK )
	{
		trace_event ( "Can Not log authorisation in Fraud table  !!!", FATAL );
	}

	/* AHA20160921  end */

	if( nProcResult != OK )
	{
		trace_event("************ processing error ****************", ERROR);
		DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
		{
			memcpy(sActionCode,MALFUNCTION_ACTION,3);
		}
		
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		nProcResult = OK;
	}

	if (memcmp(sFunctionCode, P7_F024_TER, P7_F024_LEN) == 0 || memcmp(sFunctionCode, P7_F024_TAR, P7_F024_LEN) == 0)
	{
		if ( memcmp(sActionCode, APPROVED_ACTION, 3) != 0  &&  memcmp(sActionCode, NO_REASON_TO_DECLINE, 3) != 0)
			memcpy(sActionCode,DO_NOT_HONOR,P7_F039_LEN);
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,P7_F039_LEN);
	}


	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
		PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3); 
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

		trace_event("Error InsertionAuthoActivity", ERROR);
		nProcResult = NOK;
		nRetCode = SEVERE_ERROR;
	}

	/*GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength); Moved to the top*//*NAB20022020 PLUTONL-1068*/

/*	if ( sActionFlag[0] != ACTION_FLAG_APPROVAL ) / * PLUTONL-2989 * /
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) == 0 )
			nResult = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
		else
			nResult = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
	}
	else
	{
		nResult = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
	}

	if ( nResult != OK )
	{
		trace_event ( "Can Not Update Card Activity  !!!", FATAL );
		/ * What is the action to do * /
	}



	/ * MKB070508 : si pin change, pas de maj de bal ou de settlement * /
	if ( / *memcmp(sProcCode, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 ||* /
		memcmp(sFunctionCode,P7_F024_ACC_VERIF,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TER,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TAR,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_ACN,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TCN,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TVN,P7_F024_LEN) == 0)
	{
		sprintf(sLine,"End   AuthLocal( %d)",nRetCode);
		trace_event(sLine,PROCESSING);
		return(OK);
	}


	GetTlvBuffer(AUTO_CAPTURE_RES_INFO, sTlvBuffer, sCaptureResInfo, &nLength);

	/ * ICH20170509 Need to update balance in case of declined autorisation to impact OTB with online fee * /
	/ *if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
	{ * /
	
	/ *GetResInfoData(RES_INF_RES_TP,sCaptureResInfo,sCaptureResType,&nLength);
	/ *ATM is a resource that needs to send completion to confirm authorization, so we do not credit the acc until completion* /
	if( memcmp(sProcCode, P7_F003_DEPOSIT, P7_F003_P1_LEN) != 0 || sCaptureResInfo[0] != 'A' )
	{

		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL );
			/ * What is the action to do * /
			nProcResult = NOK;
		}
	}
*/
	nRetCode = UpdatePanActAccBal  ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Update Balance  !!!", FATAL );
		nProcResult = NOK;
	}
	/*PLUTONL-3762 */
	GetTlvBuffer (AUTO_MSG_FLOW_MAPPING_ID, sTlvBuffer, sMsgID ,  &nLength);
	nMsgId = (msg_id_t)atol(sMsgID);

	MsgLog(nMsgId, sTlvBuffer );
	/*PLUTONL-3762 */

	/* MKB070508 : si pin change, pas de maj de bal ou de settlement */
	if ( /*memcmp(sProcCode, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 ||*/
		memcmp(sFunctionCode,P7_F024_ACC_VERIF,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TER,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TAR,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_ACN,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TCN,P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_TVN,P7_F024_LEN) == 0)
	{
		sprintf(sLine,"End   AuthLocal( %d)",nRetCode);
		trace_event(sLine,PROCESSING);
		return(OK);
	}	

	if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
	{
		nRetCode = UpdateSettlement(nIndiceCtx, sTlvBuffer);
	}

	/* check if the destination resource would like to be informed about approvals/declines*/
	if( memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer, sCaptureCode, sRoutingCode);
		if( nRetCode != OK )
		{
			nProcResult = NOK;
		}
	}

	if( nProcResult != OK )
	{
		trace_event("************ processing error ****************", ERROR);
		DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
		{
			memcpy(sActionCode,MALFUNCTION_ACTION,3);
		}
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);/*PLUTONL-4595*/
			
		GetTlvBuffer ( AUTO_ACTION_FLAG , sTlvBuffer , sNewActionFlag ,&nLength);
		if(sNewActionFlag [0] != ACTION_FLAG_APPROVAL )
		{
			DelTlvBuffer(AUTO_AUTH_NUMBER,sTlvBuffer,"",0);
		}
		/*nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );*/
		nRetCode = RollBackAuthoActivity ( nIndiceCtx , sTlvBuffer);/*PLUTONL-4539*/
		
		/*PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);*/
		if ( sActionFlag[0] == ACTION_FLAG_APPROVAL &&
			memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
		{
			AutomaticReversalProc(  nIndiceCtx,   CAPTURE_MODE, sCaptureCode, sRoutingCode , TableIndicator , sTlvBuffer, P7_F025_REV_MALFUNCTION );
		}
		nProcResult = OK;
	}

	StopListReplyProcessing (nIndiceCtx,sTlvBuffer);

	nResult = PrintTlvBuffer ( sTlvBuffer );


	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode ));

	sprintf(sLine,"End   AuthLocal(%d)",nRetCode);
	trace_event(sLine,PROCESSING);

	return ( nRetCode );
}

/****************************************************************************/
/* Fonction qui traite les reponses autorisations                          */
/****************************************************************************/
/****************************************************************************/

int AuthReplyProc  ( int nIndiceCtx, TSIsoInfo *IsoInfo , char *sTlvBuffer, int nEvtInd )
{
	int                 nRetCode = 0;
	int                 length   = 0;
	int					nTimeLen = 0;
	int   				nLength  = 0;
	int					nResult;

	TSTlvPrivate        sTlvPrivateInfo;
	TSTlvPrivate        s_sTlvPrivateInfo;
	TSTlvPrivate        sTlvNetPOSData;


	char 				sActionCode			[       3 + 1];
	char  				sCaptureCode   [       6 + 1];
	char  				sProcessingCode[       24 + 1];
	char  				sRoutingCode   [       6 + 1];
	char  				TableIndicator [       3 + 1];
	char  				sIssActionCode [     256 + 1];
	char				sTimeField     [         256];
	char				tmpField       [         BUF_LG_MAX];
	char				szWork         [         BUF_LG_MAX];
	char				sDataField		[BUF_LG_MAX];
	char  				sLine          [MAX_LINE_TRC];
	char				vAuthorSrc     [      2 + 1 ];
	char				sActionFlag		[4];
	char				sNewActionFlag	[4];
	char				sTlvPrivateBuffer [ 1024];
	char 				sSecResource    [ RESOURCE_ID_LEN + 1 ];
	TSTlvAutho			sTlvInfo;
	int                 nIsChipCard; /*PLUTONL-5279 AHA20221021*/
	char		        sNetworkCode[2+1];/*PLUTONL-5279 AHA20221021*/


	TSTlvPrivate		tlvInfo;
	TSTagIso			icInfo;
	char				vIccData            [2024 +1];
	char				icTmp               [512 +1];
	int					vIccLen;
	int					icLen;
	/*Start  SNO060116 */
	char				sServiceCode[3 + 1];
	char				sPosData[12 + 1];
	char   				sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char				sSecurityFlags[ SEC_FLAG_LEN + 1 ];
	int 				n_crpt_result = 4;
	char				sHsmResultCode[12];
	int					nProcResult;
	int					nChkARQC;
/*Start ICH20200131*/
	char            sCalcArc[16+1];
	char            sOrigArc[16+1];
/*End ICH20200131*/
	char  tmpData		[  256 + 1];
	char  sCurrency		[	18 + 1];
	double amount;	
	 /*START MHA20200320: PLUTONL-1405 */
	char            sEmvCardInfoData[128];
	char			offline_pin_cap;
	char sMsgID [MSG_ID_LEN+1];
	msg_id_t   nMsgId;
	 /*END MHA20200320: PLUTONL-1405 */
	char			sOriginCode[2];
	char            sNetIssSetlmntInfo [NET_SETL_INFO_L +1];
	int			ISsNetIssSetlmntInfoPresent = 0; /*SKA29032023 PLUTONL-6112*/
	int				nIsMalfunctionStip = 0;
	nIsChipCard=NOK; /*PLUTONL-5279 AHA20221021*/


	trace_event ( "Start AuthReplyProc()", PROCESSING );

	amount = 0;	
	memset ( sActionCode    , 	0, sizeof(sActionCode) ); 
	memset ( sCaptureCode   , 	0, sizeof(sCaptureCode) );
	memset ( sProcessingCode  , 	0, sizeof(sProcessingCode) ); 
	memset ( sRoutingCode   , 	0, sizeof(sRoutingCode) );
	memset ( TableIndicator , 	0, sizeof(TableIndicator) ); 
	memset ( sIssActionCode , 	0, sizeof(sIssActionCode) ); 
	memset ( sTimeField , 	0, sizeof(sTimeField) ); 
	memset ( tmpField   , 	0, sizeof(tmpField) );
	memset ( szWork   , 	        0, sizeof(szWork) );
	memset ( sLine   , 	        0, sizeof(sLine) );
	memset ( vAuthorSrc   ,       0, sizeof(vAuthorSrc) );
	memset ( sActionFlag  , 0, sizeof ( sActionFlag ));
	memset ( sNewActionFlag  , 0, sizeof ( sNewActionFlag ));
	memset(vAuthorSrc, 0, sizeof(vAuthorSrc));
	memset ( tmpData  , 0, sizeof ( tmpData ));
	memset ( sSecurityFlags  , 0, sizeof ( sSecurityFlags ));
	/*Start ICH20200131*/
	memset(sCalcArc, 0, sizeof(sCalcArc)); 	
	memset(sOrigArc, 0, sizeof(sOrigArc)); 	
	/*End ICH20200131*/
	memset(sEmvCardInfoData, 0, sizeof(sEmvCardInfoData)); /*MHA20200320: PLUTONL-1405*/
	memset(sMsgID, 0, sizeof(sMsgID)); 

	memset(sOriginCode, 0, sizeof(sOriginCode));
	memset(sNetIssSetlmntInfo, ' ', NET_SETL_INFO_L);
	sNetIssSetlmntInfo [NET_SETL_INFO_L] = '\0';

	InitTlvPrivate(&sTlvPrivateInfo);
	InitTlvPrivate(&s_sTlvPrivateInfo);
	InitTlvPrivate(&sTlvNetPOSData);
	InitTlvAutho    ( &sTlvInfo );

	PrintTlvBuffer ( sTlvBuffer );

	dump_p7_struct ( IsoInfo );


	
	nProcResult = OK;
	nChkARQC = NOK;

	/* the message has not been forwarded */
	if( nEvtInd == FWD_MSG_MALFUNCTION )
	{
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
		PutTlvBuffer( AUTO_RESPONSE_CODE, sTlvBuffer, ISSUER_UNAVAILABLE_ACTION    , 3 ); 
	}
	

	/* PROD00051860: fees are generated when host declines an auth. we should set the received action code before calling GenAuthoFees */
	if( GetIsoField ( ISO_AUTHOR_ID , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_AUTH_NUMBER       , sTlvBuffer, tmpField    , nLength ); 
	}

	if( GetIsoField ( ISO_RESPONSE_CODE , IsoInfo , sActionCode , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE       , sTlvBuffer, sActionCode    , nLength );
	}

	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	GetTlvBuffer ( AUTO_ROUTING_CODE        , sTlvBuffer, sRoutingCode   , &nLength );
	GetTlvBuffer ( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, TableIndicator , &nLength );
	GetTlvBuffer ( AUTO_PROCESSING_CODE, sTlvBuffer, sProcessingCode    ,  &nLength);/*NAB20022020 PLUTONL-1068*/	



	if ( GetIsoField (ISO_NET_POS_DATA, IsoInfo, sTlvPrivateBuffer, &nLength ) == SUCCES )
	{
		/*AHA20160826  ID: PROD00031257 put field 61 in the TLV buffer in order to insert pos entry mode in the database */
		PutTlvBuffer ( AUTO_NETWORK_POS_DATA , sTlvBuffer, sTlvPrivateBuffer ,  nLength); 
		AnalyseTlvPrivate ( sTlvPrivateBuffer , nLength , &sTlvNetPOSData);
	}

	if (GetIsoField ( ISO_ADTNL_DATA_PRIV , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		AnalyseTlvPrivate( tmpField ,nLength,&s_sTlvPrivateInfo);
	}

	if( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , sTlvPrivateBuffer ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	}


	if ( memcmp(sCaptureCode, DSCREEN_RES_CODE, 2) == 0 )
		memcpy( vAuthorSrc, "P0" , 2);
	else
		memcpy( vAuthorSrc, "E0" , 2);


	GetTlvPrivate (RESPONSE_CODE_TAG, &sTlvNetPOSData, sIssActionCode , &nLength);
	GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength);

	/* PROD00051860: we should process the standin before calling GenAuthoFees in order to have the approved action code instead of malfunction*/
	if( (memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, strlen(ISS_MALFUNCTION_ACTION)) == 0 ||
		memcmp(sIssActionCode, ISSUER_UNAVAILABLE_ACTION, strlen(ISSUER_UNAVAILABLE_ACTION)) == 0)
		&&
		memcmp(sSecResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN ) == 0
		&& 
		CheckDynamicAdvice(nIndiceCtx, sRoutingCode) == OK)
	{


		/*YDA20171123*/
		if(memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, strlen(ISS_MALFUNCTION_ACTION)) == 0)
		{
			nRetCode = StandinProc(nIndiceCtx , sTlvBuffer , P7_F025_MF_STANDIN );

		}
		else if(memcmp(sIssActionCode, ISSUER_UNAVAILABLE_ACTION, strlen(ISSUER_UNAVAILABLE_ACTION)) == 0)
		{
			nRetCode = StandinProc(nIndiceCtx , sTlvBuffer , P7_F025_NA_STANDIN );

		}

		if( nRetCode == ROUTAGE )
		{
			sprintf(sLine,"End   AuthReplyProc(StandIn Routing: %d)",nRetCode);
			trace_event(sLine,PROCESSING);
			return nRetCode;
		}		
		
		
		if( nRetCode != OK )
		{
			nProcResult = NOK;
		}
		else
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
			if ( nRetCode != OK )                                                                                                         
			{
				nProcResult = NOK;
				trace_event(">>> Error on standin process GetActionCode()",ERROR);
			}
			else
			{
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode,3);
				PutIsoField (ISO_RESPONSE_CODE , IsoInfo ,sActionCode , 3 );
				if( GetTlvBuffer ( AUTO_AUTH_NUMBER       , sTlvBuffer, tmpField    , &nLength ) == SUCCESS )
				{
					PutIsoField ( ISO_AUTHOR_ID , IsoInfo , tmpField , nLength ); 
				}
				PutTlvBuffer ( AUTO_ORIG_ACTION_CODE       , sTlvBuffer, sIssActionCode    , 3 ); 
			}
			memcpy( vAuthorSrc, "E2" , 2);

			nIsMalfunctionStip = 1;/*IBO20220803 PLUTONL-4797*/
			/* Moved after the analyse
			 * PutTlvPrivate( MALFUNCTION_STIP_TAG , &sTlvPrivateInfo , "Y" , 1 );*/
		}
		trace_event(">>> EndOf Standin",PROCESSING);
	}

	/*Start ICH11102016*/
	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )
	{
		memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
	}
	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);


	nRetCode = GenAuthoFees (  nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event("Error GenAuthoFees", ERROR);
		nProcResult = NOK;
	}
	/*End ICH11102016*/

	/*Start JOU20170522*/
	nRetCode=ControlEvents(nIndiceCtx , sTlvBuffer);
	if( nRetCode != OK )
	{
		trace_event("Error ControlEvents", ERROR);
		nProcResult = NOK;
	}
	/*End JOU20170522*/
     
	/*NAB20022020 PLUTONL-1068*/ 



	/** HHH - START ICC ARPC CPY **/
	memset(vIccData,0, sizeof(vIccData));
	vIccLen = 0;
	if ( GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &vIccLen) == SUCCESS)
	{

		InitTlvPrivate (&tlvInfo);
		if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, vIccData,&nLength) == SUCCESS)
			AnalyseTlvPrivate (vIccData ,nLength , &tlvInfo);

		AnalyseIsoIc(tmpField, vIccLen, &icInfo);

		if (GetIsoIcTag (ISO_TAG_ARPC ,&icInfo, icTmp, &icLen) == SUCCES)
			PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, icTmp, icLen);


		if (GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &icInfo, icTmp, &icLen ) == SUCCES && icLen >= 16)
			PutTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, icTmp, icLen);

		if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT1 ,&icInfo, icTmp, &icLen) == SUCCES)
			PutTlvPrivate (CHIP_ISS_SCRIPT_1, &tlvInfo, icTmp, icLen);


		if (GetIsoIcTag (ISO_TAG_ISS_SCRIPT2 ,&icInfo, icTmp, &icLen) == SUCCES)
			PutTlvPrivate (CHIP_ISS_SCRIPT_2, &tlvInfo, icTmp, icLen);

		if (GetIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE ,&icInfo, icTmp, &icLen) == SUCCES)
			PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, icTmp, icLen);



		memset(vIccData,0, sizeof(vIccData));
		nLength = PrivateBuildTlv (vIccData, &tlvInfo);
		if ( nLength > 0)
			PutTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, vIccData,nLength);

		/*Start  SNO060116 */
		GetTlvBuffer ( AUTO_POS_DATA, sTlvBuffer, sPosData ,  &nLength);
		GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults ,  &nLength);
		GetTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, sSecurityFlags ,  &nLength);
		GetTlvBuffer ( AUTO_SERVICE_CODE, sTlvBuffer, sServiceCode ,  &nLength);
		GetTlvBuffer ( AUTO_NETWORK_CODE, sTlvBuffer, sNetworkCode ,  &nLength);/*PLUTONL-5279 AHA20221021*/
		/*
		if(GetTlvBuffer(AUTO_TRACK2_DATA,sTlvBuffer,szWork,&nLength)== SUCCES)
		{
			GetTrack2Data(szWork,tmpField,tmpField,sServiceCode,tmpField,tmpField);
		}
		else if(GetTlvBuffer(AUTO_TRACK1_DATA,sTlvBuffer,szWork,&nLength) == SUCCES)
		{
			GetTrack1Data(szWork,tmpField,tmpField,sServiceCode,tmpField,tmpField);
		}
		*/
	   /*START PLUTONL-5279 AHA20221021*/
		if(memcmp(sNetworkCode, "04",2)!=0)
		{
			if((sServiceCode[0] == '2') || (sServiceCode[0] == '6')  )
			{
				nIsChipCard=OK;
			}
		}
		else 
		{
			if( memcmp(sServiceCode, "201",3)==0 || memcmp(sServiceCode, "702",3) ==0  )
			{
				nIsChipCard=OK;
			}
		}
	    /*END PLUTONL-5279 AHA20221021*/  
		if(nIsChipCard==OK &&  sPosData[6] =='5' ) /*PLUTONL-5279 AHA20221021*/
		{
			nRetCode = gen_iss_script_data(nIndiceCtx, sTlvBuffer, szWork);
			if (nRetCode != ERROR  && sSecurityResults [SEC_FLAG_CHK_ARQC]	== '0'  )
			{
				/*if ((GetTlvBuffer(CHIP_AUTHOR_RESP_CODE, sTlvBuffer, szWork, &nLength) == SUCCESS)*/ /*ARC Feature ICH20191010*/
				if ((GetTlvPrivate(CHIP_CALC_ARC, &tlvInfo, sCalcArc, &nLength) == SUCCESS)
					&& (GetTlvPrivate(CHIP_ORIG_ARC, &tlvInfo, sOrigArc, &nLength) == SUCCESS)
					&& memcmp(sCalcArc,sOrigArc,nLength ) != 0)
				{
					nChkARQC = OK;
				}
				
				sprintf(sLine,"calc ARC(%s) vs orig ARC(%s)",sCalcArc,sOrigArc);
				trace_event(sLine,PROCESSING);

				if( sSecurityFlags[SEC_FLAG_CHK_ARQC] == 'Y' && sSecurityResults[SEC_FLAG_CHK_ARQC]	== '4')
				{
					nChkARQC = OK;
				}

				if( nChkARQC == OK )
				{	
					n_crpt_result = check_crpt_data (nIndiceCtx,
													sTlvBuffer,
													sHsmResultCode);

					/*if ( n_crpt_result != OK )
					{*//*START NAB19012021 PLUTONL-2658 We fill crpt_result if the check is successful also*/
					FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
					PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS  , sTlvBuffer,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
				/* PRDXX: we already have an action code
						switch(n_crpt_result)
						{
						case CRYPTOGRAPHIC_ERROR:PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,ERROR_SYCHRONISATION_KEY , 3);break;
						case HSM_MALFUNCTION: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer ,MALFUNCTION_EVENT , 3);break;
						case HSM_UNAVAILABLE: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , HSM_UNAVAILABLE_EVENT , 3); break;
						case VERIFICATION_FAILED: PutTlvBuffer (AUTO_EVENT_CODE, sTlvBuffer , ERROR_CRYPTO , AUTH_EVENT_CODE_LEN);break;
						}
						FillSecurityResult ( n_crpt_result  , sSecurityResults , SEC_FLAG_CHK_ARQC );
						PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS  , sTlvBuffer,sSecurityResults, AUTH_SEC_VERIF_RSLT_LEN );
				*/
					/*}*//*END NAB19012021*/
				}

			}
			/*IF pin change or pin unblcok and no issuer script to send then we should decline not approve*//*NAB20022020 PLUTONL-1068*/
			/*MHA20200320: PLUTONL-1405*/
			offline_pin_cap = '0';
			if (nRetCode != OK &&( memcmp(sProcessingCode +6, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 
			                    || memcmp(sProcessingCode +6, P7_F003_PIN_UNBLOCK,P7_F003_P1_LEN) == 0    )
								&& sActionFlag[0] == ACTION_FLAG_APPROVAL)
			{	
				if( GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sEmvCardInfoData, &nLength) == SUCCESS )
				{
					offline_pin_cap		= sEmvCardInfoData[13];
					if ( offline_pin_cap == '1' ) /* if it supports offline pin only*/
					{
				 		nProcResult = NOK;
					}
					
				}
			}
		}
	}
	

	if ( GetIsoField(ISO_NETWORK_DATA, IsoInfo,sDataField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_NETWORK_DATA , sTlvBuffer, sDataField, nLength);
	}

	/* PROD00051898: we should update autho activity record with acquirer id/identifiers sent to schemes*/
	if ( GetIsoField(ISO_ACQR_ID, IsoInfo,sDataField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_ACQ_INST_IDENT_CODE , sTlvBuffer, sDataField, nLength);
	}
	if ( GetIsoField(ISO_FORWD_ID, IsoInfo,sDataField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_FOR_INST_IDENT_CODE , sTlvBuffer, sDataField, nLength);
	}
	if ( GetIsoField(ISO_OUTLET_NBR, IsoInfo,sDataField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_CARD_ACC_ID , sTlvBuffer, sDataField, nLength);
	}
	/*start NAB18122020 PLUTONL-2550*/
	if ( GetIsoField(ISO_TERMINAL_ADR, IsoInfo,sDataField, &nLength) == SUCCESS)
	{
		PutTlvBuffer (AUTO_CARD_ACC_LOCATION , sTlvBuffer, sDataField, nLength);
	}
	/*END NAB18122020 PLUTONL-2550*/
	

	/*EBE160317: PROD00025758*/
	if ( GetIsoField(ISO_STLMNT_AMOUNT, IsoInfo, tmpField, &nLength) == SUCCESS &&
		GetIsoField ( ISO_STLMNT_CRNCY , IsoInfo, sCurrency, &nLength) == SUCCESS)
	{
		nRetCode = AdjustTlvAmount (  nIndiceCtx , tmpField , sCurrency , tmpData );
		if ( nRetCode != OK )
		{
			trace_event("Error AdjustTlvAmount(ISO_STLMNT_AMOUNT)", ERROR);
			nProcResult = NOK;
		}
		else
		{
			/* Mettre le settl recu dans iss_settl au lieu de acq_settl (seulement si je suis acquereur et pas emetteur)*/
			GetTlvBuffer(AUTO_ORIGIN_CODE, sTlvBuffer, sOriginCode, &nLength);
			if(  sOriginCode[0] != '0' && sOriginCode[0] != '1' && sOriginCode[0] != '2' )
			{
				PutTlvBuffer ( AUTO_ISS_STLMNT_AMOUNT , sTlvBuffer, tmpData    , 18 );
				PutTlvBuffer ( AUTO_ISS_STLMNT_CURRENCY , sTlvBuffer, sCurrency    , 3 );

				memcpy(sNetIssSetlmntInfo, sCurrency, NET_SETL_CRNCY_L);
				memcpy(sNetIssSetlmntInfo + NET_SETL_AMOUNT_O, tmpData, NET_SETL_AMOUNT_L);
				ISsNetIssSetlmntInfoPresent = 1;

				if (GetIsoField ( ISO_STLMNT_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
				{
					if(GetTlvBuffer ( AUTO_ACQ_STLMNT_DATE, sTlvBuffer,sTimeField, &nLength )== SUCCESS )
					{
						memcpy(sTimeField + 2,tmpField,6);
						PutTlvBuffer ( AUTO_ISS_STLMNT_DATE, sTlvBuffer,sTimeField, nLength );
						memcpy(sNetIssSetlmntInfo + NET_SETL_DATE_O, sTimeField, NET_SETL_DATE_L);
					}
				}
				if (GetIsoField ( ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, &nLength) == SUCCESS)
				{
					PutTlvBuffer ( AUTO_ISS_CNV_RATE_STLMNT, sTlvBuffer,tmpField, nLength );
					memcpy(sNetIssSetlmntInfo + NET_SETL_CONV_RATE_O, tmpField, NET_SETL_CONV_RATE_L);
				}
				if (GetIsoField ( ISO_CNVRSN_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
				{
					PutTlvBuffer ( AUTO_CONVERSION_DATE, sTlvBuffer,tmpField, nLength );
					memcpy(sNetIssSetlmntInfo + NET_SETL_CONV_DATE_O, tmpField, NET_SETL_CONV_DATE_L);
				}
			}
			else
			{
				if (GetTlvPrivate(NET_ACQ_SETTL_INFO, &sTlvPrivateInfo, tmpField, &nLength) != SUCCESS)
				{
					PutTlvBuffer ( AUTO_ACQ_STLMNT_AMOUNT , sTlvBuffer, tmpData    , 18 );
					PutTlvBuffer ( AUTO_ACQ_STLMNT_CURRENCY , sTlvBuffer, sCurrency    , 3 );

					if (GetIsoField ( ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, &nLength) == SUCCESS)
					{
						PutTlvBuffer ( AUTO_ACQ_CNV_RATE_STLMNT, sTlvBuffer,tmpField, nLength );
					}
					if (GetIsoField ( ISO_STLMNT_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
					{
						GetTlvBuffer ( AUTO_ACQ_STLMNT_DATE, sTlvBuffer,sTimeField, &nLength );
						memcpy(sTimeField + 2,tmpField,6);
						PutTlvBuffer ( AUTO_ACQ_STLMNT_DATE, sTlvBuffer,sTimeField, nLength );
					}
					if (GetIsoField ( ISO_CNVRSN_DATE, IsoInfo, tmpField, &nLength) == SUCCESS)
					{
						PutTlvBuffer ( AUTO_CONVERSION_DATE, sTlvBuffer,tmpField, nLength );
					}
				}
			}
		}
	}


	if (memcmp(sActionCode,P7_F039_PARTIAL_APPROVAL,P7_F039_LEN)==0 &&
		GetTlvPrivate( PARTIAL_AMOUNT_TYPE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		/*AMER20170310 Start: PROD00039737*/
		if ( tmpField[0] == P7_F048_K02_BILL_AMNT_TYPE)
		{
			if(GetIsoField ( ISO_BILLING_AMOUNT , IsoInfo , tmpField , &nLength ) == SUCCESS )
			{
				if (GetIsoField ( ISO_BILLING_CRNCY , IsoInfo, sCurrency, &nLength) == SUCCESS)
				{
					nRetCode = AdjustTlvAmount (  nIndiceCtx , tmpField , sCurrency , tmpData );
					if ( nRetCode != OK ) 															             
					{ 				
						trace_event("Error AdjustTlvAmount(ISO_BILLING_AMOUNT)", ERROR); 										     
						nProcResult = NOK;
					}
					else
					{
						amount = (double)(atoi(tmpData));
						/*HAL18082020 PLUTONL-2167*/
						amount = amount/1000;
						/*HAL18082020 PLUTONL-2167*/
						ReplaceBillingAmnt(nIndiceCtx,amount,sTlvBuffer);
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
					nRetCode = AdjustTlvAmount (  nIndiceCtx , tmpField , sCurrency , tmpData );
					if ( nRetCode != OK )
					{
						trace_event("Error AdjustTlvAmount(ISO_TRANS_AMOUNT)", ERROR);
						nProcResult = NOK;
					}
					else
					{
						amount = (double)(atoi(tmpData));
						/*HAL18082020 PLUTONL-2167*/
						amount = amount/1000;
						/*HAL18082020 PLUTONL-2167*/
						ReplaceTransactionAmnt(nIndiceCtx,amount,sTlvBuffer);
						PrintTlvBuffer(sTlvBuffer);
					}
				}
			}
		}

		/*AMER20170310 End: PROD00039737*/
	}	
	

	/** getting the CPS data **/
	if(GetIsoField ( ISO_CPS_FIELDS , IsoInfo , tmpField , &nLength ) == SUCCESS )
	{
		PutTlvBuffer ( AUTO_CPS_DATA , sTlvBuffer, tmpField ,  nLength);
	}	
	/** getting response of the AVS  and the Trace_ID for incremental authorisation**/

	/*decomment IBO20220802 PLUTONL-4797*/
	if( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , sTlvPrivateBuffer ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	}

	if(nIsMalfunctionStip == 1)
		PutTlvPrivate( MALFUNCTION_STIP_TAG , &sTlvPrivateInfo , "Y" , 1 );
	/*End IBO20220802 PLUTONL-4797*/

	/*Start AMER20171218 PROD00050359*/
	if (GetTlvPrivate(NET_POS_ENTRY_MODE, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (NET_POS_ENTRY_MODE, &sTlvPrivateInfo, tmpField, nLength);
	}
	/*End AMER20171218 PROD00050359*/
	
    /*Start HAL 20201303 Enh Q2.2020 Art 1.1 */
	if (GetTlvPrivate(NET_REFERENCE_NUMBER, &s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvPrivate (NET_REFERENCE_NUMBER, &sTlvPrivateInfo, tmpField, nLength);
	}
    /*End HAL 20201303 Enh Q2.2020 Art 1.1 */


	/*--Start AMER20170727: PROD00042216*/
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
	/*--End AMER20170727*/
	
	/*Start IBO20200420*/
	if(GetTlvPrivate( EXT_SECURITY_RESULTS_DTL ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (EXT_SECURITY_RESULTS_DTL , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*End IBO20200420*/
	
	/*Start AMER20170928 PROD00047410 Enh17.Q4*/
	if(GetTlvPrivate( SECURITY_LEVEL_IND_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (SECURITY_LEVEL_IND_TAG , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*PLUTONL-3732*/
	if(GetTlvPrivate( AUTH_ADD_INFO ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (AUTH_ADD_INFO , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*PLUTONL-3732*/

	if(GetTlvPrivate( ECOM_AUTH_INFO ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (ECOM_AUTH_INFO , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*End AMER20170928 PROD00047410 Enh17.Q4*/

	/*Start AMER20180305 PROD00055121*/
	if(GetTlvPrivate( AAV_RESULT ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (AAV_RESULT , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*End AMER20180305 PROD00055121*/
	
	if(GetTlvPrivate( ADDRESS_VERIF_REP_TYPE_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (ADDRESS_VERIF_REP_TYPE_TAG, &sTlvPrivateInfo, tmpField, nLength);			
	}

   /*Start HAL20202508 Enh Q4.2020 Art 2.4 */
	if(GetTlvPrivate( EXTERNAL_RESP_SOURCE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (EXTERNAL_RESP_SOURCE, &sTlvPrivateInfo, tmpField, nLength);			
	}
    /*End HAL20202508 Enh Q4.2020 Art 2.4 */
	
	if(GetTlvPrivate( CVV2_RESULT_CODE_TAG ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (CVV2_RESULT_CODE_TAG , &sTlvPrivateInfo, tmpField, nLength);
	}

	if(GetTlvPrivate( MCI_TRACEID ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (MCI_TRACEID, &sTlvPrivateInfo, tmpField, nLength);
	}
	/*AMER20170310 Start*/
	if(GetTlvPrivate( PARTIAL_AMOUNT_TYPE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (PARTIAL_AMOUNT_TYPE, &sTlvPrivateInfo, tmpField, nLength);
	}
	/*AMER20170310 End*/

    /*HAL ENHS 2020Q2 ART 9.3.3 9.3.4*/
    if (GetTlvPrivate(PSD2_DATA ,&s_sTlvPrivateInfo, tmpField, &nLength) == SUCCESS)
    {
	    PutTlvPrivate (PSD2_DATA, &sTlvPrivateInfo, tmpField, nLength);
    } 
    /*HAL ENHS 2020Q2 ART 9.3.3 9.3.4*/
	
	/*AMER20170417 Start: PROD00041307*/
	if(GetTlvPrivate( PAYMENT_ACC_REFERENCE ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (PAYMENT_ACC_REFERENCE, &sTlvPrivateInfo, tmpField, nLength);
	}
	/*AMER20170417 End: PROD00041307*/ 
	if (ISsNetIssSetlmntInfoPresent == 1) /*SKA29032023 PLUTONL-6112*/
	{
		PutTlvPrivate (NET_ISS_SETTL_INFO, &sTlvPrivateInfo, sNetIssSetlmntInfo, NET_SETL_INFO_L);
	}
	
	/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	if(GetTlvPrivate( ECOM_AUTHENTIF_DATA ,&s_sTlvPrivateInfo ,tmpField ,&nLength)== SUCCESS)
	{
		PutTlvPrivate (ECOM_AUTHENTIF_DATA , &sTlvPrivateInfo, tmpField, nLength);
	}
	/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	
	nLength = PrivateBuildTlv (tmpField, &sTlvPrivateInfo);
	if ( nLength > 0 )
	{
		PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer, tmpField ,  nLength);
	}


	/* should not overwirte*/
	if ( GetIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField, &nLength) == SUCCESS)
	{
		PutTlvBuffer ( AUTO_ADD_AMOUNTS  , sTlvBuffer,tmpField, nLength );
	}



	PutTlvBuffer ( AUTO_AUTHO_FLAG         , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC         , sTlvBuffer , vAuthorSrc ,  2);

	PrintTlvBuffer ( sTlvBuffer );

	
	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )                                                                                                         
	{
		nProcResult = NOK;
	}
	else
	{
		/*we should put ActionCode in Iso as well in case we have a VIP translation of an existing ActionCode*/
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, sActionCode, 3);
		
		if( sActionFlag[0] == ACTION_FLAG_APPROVAL )
		{
			PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , APPROVED_EVENT  , 3);/*NAB06042020 PLUTONL-1127*/
		}
	}

	if( nProcResult != OK )
	{
		trace_event("************** Processing Error **************",ERROR);
		DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
		{
			memcpy(sActionCode,MALFUNCTION_ACTION,3);
		}
		
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, sActionCode, 3);
		
		AutomaticReversalProc(  nIndiceCtx,   CAPTURE_MODE, sCaptureCode, sRoutingCode , TableIndicator , sTlvBuffer, P7_F025_REV_MALFUNCTION );		
		nProcResult = OK;
	}


	if( GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength) != SUCCESS )
	{
		if( memcmp(sActionCode,"000",3) == 0 || memcmp(sActionCode,"002",3) == 0)
		{
			sActionFlag[0] = ACTION_FLAG_APPROVAL;
		}
	}


	nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event("Error in UpdateAuthoActivity 1",TRACE);
		if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
		{
			trace_event("Error in UpdateAuthoActivity 2",TRACE);
			AutomaticReversalProc(  nIndiceCtx,   CAPTURE_MODE, sCaptureCode, sRoutingCode , TableIndicator , sTlvBuffer, P7_F025_REV_MALFUNCTION );
		}


		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
		InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );

		DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
		{
			memcpy(sActionCode,MALFUNCTION_ACTION,3);
		}
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, sActionCode, 3);	/*PRODXX: should update the response code to decline */
		

		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event  ( "SEVERE ERROR !!!!!: Can Not Update Autho Activity File ", ERROR );
		return ( NOK );
	}


/*	/ * PLUTONL-2989 * /
	if ( sActionFlag[0] != ACTION_FLAG_APPROVAL )
	{

		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
			nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
		else
			nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );

		/ *ICH20170509 Need to update balance in case of declined autorisation to impact OTB with online fee* /
		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL );
		}
	}
	else
	{
		nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Card Activity  !!!", FATAL );
			/ * What is the action to do * /
		}


	nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK ) 
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL ); 
			/ * What is the action to do * /
			nProcResult = NOK;
		}

		UpdateSettlement( nIndiceCtx , sTlvBuffer);
	}
*/
	nRetCode = UpdatePanActAccBal  ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Update Balance  !!!", FATAL );
		nProcResult = NOK;
	}

	if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
	{
		UpdateSettlement( nIndiceCtx , sTlvBuffer);
	}

	if( nProcResult != OK )
	{
		trace_event("************** Processing Error **************",ERROR);
		DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
		{
			memcpy(sActionCode,MALFUNCTION_ACTION,3);
		}
		
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, sActionCode, 3);
		
		GetTlvBuffer ( AUTO_ACTION_FLAG , sTlvBuffer , sNewActionFlag ,&nLength);
		if(sNewActionFlag [0] != ACTION_FLAG_APPROVAL )
		{
			DelTlvBuffer(AUTO_AUTH_NUMBER,sTlvBuffer,"",0);
			PutIsoField (ISO_AUTHOR_ID , IsoInfo , "" , 0 );
		}
		/*nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );*/
		nRetCode = RollBackAuthoActivity ( nIndiceCtx , sTlvBuffer);/*PLUTONL-4539*/
		if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
		{
			trace_event("Error in UpdateAuthoActivity 2",TRACE);
			/* we do not reversal local balance, we do not know if local balance has been updated or not */
			AutomaticReversalProc(  nIndiceCtx,   CAPTURE_MODE, sCaptureCode, sRoutingCode , TableIndicator , sTlvBuffer, P7_F025_REV_MALFUNCTION );
		}
		nProcResult = OK;
	}
	/*PLUTONL-3762 */
	GetTlvBuffer (AUTO_MSG_FLOW_MAPPING_ID, sTlvBuffer, sMsgID ,  &nLength);
	nMsgId = (msg_id_t)atol(sMsgID);
	MsgLog(nMsgId, sTlvBuffer );
	/*PLUTONL-3762 */
 

	PrintTlvBuffer(sTlvBuffer);
	StopListReplyProcessing (nIndiceCtx,sTlvBuffer);
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	TlvToIso ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );


	memset(tmpField, 0, sizeof(tmpField));
	memset(szWork, 0, sizeof(szWork));
	memset(sLine, 0, sizeof(sLine));
	memset(tmpData, 0, sizeof(tmpData));

	InitTlvPrivate(&sTlvPrivateInfo);
	InitTlvPrivate(&s_sTlvPrivateInfo);
	InitTlvPrivate(&sTlvNetPOSData);
	InitTlvAutho(&sTlvInfo);
	InitTlvPrivate(&tlvInfo);
	InitIsoIcTag(&icInfo);

	trace_event ( "End   AuthReplyProc(OK)", PROCESSING );
	return ( OK );
}

/******************************************************************************/
int  AuthLocalAdm(int nIndiceCtx, char *sTlvBuffer)
{

	char		sActionCode 		[ 3 + 1 ];
	char		sLine 				[ MAX_LINE_TRC ];
	char		tmpField 			[  1024 ];
	int			nRetCode;
	int			nLength;
	char		szWork[256];
	char		sProcCode[32];
	char		sActionFlag[4];
	char		slTransAmount[32];
	char		sFunctionCode[P7_F024_LEN + 4];
	char		sRoutingCode[RESOURCE_ID_LEN + 1];
	char		sCaptureCode[RESOURCE_ID_LEN + 1];
	int			nResult;



	trace_event("Start AuthLocalAdm()",PROCESSING);

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sLine  , 0, sizeof ( sLine ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode ));
	memset ( sActionFlag  , 0, sizeof ( sActionFlag ));
	memset ( slTransAmount  , 0, sizeof ( slTransAmount ));
	memset ( sFunctionCode  , 0, sizeof ( sFunctionCode ));
	memset ( sRoutingCode  , 0, sizeof ( sRoutingCode ));
	memset ( sCaptureCode  , 0, sizeof ( sCaptureCode ));

	GetTlvBuffer ( AUTO_PROCESSING_CODE, sTlvBuffer, sProcCode ,  &nLength);
	GetTlvBuffer ( AUTO_FUNCTION_CODE, sTlvBuffer, sFunctionCode ,  &nLength);
	GetTlvBuffer ( AUTO_CAPTURE_CODE, sTlvBuffer, sCaptureCode ,  &nLength);

	if (GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sRoutingCode, &nLength) != SUCCESS)
	{
		PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
		memcmp(sRoutingCode,STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	}

	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);


	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )
		memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
	else
	{
		if ( memcmp(sActionCode, APPROVED_ACTION, 3) == 0 )
		{
			if( memcmp(sFunctionCode,P7_F024_ACC_VERIF,P7_F024_LEN) == 0 )
			{
				memcpy(sActionCode,NO_REASON_TO_DECLINE,P7_F039_LEN);
			}
		}
	}
	PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,P7_F039_LEN);




	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
		PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3); 
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		PutTlvBuffer ( AUTO_RESPONSE_CODE ,sTlvBuffer  , MALFUNCTION_ACTION , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
		if ( nRetCode != OK )                                                                                                         
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		nRetCode = SEVERE_ERROR;
	}



	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);


	if ( sActionFlag[0] != ACTION_FLAG_APPROVAL )
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) == 0 )
			nResult = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
		else
			nResult = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
	}
	else
	{
		nResult = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
	}

	if ( nResult != OK )
	{
		trace_event ( "Can Not Update Card Activity  !!!", FATAL );
		/* What is the action to do */
	}


	/* MKB070508 : si pin change, pas de maj de bal ou de settlement */
	if (memcmp(sProcCode, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_ACC_VERIF,P7_F024_LEN) == 0 )
	{
		sprintf(sLine,"End   AuthLocalAdm( %d)",nRetCode);
		trace_event(sLine,PROCESSING);
		return(OK);
	}



	/* check if the destination resource would like to be informed about approvals/declines*/
	if( memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer, sCaptureCode, sRoutingCode);
	}



	nResult = PrintTlvBuffer ( sTlvBuffer );



	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sLine  , 0, sizeof ( sLine ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode ));

	sprintf(sLine,"End   AuthLocalAdm(%d)",nRetCode);
	trace_event(sLine,PROCESSING);

	return ( nRetCode );
}

/*YDA20171006 see PROD00047328*/
int CheckCrossStandInFlags (  int nIndiceCtx, char *sTlvBuffer, int nIsFinancial, char *sAlternAmount,char *szCaptureCode){

	char			sOrigSecurityFlags[ SEC_FLAG_LEN + 1 ];
	char			sDeltaSecurityFlags[ SEC_FLAG_LEN + 1 ];
	char			sNewSecurityFlags[ SEC_FLAG_LEN + 1 ];
	char			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char            sAavResult [1 + 1];
	int				nLength;
	int				flagsUpdated = 0;
	char			sLine 				[ MAX_LINE_TRC ];
	int				i;

	trace_event("Start CheckCrossStandInFlags()", PROCESSING);


	memset ( sOrigSecurityFlags   , 0,  sizeof (sOrigSecurityFlags));
	memset ( sNewSecurityFlags , 0,  sizeof ( sNewSecurityFlags));
	memset ( sDeltaSecurityFlags , 0,  sizeof ( sDeltaSecurityFlags));
	memset ( sSecurityResults   , 0,  sizeof (sSecurityResults));
	memset ( sAavResult   , 0,  sizeof (sAavResult));


	/*INIT EVENT CODE & RESPONSE CODE*/
	DelTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,"",0);
	DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"",0);

	/*GET ON MODE SECURITY RESULTS*/
	GetTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, sOrigSecurityFlags ,  &nLength);
	GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults ,  &nLength);

	memcpy(sNewSecurityFlags,sOrigSecurityFlags,SEC_FLAG_LEN);

	/*GET OFF MODE FLAGS,RESULTS*/
	if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "OFF") != OK)
		return NOK;



	/*SET ON MODE SECURITY RESULT INTO OFF MODE SECURITY RESULTS*/
	/*YDA201171222 : Don't put authorization online security verification results in case when is empty*/
	if(strlen(sSecurityResults) > 0)
	{
		PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults ,  nLength);

		/*YDA20180104 SEE PROD00047328*/
		/*FILL VALUE N INTO CHECKED FLAGS IN ON MODE*/

		/*GET OFF MODE SECURITY FLAGS*/
		GetTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, sDeltaSecurityFlags ,  &nLength);

		for(i=0; i < SEC_FLAG_LEN; i++ )
		{
			if( sDeltaSecurityFlags[i] != 'N' && sNewSecurityFlags[i] != sDeltaSecurityFlags[i] )
			{
				sNewSecurityFlags[i] = sDeltaSecurityFlags[i];
			}

			switch( i  )
			{
			case SEC_FLAG_PIN_MTHD:
				continue;
				break;
			case SEC_FLAG_CHK_LIMITS:
				continue;
				break;
			case SEC_FLAG_CHK_AAV:
				if( sDeltaSecurityFlags[i] == 'Y' && sSecurityResults[i] != P7_F048_P54_CAVV_NOT_YET_VERIFIED /*P7_F048_P54_CAVV_NOT_PRESENT*/ )		/*AMER20180322 PROD00054205 Art 1.1*/
				{
					sDeltaSecurityFlags[i] = 'N';
					sAavResult [0] = sSecurityResults[i];
				}
				break;
			case SEC_FLAG_CHK_CVV1:
				if( sDeltaSecurityFlags[SEC_FLAG_CHK_CVV1] == 'Y' && 
					( sSecurityResults[SEC_FLAG_CHK_CVV1] != P7_CTRL_CHECK_NOPROC || sSecurityResults[SEC_FLAG_CHK_ICVV] != P7_CTRL_CHECK_NOPROC || sSecurityResults[SEC_FLAG_CHK_ARQC] != P7_CTRL_CHECK_NOPROC ))
				{
					sDeltaSecurityFlags[i] = 'N';
				}
				break;
			case SEC_FLAG_CHK_ICVV:
				if( sDeltaSecurityFlags[SEC_FLAG_CHK_ICVV] == 'Y' && 
					( sSecurityResults[SEC_FLAG_CHK_ICVV] != P7_CTRL_CHECK_NOPROC || sSecurityResults[SEC_FLAG_CHK_CVV1] != P7_CTRL_CHECK_NOPROC || sSecurityResults[SEC_FLAG_CHK_ARQC] != P7_CTRL_CHECK_NOPROC ))
				{
					sDeltaSecurityFlags[i] = 'N';
				}
				break;
			default:
				if( sDeltaSecurityFlags[i] != 'N' && sSecurityResults[i] != P7_CTRL_CHECK_NOPROC )
				{
					sDeltaSecurityFlags[i] = 'N';
				}
				break;
			}
		}

		trace_event("SECURITY OFF FLAGS",TRACE);
		sprintf(sLine,"Original: [%s]", sOrigSecurityFlags );
		trace_event(sLine,PROCESSING);

		sprintf(sLine,"Delta:    [%s]", sDeltaSecurityFlags );
		trace_event(sLine,PROCESSING);

		sprintf(sLine,"New:      [%s]", sNewSecurityFlags );
		trace_event(sLine,PROCESSING);

		PutTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS , sTlvBuffer, sDeltaSecurityFlags ,  nLength);
	}

	/*OFF MODE PRECONTROL*/
	if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
		return NOK;

	/*OFF MODE SECURITY VERIFICATION*/
	if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
		return NOK;

	if (strlen (sAavResult) > 0 )
	{
		GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityResults ,  &nLength);
		sSecurityResults [SEC_FLAG_CHK_AAV] = sAavResult [0];
		PutTlvBuffer ( AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults ,  nLength);
	}
	
	/*Check Limits with OFF flag*/
	if( nIsFinancial == OK  )
	{
		if (CheckLimits(nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount) != OK)
			return NOK;
	}

	/*Check FallBack with OFF flag*/
	/*No fallback check for voice auths*/
	if( memcmp(szCaptureCode,DSCREEN_RES_CODE,RESOURCE_CODE_LEN) != 0 )
	{
		if (CheckFallBack(nIndiceCtx, "OFF", sTlvBuffer) != OK)
			return NOK;
	}


	PutTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS , sTlvBuffer, sNewSecurityFlags ,  nLength);


	PrintTlvBuffer(sTlvBuffer);

	trace_event("End CheckCrossStandInFlags(OK)", PROCESSING);
	return OK;
}
