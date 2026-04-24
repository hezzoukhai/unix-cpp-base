/****************************************************************************/
/* NETWORK MANAGEMENT ROUTINES                            */
/****************************************************************************/
#include <define.h>

#include <iso_hps.h>
#include <iso_visa.h>
#include <p7_sid.h>
#include <visa_param.h>
#include <visa_define.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <event_log.h>
#include <tcp_var.h>
#include <hsm_inc.h>

static int	MaxSendEchoTest = 0;/*HAL26082020 PLUTONL-2185*/

int             VisaEchoedField_0800[] =
{
	0,				/* BIT_MAP2                            */
	1,				/* CARD_NBR                            */
	1,				/* PROC_CODE                           */
	1,				/* TRANS_AMOUNT                        */
	0,				/* STLMNT_AMOUNT                       */
	0,				/* CARDHOLDER BILLING AMOUNT           */
	1,				/* XMIT_TIME                           */
	0,				/* CARDHOLDER BILLING FEE              */
	0,				/* CONVRSN_RATE_STLMNT                 */
	0,				/* CONVRSN_RATE_CARD_HLDR_BILLING      */

	1,				/* AUDIT_NBR             */
	0,				/* TRANS_TIME            */
	0,				/* TRANS_DATE            */
	1,				/* EXPIRY_DATE           */
	0,				/* STLMNT_DATE           */
	0,				/* CNVRSN_DATE           */
	0,				/* CAPTURE_DATE          */
	1,				/* MERCHANT_TYPE         */
	1,				/* ACQR_COUNTRY          */
	1,				/* PAN_COUNTRY           */

	0,				/* FORWD_COUNTRY                       */
	1,				/* POS_ENTRY_MODE                      */
	0,				/* CARD SEQUENCE NUMBER                */
	0,				/* NETWORK INTERNATIONAL IDENTIFIER    */
	1,				/* POS_CONDITION                       */
	1,				/* POS_PIN_CAPTURE                     */
	0,				/* AUTHORIZATION ID RESPONSE LENGTH    */
	0,				/* TRANS_FEE                           */
	0,				/* AMOUNT SETTLEMENT FEE               */
	0,				/* AMOUNT TRANSACTION PROCESSING FEE   */

	0,				/* AMOUNT SETLMNT PROCESSING FEE       */
	1,				/* ACQR_ID                             */
	1,				/* FORWD_ID                            */
	1,				/* PAN_EXTENDED                        */
	1,				/* T2_DATA                             */
	0,				/* TRACK 3 DATA                        */
	1,				/* REFERENCE_NBR                       */
	0,				/* AUTHOR_ID                           */
	0,				/* RESPONSE_CODE                       */
	1,				/* SERVICE RESTRICTION CODE            */

	1,				/* TERMINAL_NBR                        */
	1,				/* OUTLET_NBR                          */
	1,				/* TERMINAL_ADR                        */
	0,				/* ADTNL_RESP_DATA                     */
	1,				/* T1_DATA                             */
	0,				/* ADDITRIONAL DATA ISO                */
	0,				/* ADDITIONAL DATA NATIONAL            */
	1,				/* ADTNL_DATA_PRIV                     */
	1,				/* TRANS_CRNCY                         */
	0,				/* STLMNT_CRNCY                        */

	0,				/* BILLING_CRNCY         */
	1,				/* PIN_DATA              */
	1,				/* SECURITY_DATA         */
	0,				/* ADTNL_AMOUNTS         */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* GEOGR_DATA            */
	1,				/* ADTNL_POS_INFO        */

	0,				/* OTHER_AMOUNTS         */
	0,				/* CPS_FIELDS            */
	1,				/* PRIV_USE_FIELDS       */
	0,				/* MAC1                  */
	0,				/* */
	0,				/* STLMNT_CODE           */
	0,				/* Extended Payment Code */
	0,				/* RECEIVER_COUNTRY      */
	0,				/* STLMNT_INST_COUNTRY   */
	1,				/* NW_MNGMT_INFO         */

	0,				/* Message Number              */
	0,				/* Message number Last         */
	0,				/* ACTION_DATE                 */
	0,				/* CR_NBR                      */
	0,				/* CR_REV_NBR                  */
	0,				/* DB_NBR                      */
	0,				/* DB_REV_NBR                  */
	0,				/* Transfer Number             */
	0,				/* Transfer Reversal Number    */
	0,				/* Inquiries Number            */

	0,				/* Authorizations Number           */
	0,				/* Credit, Processing Fee Amount   */
	0,				/* Credit, Transaction Fee Amount  */
	0,				/* Debit, Processing Fee Amount    */
	0,				/* Debit, Transaction Fee Amount   */
	0,				/* CR_AMOUNT                       */
	0,				/* CR_REV_AMOUNT                   */
	0,				/* DB_AMOUNT                       */
	0,				/* DB_REV_AMOUNT                   */
	0,				/* ORIG_DATA                       */

	0,				/* FILE_UPDATE           */
	0,				/* FILE_SECURITY         */
	0,				/* Response Indicator    */
	0,				/* Service Indicator     */
	0,				/* REPLACE_AMOUNTS       */
	0,				/* MSG_SECURITY          */
	0,				/* NET_AMOUNT            */
	0,				/* Payee                 */
	0,				/* STLMNT_INSTITN        */
	0,				/* RECEIVER_ID           */

	0,				/* FILE_NAME             */
	0,				/* ACCOUNT_ID1           */
	0,				/* ACCOUNT_ID2           */
	0,				/* Transaction Description   */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */

	0,				/* Reserved Iso          */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* ADTNL_TRACE_DATA      */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* Intra Country Data    */
	0,				/* Reserved National     */
	0,				/* Original Message Type Id  */

	0,				/* Issuing Institution Id    */
	0,				/* Remaining Open to Use     */
	0,				/* ADR_VERIF_DATA            */
	0,				/* Free Form Text - Japan    */
	0,				/* SUPPORT_INFO              */
	0,				/* Reserved Private          */
	0,				/* File Record Action and Data   */
	0,				/* MAC2                          */
	0,				/* Field 129                     */
	0,				/* Terminal Capability Profile   */

	0,				/* Terminal Verification Results */
	0,				/* Unpredictable Number          */
	0,				/* Terminal Serial Number        */
	0,				/* Visa Discretionary Data       */
	0,				/* Issuer Discretionary Data     */
	0,				/* Issuer Discretionary Data     */
	0,				/* Application Trans. Counter    */
	0,				/* Application Inter. Profile    */
	0,				/* ARPC + Response Code          */
	0,				/* Field 140                     */

	0,				/* Field 141                     */
	0,				/* Issuer Script                 */
	0,				/* Issuer Script Results         */
	0,				/* Cryptogram Trans Type         */
	0,				/* Terminal Country Code         */
	0,				/* Terminal Trans  Date          */
	0,				/* Cryptogram Amount             */
	0,				/* Cryptogram Currency Code      */
	0,				/* Cryptogram Cash Back          */
	0,				/* Cardholder Card Scheme Crypto. */

	0,				/* Field 151             */
	0,				/* Field 152             */
	0,				/* Field 153             */
	0,				/* Field 154             */
	0,				/* Field 155             */
	0,				/* Field 156             */
	0,				/* Field 157             */
	0,				/* Field 158             */
	0,				/* Field 159             */
	0,				/* Field 160             */

	0,				/* Field 161             */
	0,				/* Field 162             */
	0,				/* Field 163             */
	0,				/* Field 164             */
	0,				/* Field 165             */
	0,				/* Field 166             */
	0,				/* Field 167             */
	0,				/* Field 168             */
	0,				/* Field 169             */
	0,				/* Field 170             */

	0,				/* Field 171             */
	0,				/* Field 172             */
	0,				/* Field 173             */
	0,				/* Field 174             */
	0,				/* Field 175             */
	0,				/* Field 176             */
	0,				/* Field 177             */
	0,				/* Field 178             */
	0,				/* Field 179             */
	0,				/* Field 180             */

	0,				/* Field 181             */
	0,				/* Field 182             */
	0,				/* Field 183             */
	0,				/* Field 184             */
	0,				/* Field 185             */
	0,				/* Field 186             */
	0,				/* Field 187             */
	0,				/* Field 188             */
	0,				/* Field 189             */
	0,				/* Field 190             */

	0,				/* Field 191             */
	0				/* Message Authentication Code   */
};


/****************************************************************************/
/* FCT_NW_MNG_REQ_TO_NW()                                                  */
/* */
/* Sends a Network Management Request Message (0800) to VISA upon          */
/* reception of a 1606 (service message) request.                          */
/****************************************************************************/
int 
	FCT_NW_MNG_REQ_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	TSVisaInfo      VisaInfo;
	TSNodeData      Node;
	char            FieldData[256];
	int             FieldLen;
	char            sPurgeTime[10];
	char            sBuffer[1024];
	char            Stan[6 + 1];
	char            sGmtDate[32];
	int             Length;
	struct	timeval stPurgeTime;		
	char            app_pid[9];
	char            app_ppid[9];



	trace_event("Start  FCT_NW_MNG_REQ_TO_NW()", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));
	memset(sGmtDate, 0, sizeof(sGmtDate));
	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());


	/* Insert original request 1606 in queue for later processing */
	InitNodeData(&Node);

	GetIsoField(ISO_SECURITY_DATA, IsoInfo, FieldData, &FieldLen);

	GetSafPurgeTimeMS(&stPurgeTime);
	Node.stPurgeTime = stPurgeTime;



	memset(Stan, 0, sizeof(Stan));
	GetInternalStan(nIndexCtx, Stan);

	memcpy(Node.szFstKey, "1606", 4);
	memcpy(Node.szFstKey + 4, Stan, 6);

	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	if (InsertRequest('B', &Node) != OK) 
	{
		trace_event("End FCT_NW_MNG_REQ_TO_NW(NOK) ERROR", PROCESSING);
		return (NOK);
	}
	dump_p7_struct(IsoInfo);

	InitVisaInfo(&VisaInfo);
	VisaInfo.nMsgType = 800;

	/* (7) Transmission Date/Time */
	current_gmt_date(sGmtDate);
	AddVisaField(VISA_XMIT_TIME, &VisaInfo, sGmtDate + 2, 10);

	/* (11) System Trace Audit Number */
	AddVisaField(VISA_AUDIT_NBR, &VisaInfo, Stan, 6);

	/* (37) Reference Number */
	current_julian_date_qjjj(FieldData);
	memcpy(FieldData + 4, sGmtDate + 6, 2);
	memcpy(FieldData + 6, Stan, 6);
	AddVisaField(VISA_REFERENCE_NBR, &VisaInfo, FieldData, 12);

	/* (70) Network Management Info Code */
	/* ISO field 24 holds the code */
	GetIsoField(ISO_FUNCTION_CODE, IsoInfo, FieldData, &FieldLen);
	switch (atoi(FieldData)) {
	case 603:		/* signon */
		memcpy(FieldData, B1_F070_SIGNON, B1_F070_LEN);
		LogEvent(	nIndexCtx,
					ResourceId,
					"##",
					INTERFACE_SIGN_ON,
					"##",
					"##",
					"##",
					app_pid,
					app_ppid);

		break;
	case 604:		/* signoff */
		memcpy(FieldData, B1_F070_SIGNOFF, B1_F070_LEN);
		memcpy(ResourceStatus, SIGNOFF, 3);
		UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNOFF);

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
	case 607:		/* start SAF */
		memcpy (FieldData, B1_F070_START_SAF, B1_F070_LEN);
		break;
	case 608:		/* end SAF */
		memcpy (FieldData, B1_F070_STOP_SAF, B1_F070_LEN);
		break;
	case 609:		/* echo test */
		memcpy(FieldData, B1_F070_ECHO_TEST, B1_F070_LEN);
		break;

	}
	AddVisaField(VISA_NW_MNGMT_INFO, &VisaInfo, FieldData, B1_F070_LEN);

	/* Prepare to send message */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_event("Failed to send response.", ERROR);
		FetchRequest('B', &Node);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}
	trace_event("End  FCT_NW_MNG_REQ_TO_NW(OK)", PROCESSING);
	return (OK);
}


/***************************************************************************/
/* FCT_NW_MNG_REP_FROM_NW()                                               */
/* */
/* Upon reception of a network management response (0810) from VISA, we   */
/* should reply back to the deamon screen via a 1616 service message.     */
/* Fields in the response message 0810 are not used.                      */
/***************************************************************************/
int 
	FCT_NW_MNG_REP_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	TSBalExchange   BalMessage;
	TSTlvPrivate    TlvInfo;
	char            FieldData[256];
	char            SourceId[8 + 1];
	int             FieldLen;
	int             Length;
	int             nBal = nBalMsg;
	char            sSecurityData[64];
	char			sLine[MAX_LINE_TRC];
	char            FieldData1[256];/*HAL26082020 PLUTONL-2185*/	
	int             FieldLen1;	/*HAL26082020 PLUTONL-2185*/
	char			sFuncCodeData [3 + 1]; /* MJI 23/09/2020 */

	trace_event("Start  FCT_NW_MNG_REP_FROM_NW()", PROCESSING);

	/* Retrieve original requester */
	GetVisaField(VISA_AUDIT_NBR, VisaInfo, FieldData, &FieldLen);

	GetVisaField(VISA_NW_MNGMT_INFO, VisaInfo, FieldData1, &FieldLen1); /*HAL26082020 PLUTONL-2185*/

	InitNodeData(&Node);

	/*HAL26082020 PLUTONL-2185*/ 
	if  ( (VisaInfo->nMsgType==810) && (memcmp(FieldData1, B1_F070_ECHO_TEST , 3) == 0) )
	{
		memcpy(Node.szFstKey, "0800", 4);
		memcpy(Node.szFstKey + 4, FieldData, 6);
  	}	

  	else
  	{
		memcpy(Node.szFstKey, "1606", 4);
		memcpy(Node.szFstKey + 4, FieldData, 6);
  	}

	sprintf(sLine, "Fetch key = %s", Node.szFstKey);
	trace_event(sLine, PROCESSING);
	/*HAL26082020 PLUTONL-2185*/ 


	switch (FetchRequest('B', &Node)) 
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_NW_MNG_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;

	}

	/*HAL26082020 PLUTONL-2185*/ 
	if  ( (VisaInfo->nMsgType==810) && (memcmp(FieldData1, B1_F070_ECHO_TEST, 3) == 0) )
	{
		return(PeriodicalSignOnResponseReceived(FieldData1));
  	}	
  	/*HAL26082020 PLUTONL-2185*/

	InitIsoInfo(&IsoInfo);
	AnalyseIso(Node.sFstBuf, &IsoInfo);
	IsoInfo.nMsgType = 1616;

	InsertIsoField(ISO_RESPONSE_CODE, &IsoInfo, P7_F039_APPROVAL, P7_F039_LEN);

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

	dump_p7_struct(&IsoInfo);

	/**************************************/
	/* Field 44: Additional Response Data */
	/* BAL_TYPE                           */
	/**************************************/
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
	GetIsoField(ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(SOURCE_PID, sSecurityData, SourceId, &Length);

	BalMessage.sHeader.nPartnerId = atoi(SourceId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1616;
	Length = IsoBuildMsg(BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg(nBal, &BalMessage) != OK) 
	{
		trace_event("End  FCT_NW_MNG_REP_FROM_NW(NOK): WriteBalMsg failed.", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}
	trace_event("End  FCT_NW_MNG_REP_FROM_NW()", PROCESSING);
	return (OK);
}


/****************************************************************************/
/* FCT_NW_MNG_REQ_FROM_NW()                                               */
/* */
/* Handles a network management message (0800) received from VISA.        */
/* We simply change the messsage type to 810 and send back the message.   */
/****************************************************************************/
int 
	FCT_NW_MNG_REQ_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
    int             i, Length, response_code;
	char            sBuffer[BUF_LG_MAX];
	TSVisaInfo      NewVisaInfo;
	TlvInfo         kTlvInfo;
	char            app_pid[9];
	char            app_ppid[9];
    char            sLine[MAX_LINE_TRC];
	char            vZmkLmk[256];
	char            szZpkZmk[256];
	char            szCheckValueIn[256];
    char            szZpkLmk[256];
	char            szCheckValueOut[256];
	int             error_code = 0;
	char            key_type = ' ';

	trace_event("Start  FCT_NW_MNG_REQ_FROM_NW()", PROCESSING);
	memset(vZmkLmk, 0, sizeof(vZmkLmk));
	memset(szZpkZmk, 0, sizeof(szZpkZmk));
	memset(szCheckValueIn, 0, sizeof(szCheckValueIn));
	memset(szZpkLmk, 0, sizeof(szZpkLmk));
	memset(szCheckValueOut, 0, sizeof(szCheckValueOut));
	memset(sBuffer, 0, sizeof(sBuffer));
    memset(sLine, 0, sizeof(sLine));

	if (GetVisaField(VISA_NW_MNGMT_INFO, VisaInfo, sBuffer, &Length) == SUCCESS) 
	{
		if ( memcmp ( sBuffer , B1_F070_SIGNON , B1_F070_LEN ) == 0 ) 
		{
			trace_event("Sign On Received From The Net ", PROCESSING);
			if (UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNON) != OK)
			{
				trace_event(">>> Error on update resource", ERROR);
				shutdown_resource(nIndexCtx, ResourceId);
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
		else if ( memcmp ( sBuffer , B1_F070_SIGNOFF , B1_F070_LEN ) == 0 )
		{
				trace_event("Sign Off Received From The Net ", ERROR);
				memcpy(ResourceStatus, OFF, 3);
				/*AMER: 20160314
 				Turn MAIN_THREAD_ID into nIndexCtx in the below UpdateResource
				*/
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
		/*  Start FZL20220822 PLUTONL-4979 */
		else if ( memcmp ( sBuffer , B1_F070_IWK_INCOMING , B1_F070_LEN  ) == 0)
		{
			key_type ='I';
		}
		else if(memcmp ( sBuffer , B1_F070_AWK_INCOMING, B1_F070_LEN  ) == 0)
		{
			key_type ='A';
		}
		
		if(key_type != ' ')
		{
			
		    sprintf(sLine, "Key %cWK Received From the Network", key_type);
			trace_event(sLine, PROCESSING);
			if (GetVisaField(VISA_ENCRYPTION_DATA, VisaInfo, sBuffer, &Length) == SUCCESS)
			{  
				InitVisaAdditionalDataTlv(&kTlvInfo);
				AnalyseTlvInfo(sBuffer, Length, &kTlvInfo);

			    if( GetTlvInfoField (B1_F110T04_KEY_DATA , &kTlvInfo, sBuffer, &Length) == SUCCESS )
				{
					InitVisaKeyDataTlv(&kTlvInfo);
					AnalyseTlvInfo(sBuffer, Length, &kTlvInfo); 

					if( GetTlvInfoField (B1_F110_T04_ENCT_DATA , &kTlvInfo, sBuffer, &Length) == SUCCESS )
					{ 

						szZpkZmk[0]='R';
						memcpy(szZpkZmk+1, sBuffer, Length);
						if(debug_option == 1)
						{
							sprintf(sLine, "%cwK Under ZmK ==> %s",key_type, szZpkZmk+1 );
							trace_event(sLine, STREAM);
						}	
					}
					if( GetTlvInfoField (B1_F110_T04_CHECK_VALUE , &kTlvInfo, sBuffer, &Length) == SUCCESS )
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
	    /* end FZL20220822 PLUTONL-4979 */     
	}
	
	memset(sBuffer, 0, sizeof(sBuffer));
	InitVisaInfo(&NewVisaInfo);

	NewVisaInfo.nMsgType = 810;

	for (i = 0; i < VISA_RESPONSE_CODE; i++) 
	{
		if (VisaEchoedField_0800[i])
			if (GetVisaField(i, VisaInfo, sBuffer, &Length) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, sBuffer, Length);
	}

	if( error_code == 0)
	        AddVisaField(VISA_RESPONSE_CODE, &NewVisaInfo,  B1_F039_APPROVAL, B1_F039_LEN);		
	else if( error_code == 1)
     		AddVisaField(VISA_RESPONSE_CODE, &NewVisaInfo,  B1_F039_CRYPTOGRAPHIC_ERROR, B1_F039_LEN);
	else 
			AddVisaField(VISA_RESPONSE_CODE, &NewVisaInfo,  B1_F039_ERROR, B1_F039_LEN);

	for (i = VISA_RESPONSE_CODE + 1; i <= MAX_VISA_FIELDS; i++) 
	{
		if (VisaEchoedField_0800[i])
			if (GetVisaField(i, VisaInfo, sBuffer, &Length) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, sBuffer, Length);
	}


	/* Now send the answer to VISA */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &NewVisaInfo);
	VisaBuildHeader(RESPONSE, &(NewVisaInfo.sHeader), &(VisaInfo->sHeader), Length, StationId);

	memcpy(sBuffer, (char *) &(NewVisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_event("End  FCT_NW_MNG_REQ_FROM_NW(NOK): Failed to send response.", ERROR);
		return (NOK);
	}
	trace_event("End  FCT_NW_MNG_REQ_FROM_NW(OK)", PROCESSING);
	return (OK);
}
/****************************************************************************/
/* initial_sign_on_to_nw()                                                 */
/* */
/* Called during interface initialization to sign on to network.           */
/* It sends a sign on request to the network, waits for 10 seconds, then   */
/* checks the answer. The sign on fails after 5 unsuccessful attempts.     */
/****************************************************************************/
int 
	initial_sign_on_to_nw(int nIndexCtx)
{
	TSVisaInfo      VisaInfo;
	TSVisaInfo      inVisaInfo;
	char            FieldData[256];
	char            sLine[MAX_LINE_TRC];
	char            outBuffer[1024];
	char            inBuffer[1024];
	int             inLength;
	int             outLength;
	int             FieldLen;
	char            visaRefNbr[13];
	char            sentRefNbr[13];
	char            sGmtDate[32];
	int             sign_on;
	int             attempt;
	int             flag_alarm = NOK;
	int             retour;


	memset(inBuffer, 0, sizeof(inBuffer));
	memset(outBuffer, 0, sizeof(outBuffer));
	memset(visaRefNbr, 0, sizeof(visaRefNbr));
	memset(sentRefNbr, 0, sizeof(sentRefNbr));
	memset(sGmtDate, 0, sizeof(sGmtDate));
	memset (FieldData, 0, sizeof(FieldData));

	InitVisaInfo(&VisaInfo);
	VisaInfo.nMsgType = 800;

	current_gmt_date(sGmtDate);
	AddVisaField(VISA_XMIT_TIME, &VisaInfo, sGmtDate + 2, 10);

	GetInternalStan(nIndexCtx, FieldData);
	AddVisaField(VISA_AUDIT_NBR, &VisaInfo, FieldData, 6);

	current_julian_date_qjjj(sentRefNbr);
	memcpy(sentRefNbr + 4, sGmtDate + 6, 2);
	memcpy(sentRefNbr + 6, FieldData, 6);
	AddVisaField(VISA_REFERENCE_NBR, &VisaInfo, sentRefNbr, 12);

	AddVisaField (VISA_NW_MNGMT_INFO, &VisaInfo, B1_F070_SIGNON, B1_F070_LEN);

	/* send message */
	outLength = VisaBuildMsg(outBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, outLength, StationId);
	memcpy(outBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	outLength += VISA_HDR_LEN;

	sign_on = NOK;
	attempt = 0;
	while ((sign_on != OK) && (attempt < 5)) 
	{
		flag_alarm = NOK;
		sprintf(sLine, "Send SignOn Atempt (%d)", attempt + 1);
		trace_event(sLine, ERROR);

		dump_buffer(outBuffer, outLength, 'E', 'L', 'O');

		DumpVisa(outBuffer);
		retour = WriteLineMsg(outBuffer, outLength);
		if (retour != OK) {
			trace_event("Can Not Write The First SignOn To Line", FATAL);
			shutdown_resource(nIndexCtx, ResourceId);
		}
		THR_Alarm(10, &flag_alarm);
		retour = ReadLineMsg(inBuffer, &inLength);
		THR_Alarm(0, &flag_alarm);
		if (retour != OK) 
		{
			if (flag_alarm == OK) 
			{
				trace_event("Response Not Received.....", ERROR);
				attempt += 1;
			} 
			else 
			{
				trace_event("Probleme On Receiving Response.....", ERROR);
				shutdown_resource(nIndexCtx, ResourceId);
			}
		} 
		else 
		{
			trace_event("Sign On Response Received .....", ERROR);

			dump_buffer(inBuffer, inLength, 'E', 'L', 'I');

			DumpVisa(inBuffer);

			AnalyseVisa(inBuffer, &inVisaInfo);

			/* check if this is the answer to our request */
			GetVisaField(VISA_REFERENCE_NBR, &inVisaInfo, visaRefNbr, &FieldLen);
			sprintf(sLine, "type  = %d , RRN R = [%.12s], RRN S = [%.12s]", inVisaInfo.nMsgType,visaRefNbr,sentRefNbr);
			trace_event(sLine, TRACE);

			GetVisaField (VISA_RESPONSE_CODE, &inVisaInfo, FieldData, &FieldLen);


			if ((inVisaInfo.nMsgType == 810) &&
				(memcmp(visaRefNbr, sentRefNbr, 12) == 0) &&
				(memcmp(FieldData, B1_F039_APPROVAL, B1_F039_LEN) == 0))
				return (OK);
			else
				attempt++;
		}
	}

	trace_event("Max attempts reached, shutdown.....", FATAL);
	shutdown_resource(nIndexCtx, ResourceId);
}

/****************************************************************************/
/* SendNwMsgMsg()                                                          */
/* */
/* Sends a network management message to the network. InfoCode parameter   */
/* specifies the type of message (signon, signoff, echotest)               */
/****************************************************************************/
int 
	SendNwMngMsg(int nIndexCtx, char *InfoCode)
{
	TSVisaInfo      VisaInfo;
	char            FieldData[256];
	char            sBuffer[1024];
	int             Length;
	int             FieldLen;
	char            RefNbr[13];
	char            sGmtDate[32];

	trace_event("Start  SendNwMngMsg()", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));
	memset(RefNbr, 0, sizeof(RefNbr));
	memset(sGmtDate, 0, sizeof(sGmtDate));

	InitVisaInfo(&VisaInfo);
	VisaInfo.nMsgType = 800;

	current_gmt_date(sGmtDate);
	AddVisaField(VISA_XMIT_TIME, &VisaInfo, sGmtDate + 2, 10);

	GetInternalStan(nIndexCtx, FieldData);
	AddVisaField(VISA_AUDIT_NBR, &VisaInfo, FieldData, 6);

	current_julian_date_qjjj(RefNbr);
	memcpy(RefNbr + 4, sGmtDate + 6, 2);
	memcpy(RefNbr + 6, FieldData, 6);
	AddVisaField(VISA_REFERENCE_NBR, &VisaInfo, RefNbr, 12);

	AddVisaField(VISA_NW_MNGMT_INFO, &VisaInfo, InfoCode, 3);

	/* send message */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length) == OK) 
	{
		trace_event("End  SendNwMngMsg(OK)", PROCESSING);
		return (OK);
	} 
	
	trace_event("SendNwMngMsg(NOK): WriteLineMsg() failed", ERROR);
	return (NOK);
	
}

/****************************************************************************/
/* sign_off()                                                          */
/****************************************************************************/
int  sign_off(int nIndexCtx)
{
	SendNwMngMsg(nIndexCtx, B1_F070_SIGNOFF);
	return(OK);
}


/****************************************************************************/
/* sign_on()                                                          */
/****************************************************************************/
int 
	sign_on(int nIndexCtx)
{
	SendNwMngMsg ( nIndexCtx, B1_F070_SIGNON );
	return(OK);
}

/****************************************************************************/
/* retrieval_advice_on()                                                    */
/****************************************************************************/
int 
	retrieval_advice_on(int nIndexCtx)
{

	TSVisaInfo      VisaInfo;
	TSVisaInfo      inVisaInfo;
	char            FieldData[256];
	char            sLine[MAX_LINE_TRC];
	char            outBuffer[1024];
	char            inBuffer[1024];
	int             inLength;
	int             outLength;
	int             FieldLen;
	char            visaRefNbr[13];
	char            sentRefNbr[13];
	char            sGmtDate[32];
	int             sign_on;
	int             attempt;
	int             flag_alarm = NOK;
	int             retour;


	memset(inBuffer, 0, sizeof(inBuffer));
	memset(outBuffer, 0, sizeof(outBuffer));
	memset(visaRefNbr, 0, sizeof(visaRefNbr));
	memset(sentRefNbr, 0, sizeof(sentRefNbr));
	memset(sGmtDate, 0, sizeof(sGmtDate));
	memset (FieldData, 0, sizeof(FieldData));

	trace_event("Start  retrieval_advice_on()", PROCESSING);

	AllocateOracleCtx(nIndexCtx);
	ConnectOracle(nIndexCtx);

	InitVisaInfo(&VisaInfo);
	VisaInfo.nMsgType = 800;

	current_gmt_date(sGmtDate);
	AddVisaField(VISA_XMIT_TIME, &VisaInfo, sGmtDate + 2, 10);

	GetInternalStan(nIndexCtx, FieldData);
	AddVisaField(VISA_AUDIT_NBR, &VisaInfo, FieldData, 6);

	current_julian_date_qjjj(sentRefNbr);
	memcpy(sentRefNbr + 4, sGmtDate + 6, 2);
	memcpy(sentRefNbr + 6, FieldData, 6);
	AddVisaField(VISA_REFERENCE_NBR, &VisaInfo, sentRefNbr, 12);


	AddVisaField (VISA_NW_MNGMT_INFO, &VisaInfo, B1_F070_START_SAF, B1_F070_LEN); 

	/* send message */
	outLength = VisaBuildMsg(outBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, outLength, StationId);
	memcpy(outBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	outLength += VISA_HDR_LEN;


	dump_buffer(outBuffer, outLength, 'E', 'L', 'O');

	DumpVisa(outBuffer);
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

		DumpVisa(inBuffer);

		AnalyseVisa(inBuffer, &inVisaInfo);

		/* check if this is the answer to our request */
		GetVisaField(VISA_REFERENCE_NBR, &inVisaInfo, visaRefNbr, &FieldLen);
		sprintf(sLine, "type  = %d , RRN R = [%.12s], RRN S = [%.12s]", inVisaInfo.nMsgType,visaRefNbr,sentRefNbr);
		trace_event(sLine, TRACE);


		GetVisaField (VISA_RESPONSE_CODE, &inVisaInfo, FieldData, &FieldLen);


		if ((inVisaInfo.nMsgType == 810) &&
			(memcmp(visaRefNbr, sentRefNbr, 12) == 0) &&
			(memcmp(FieldData, B1_F039_APPROVAL, B1_F039_LEN) == 0))  
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
	SendNwMngMsg ( nIndexCtx, B1_F070_STOP_SAF ); 
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
	TSVisaInfo      VisaInfo;
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

			InitVisaInfo(&VisaInfo);
			VisaInfo.nMsgType = 800;

			/* (007) VISA_XMIT_TIME */
			current_gmt_date(FieldData);
			AddVisaField(VISA_XMIT_TIME, &VisaInfo, FieldData + 2, 10);

			/* (011)VISA_AUDIT_NBR */
			GetInternalStan(nIndexCtx, FieldData);
			AddVisaField(VISA_AUDIT_NBR, &VisaInfo, FieldData, 6);

			/* (070) VISA_NW_MNGMT_INFO */
			AddVisaField(VISA_NW_MNGMT_INFO, &VisaInfo, B1_F070_ECHO_TEST, B1_F070_LEN);

			GetVisaField(VISA_AUDIT_NBR, &VisaInfo, Stan, &nLength);

			InitNodeData(&Node);
			GetAcquirerPurgeTime(&nPurgeTime);
			Node.tPurgeTime = nPurgeTime;

			sprintf(Node.szFstKey, "%04d", VisaInfo.nMsgType);
			memcpy(Node.szFstKey + 4, Stan, 6);

			memcpy(Node.sFstPrvtData, "PERIODIQUE", 10);
			Node.nFstPrvtDataLen = 10;

			outLength = VisaBuildMsg(outBuffer + VISA_HDR_LEN, &VisaInfo);
			VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, outLength, StationId);
			memcpy(outBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
			outLength += VISA_HDR_LEN;
			Node.nFstBufLen = outLength;

			if (InsertRequest('B', &Node) != OK)
				trace_event("Can Not Chain Sign On Repeat Request... ", PROCESSING);	
			else
				trace_event("Echo test Request... ", PROCESSING);				

			
			dump_buffer(outBuffer,outLength ,'E', 'L', 'O'); 
			
			DumpVisa(outBuffer);		

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
		shutdown_resource(nIndexCtx, ResourceId);
	}
	return (0);
}
/*HAL26082020 PLUTONL-2185*/
