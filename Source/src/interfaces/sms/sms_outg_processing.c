#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <tlv_autho.h>
#include <list_thr.h>
#include <resources.h>
#include <tlv_autho.h>
#include <sms_var.h>
#include <sms_param.h>

#include <tlv_privfields.h>
#include <tlv_private.h>

int OutgoingManagement( int nIndiceCtx , char *sResource )
{
	TSSmsInfo    sSmsInfo;
	TSTlvAutho   sTlvInfo;
	TSNodeData   Node;

	char     sMti          [ 4 + 1];
	char     sResponseCode [ 4 + 1];
	char     sBuffer       [ LG_MAX ];
	char     sTlvBuffer    [ LG_MAX ];
	char     sLine         [ MAX_LINE_TRC ];
	time_t   tPurgeTime;
	int      nMti, nLength;
	int      nRetCode = NOK;

	/****** Initialisation des variables et structures de travail *****/
	memset ( sMti  ,         0 , sizeof ( sMti  ));
	memset ( sResponseCode , 0 , sizeof ( sResponseCode ));
	memset ( sBuffer ,       0 , sizeof ( sBuffer ));
	memset ( sTlvBuffer ,    0 , sizeof ( sTlvBuffer ));

	InitSmsInfo ( &sSmsInfo );

	do { 
		sleep ( 30 );
		if (!memcmp(ResourceStatus, "ONL", 3))
			nRetCode = GetOutgoingData  ( nIndiceCtx, "0", sTlvBuffer  );
	} while ( nRetCode != OK );

	trace_event ( "Read  Outgoing Data", PROCESSING );
	trace_event ( "========= === ======", PROCESSING );

	PrintTlvBuffer (  sTlvBuffer );

	InitTlvAutho( &sTlvInfo);
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);

	TlvToSms ( nIndiceCtx ,  &sTlvInfo , &sSmsInfo );

	nRetCode = FCT_OUTG_REQ_TO_NW(nIndiceCtx,  &sSmsInfo, sTlvBuffer);
	if (nRetCode != OK )
	{
		sprintf (sLine, "Could not send Outgoing message %4d through line.", sSmsInfo.nMsgType);
		trace_event ( sLine , PROCESSING );
		return ( NOK );
	}

	trace_event ("Sent Outgoing message.", PROCESSING);
	nRetCode = WaitOutgoingAnswer();
	trace_event ("Response to OUTGOING message received.", PROCESSING);

	return ( nRetCode );
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
int OutgoingResponseTraitement( int nIndiceCtx, 
	char *sTlvBuffer, 
	char *sResponseCode)
{
	TSTlvAutho          sTlvInfo;
	TSTlvPrivate        sPrivateTlvInfo;
	char                sInternalTlvBuff [ LG_MAX ]; 
	char                tmpField  [ BUF_LG_MAX ]; 
	char                sFlagSend [2];
	char           sLine [MAX_LINE_TRC];
	char           yymmdd [8];
	char           sTime    [7];
	char           sDate    [5];
	char           sIntMonth    [3];
	char           sCurrency    [4];
	char           tmpData      [20];
	char           sExtMonth    [3];
	char           sIntDate     [5];
	char           sIntYear     [3];
	int            nYear;
	int            nOrigYear;
	int            nLength;
	int        		nRetCode , tmpFieldLen; 


	trace_event("Start OutgoingResponseTraitement()",PROCESSING);

	/**** Initialisation des variables de travail  ***/
	memset ( tmpField,         0,   sizeof ( tmpField));
	memset ( sInternalTlvBuff, 0,   sizeof ( sInternalTlvBuff));
	memset ( sFlagSend,        0,   sizeof ( sFlagSend));
	memset (yymmdd,          0, sizeof (yymmdd));
	memset (sIntDate,        0, sizeof (sIntDate));
	memset (sIntYear,   0, sizeof (sIntYear));
	memset (sIntMonth,  0, sizeof (sIntMonth));
	memset (sExtMonth,  0, sizeof (sExtMonth));
	memset (sTime,  0, sizeof (sTime));
	memset (sDate,  0, sizeof (sDate));


	trace_event ( "Outgoing Response Received Processing ..", PROCESSING ); 

	if (!memcmp(sResponseCode, "00", 2))
		sFlagSend[0] = '2';
	else 
		sFlagSend[0] = '1';

	sFlagSend[1] = '\0';

	nRetCode = UpdateOutgoingData ( nIndiceCtx, sFlagSend, sTlvBuffer);

	InitTlvAutho( &sTlvInfo);                                                                                                      
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);

	TranslateRespCode (nIndiceCtx, ResourceId, sResponseCode, "RP", tmpField);

	PutTlvAutho ( AUTO_RESPONSE_CODE, &sTlvInfo, tmpField, 3);
	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, "MSC", 3);
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo, ResourceId, 6);
	PutTlvAutho ( AUTO_CAPTURE_CODE, &sTlvInfo, ResourceId, 6);

	

	memset ( tmpField,     0 , sizeof ( tmpField));
	memset ( tmpData,      0 , sizeof ( tmpData));
	memset ( sCurrency,    0 , sizeof ( sCurrency));

	GetTlvAutho ( AUTO_TRAN_CURRENCY_CODE, &sTlvInfo, sCurrency, &nLength) ;
	if ( GetTlvAutho ( AUTO_AMOUNT_TRANSACTION, &sTlvInfo, tmpField, &nLength) ==SUCCES )
	{
		nRetCode = AdjustTlvAmount (  nIndiceCtx , tmpField , sCurrency , tmpData );
		if ( nRetCode != OK )
		{
			trace_event("Error AdjustTlvAmount(ISO_TRANS_AMOUNT)", ERROR);
			return ( NOK );
		}
		PutTlvAutho (AUTO_AMOUNT_TRANSACTION, &sTlvInfo, tmpData, 18);
	}

	/*****   Adjust  Processing Code *****/ /* HF  10.04.2006 */
	memset ( tmpField,     0 , sizeof ( tmpField));
	memset ( tmpData,      0 , sizeof ( tmpData));
	if (GetTlvAutho ( AUTO_PROCESSING_CODE, &sTlvInfo, tmpField, &nLength) ==SUCCES ) 
	{

		sprintf(tmpData,    "%.3s002%.2s", PROC_TRANS_TYPE, tmpField );
		sprintf(tmpData+8,  "%.3s002%.2s", PROC_ACCOUNT_FROM, tmpField+2);
		sprintf(tmpData+16, "%.3s002%.2s", PROC_ACCOUNT_TO, tmpField+4);
		PutTlvAutho (AUTO_PROCESSING_CODE, &sTlvInfo, tmpData, 24);
	}
	/*****   End Adjust  *****/ /* HF  10.04.2006 */

	memset(tmpField, 0, sizeof(tmpField));
	current_date_yymmdd(tmpField);
	current_hour       (tmpField + 6);

	PutTlvAutho ( AUTO_DATE_TIME_LOCAL_TRAN, &sTlvInfo, tmpField, 12);

	/* 48 in TLV format. */
	InitTlvPrivate (&sPrivateTlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG, &sPrivateTlvInfo, AcquirerBank, 6);
	if (GetTlvAutho (AUTO_INTERNAL_STAN, &sTlvInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG, &sPrivateTlvInfo, tmpField, 6);

	tmpFieldLen = PrivateBuildTlv (tmpField, &sPrivateTlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sTlvInfo, tmpField, tmpFieldLen);

	AuthoBuildTlv ( sInternalTlvBuff, &sTlvInfo);

	/********* Insertion dans Autho Activity **********/
	InsertionAuthoActivity( nIndiceCtx, sInternalTlvBuff);

	OutgoingRespReceived();

	trace_event("End   OutgoingResponseTraitement(OK)",PROCESSING);
	return ( OK );
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
int OutgoingTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  )
{
	TSTlvAutho      sTlvInfo;
	int        		nRetCode , nLength;

	trace_event("Start OutgoingTimeOutTraitement()",PROCESSING);
	PrintTlvBuffer(sTlvBuffer);

	nRetCode = UpdateOutgoingData ( nIndiceCtx, "1", sTlvBuffer);

	nRetCode = OutgoingTimeOutDetected();

	trace_event("End   OutgoingTimeOutTraitement()",PROCESSING);
	return(OK);
}

/******************************************************************************/
/** Fonction    : FCT_OUTG_REQ_TO_NW ()                                       */
/** Description : Fonction qui traite et envoie a Visa les messages Outgoing  */ 
/******************************************************************************/
int FCT_OUTG_REQ_TO_NW (int         P_nIndexCtx,
	TSSmsInfo   *P_psSmsInfo,
	char        *sTlvBuffer)
{
	TSNodeData      node;
	char            sLine     [MAX_LINE_TRC];
	char            data      [BUF_LG_MAX];
	char            tmpField  [BUF_LG_MAX];
	char            sPurgeTime[10];
	char            sBuffer   [LG_MAX];
	int             tmpFieldLen;
	int             nLength;
	int             length;
	int             result;
	int             nBalMsg;
	/*time_t          tPurgeTime;*/
	struct timeval	sIssPurgeTimeVal;

	char            CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));

	trace_event ("Start FCT_OUTG_REQ_TO_NW()", PROCESSING);

	/**** Initialisation des variables et structures de travail ****/
	memset (sLine ,         0,    sizeof (sLine));
	memset (data ,          0,    sizeof (data));
	memset (tmpField ,      0,    sizeof (tmpField));
	memset (sPurgeTime ,    0,    sizeof (sPurgeTime));
	memset (sBuffer ,       0,    sizeof (sBuffer));

	/* Insert original ISO message in list for later retrieval */
	trace_event("----- Start Chainage Outgoing ------",TRACE);
	InitNodeData(&node);
	/*GetIssuerPurgeTime(&tPurgeTime);
	node.tPurgeTime= tPurgeTime ;*/
	
	/*Start AMER20211004 PLUTONL-3579*/
	GetIssuerPurgeTimeMS(&sIssPurgeTimeVal);

	node.tPurgeTime = sIssPurgeTimeVal.tv_sec;
	node.stPurgeTime.tv_sec = sIssPurgeTimeVal.tv_sec;
	node.stPurgeTime.tv_usec = sIssPurgeTimeVal.tv_usec;
	/*End AMER20211004 PLUTONL-3579*/

	sprintf (tmpField,  "%04d", P_psSmsInfo->nMsgType);

	GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, data, &length);

	memcpy (node.szFstKey, tmpField, 4);
	memcpy (node.szFstKey + 4,  data, 6);

	sprintf(sLine,"Cle de Chainage REQ Is :[%s]", node.szFstKey);
	trace_event(sLine,TRACE);

	memcpy (node.sSndPrvtData + 0 , ResourceId    , 6 );
	node.nSndPrvtDataLen = 6;

	memcpy (node.sFstPrvtData , "OUT" , 3);
	node.nFstPrvtDataLen = 3;

	node.nSndBufLen = strlen ( sTlvBuffer );
	memcpy (node.sSndBuf, sTlvBuffer , node.nSndBufLen);

	if (InsertRequest('L', &node) != OK)
	{
		trace_event ("FCT_OUTG_REQ_TO_NW(NOK): InsertRequest() failure", ERROR);
		return (NOK) ;
	}

	/***** Send this buffer to the Network *****/
	memset (tmpField,      0, sizeof (tmpField));  
	GetSmsField (SMS_CARD_NBR, P_psSmsInfo, tmpField, &tmpFieldLen);
	if (debug_option == 1)
	{
		sprintf (sLine, " Card Nbr is: [%s]", tmpField);
		trace_event (sLine, PROCESSING);
	}
	else
	{
		TrunCard(tmpField, CardTrunc);
		sprintf (sLine, " Card Nbr is: [%s]", CardTrunc);
		trace_event (sLine, PROCESSING);
	}

	nLength = SmsBuildMsg (sBuffer+SMS_HDR_LEN, P_psSmsInfo);
	SmsBuildHeader(REQUEST , &(P_psSmsInfo->sHeader), NULL, nLength, StationId);

	memcpy(sBuffer, (char *) &(P_psSmsInfo->sHeader), SMS_HDR_LEN);
	nLength += SMS_HDR_LEN;

	DumpSms (sBuffer);

	if (WriteLineMsg(sBuffer, nLength) != OK)
	{
		/* free request from list */
		FetchRequest ('L', &node);

		trace_event ("FCT_OUTG_REQ_TO_NW(NOK): WriteLineMsg() failure", ERROR);
		return (NOK) ;
	}

	trace_event ("End  FCT_OUTG_REQ_TO_NW (OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/** Fonction    : FCT_OUTG_REP_FROM_NW ()                                     */
/** Description : Fonction qui traite les reponses Outgoing recues de VISA    */
/******************************************************************************/
int FCT_OUTG_REP_FROM_NW (int         P_nIndexCtx,
	TSSmsInfo   *P_psSmsInfo)
{
	TSNodeData      node;
	char            sLine      [MAX_LINE_TRC];
	char            data       [BUF_LG_MAX];
	char            tmpField   [BUF_LG_MAX];
	char            sTlvBuffer [LG_MAX];
	char            sPurgeTime [10];
	char            sBuffer    [LG_MAX];
	char            sRespCode  [2+1];
	int             tmpFieldLen;
	int             nLength;
	int             Length;
	int             result;
	int             nBalMsg;
	time_t          tPurgeTime;

	trace_event ("Start FCT_OUTG_REP_FROM_NW()", PROCESSING);

	/**** Initialisation des variables et structures de travail ****/
	memset (data ,          0,    sizeof (data));
	memset (tmpField ,      0,    sizeof (tmpField));
	memset (sTlvBuffer ,    0,    sizeof (sTlvBuffer));
	memset (sPurgeTime ,    0,    sizeof (sPurgeTime));
	memset (sBuffer ,       0,    sizeof (sBuffer));
	memset (sLine ,         0,    sizeof (sLine));
	memset (sRespCode ,   ' ',    sizeof (sRespCode));

	/***** Fetch Request   ****/ 
	trace_event("----- Start Dechainage Outgoing ------",TRACE);
	InitNodeData (&node);

	GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, data, &Length);

	sprintf (tmpField, "%04d", P_psSmsInfo->nMsgType - 10);
	memcpy (node.szFstKey, tmpField, 4);
	memcpy (node.szFstKey + 4,  data, 6);
	sprintf(sLine,"Cle de dechainage REP Is : [%s]", node.szFstKey);
	trace_event(sLine,TRACE);

	switch (FetchRequest ('L', &node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End FCT_OUTG_REP_FROM_NW(NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	memcpy ( sTlvBuffer , node.sSndBuf, node.nSndBufLen );

	if (GetSmsField (SMS_RESPONSE_CODE, P_psSmsInfo, sRespCode, &Length) == SUCCESS)
	{
		sprintf (sLine, "RESPONSE CODE IS : %s", sRespCode );
		trace_event (sLine, PROCESSING);
		OutgoingResponseTraitement ( P_nIndexCtx, sTlvBuffer, sRespCode );
	}

	trace_event ("End   FCT_OUTG_REP_FROM_NW(OK)", PROCESSING);
	return(OK);
}

