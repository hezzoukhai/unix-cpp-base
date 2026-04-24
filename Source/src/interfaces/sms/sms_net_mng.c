/****************************************************************************/
/*                   NETWORK MANAGEMENT ROUTINES                            */
/****************************************************************************/
#include <define.h>
#include <unistd.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_puf.h>
#include <sms_param.h>
#include <main_thr.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>
#include <sms_define.h>
#include <p7_sid.h>
#include <iso8583.h>
#include <event_log.h>

static int	MaxSendEchoTest = 0;/*HAL26082020 PLUTONL-2185*/

int    SmsEchoedField_0800[] =
{
	/*001*/   0,   /* BIT_MAP2                            */
	/*002*/   1,   /* CARD_NBR                            */
	/*003*/   1,   /* PROC_CODE                           */
	/*004*/   1,   /* TRANS_AMOUNT                        */
	/*005*/   0,   /* STLMNT_AMOUNT                       */
	/*006*/   0,   /* CARDHOLDER BILLING AMOUNT           */
	/*007*/   1,   /* XMIT_TIME                           */
	/*008*/   0,   /* CARDHOLDER BILLING FEE              */
	/*009*/   0,   /* CONVRSN_RATE_STLMNT                 */
	/*010*/   0,   /* CONVRSN_RATE_CARD_HLDR_BILLING      */
	/*011*/   1,   /* AUDIT_NBR                           */
	/*012*/   0,   /* TRANS_TIME                          */
	/*013*/   0,   /* TRANS_DATE                          */
	/*014*/   0,   /* EXPIRY_DATE                         */
	/*015*/   0,   /* STLMNT_DATE                         */
	/*016*/   0,   /* CNVRSN_DATE                         */
	/*017*/   0,   /* CAPTURE_DATE                        */
	/*018*/   0,   /* MERCHANT_TYPE                       */
	/*019*/   1,   /* ACQR_COUNTRY                        */
	/*020*/   0,   /* PAN_COUNTRY                         */
	/*021*/   0,   /* FORWD_COUNTRY                       */
	/*022*/   0,   /* POS_ENTRY_MODE                      */
	/*023*/   0,   /* CARD SEQUENCE NUMBER                */
	/*024*/   0,   /* NETWORK INTERNATIONAL IDENTIFIER    */
	/*025*/   1,   /* POS_CONDITION                       */
	/*026*/   0,   /* POS_PIN_CAPTURE                     */
	/*027*/   0,   /* AUTHORIZATION ID RESPONSE LENGTH    */
	/*028*/   0,   /* TRANS_FEE                           */
	/*029*/   0,   /* AMOUNT SETTLEMENT FEE               */
	/*030*/   0,   /* AMOUNT TRANSACTION PROCESSING FEE   */
	/*031*/   0,   /* AMOUNT SETLMNT PROCESSING FEE       */
	/*032*/   1,   /* ACQR_ID                             */
	/*033*/   1,   /* FORWD_ID                            */
	/*034*/   0,   /* PAN_EXTENDED                        */
	/*035*/   0,   /* T2_DATA                             */
	/*036*/   0,   /* TRACK 3 DATA                        */
	/*037*/   1,   /* REFERENCE_NBR                       */
	/*038*/   0,   /* AUTHOR_ID                           */
	/*039*/   1,   /* RESPONSE_CODE                       */
	/*040*/   0,   /* SERVICE RESTRICTION CODE            */
	/*041*/   1,   /* TERMINAL_NBR                        */
	/*042*/   1,   /* OUTLET_NBR                          */
	/*043*/   0,   /* TERMINAL_ADR                        */
	/*044*/   0,   /* ADTNL_RESP_DATA                     */
	/*045*/   0,   /* T1_DATA                             */
	/*046*/   0,   /* ADDITRIONAL DATA ISO                */
	/*047*/   0,   /* ADDITIONAL DATA NATIONAL            */
	/*048*/   0,   /* ADTNL_DATA_PRIV                     */
	/*049*/   0,   /* TRANS_CRNCY                         */
	/*050*/   0,   /* STLMNT_CRNCY                        */
	/*051*/   0,   /* BILLING_CRNCY                       */
	/*052*/   0,   /* PIN_DATA                            */
	/*053*/   1,   /* SECURITY_DATA                       */
	/*054*/   0,   /* ADTNL_AMOUNTS                       */
	/*055*/   0,   /* Reserved Iso                        */
	/*056*/   0,   /* Reserved Iso                        */
	/*057*/   0,   /* Reserved National                   */
	/*058*/   0,   /* Reserved National                   */
	/*059*/   0,   /* GEOGR_DATA                          */
	/*060*/   0,   /* ADTNL_POS_INFO                      */
	/*061*/   0,   /* OTHER_AMOUNTS                       */
	/*062*/   1,   /* CPS_FIELDS                          */
	/*063*/   1,   /* PRIV_USE_FIELDS                     */
	/*064*/   0,   /* MAC1                                */
	/*065*/   0,   /*                                     */
	/*066*/   0,   /* STLMNT_CODE                         */
	/*067*/   0,   /* Extended Payment Code               */
	/*068*/   0,   /* RECEIVER_COUNTRY                    */
	/*069*/   0,   /* STLMNT_INST_COUNTRY                 */
	/*070*/   1,   /* NW_MNGMT_INFO                       */
	/*071*/   0,   /* Message Number                      */
	/*072*/   0,   /* Message number Last                 */
	/*073*/   0,   /* ACTION_DATE                         */
	/*074*/   0,   /* CR_NBR                              */
	/*075*/   0,   /* CR_REV_NBR                          */
	/*076*/   0,   /* DB_NBR                              */
	/*077*/   0,   /* DB_REV_NBR                          */
	/*078*/   0,   /* Transfer Number                     */
	/*079*/   0,   /* Transfer Reversal Number            */
	/*080*/   0,   /* Inquiries Number                    */
	/*081*/   0,   /* Authorizations Number               */
	/*082*/   0,   /* Credit, Processing Fee Amount       */
	/*083*/   0,   /* Credit, Transaction Fee Amount      */
	/*084*/   0,   /* Debit, Processing Fee Amount        */
	/*085*/   0,   /* Debit, Transaction Fee Amount       */
	/*086*/   0,   /* CR_AMOUNT                           */
	/*087*/   0,   /* CR_REV_AMOUNT                       */
	/*088*/   0,   /* DB_AMOUNT                           */
	/*089*/   0,   /* DB_REV_AMOUNT                       */
	/*090*/   0,   /* ORIG_DATA                           */
	/*091*/   0,   /* FILE_UPDATE                         */
	/*092*/   0,   /* FILE_SECURITY                       */
	/*093*/   0,   /* Response Indicator                  */
	/*094*/   0,   /* Service Indicator                   */
	/*095*/   0,   /* REPLACE_AMOUNTS                     */
	/*096*/   0,   /* MSG_SECURITY                        */
	/*097*/   0,   /* NET_AMOUNT                          */
	/*098*/   0,   /* Payee                               */
	/*099*/   0,   /* STLMNT_INSTITN                      */
	/*100*/   0,   /* RECEIVER_ID                         */
	/*101*/   0,   /* FILE_NAME                           */
	/*102*/   0,   /* ACCOUNT_ID1                         */
	/*103*/   0,   /* ACCOUNT_ID2                         */
	/*104*/   0,   /* Transaction Description             */
	/*105*/   0,   /* Reserved Iso                        */
	/*106*/   0,   /* Reserved Iso                        */
	/*107*/   0,   /* Reserved Iso                        */
	/*108*/   0,   /* Reserved Iso                        */
	/*109*/   0,   /* Reserved Iso                        */
	/*110*/   0,   /* Reserved Iso                        */
	/*111*/   0,   /* Reserved Iso                        */
	/*112*/   0,   /* Reserved National                   */
	/*113*/   0,   /* Reserved National                   */
	/*114*/   0,   /* Reserved National                   */
	/*115*/   0,   /* ADTNL_TRACE_DATA                    */
	/*116*/   0,   /* Reserved National                   */
	/*117*/   0,   /* Reserved National                   */
	/*118*/   0,   /* Intra Country Data                  */
	/*119*/   0,   /* Reserved National                   */
	/*120*/   0,   /* Original Message Type Id            */
	/*121*/   0,   /* Issuing Institution Id              */
	/*122*/   0,   /* Remaining Open to Use               */
	/*123*/   0,   /* ADR_VERIF_DATA                      */
	/*124*/   0,   /* Free Form Text - Japan              */
	/*125*/   0,   /* SUPPORT_INFO                        */
	/*126*/   0,   /* Reserved Private                    */
	/*127*/   0,   /* File Record Action and Data         */
	/*128*/   0,   /* MAC2                                */
	/*129*/   0,   /* Field 129                           */
	/*130*/   0,   /* Terminal Capability Profile         */
	/*131*/   0,   /* Terminal Verification Results       */
	/*132*/   0,   /* Unpredictable Number                */
	/*133*/   0,   /* Terminal Serial Number              */
	/*134*/   0,   /* Sms Discretionary Data             */
	/*135*/   0,   /* Issuer Discretionary Data           */
	/*136*/   0,   /* Cryptogram                          */
	/*137*/   0,   /* Application Trans. Counter          */
	/*138*/   0,   /* Application Inter. Profile          */
	/*139*/   0,   /* ARPC + Response Code                */
	/*140*/   0,   /* Field 140                           */
	/*141*/   0,   /* Field 141                           */
	/*142*/   0,   /* Issuer Script                       */
	/*143*/   0,   /* Issuer Script Results               */
	/*144*/   0,   /* Cryptogram Trans Type               */
	/*145*/   0,   /* Terminal Country Code               */
	/*146*/   0,   /* Terminal Trans  Date                */
	/*147*/   0,   /* Cryptogram Amount                   */
	/*148*/   0,   /* Cryptogram Currency Code            */
	/*149*/   0,   /* Cryptogram ChargeBack Amount        */

};


/****************************************************************************/
/*  FCT_NW_MNG_REQ_TO_NW()                                                  */
/*                                                                          */
/*  Sends a Network Management Request Message (0800) to SMS upon          */
/*  reception of a 1606 (service message) request.                          */
/****************************************************************************/
int  FCT_NW_MNG_REQ_TO_NW (int nIndexCtx, TSIsoInfo *IsoInfo)
{
	TSSmsInfo    SmsInfo;
	TSNodeData   Node;
	TSPufInfo    PufInfo;
	char         FieldData [BUF_LG_MAX];
	char         sLine     [MAX_LINE_TRC];
	char         sStan     [7];
	char         sentRefNbr[13];
	char         sGmtDate  [13];
	char         sBuffer   [LG_MAX];
	int          FieldLen;
	time_t       tPurgeTime;
	int          Length;    		
	char            app_pid[9];
	char            app_ppid[9];

	trace_event ("Start  FCT_NW_MNG_REQ_TO_NW()", PROCESSING);

	/***** Initialisation des variables de travail ***/
	memset (FieldData,   0, sizeof(FieldData));
	memset (sStan,       0, sizeof(sStan));
	memset (sentRefNbr,  0, sizeof(sentRefNbr));
	memset (sGmtDate,    0, sizeof(sGmtDate));
	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	memset(sBuffer, 0, sizeof(sBuffer));

	/* Insert original request 1606 in queue for later processing */
	InitNodeData(&Node);

	GetAcquirerPurgeTime(&tPurgeTime);
	Node.tPurgeTime= tPurgeTime;

	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1606", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	sprintf(sLine, " Cle de chainage %s", Node.szFstKey);
	trace_event(sLine, PROCESSING);

	Length = IsoBuildMsg (Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	if (InsertRequest('B', &Node) != OK)
	{
		trace_event ("FCT_NW_MNG_REQ_TO_NW(NOK): InsertRequest() failed", ERROR);
		return(NOK);
	}
	dump_p7_struct(IsoInfo);

	InitSmsInfo (&SmsInfo);
	SmsInfo.nMsgType = 800;

	/* (7) Transmission Date/Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, FieldData, &FieldLen);
	AddSmsField (SMS_XMIT_TIME, &SmsInfo, FieldData, FieldLen);

	/* (11) System Trace Audit Number */
	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	AddSmsField (SMS_AUDIT_NBR, &SmsInfo, FieldData, FieldLen);
	memcpy (sStan, FieldData, 6);

	/* (37) Reference Number */ 
	/* (63.1) Network Id */
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	FieldLen = PufBuildField (FieldData, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &SmsInfo, FieldData, FieldLen);

	/* (70) Network Management Info Code */
	/* ISO field 24 holds the code */
	GetIsoField (ISO_FUNCTION_CODE, IsoInfo, FieldData, &FieldLen);
	switch (atoi(FieldData))
        {
	case 603:/* signon */
		memcpy(FieldData, SMS_F070_SIGNON, SMS_F070_LEN);
		LogEvent(	nIndexCtx,
					ResourceId,
					"##",
					INTERFACE_SIGN_ON,
					"##",
					"##",
					"##",
					app_pid,
					app_ppid);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Sign On Request              -------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;

	case 604: /* signoff */
		memcpy (FieldData, SMS_F070_SIGNOFF, SMS_F070_LEN);
		memcpy(ResourceStatus, SIGNOFF, 3);
		UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNOFF);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Sign Off Request              ------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );

		LogEvent(	nIndexCtx,
					ResourceId,
					"##",
					INTERFACE_SIGN_OFF,
					"##",
					"##",
					"##",
					app_pid,
					app_ppid);

		break;
		/*Start ADD KS050706*/
	case 607:		/* start SAF */
		memcpy (FieldData, SMS_F070_START_SAF, SMS_F070_LEN);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    start saf Request              ------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;
	case 608: /* end SAF */
		memcpy (FieldData, SMS_F070_STOP_SAF, SMS_F070_LEN);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    end saf Request              ------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;


	case 609: /* echo test */
		memcpy(FieldData, SMS_F070_ECHO_TEST, SMS_F070_LEN);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Echo Test Request               ----------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;
	case 613: /* signon advice */
		memcpy (FieldData, "078", 3);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Sign On Advice               -------------" , PROCESSING );                          
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;
	case 614: /* signoff */
		memcpy (FieldData, "079", 3);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Sign Off Advice              -------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;
	case 615: /* Request for Reconciliation */
		memcpy (FieldData, "280", 3);
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		trace_event ( "-----------    Request for Reconciliation   -------------" , PROCESSING );
		trace_event ( "---------------------------------------------------------" , PROCESSING );
		break;

	}

	AddSmsField (SMS_NW_MNGMT_INFO, &SmsInfo, FieldData, SMS_F070_LEN);

	if (!memcmp(FieldData, "280", 3))
		AddSmsField (SMS_STLMNT_INSTITN, &SmsInfo, AcquirerId, strlen(AcquirerId));

	/* send message */
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	dump_buffer((unsigned char *)sBuffer, Length, 'E', 'L', 'O');

 
	DumpSms(sBuffer);
 


	if (WriteLineMsg(sBuffer, Length))
	{
		trace_event ("FCT_NW_MNG_REQ_TO_NW(): WriteLineMsg() failed.", ERROR);
		FetchRequest('B', &Node);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	trace_event ("End  FCT_NW_MNG_REQ_TO_NW(OK)", PROCESSING);
	return (OK);
}


/***************************************************************************/
/*  FCT_NET_MGT_REP_FROM_NW()                                               */
/*                                                                         */
/*  Upon reception of a network management response (0810) from SMS, we   */
/*  should reply back to the deamon screen via a 1616 service message.     */
/*  Fields in the response message 0810 are not used.                      */
/***************************************************************************/
int  FCT_NW_MNG_REP_FROM_NW (int nIndexCtx, TSSmsInfo *SmsInfo)
{
	TSNodeData     Node;
	TSIsoInfo      IsoInfo;
	TSBalExchange  BalMessage;
	TSTlvPrivate   TlvInfo;
	char           FieldData[BUF_LG_MAX];
	char           sLine[MAX_LINE_TRC];
	char           SourceId[8+1];
	int            FieldLen;
	int             FieldLen1;	/*HAL26082020 PLUTONL-2185*/
	char            FieldData1[256];/*HAL26082020 PLUTONL-2185*/
	int             Length;
	char	        sFuncCodeData [3 + 1]; /* MJI 23/09/2020 */
	int             nBal = nBalMsg;
	char            sSecurityData[64];

	trace_event ("Start  FCT_NW_MNG_REP_FROM_NW()", PROCESSING);

	/********* Initialisation des variables de travail ***************/
	memset (FieldData,       0,  sizeof (FieldData));

	/* Retrieve original requester */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, FieldData, &FieldLen);

	InitNodeData (&Node);
	memcpy (Node.szFstKey, "1606", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);
	sprintf (sLine ," Cle de DeChainage = %s ", Node.szFstKey );
	trace_event(sLine, PROCESSING);

	switch (FetchRequest ('B', &Node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End FCT_NW_MNG_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;

	}

	/*HAL26082020 PLUTONL-2185*/ 
	if  ( (SmsInfo->nMsgType==810) && (memcmp(FieldData1, SMS_F070_ECHO_TEST, 3) == 0) )
	{
		return(PeriodicalSignOnResponseReceived(FieldData1));
  	}	
  	/*HAL26082020 PLUTONL-2185*/

	InitIsoInfo (&IsoInfo);
	AnalyseIso (Node.sFstBuf, &IsoInfo);
	IsoInfo.nMsgType = 1616;

	/* Add field 39, approved */
	InsertIsoField (ISO_RESPONSE_CODE, &IsoInfo, P7_F039_APPROVAL, P7_F039_LEN);

	/* MJI 24/09/2020 start*/
	if (GetIsoField(ISO_FUNCTION_CODE, &IsoInfo, sFuncCodeData, &FieldLen) == SUCCESS) 
	{
		if(strcmp(sFuncCodeData, "603") == 0 )
		{
			memcpy(ResourceStatus, SIGNON, 3);					
			UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNON);
		}
	}
	/* MJI 24/09/2020 end*/

	dump_p7_struct (  &IsoInfo );

	if (GetIsoField(ISO_ADTNL_RESP_DATA, &IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		InitTlvPrivate(&TlvInfo);
		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);
		if (
			(GetTlvPrivate(BAL_TYPE_TAG, &TlvInfo, FieldData, &FieldLen) == SUCCESS) &&
			(memcmp(FieldData, "ADM", 3) == 0)
			)
			nBal = nBalAdm;
	}

	/* Put answer message in queue for pick up by requesting party */
	GetIsoField (ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(SOURCE_PID,sSecurityData, SourceId,&Length );

	BalMessage.sHeader.nPartnerId = atoi(SourceId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1616;
	Length = IsoBuildMsg (BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg (nBalAdm, &BalMessage) != OK)
	{
		trace_event ("FCT_NW_MNG_REP_FROM_NW(): WriteBalMsg() failed.", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}

	if (GetSmsField ( SMS_NW_MNGMT_INFO , SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		if ( memcmp ( FieldData , "071" , 3 ) == 0 )
		{
			trace_event ( "Sign On Response Received From The Net ", PROCESSING );
			memcpy (ResourceStatus, SIGNON, 3);
			if ( UpdateResource ( nIndexCtx, ResourceId, LIVE, SIGNON ) != OK)
				shutdown_resource( nIndexCtx , ResourceId );
		}
		else
			if ( memcmp ( FieldData , "072" , 3 ) == 0 )
			{
				trace_event ( "Sign Off Response Received From The Net ", ERROR );
				memcpy (ResourceStatus, OFF, 3);
				if (UpdateResource (nIndexCtx, ResourceId, LIVE, SIGNOFF) != OK)
					shutdown_resource( nIndexCtx , ResourceId );
			}
	}

	trace_event ("End  FCT_NW_MNG_REP_FROM_NW()", PROCESSING);
	return (OK);
}


/****************************************************************************/
/*   FCT_NET_MGT_REQ_FROM_NW()                                              */
/*                                                                          */
/*   Handles a network management message (0800) received from SMS.         */
/*   We simply change the messsage type to 810 and send back the message.   */
/****************************************************************************/  
int  FCT_NW_MNG_REQ_FROM_NW (int nIndexCtx, TSSmsInfo *SmsInfo)
{
    int        i, Length, response_code;
	char       FieldData[BUF_LG_MAX];
	char       sLine[MAX_LINE_TRC];
	char       sBuffer[BUF_LG_MAX];
	TSSmsInfo  NewSmsInfo;
	TlvInfo    kTlvInfo;
	char       app_pid[9];
	char       app_ppid[9];
	int        FieldLen;
	char       vZmkLmk[256];
	char       szZpkZmk[256];
	char       szCheckValueIn[256];
    char       szZpkLmk[256];
	char       szCheckValueOut[256];
	int        error_code = 0;
	char       key_type = ' ';


	trace_event ("Start  FCT_NW_MNG_REQ_FROM_NW()", PROCESSING);
	memset(vZmkLmk, 0, sizeof(vZmkLmk));
	memset(szZpkZmk, 0, sizeof(szZpkZmk));
	memset(szCheckValueIn, 0, sizeof(szCheckValueIn));
	memset(szZpkLmk, 0, sizeof(szZpkLmk));
	memset(szCheckValueOut, 0, sizeof(szCheckValueOut));
	memset(sBuffer, 0, sizeof(sBuffer));
        memset(sLine, 0, sizeof(sLine));

	if (GetSmsField ( SMS_NW_MNGMT_INFO , SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		if ( memcmp ( FieldData , SMS_F070_SIGNON , SMS_F070_LEN ) == 0 )
		{
			trace_event ( "Sign On Received From The Net ", PROCESSING );
			memcpy (ResourceStatus, SIGNON, 3);
			if ( UpdateResource ( nIndexCtx, ResourceId, LIVE, SIGNON ) != OK)
			{
				trace_event(">>> Error on update resource", ERROR);
				shutdown_resource(nIndexCtx, ResourceId );
			}
			sprintf(app_pid, "%08d", getpid());
			sprintf(app_ppid, "%08d", getppid());
			LogEvent(	nIndexCtx,
						ResourceId,
						"##",
						INTERFACE_SIGN_ON,
						"##",
						"##",
						"##",
						app_pid,
						app_ppid);
		}
		else if ( memcmp ( FieldData , SMS_F070_SIGNOFF , SMS_F070_LEN ) == 0 )
		{
			trace_event ( "Sign Off Received From The Net ", ERROR );
			memcpy (ResourceStatus, OFF, 3);
			if (UpdateResource(nIndexCtx, ResourceId, CRITICAL, CONNECT) != OK)
			{
				trace_event(">>> Error on update resource", ERROR);
				shutdown_resource(nIndexCtx, ResourceId);
			}
			sprintf(app_pid, "%08d", getpid());
				sprintf(app_ppid, "%08d", getppid());
				LogEvent(	nIndexCtx,
							ResourceId,
							"##",
							INTERFACE_SIGN_OFF,
							"##",
							"##",
							"##",
							app_pid,
							app_ppid);
		}
		/*  Start FZL20220829 PLUTONL-4979 */
		else if ( memcmp ( FieldData , SMS_F070_IWK_INCOMING , SMS_F070_LEN  ) == 0)
		{
			key_type ='I';
		}
		else if ( memcmp ( FieldData , SMS_F070_AWK_INCOMING, SMS_F070_LEN  ) == 0)
		{
			key_type ='A';
		}
		
		if ( key_type != ' ' )
		{
		    sprintf(sLine, "Key %cWK Received From the Network", key_type);
			trace_event(sLine, PROCESSING);

			if (GetSmsField(SMS_ENCRYPTION_DATA, SmsInfo, sBuffer, &Length) == SUCCESS)
			{  
				InitSmsAdditionalDataTlv(&kTlvInfo);
				AnalyseTlvInfo(sBuffer, Length, &kTlvInfo);

			    if (GetTlvInfoField (SMS_F110T04_KEY_DATA , &kTlvInfo, sBuffer, &Length) == SUCCESS )
				{
					InitSmsKeyDataTlv(&kTlvInfo);
					AnalyseTlvInfo(sBuffer, Length, &kTlvInfo); 

					if (GetTlvInfoField (SMS_F110_T04_ENCT_DATA , &kTlvInfo, sBuffer, &Length) == SUCCESS )
					{  
						szZpkZmk[0] ='R';
						memcpy(szZpkZmk+1, sBuffer, Length);
						if (debug_option == 1)
						{
							sprintf(sLine, "%cwK Under ZmK ==> %s",key_type, szZpkZmk+1);
							trace_event(sLine, STREAM);
						}	
					}
					if (GetTlvInfoField (SMS_F110_T04_CHECK_VALUE , &kTlvInfo, sBuffer, &Length) == SUCCESS )
					{
						memcpy(szCheckValueIn, sBuffer, Length);
						sprintf(sLine, "szCheckValueIn ==> %s", szCheckValueIn);
						trace_event(sLine, STREAM);
						
								
					}
					
					response_code = GetZMK ( nIndexCtx, ResourceId, vZmkLmk );
           
					if ( OK == response_code )
					{
						response_code = key_exchange_scheme(
						nIndexCtx,
						vZmkLmk,
						szZpkZmk,
						szCheckValueIn,
						szZpkLmk,
						szCheckValueOut);

                        sprintf(sLine, "==> szCheckValueOut : [%s]", szCheckValueOut);
				     	trace_event(sLine, STREAM);

						if ( response_code == 0 )
						{ 
							if(key_type == 'I')
								response_code =   SetIWK( nIndexCtx, ResourceId , szZpkLmk, szCheckValueOut);
							else
								response_code =   SetAWK( nIndexCtx, ResourceId , szZpkLmk, szCheckValueOut);

							if (response_code !=0)
								error_code = 2;	
						}
						else if (response_code == CRYPTOGRAPHIC_ERROR)
  							error_code = 1;
						else
							error_code = 2;
					}
					else
					{
						trace_event("Error while fetching the ZMK", ERROR);				
						error_code = 2;
					}
				}	
			}
		}
	    /* end FZL20220829 PLUTONL-4979 */     
	}
	
	memset (sBuffer, 0, sizeof(sBuffer));

	InitSmsInfo ( &NewSmsInfo );

	NewSmsInfo.nMsgType = 810;

	for (i = 0; i <SMS_RESPONSE_CODE ; i++)
	{
		if (SmsEchoedField_0800[i])
			if (GetSmsField (i, SmsInfo, FieldData, &FieldLen) == SUCCESS)          
				AddSmsField (i, &NewSmsInfo,  FieldData,  FieldLen);                
	}

	/* FZL20220829 PLUTONL-4979	*/
    if (error_code == 0)
	        AddSmsField(SMS_RESPONSE_CODE, &NewSmsInfo,  SMS_F039_APPROVAL, SMS_F039_LEN);		
	else if (error_code == 1)
     		AddSmsField(SMS_RESPONSE_CODE, &NewSmsInfo,  SMS_F039_CRYPTOGRAPHIC_ERROR, SMS_F039_LEN);
	else 
			AddSmsField(SMS_RESPONSE_CODE, &NewSmsInfo,  SMS_F039_ERROR, SMS_F039_LEN);               

	for (i =  SMS_RESPONSE_CODE + 1; i <= MAX_SMS_FIELDS; i++)                  
	{
		if (SmsEchoedField_0800[i])                                             
			if (GetSmsField (i, SmsInfo, FieldData, &FieldLen) == SUCCESS)       
				AddSmsField (i, &NewSmsInfo,  FieldData,  FieldLen);             
	}

	/* Now send the answer to SMS */
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader( RESPONSE , &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	sprintf(sLine, " Message type %d", NewSmsInfo.nMsgType);
	trace_event(sLine, PROCESSING);

	dump_buffer((unsigned char *)sBuffer, Length, 'E', 'L', 'O'); 

	DumpSms(sBuffer);

	if (WriteLineMsg(sBuffer, Length))
	{
		trace_event ("FCT_NW_MGT_REQ_FROM_NW(): WriteLineMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End  FCT_NW_MNG_REQ_FROM_NW()", PROCESSING);
	return (OK);
}
/****************************************************************************/
/*  initial_sign_on_to_nw()                                                 */
/*                                                                          */
/*  Called during interface initialization to sign on to network.           */
/*  It sends a sign on request to the network, waits for 10 seconds, then   */
/*  checks the answer. The sign on fails after 5 unsuccessful attempts.     */
/****************************************************************************/
int  initial_sign_on_to_nw (int nIndexCtx )
{
	TSSmsInfo   SmsInfo;
	TSSmsInfo   inSmsInfo;
	TSPufInfo   PufInfo;
	char        FieldData[256];
	char        sLine[MAX_LINE_TRC];
	char        outBuffer[1024];
	char        inBuffer[1024];
	char        smsRefNbr [13];
	char        sStan     [7];
	char        sInStan   [7];
	char        sentRefNbr[13];
	char        sGmtDate  [13];
	int         sign_on;
	int         attempt;
	int         flag_alarm=NOK;
	int         retour;
	int         inLength;
	int         outLength;
	int         FieldLen;

	memset (FieldData,   0,  sizeof(FieldData));
	memset (inBuffer,    0,  sizeof(inBuffer));
	memset (outBuffer,   0,  sizeof(outBuffer));
	memset (smsRefNbr,   0,  sizeof(smsRefNbr));
	memset (sentRefNbr,  0,  sizeof(sentRefNbr));
	memset (sGmtDate,    0,  sizeof(sGmtDate));
	memset (sStan,       0,  sizeof(sStan));
	memset (sInStan,     0,  sizeof(sInStan));

	InitSmsInfo ( &SmsInfo );
	SmsInfo.nMsgType = 800;

	current_gmt_date (sGmtDate);
	AddSmsField (SMS_XMIT_TIME, &SmsInfo, sGmtDate + 2, 10);

	/* (11) System Trace Audit Number */
	GetInternalStan ( nIndexCtx ,  FieldData );
	memcpy( sStan , FieldData, 6 );
	AddSmsField (SMS_AUDIT_NBR, &SmsInfo, sStan, 6);

	/* (37) Reference Number */
	/* (63.1) Network Id */
	memset (FieldData,   0,  sizeof(FieldData));
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	FieldLen = PufBuildField (FieldData, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &SmsInfo, FieldData, FieldLen);

	/* (70) Network Mngt Info Code */
	AddSmsField (SMS_NW_MNGMT_INFO, &SmsInfo, SMS_F070_SIGNON, SMS_F070_LEN);

	/* send message */
	outLength = SmsBuildMsg(outBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, outLength,StationId);
	memcpy(outBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	outLength += SMS_HDR_LEN;

	sign_on = NOK;
	attempt = 0;
	while ((sign_on != OK) && (attempt < 5))
	{
		flag_alarm=NOK;
		sprintf ( sLine,"Send SignOn Atempt (%d)", attempt + 1 );
		trace_event ( sLine , ERROR );
		dump_buffer((unsigned char *)outBuffer, outLength, 'E', 'L', 'O');

		DumpSms(outBuffer);

		retour = WriteLineMsg (outBuffer, outLength);
		if ( retour != OK )
		{
			trace_event ( "Can Not Write The First SignOn To Line" , FATAL );
			memcpy (ResourceStatus, OFF, 3);
			return(NOK);
			/*YK11042007 not stop the interface, case 16 and 17 sms certification, to let the reversal generation*/
		}

		THR_Alarm(10, &flag_alarm);
		retour = ReadLineMsg (inBuffer, &inLength);
		THR_Alarm(0, &flag_alarm);
		if ( retour != OK )
		{
			if ( flag_alarm == OK )
			{
				trace_event ( "Response Not Received.....", ERROR );
				attempt += 1;
			}

			else
			{
				trace_event ( "Probleme On Receiving Response.....", ERROR );
				/*YK11042007 not stop the interface, case 16 and 17 sms certification, to let the reversal generation*/
				memcpy (ResourceStatus, OFF, 3);
				return(NOK);
			}
		}
		else
		{
			trace_event ( "Sign On Response Received .....", ERROR );

			dump_buffer ((unsigned char *)inBuffer ,inLength , 'E', 'L', 'I' );

			DumpSms(inBuffer);

			AnalyseSms (inBuffer, &inSmsInfo);

			/* check if this is the answer to our request */

			if (GetSmsField (SMS_AUDIT_NBR, &inSmsInfo, sInStan, &FieldLen) == SUCCESS)
			{
				sprintf (sLine,"MsgType  = %d, STAN = %.6s, ORIG_STAN = %.6s", inSmsInfo.nMsgType, sInStan, sStan);
				trace_event (sLine, PROCESSING);
			}

			if ((inSmsInfo.nMsgType == 810) && (memcmp (sInStan, sStan, 6) == 0))
				return ( OK );
			else
				attempt++;
		}
	}

	/*YK11042007 not stop the interface, case 16 and 17 sms certification, to let the reversal generation*/ 
	memcpy (ResourceStatus, OFF, 3);
	return(NOK);
}
/*****************************************************************************/
/** Fonction    : FCT_SIGN_ON_ADVICE()                                      **/
/** Description : Sending this advice for Reconciliation messages reception **/
/*****************************************************************************/
int FCT_SIGN_ON_ADVICE (int nIndexCtx)
{
	TSSmsInfo   SmsInfo;
	TSSmsInfo   inSmsInfo;
	TSPufInfo   PufInfo;
	char        FieldData[256];
	char        sLine[MAX_LINE_TRC];
	char        outBuffer[1024];
	char        inBuffer[1024];
	char        sStan     [7];
	char        sInStan   [7];
	char        sGmtDate  [13];
	int         flag_alarm=NOK;
	int         retour;
	int         inLength;
	int         outLength;
	int         FieldLen;

	sprintf (sLine, "Start FCT_SIGN_ON_ADVICE()");
	trace_event (sLine, PROCESSING);

	/*** Initialisation des variables de travail ****/
	memset (FieldData,   0,  sizeof(FieldData));
	memset (inBuffer,    0,  sizeof(inBuffer));
	memset (outBuffer,   0,  sizeof(outBuffer));
	memset (sGmtDate,    0,  sizeof(sGmtDate));
	memset (sStan,       0,  sizeof(sStan));
	memset (sInStan,     0,  sizeof(sInStan));

	InitSmsInfo ( &SmsInfo );
	SmsInfo.nMsgType = 800;

	current_gmt_date (sGmtDate);
	AddSmsField (SMS_XMIT_TIME, &SmsInfo, sGmtDate + 2, 10);

	/* (11) System Trace Audit Number */
	GetInternalStan ( nIndexCtx ,  FieldData );
	memcpy( sStan , FieldData, 6 );
	AddSmsField (SMS_AUDIT_NBR, &SmsInfo, sStan, 6);

	/* (37) Reference Number */

	/* (63.1) Network Id */
	memset (FieldData,   0,  sizeof(FieldData));
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	FieldLen = PufBuildField (FieldData, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &SmsInfo, FieldData, FieldLen);

	/* (70) Network Mngt Info Code */
	AddSmsField (SMS_NW_MNGMT_INFO, &SmsInfo, "078", 3);

	/* send message */
	outLength = SmsBuildMsg(outBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, outLength,StationId);
	memcpy(outBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	outLength += SMS_HDR_LEN;

	flag_alarm=NOK;
	sprintf ( sLine,"Send SignOn Advice ");
	trace_event ( sLine , PROCESSING );


	dump_buffer((unsigned char *)outBuffer, outLength, 'E', 'L', 'O');


	DumpSms(outBuffer);

	retour = WriteLineMsg (outBuffer, outLength);
	if ( retour != OK )
	{
		trace_event ( "Can Not Write the SignOn Advice On Line" , PROCESSING);
		shutdown_resource( nIndexCtx , ResourceId );
	}

	THR_Alarm(10, &flag_alarm);
	retour = ReadLineMsg (inBuffer, &inLength);
	THR_Alarm(0, &flag_alarm);
	if ( retour != OK )
	{
		if ( flag_alarm == OK )
		{
			trace_event ( "Response Not Received.....", ERROR );
			shutdown_resource( nIndexCtx , ResourceId );
		}
		else
		{
			trace_event ( "Probleme On Receiving Response.....", ERROR );
			shutdown_resource( nIndexCtx , ResourceId );
		}
	}
	else
	{
		trace_event ( "Sign On Advice Response Received .....", ERROR );

		dump_buffer ((unsigned char *)inBuffer ,inLength , 'E', 'L', 'I' );

		DumpSms(inBuffer);

		AnalyseSms (inBuffer, &inSmsInfo);

		/* check if this is the answer to our request */

		if (GetSmsField (SMS_AUDIT_NBR, &inSmsInfo, sInStan, &FieldLen) == SUCCESS)
		{
			sprintf (sLine,"\n type  = %d, RRN R = %.6s, RRN S = %.6s", inSmsInfo.nMsgType, sInStan, sStan);
			trace_event (sLine, PROCESSING);
		}

		if ((inSmsInfo.nMsgType == 810) && (memcmp (sInStan, sStan, 6) == 0))
		{
			sprintf (sLine, "End   FCT_SIGN_ON_ADVICE(OK)");
			trace_event (sLine, PROCESSING);
			return ( OK );
		}
		else
		{
			sprintf (sLine, "End   FCT_SIGN_ON_ADVICE(NOK): ShutDownResource");
			trace_event (sLine, PROCESSING);
			shutdown_resource( nIndexCtx , ResourceId );
		}
	}
	return(OK);
} 
/****************************************************************************/
/*  sign_off()                                                              */
/****************************************************************************/
int  sign_off (int nIndexCtx )
{
	SendNwMngMsg ( nIndexCtx, SMS_F070_SIGNOFF );
	return(OK);
}


/****************************************************************************/
/*  sign_on()                                                          */
/****************************************************************************/
int  sign_on (int nIndexCtx )
{
	SendNwMngMsg ( nIndexCtx, SMS_F070_SIGNON );
	return(OK);
}
/****************************************************************************/
/*  SendNwMsgMsg()                                                          */
/*                                                                          */
/*  Sends a network management message to the network. InfoCode parameter   */
/*  specifies the type of message (signon, signoff, echotest)               */
/****************************************************************************/
int  SendNwMngMsg (int nIndexCtx, char *InfoCode )
{
	TSSmsInfo   SmsInfo;
	TSPufInfo   PufInfo;
	char        FieldData[256];
	char        sBuffer[1024];
	int         Length;
	int         FieldLen;
	char        RefNbr[13];

	trace_event ("Start  SendNwMngMsg()", PROCESSING);

	memset (FieldData, 0, sizeof(FieldData));
	memset (sBuffer,   0, sizeof(sBuffer));
	memset (RefNbr,    0, sizeof(RefNbr));

	InitSmsInfo ( &SmsInfo );
	SmsInfo.nMsgType = 800;

	current_gmt_date (FieldData);
	AddSmsField (SMS_XMIT_TIME, &SmsInfo, FieldData+2, 10);

	GetInternalStan ( nIndexCtx ,  FieldData );
	AddSmsField (SMS_AUDIT_NBR, &SmsInfo, FieldData, 6);

	/* (63.1) Network Id */
	memset (FieldData,   0,  sizeof(FieldData));
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	FieldLen = PufBuildField (FieldData, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &SmsInfo, FieldData, FieldLen);


	AddSmsField (SMS_NW_MNGMT_INFO, &SmsInfo, InfoCode, 3);

	/* send message */
	Length = SmsBuildMsg (sBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length,StationId);
	memcpy (sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;


	dump_buffer((unsigned char *)sBuffer, Length, 'E', 'L', 'O');

	DumpSms(sBuffer);

	if (WriteLineMsg (sBuffer, Length) == OK)
	{
		trace_event ("End  SendNwMngMsg(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event ("SendNwMngMsg(NOK): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}
}

/****************************************************************************/
/* retrieval_advice_on()                                                    */
/****************************************************************************/
int 
	retrieval_advice_on(int nIndexCtx)
{

	TSSmsInfo      SmsInfo;
	TSSmsInfo      inSmsInfo;
	char            FieldData[256];
	char            sLine[MAX_LINE_TRC];
	char            outBuffer[1024];
	char            inBuffer[1024];
	int             inLength;
	int             outLength;
	int             FieldLen;
	char            smsRefNbr[13];
	char            sentRefNbr[13];
	char            sGmtDate[32];
	int             sign_on;
	int             attempt;
	int             flag_alarm = NOK;
	int             retour;


	memset(inBuffer, 0, sizeof(inBuffer));
	memset(outBuffer, 0, sizeof(outBuffer));
	memset(smsRefNbr, 0, sizeof(smsRefNbr));
	memset(sentRefNbr, 0, sizeof(sentRefNbr));
	memset(sGmtDate, 0, sizeof(sGmtDate));
	memset (FieldData, 0, sizeof(FieldData));

	trace_event("Start  retrieval_advice_on()", PROCESSING);

	AllocateOracleCtx(nIndexCtx);
	ConnectOracle(nIndexCtx);

	InitSmsInfo(&SmsInfo);
	SmsInfo.nMsgType = 800;

	current_gmt_date(sGmtDate);
	AddSmsField(SMS_XMIT_TIME, &SmsInfo, sGmtDate + 2, 10);

	GetInternalStan(nIndexCtx, FieldData);
	AddSmsField(SMS_AUDIT_NBR, &SmsInfo, FieldData, 6);

	current_julian_date_qjjj(sentRefNbr);
	memcpy(sentRefNbr + 4, sGmtDate + 6, 2);
	memcpy(sentRefNbr + 6, FieldData, 6);
	AddSmsField(SMS_REFERENCE_NBR, &SmsInfo, sentRefNbr, 12);


	AddSmsField (SMS_NW_MNGMT_INFO, &SmsInfo, SMS_F070_START_SAF, SMS_F070_LEN); 

	/* send message */
	outLength = SmsBuildMsg(outBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST, &(SmsInfo.sHeader), NULL, outLength, StationId);
	memcpy(outBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	outLength += SMS_HDR_LEN;


	dump_buffer(outBuffer, outLength, 'E', 'L', 'O');

	DumpSms(outBuffer);
	retour = WriteLineMsg(outBuffer, outLength);
	if (retour != OK) {
		trace_event("Can Not Write Retrieval Advice ON", FATAL);
		DeconnectOracle(nIndexCtx);
		FreeOracleCtx(nIndexCtx);
		trace_event("End  retrieval_advice_on(NOK)", ERROR);
		return (NOK);
	}

	THR_Alarm(20, &flag_alarm);
	retour = ReadLineMsg(inBuffer, &inLength);
	THR_Alarm(0, &flag_alarm);

	if (retour != OK) 
	{
		if (flag_alarm == OK) 
		{
			trace_event("Response Not Received.....", ERROR);
			DeconnectOracle(nIndexCtx);
			FreeOracleCtx(nIndexCtx);
			trace_event("End  retrieval_advice_on(NOK)", ERROR);
			return (NOK);
		} 
		else 
		{
			trace_event("Probleme On Receiving Response.....", ERROR);
			DeconnectOracle(nIndexCtx);
			FreeOracleCtx(nIndexCtx);
			trace_event("End  retrieval_advice_on(NOK)", ERROR);
			return (NOK);
		}
	} 
	else 
	{
		trace_event("Sign On Response Received .....", ERROR);

		dump_buffer(inBuffer, inLength, 'E', 'L', 'I');

		DumpSms(inBuffer);

		AnalyseSms(inBuffer, &inSmsInfo);

		/* check if this is the answer to our request */
		GetSmsField(SMS_REFERENCE_NBR, &inSmsInfo, smsRefNbr, &FieldLen);
		sprintf(sLine, "type  = %d , RRN R = [%.12s], RRN S = [%.12s]", inSmsInfo.nMsgType,smsRefNbr,sentRefNbr);
		trace_event(sLine, TRACE);


		GetSmsField (SMS_RESPONSE_CODE, &inSmsInfo, FieldData, &FieldLen);


		if ((inSmsInfo.nMsgType == 810) &&
			(memcmp(smsRefNbr, sentRefNbr, 12) == 0) &&
			(memcmp(FieldData, SMS_F039_APPROVAL, SMS_F039_LEN) == 0))  
		{

			DeconnectOracle(nIndexCtx);
			FreeOracleCtx(nIndexCtx);
			trace_event("End  retrieval_advice_on(OK)", PROCESSING);
			return (OK);

		}else
		{
			DeconnectOracle(nIndexCtx);
			FreeOracleCtx(nIndexCtx);
			trace_event("End  retrieval_advice_on(NOK)", PROCESSING);
			return (NOK);
		}
	}


	trace_event("End  retrieval_advice_on()", PROCESSING);
	return(OK);
}
/****************************************************************************/
/* retrieval_advice_off()                                                   */
/****************************************************************************/
int  retrieval_advice_off(int nIndexCtx)
{
	SendNwMngMsg ( nIndexCtx, SMS_F070_STOP_SAF ); 
	return(OK);

}

/*HAL26082020 PLUTONL-2185*/
void*	thread_signin(void *arg)
{
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event("> Start thread_signin    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);

	pthread_detach(pthread_self());
	ThreadMaskSignals();
	fprintf(stderr, " Thread SIGN ID = %lu ", pthread_self());

	AllocateOracleCtx(SIGNIN_THREAD_ID);
	ConnectOracle(SIGNIN_THREAD_ID);
	PeriodicalNetMsgManagement(SIGNIN_THREAD_ID);
	return(OK);

}
/*********************************************************************************************/
int PeriodicalNetMsgManagement(int nIndexCtx )
{
	TSSmsInfo      SmsInfo;
	TSNodeData      Node;
	char            Stan[6 + 1];
	int             nLength;
	time_t          nPurgeTime;
	char            sLine[MAX_LINE_TRC];
	char            FieldData[256];
	int             result;
	char            outBuffer[1024];
	int             outLength;
	struct timeval	lastTime;
	int				nResult;

	memset(outBuffer, 0, sizeof(outBuffer));

	trace_event("Start PeriodicalNetMsgManagement () ", PROCESSING);
	gettimeofday(&lastTime,NULL);


	sprintf(sLine, "Echo Test Timer = %d", EchoTestTimer);
	trace_event(sLine, PROCESSING);

	while (1) 
	{

		sleep(EchoTestTimer);
		if ( memcmp(ResourceStatus, SIGNON, RESOURCE_STATUS_LEN) != 0 ) 
		{
			continue;
		}
		
		nResult = CheckAndRefreshDBCon(nIndexCtx,lastTime);

			InitSmsInfo(&SmsInfo);
			SmsInfo.nMsgType = 800;

			/* (007) SMS_XMIT_TIME */
			current_gmt_date(FieldData);
			AddSmsField(SMS_XMIT_TIME, &SmsInfo, FieldData + 2, 10);

			/* (011)SMS_AUDIT_NBR */
			GetInternalStan(nIndexCtx, FieldData);
			AddSmsField(SMS_AUDIT_NBR, &SmsInfo, FieldData, 6);

			/* (070) SMS_NW_MNGMT_INFO */
			AddSmsField(SMS_NW_MNGMT_INFO, &SmsInfo, SMS_F070_ECHO_TEST, SMS_F070_LEN);

			GetSmsField(SMS_AUDIT_NBR, &SmsInfo, Stan, &nLength);

			InitNodeData(&Node);
			GetAcquirerPurgeTime(&nPurgeTime);
			Node.tPurgeTime = nPurgeTime;

			sprintf(Node.szFstKey, "%04d", SmsInfo.nMsgType);
			memcpy(Node.szFstKey + 4, Stan, 6);

			memcpy(Node.sFstPrvtData, "PERIODIQUE", 10);
			Node.nFstPrvtDataLen = 10;

			outLength = SmsBuildMsg(outBuffer + SMS_HDR_LEN, &SmsInfo);
			SmsBuildHeader(REQUEST, &(SmsInfo.sHeader), NULL, outLength, StationId);
			memcpy(outBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
			outLength += SMS_HDR_LEN;
			Node.nFstBufLen = outLength;

			if (InsertRequest('B', &Node) != OK)
				trace_event("Can Not Chain Sign On Repeat Request... ", PROCESSING);	
			else
				trace_event("Echo test Request... ", PROCESSING);				

			
			dump_buffer(outBuffer,outLength ,'E', 'L', 'O'); 
			
			DumpSms(outBuffer);		

			if (WriteLineMsg(outBuffer, outLength))
				trace_event("Failed to send repeat signin. ", ERROR);
			else
				trace_event("Periodical echo request sent.....", ERROR);
			
			gettimeofday(&lastTime,NULL);

	}
	return(OK);
}

/***********************************************************************************/
int PeriodicalSignOnResponseReceived(char *buffer)
{
	trace_event("Response of Periodical 810 Received.....", ERROR);
	SignRespReceived();
	MaxSendEchoTest = 0;
	return (0);
}
/***********************************************************************************/
int TimeOutSignOnResponseDetected(int nIndexCtx, char *buffer)
{
	char            app_pid[9];
	char            app_ppid[9];

	trace_event("Time Out of Periodical 800 Detected.....", ERROR);

	/* Log Event */
	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	MaxSendEchoTest += 1;

	LogEvent(nIndexCtx,
		ResourceId,
		"##",
		INTERFACE_TIME_OUT,
		"##",
		"##",
		"##",
		app_pid,
		app_ppid);

	SignTimeOutDetected();
	if (MaxSendEchoTest == 3) 
	{
		MaxSendEchoTest = 0;
		trace_event("MaximunSendEchoTestwithoutresponse is attented....", FATAL);
		StopCltCom(nFdConnect);

	}
	return (0);
}
/*HAL26082020 PLUTONL-2185*/