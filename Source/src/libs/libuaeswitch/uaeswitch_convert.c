/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
-------------------------------------------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
-------------------------------------------------------------------------------------------------------------------------------------
-- Version    Date          	Author       		Jalon                     Activity                          Comment
-------------------------------------------------------------------------------------------------------------------------------------
-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/

#include <uaeswitch.h>
#include <globals.h> 
#include <uaeswitch_ictag.h>
#include <p7_define.h>




/*-------------------------------------------------------------------------------*/
typedef struct STransType {
	int           nNwMsgType;
	char          *szNwTransType;
	char          *szIsoTransType;
	char          *szIsoFuncCode;
} TSTransType;
static TSTransType sTransType[] = {
	/* trans name              MsgType NwTransType  IsoTransType IsoFuncCode */
	{/* Cash Withd/Advn      */ 200,    "01",        "01",        "200"},
	{/* Cash Withd/Advn rev  */ 420,    "01",        "01",        "400"},
	{/* Cash Withd/Advn rev  */ 421,    "01",        "01",        "400"},
	{/* Cash Withd/Advn rev  */ 400,    "01",        "01",        "400"},
	{/* Cash Withd/Advn rev  */ 401,    "01",        "01",        "400"},
	{/* Bal Inq              */ 200,    "31",        "31",        "281"},
};

/*-------------------------------------------------------------------------------*/
typedef struct SAccntType {
	char          *szNwAccntType;
	char          *szIsoAccntType;
} TSAccntType;
static TSAccntType sAccntType[] = {
	/* accnt type name         NwAccntType  IsoAccntType */
	{/* Primary/Billing      */ "00",        "00"},
	{/* Saving               */ "10",        "10"},
	{/* Checking             */ "20",        "20"},
	{/* Credit               */ "30",        "30"},
};

static int ToIsoProcCode(char * szNwProcCode, char * szIsoProcCode);
static int ToIsoFuncCode(int nNwMsgType, char * szNwProcCode, char * szIsoFuncCode);
static int ToNwAdtnlAmnt(
	int   nLen,
	char *szIsoAdtnlAmnt,
	char *szNwAdtnlAmnt);
static int ToIsoAdtnlAmnt(
	int   nLen,
	char *szNwAdtnlAmnt,
	char *szIsoAdtnlAmnt);
/*
static int ToIsoPosData (
char *szMerchType,
char *szNwPosEntryMode,
char *szIsoPosData);
*/
static int ToIsoScrtyData (
	char *szNwScrtyData,
	char *szIsoScrtyData);
static int ToIsoOrgnlData (
	char *szNwOrgnlData,
	char *szTransDate,
	char *szIsoOrgnlData);
static int ToIsoAdtnlDataPriv (
	int              nIndexCtx,
	TSUaeSwitchInfo *sNwInfo,
	char            *szIsoAdtnlDataPriv);
static int ToNwOrgnlData (
	char *szIsoOrgnlData,
	char *szNwOrgnlData);

static TSMsgBitMap      sAutRevReqMsgBitMap [] = {
	/* 001->008 */    C ,M ,M ,M ,   A ,A ,M ,A ,
	/* 009->016 */    A ,A ,M ,M ,   M ,M ,M ,A ,
	/* 017->024 */    M ,M ,M ,A ,   A ,M ,A ,A ,
	/* 025->032 */    M ,A ,A ,A ,   A ,A ,A ,M ,

	/* 033->040 */    A ,A ,A ,A ,   M ,C ,M ,A ,
	/* 041->048 */    M ,M ,M ,A ,   A ,A ,A ,A ,
	/* 049->056 */    M ,A ,A ,A ,   A ,A ,A ,A ,
	/* 057->064 */    A ,A ,A ,A ,   A ,A ,A ,A ,

	/* 065->072 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 073->080 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 081->088 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 089->096 */    A ,C ,A ,A ,   A ,A ,A ,A ,

	/* 097->104 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 105->112 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 113->120 */    A ,A ,A ,A ,   A ,A ,A ,A ,
	/* 121->128 */    A ,A ,A ,A ,   A ,A ,A ,A ,
};
/***********************************************************************
**  name      : BuildReqToNw()
**  desc      : convert a PowerSEVEN Request into a Network Request
**  in        : nIndexCtx  : index of Oracle connection context
**              sMsgBitMap : BitMap of Network Request. It will be used to know
**                           which fileds should be present into the Request
**              sMyIsoInfo : PowerSEVEN Request
**  out       : sNwInfo : Network Request
**  return    : OK        : completed successfuly
**              NOK       : error while processing
***********************************************************************/
int BuildReqToNw(
	int              nIndexCtx,
	TSMsgBitMap      sMsgBitMap[],
	TSIsoInfo       *sMyIsoInfo,
	TSUaeSwitchInfo *sNwInfo)
{
	TSTlvPrivate  		sTlvInfo;
	TSTagUaeSwitch     	UaeSwitchIccInfo; 
	TSTagIso      		IsoIccInfo; 
	char                 sLine[MAX_LINE_TRC]; 
	char                 szTermAddr[64]; 
	char                 szGmtDate[38]; 
	char                 szStan[38];
	char                 szPan[20]; 
	char                 szExpiryDate[5]; 
	char                 szCVV[4]; 
	char                 szPVV[6]; 
	char                 szServiceCode[4]; 
	char                 szT1[77]; 
	char                 szT2[38];
	char                 szScrtyData[128]; 
	char                 szScrtyFmt[3]; 
	char                 szPinKey[128]; 
	char                 szZPK[128]; 
	char                 szPinBlk[17];
	char                 szPinBlkFmt[3]; 
	char                 szTermNbr[17]; 
	char                 cTransMode;
	char                 szWork2[256]; 
	char                 szWork[256]; 
	char                 szFuncName[] = "BuildReqToNw"; 
	char                 szBusinessDate[256];
	char                 tmpField       [LG_MAX];
	char                 isoPosMode     [13];
	char                 sIccBuffer[LG_MAX];
	char                 tmpField1[LG_MAX];     
	char                 FieldData[256]; 
	char                 szAcquirerBank[6+1];
	char                 szAcquirerID[11+1];
	char                 sUaeSwitchTermID[16+1];
	char                 sVisaCountryCode[2+1];
	int                  nKeyIndex;
	int                  nLen; 
	int                  nFldNbr; 
	int                  nStatus;
	int    	            tmpFieldLen;        
	int 		            ChipTranFlag = 0;
	int    	            FieldLen;        



	trace_event("Start BuildReqToNw()",TRACE); 

	memset( sLine          , '\0' , sizeof( sLine          ));    
	memset( szTermAddr     , '\0' , sizeof( szTermAddr     ));
	memset( szGmtDate      , '\0' , sizeof( szGmtDate      ));
	memset( szStan         , '\0' , sizeof( szStan         ));
	memset( szPan          , '\0' , sizeof( szPan          ));
	memset( szExpiryDate   , '\0' , sizeof( szExpiryDate   ));
	memset( szCVV          , '\0' , sizeof( szCVV          ));
	memset( szPVV          , '\0' , sizeof( szPVV          ));
	memset( szServiceCode  , '\0' , sizeof( szServiceCode  ));
	memset( szT1           , '\0' , sizeof( szT1           ));
	memset( szT2           , '\0' , sizeof( szT2           ));
	memset( szScrtyData    , '\0' , sizeof( szScrtyData    ));
	memset( szScrtyFmt     , '\0' , sizeof( szScrtyFmt     ));
	memset( szPinKey       , '\0' , sizeof( szPinKey       ));
	memset( szZPK          , '\0' , sizeof( szZPK          ));
	memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));
	memset( szPinBlkFmt    , '\0' , sizeof( szPinBlkFmt    ));
	memset( szTermNbr      , '\0' , sizeof( szTermNbr      ));
	memset( szWork2        , '\0' , sizeof( szWork2        ));
	memset( szWork         , '\0' , sizeof( szWork         ));
	memset( szBusinessDate , '\0' , sizeof( szBusinessDate ));
	memset( tmpField       , '\0' , sizeof( tmpField       ));
	memset( isoPosMode     , '\0' , sizeof( isoPosMode     ));
	memset( sIccBuffer     , '\0' , sizeof( sIccBuffer     ));
	memset( tmpField1      , '\0' , sizeof( tmpField1      ));
	memset( FieldData      , '\0' , sizeof( FieldData      ));
	memset( szAcquirerBank     , '\0' , sizeof( szAcquirerBank ));
	memset( szAcquirerID       , '\0' , sizeof( szAcquirerID   ));
	memset( sUaeSwitchTermID   , '\0' , sizeof( sUaeSwitchTermID   ));   
	memset( sVisaCountryCode   , '\0' , sizeof( sVisaCountryCode   ));   





	/* Initialize Iso structure */
	InitUaeSwitchInfo (sNwInfo);

	/* Message Type */
	switch (sMyIsoInfo->nMsgType){
	case 1420:
	case 1421: sNwInfo->nMsgType = 400; 
		break;
	default:   sNwInfo->nMsgType = sMyIsoInfo->nMsgType - 1000;
	}

	/* Extracting Data From T1 or T2 */
	if (GetIsoField (ISO_T2_DATA, sMyIsoInfo, szT2, &nLen) == SUCCESS)
		GetTrack2Data (szT2, szPan, szExpiryDate, szServiceCode, szPVV, szCVV);
	else if (GetIsoField (ISO_T1_DATA, sMyIsoInfo, szT1, &nLen) == SUCCESS)
		GetTrack1Data (szT1, szPan, szExpiryDate, szServiceCode, szPVV, szCVV);

	for (nFldNbr = 0; nFldNbr < 128; nFldNbr ++)
	{
		if ( M == sMsgBitMap[nFldNbr].nPresenceFlag
			|| C == sMsgBitMap[nFldNbr].nPresenceFlag
			)
		{ /* Only Mandatory and Conditional Fields */
			switch (nFldNbr)
			{
			case ISO_CARD_NBR :
				if (GetIsoField(nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
					AddUaeSwitchField (nFldNbr, sNwInfo, szWork, nLen);
				else if (strlen(szPan) != 0)
					AddUaeSwitchField (nFldNbr, sNwInfo, szPan, strlen(szPan));
				break;

			case ISO_TRANS_AMOUNT :
				if (GetIsoField(nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
					AddUaeSwitchField (nFldNbr, sNwInfo, szWork, nLen);
				else
					AddUaeSwitchField (nFldNbr, sNwInfo, "000000000000", 12);
				break;

			case ISO_XMIT_TIME :
				if (1804 == sMyIsoInfo->nMsgType || 1420 == sMyIsoInfo->nMsgType )
				{
					current_gmt_date (szGmtDate);
					AddUaeSwitchField (nFldNbr, sNwInfo, szGmtDate + 2, 10);
				} 
				else 
				{
					if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS) 
						AddUaeSwitchField (nFldNbr, sNwInfo,  szWork,  nLen);
				}
				break;

			case ISO_AUDIT_NBR :
				if (1804 == sMyIsoInfo->nMsgType){
					GetInternalStan ( nIndexCtx , szStan );
					AddUaeSwitchField (nFldNbr, sNwInfo, szWork, 6);
				}
				else {
					if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
						AddUaeSwitchField (nFldNbr, sNwInfo,  szWork,  nLen);
				}
				break;
			case ISO_TRANS_DATE_TIME :
				if (1420 == sMyIsoInfo->nMsgType) 
				{
					current_hour(szWork);
					AddUaeSwitchField(UAESWITCH_TRANS_TIME, sNwInfo, szWork, 6);

					current_date_mmdd(szWork);
					AddUaeSwitchField(UAESWITCH_TRANS_DATE, sNwInfo, szWork, 4);
				} 
				else  
					if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS) 
					{    
						AddUaeSwitchField(UAESWITCH_TRANS_TIME, sNwInfo, szWork+6, 6);
						AddUaeSwitchField(UAESWITCH_TRANS_DATE, sNwInfo, szWork+2, 4);
					}
					break;

			case ISO_EXPIRY_DATE :
				if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
					AddUaeSwitchField (nFldNbr, sNwInfo, szWork, 4);
				else if (strlen (szExpiryDate) > 0)
					AddUaeSwitchField (nFldNbr, sNwInfo, szExpiryDate, 4);
				break;

			case ISO_STLMNT_DATE :
				memset(szBusinessDate, 0, sizeof(szBusinessDate));
				memset(szWork, 0, sizeof(szWork));
				if (GetStlmntDate(nIndexCtx, UAESWITCH_NETWORK_CODE, szBusinessDate, szWork) == OK)
					AddUaeSwitchField(nFldNbr, sNwInfo, szWork, 4); 
				else if (GetIsoField(ISO_TRANS_DATE_TIME, sMyIsoInfo, szWork, &nLen) == SUCCESS )
					AddUaeSwitchField(nFldNbr, sNwInfo, szWork+2, 4);
				break;

			case ISO_CAPTURE_DATE :
				if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS) 
					AddUaeSwitchField(nFldNbr, sNwInfo, szWork, nLen);
				break;

			case ISO_POS_DATA :
				if (GetIsoField (ISO_POS_DATA, sMyIsoInfo, isoPosMode, &nLen) == SUCCESS)
				{ 	if (memcmp(isoPosMode+3,"___",3)==0)
				{
					AddUaeSwitchField (UAESWITCH_POS_ENTRY_MODE, sNwInfo, isoPosMode, 3);
				}	
				else 
				{  
					sprintf(sLine,"isoPosMode %s", isoPosMode);
					trace_event(sLine,TRACE); 
					IsoToUAESPosMode (isoPosMode, &tmpField);
					sprintf(sLine,"tmpField %s", tmpField);
					trace_event(sLine,TRACE); 
					memcpy (tmpField,"021",3); /* all POS have pin capability */
					if ( '5' == isoPosMode[6] ) /** ICC **/
						memcpy( tmpField, "05", 2);
					if (isoPosMode[6] != '5' && szServiceCode[0] == '2') 
					{
						memcpy( tmpField, "80", 2);
						ChipTranFlag = 1;
					} 
					AddUaeSwitchField (UAESWITCH_POS_ENTRY_MODE, sNwInfo, tmpField, 3);
				}
				}
				break;

			case ISO_CARD_SEQUENCE_NBR : 
				if ( GetIsoField(ISO_ICC_DATA, sMyIsoInfo, tmpField, &nLen) == SUCCESS && 
					GetIsoField(ISO_CARD_SEQUENCE_NBR, sMyIsoInfo, tmpField, &nLen) == SUCCESS )
					AddUaeSwitchField (ISO_CARD_SEQUENCE_NBR, sNwInfo, tmpField, nLen);
				break; 
			case ISO_MSG_REASON :  	
				AddUaeSwitchField(UAESWITCH_POS_CONDITION_CODE, sNwInfo, "00", 2);
				break;
			case UAESWITCH_ACQR_ID :     
				InitTlvPrivate (&sTlvInfo);
				if ((nStatus = GetIsoField (ISO_ADTNL_DATA_PRIV, sMyIsoInfo, szWork, &nLen)) == SUCCESS)
				{
					AnalyseTlvPrivate (szWork, nLen, &sTlvInfo);
					if (GetTlvPrivate(ACQUIRER_BANK_TAG, &sTlvInfo, szAcquirerBank , &tmpFieldLen) != SUCCESS)
						if (GetIsoField (ISO_ACQR_ID, sMyIsoInfo, szAcquirerBank, &tmpFieldLen) != SUCCESS) 
						{
							trace_event("End BuildReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
							return(NOK);
						}
				}	
				else
					if (GetIsoField (ISO_ACQR_ID, sMyIsoInfo, szAcquirerBank, &tmpFieldLen) != SUCCESS) 		
					{
						trace_event("End BuildReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
						return(NOK);
					}

					nStatus = GetAquirerBin( nIndexCtx, szAcquirerBank, NET_UAESWITCH, szAcquirerID)  ;
					if (nStatus != OK)
					{
						sprintf(sLine,"End BuildReqToNw (NOK) : ERROR Getting szAcquirerID for bank[%s]",szAcquirerBank);
						trace_event(sLine,ERROR);
						return(NOK);
					}

					AddUaeSwitchField(nFldNbr, sNwInfo, szAcquirerID, strlen(szAcquirerID));
					break;

			case ISO_REFERENCE_NBR :     
				if (1804 == sMyIsoInfo->nMsgType){
					current_julian_date_qjjj(szWork);
					memcpy( szWork + 4, szGmtDate + 6, 2);
					memcpy( szWork + 6, szStan, 6);
					AddUaeSwitchField (nFldNbr, sNwInfo, szWork, 12);
				}
				else{
					if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
						AddUaeSwitchField (nFldNbr, sNwInfo,  szWork,  nLen);
				}
				break;

			case ISO_RESPONSE_CODE :
				if (1420 == sMyIsoInfo->nMsgType || 1421 == sMyIsoInfo->nMsgType)
					AddUaeSwitchField (nFldNbr, sNwInfo,  "68",  2);
				break;
			case ISO_TERMINAL_ADR :
				if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS) 
				{
					nLen = sprintf(szTermAddr, "%22.22s%13.13s%3.3s%2.2s",
						szWork,       /* terminal owner */
						szWork + 23,  /* Old: 25, terminal city */
						" ",          /* terminal state */
						szWork + nLen -2   /* Old: 38 terminal country */
						);
					GetIsoField (ISO_ACQR_COUNTRY, sMyIsoInfo, szWork, &FieldLen);	
					nStatus = GetVisaCountryCode( nIndexCtx, szWork, sVisaCountryCode)  ;
					if (nStatus != OK)
					{
						sprintf(sLine,"End BuildReqToNw (NOK) : ERROR Getting Visa Country Code for country[%s]",szWork);
						trace_event(sLine,ERROR);
						return(NOK);
					}
					memcpy(szTermAddr+ nLen -2,sVisaCountryCode,2);
					AddUaeSwitchField (nFldNbr, sNwInfo, szTermAddr,  nLen);
				}
				break;

			case ISO_PIN_DATA :
				if (GetIsoField (nFldNbr, sMyIsoInfo, szPinBlk, &nLen) == SUCCESS)
				{
					nKeyIndex = UNKOWN;
					if ( GetIsoField(ISO_CARD_NBR,      sMyIsoInfo, szPan,       &nLen)    == SUCCESS
						&& GetIsoField(ISO_SECURITY_DATA, sMyIsoInfo, szScrtyData, &nLen)    == SUCCESS
						&& GetIsoField(ISO_TERMINAL_NBR,  sMyIsoInfo, szTermNbr,   &nLen)    == SUCCESS
						&& GetKey(nIndexCtx, G_sUaeSwitch.F_szRsrcId, AWK, &nKeyIndex, szZPK) == OK
						)
					{
						/* looking for the key used to encrypt the pin block */

						if (debug_option == 1)
						{
							sprintf(sLine,"[ISO_SECURITY_DATA] %s", szScrtyData);
							trace_event(sLine, TRACE);
						}

						InitTlvPrivate (&sTlvInfo);
						if ((nStatus = GetIsoField (ISO_ADTNL_DATA_PRIV, sMyIsoInfo, szWork, &nLen)) == SUCCESS)
						{
							AnalyseTlvPrivate (szWork, nLen, &sTlvInfo);
							if((nStatus = GetTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &sTlvInfo, szPinKey, &nLen)) != SUCCES )
							{
								trace_event ("ZPK NOT PRESENT", PROCESSING);
								sMyIsoInfo->nMsgType += 10;
								ReplyIsoMsgToBal (nBalMsg, sMyIsoInfo, "", FIELD_CONTROL_ERROR_ACTION);
								memset( szPan          , '\0' , sizeof( szPan          ));
								memset( szCVV          , '\0' , sizeof( szCVV          ));
								memset( szPVV          , '\0' , sizeof( szPVV          ));
								memset( szT1           , '\0' , sizeof( szT1           ));
								memset( szT2           , '\0' , sizeof( szT2           ));
								memset( szPinKey       , '\0' , sizeof( szPinKey       ));
								memset( szZPK          , '\0' , sizeof( szZPK          ));
								memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));
								trace_event("End  BuildReqToNw(NOK)", PROCESSING);
								return (NOK);
							}
						}
						else
						{
							trace_event ("DE48 NOT PRESENT", PROCESSING);
							sMyIsoInfo->nMsgType += 10;
							ReplyIsoMsgToBal (nBalMsg, sMyIsoInfo, "", FIELD_CONTROL_ERROR_ACTION);
							memset( szPan          , '\0' , sizeof( szPan          ));
							memset( szCVV          , '\0' , sizeof( szCVV          ));
							memset( szPVV          , '\0' , sizeof( szPVV          ));
							memset( szT1           , '\0' , sizeof( szT1           ));
							memset( szT2           , '\0' , sizeof( szT2           ));
							memset( szPinKey       , '\0' , sizeof( szPinKey       ));
							memset( szZPK          , '\0' , sizeof( szZPK          ));
							memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));
							trace_event("End  BuildReqToNw(NOK)", PROCESSING);
							return (NOK);
						}

						/* looking for translation parameters */
						GetSecurityData( SECURITY_FORMAT , szScrtyData, szScrtyFmt , &nLen);
						GetSecurityData( PIN_BLOCK_FORMAT, szScrtyData, szPinBlkFmt, &nLen);

						if ( memcmp ( szScrtyFmt  , "01" , 2 ) == 0 )
							cTransMode = TERMINAL_MODE;
						else
							cTransMode = SCHEME_MODE;

						/* translate the pin */
						if ((nStatus = pin_translate(nIndexCtx, szPan, szPinKey, szZPK, szPinBlk, szPinBlkFmt, "01", cTransMode)) != OK)
						{
							trace_event ("HSM TRANSLATION ERROR", PROCESSING);
							sMyIsoInfo->nMsgType += 10;
							ReplyIsoMsgToBal (nBalMsg, sMyIsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
							memset( szPan          , '\0' , sizeof( szPan          ));
							memset( szCVV          , '\0' , sizeof( szCVV          ));
							memset( szPVV          , '\0' , sizeof( szPVV          ));
							memset( szT1           , '\0' , sizeof( szT1           ));
							memset( szT2           , '\0' , sizeof( szT2           ));
							memset( szPinKey       , '\0' , sizeof( szPinKey       ));
							memset( szZPK          , '\0' , sizeof( szZPK          ));
							memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));
							trace_event("End  BuildReqToNw(NOK)", PROCESSING);
							return (NOK);
						}
						/* add the translated pin */
						AddUaeSwitchField (nFldNbr, sNwInfo, szPinBlk, 16);

					}
					else
					{
						trace_event ("PIN NOT PRESENT", PROCESSING);
						sMyIsoInfo->nMsgType += 10;
						ReplyIsoMsgToBal (nBalMsg, sMyIsoInfo, "", FIELD_CONTROL_ERROR_ACTION);
						memset( szPan          , '\0' , sizeof( szPan          ));
						memset( szCVV          , '\0' , sizeof( szCVV          ));
						memset( szPVV          , '\0' , sizeof( szPVV          ));
						memset( szT1           , '\0' , sizeof( szT1           ));
						memset( szT2           , '\0' , sizeof( szT2           ));
						memset( szPinKey       , '\0' , sizeof( szPinKey       ));
						memset( szZPK          , '\0' , sizeof( szZPK          ));
						memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));
						trace_event("End  BuildReqToNw(NOK)", PROCESSING);
						return (NOK);
					}
				}
				break;
			case ISO_SECURITY_DATA :
				nLen = sprintf(szScrtyData, "200101%02d00000000", nKeyIndex);
				sprintf(sLine,"[In case ISO_SECURITY_DATA] length: %d|%d, data: %s", nKeyIndex,nLen, szScrtyData);   
				trace_event(sLine, TRACE);
				AddUaeSwitchField (nFldNbr, sNwInfo, szScrtyData, nLen);
				break; 

			case UAESWITCH_ORIG_DATA :
				if (GetIsoField(ISO_ORIG_DATA, sMyIsoInfo, szWork, &nLen ) == SUCCES )
				{
					sprintf(sLine,"Field 56 PWC is %s", szWork);
					trace_event(sLine,TRACE); 
					nLen = ToNwOrgnlData(szWork, szWork2);
					sprintf(sLine,"Field 90 UAE is %d|%s", nLen,szWork2);
					trace_event(sLine,TRACE); 
					AddUaeSwitchField (UAESWITCH_ORIG_DATA, sNwInfo,  szWork2,  42);
				}
				break;
			case ISO_T2_DATA :
				if (GetIsoField(nFldNbr, sMyIsoInfo, szWork, &nLen ) == SUCCES )
				{
					replace(szWork, '=','D',nLen);
					AddUaeSwitchField (UAESWITCH_T2_DATA, sNwInfo,  szWork,  nLen);
				}
				break;

			case ISO_ICC_DATA :
				if (GetIsoField (ISO_ICC_DATA, sMyIsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					InitIsoIcTag ( &IsoIccInfo );
					AnalyseIsoIc  ( tmpField, tmpFieldLen , &IsoIccInfo );
					InitUaeSwitchIcTag ( &UaeSwitchIccInfo );

					/** TAG 9F27 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_CRYPTO_INFO_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_CRYPTO_INFO_DATA , &UaeSwitchIccInfo, FieldData,  FieldLen );
					else    
						AddUaeSwitchIcTag (UAESWITCH_TAG_CRYPTO_INFO_DATA , &UaeSwitchIccInfo, "80", 2 );             
					/** TAG 9F33 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TERM_CAP , &UaeSwitchIccInfo, FieldData,  FieldLen );

					/** TAG 95 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TVR , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TVR , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F37 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_UNPRED_NUMBER , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F1E **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_IFD_SERIAL_NUM , &UaeSwitchIccInfo, FieldData,  FieldLen );
					else 
						AddUaeSwitchIcTag (UAESWITCH_TAG_IFD_SERIAL_NUM , &UaeSwitchIccInfo, "0000000000000001",  16 );
					/** TAG 9F10 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag(UAESWITCH_TAG_ISS_APP_DATA , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F26 **/
					memset(FieldData, 0, sizeof(FieldData));
					if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, FieldData, &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_APP_CRYPTOGRAM, &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F36 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_ATC , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 82 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_APP_INTER_PROFILE , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9C **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TRANS_TYPE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_TYPE , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F1A **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TERM_COUNTRY_CODE , &UaeSwitchIccInfo,FieldData , FieldLen);
					/** TAG 9A **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TRANS_DATE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_DATE , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F02 **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_AMOUNT , &UaeSwitchIccInfo, FieldData,  FieldLen);
					/** TAG 5F2A **/
					memset(FieldData, 0, sizeof(FieldData));
					if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_CUR_CODE , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F03 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_OTHER_AMOUNT , &UaeSwitchIccInfo, FieldData,  FieldLen );
					else 
						AddUaeSwitchIcTag (UAESWITCH_TAG_OTHER_AMOUNT , &UaeSwitchIccInfo, "000000000000",  12 );
					/** TAG 9F34 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_CVM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_CVM , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F53 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_CATEG_CODE , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 84 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_DED_FILE_NAME , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F09 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TERM_APP_VER_NUM , &UaeSwitchIccInfo, FieldData,  FieldLen );
					/** TAG 9F41 **/
					memset(FieldData, 0, sizeof(FieldData));
					if ( GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
						AddUaeSwitchIcTag (UAESWITCH_TAG_TRANS_SEQ_NUM , &UaeSwitchIccInfo, FieldData,  FieldLen );

					FieldLen = UaeSwitchBuildIcFld ( sIccBuffer, &UaeSwitchIccInfo);

					IsoDumpIccFields(sIccBuffer,FieldLen);

					AddUaeSwitchField (UAESWITCH_ICC_DATA, sNwInfo, sIccBuffer, FieldLen);
					AddUaeSwitchField(UAESWITCH_AUTH_LIFE_CYCLE_CODE, sNwInfo, "003510", 6);
				}
				else 
				{ 
					if (ChipTranFlag== 1)
						AddUaeSwitchField(UAESWITCH_AUTH_LIFE_CYCLE_CODE, sNwInfo, "003500", 6);
				}
				break;	
			default :
				if (GetIsoField (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCESS)
					AddUaeSwitchField (nFldNbr, sNwInfo,  szWork,  nLen);
			}
		}
	}

	memset( szPan          , '\0' , sizeof( szPan          ));
	memset( szExpiryDate   , '\0' , sizeof( szExpiryDate   ));
	memset( szCVV          , '\0' , sizeof( szCVV          ));
	memset( szPVV          , '\0' , sizeof( szPVV          ));
	memset( szServiceCode  , '\0' , sizeof( szServiceCode  ));
	memset( szT1           , '\0' , sizeof( szT1           ));
	memset( szT2           , '\0' , sizeof( szT2           ));
	memset( szPinKey       , '\0' , sizeof( szPinKey       ));
	memset( szZPK          , '\0' , sizeof( szZPK          ));
	memset( szPinBlk       , '\0' , sizeof( szPinBlk       ));   

	return_OK(szFuncName);
}


/***********************************************************************
**  name      : BuildReqFromNw()
**  desc      : convert a Network Request into a PowerSEVEN Request
**  in        : nIndexCtx  : index of Oracle connection context
**              sMsgBitMap : BitMap of Network Request. It will be used to know
**                           which fileds should be present into the Request
**              sNwInfo : Network Request
**  out       : sMyIsoInfo : PowerSEVEN Request
**  return    : OK        : completed successfuly
**              NOK       : error while processing
**  author    : 
***********************************************************************/
int BuildReqFromNw(
	int              nIndexCtx,
	TSMsgBitMap      sMsgBitMap[],
	TSUaeSwitchInfo *sNwInfo,
	TSIsoInfo       *sMyIsoInfo)
{
	char         szPan[20], szExpiryDate[5], szCVV[4], szPVV[6], szServiceCode[4], szT1[77], szT2[38], szStan[38];
	char         szScrtyData[128];
	char         szMerchType[12], szTransDate[24], szTransAmnt[24], szTransCrncy[8], szSettlCrncy[8];
	char         szWork2[256], szWork[256], sLine[MAX_LINE_TRC], szFuncName[] = "BuildReqFromNw";
	int          nKeyIndex;
	int          nLen, nFldNbr, nStatus;
	char        szProcCode[256];
	char        szPosMode[4];
	char        tmpField[256];
	char        sIsoTermID[8+1];

	START_TRACE_PAR1(szFuncName, "%d", nIndexCtx);

	memset (szPan,           0, sizeof(szPan));
	memset (szExpiryDate,    0, sizeof(szExpiryDate));
	memset (szCVV,           0, sizeof(szCVV));
	memset (szPVV,           0, sizeof(szPVV));
	memset (szServiceCode,   0, sizeof(szServiceCode));
	memset (szT1,            0, sizeof(szT1));
	memset (szT2,            0, sizeof(szT2));
	memset (szScrtyData,     0, sizeof(szScrtyData));
	memset (szTransDate,     0, sizeof(szTransDate));
	memset (szTransAmnt,     0, sizeof(szTransAmnt));
	memset (szSettlCrncy,    0, sizeof(szSettlCrncy));
	memset (szTransCrncy,    0, sizeof(szTransCrncy));
	memset (szMerchType,     0, sizeof(szMerchType));
	memset (szProcCode,      0, sizeof(szProcCode));
	memset (szStan,          0, sizeof(szStan));
	memset (sIsoTermID,      0, sizeof(sIsoTermID));
	memset (szPosMode,      0, sizeof(szPosMode));


	/* Initialize Iso structure */
	InitIsoInfo (sMyIsoInfo);

	/* Message Type */
	switch (sNwInfo->nMsgType){
	case 400:
	case 401:
	case 420:
	case 210: 
	case 421: sMyIsoInfo->nMsgType = 1420; break;
	default:   sMyIsoInfo->nMsgType = sNwInfo->nMsgType + 1000;
	}

	/* Extracting Data From T1 or T2 */
	if (GetUaeSwitchField (ISO_T2_DATA, sNwInfo, szT2, &nLen) == SUCCESS)
		GetTrack2Data (szT2, szPan, szExpiryDate, szServiceCode, szPVV, szCVV);
	else if (GetUaeSwitchField (ISO_T1_DATA, sNwInfo, szT1, &nLen) == SUCCESS)
		GetTrack1Data (szT1, szPan, szExpiryDate, szServiceCode, szPVV, szCVV);

	for (nFldNbr = 0; nFldNbr < 128; nFldNbr ++)
	{
		switch (nFldNbr)
		{
		case ISO_CARD_NBR :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szWork, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo, szWork, nLen);
			else if (strlen(szPan) != 0)
				AddIsoField (nFldNbr, sMyIsoInfo, szPan, strlen(szPan));
			break;

		case ISO_PROC_CODE :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szProcCode, &nLen) == SUCCESS){
				nLen = ToIsoProcCode(szProcCode, szWork2);
				AddIsoField (nFldNbr, sMyIsoInfo, szWork2, nLen);
			}
			break;

		case ISO_TRANS_AMOUNT :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szTransAmnt, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo, szTransAmnt, nLen);
			break;

		case ISO_STLMNT_AMOUNT :
			if (strlen(szTransAmnt) != 0) 
				AddIsoField (nFldNbr, sMyIsoInfo, szTransAmnt, 12);
			break;

		case ISO_BILLING_AMOUNT :
			if (strlen(szTransAmnt) != 0)
				AddIsoField (nFldNbr, sMyIsoInfo, szTransAmnt, 12);
			break;

		case ISO_STLMNT_CNVRSN_RATE :
			AddIsoField (nFldNbr, sMyIsoInfo, "10000000", 8);
			break;

		case ISO_BILLING_CNVRSN_RATE :
			AddIsoField (nFldNbr, sMyIsoInfo, "10000000", 8);
			break;

		case ISO_TRANS_DATE_TIME :
			current_date_yymmdd(szTransDate);
			if( GetUaeSwitchField (UAESWITCH_TRANS_DATE, sNwInfo, szWork, &nLen) == SUCCESS )
				memcpy(szTransDate + 2, szWork, 6);

			current_hour(szTransDate + 6);
			if( GetUaeSwitchField (UAESWITCH_TRANS_TIME, sNwInfo, szWork, &nLen) == SUCCESS)
				memcpy(szTransDate + 6, szWork, 6);

			AddIsoField(nFldNbr, sMyIsoInfo, szTransDate, 12);
			break;

		case ISO_AUDIT_NBR :
			/* Start IBO 19042009 : Use the original STAN in case of late response received */
			if ( sNwInfo->nMsgType == 210)
			{
				sNwInfo->nMsgType = 400;
				if( GetUaeSwitchField (UAESWITCH_AUDIT_NBR, sNwInfo, szStan, &nLen) != SUCCESS)
					GetInternalStan ( nIndexCtx , szStan );
			}
			else
				/* End   IBO 19042009 : Use the original STAN */
				GetInternalStan ( nIndexCtx , szStan );

			AddIsoField (nFldNbr, sMyIsoInfo, szStan, 6);
			break;

		case ISO_EXPIRY_DATE :
			if (GetUaeSwitchField (nFldNbr, sNwInfo, szWork, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo, szWork, 4);
			else if (strlen (szExpiryDate) > 0)
				AddIsoField (nFldNbr, sMyIsoInfo, szExpiryDate, 4);
			break;

		case ISO_POS_DATA :
			if ( GetUaeSwitchField(nFldNbr, sNwInfo, szPosMode, &nLen) == SUCCESS
				&&  GetUaeSwitchField(UAESWITCH_MERCHANT_TYPE, sNwInfo, szWork, &nLen) == SUCCESS
				&& sNwInfo->nMsgType != 420)
			{
				nLen = ToIsoPosData(szWork, szPosMode, szWork2);
				AddIsoField (nFldNbr, sMyIsoInfo, szWork2, nLen);
			}

			else if ( GetUaeSwitchField(nFldNbr, sNwInfo, szWork, &nLen) == SUCCESS){
				memcpy(szWork2,szWork,3);
				memcpy(szWork2+3,"___",3);
				memcpy(szWork2+6,"      ",6);
				AddIsoField (nFldNbr, sMyIsoInfo, szWork2, 12);
			}
			break;
		case ISO_CARD_SEQUENCE_NBR :
			/* Field 23: Card Seq Number EMV*/
			if (GetUaeSwitchField (UAESWITCH_CARD_SEQUENCE_NBR, sNwInfo, szWork, &nLen) == SUCCESS )
				AddIsoField  (ISO_CARD_SEQUENCE_NBR, sMyIsoInfo, szWork, nLen);
			else{
				trace_event ("SEQ NOT RECEIVED", FATAL);
			}
		case ISO_FUNCTION_CODE :
			nLen = ToIsoFuncCode(sNwInfo->nMsgType, szProcCode, szWork2);
			AddIsoField (nFldNbr, sMyIsoInfo, szWork2, nLen);
			break;

		case ISO_ACQR_ID :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szWork, &nLen) == SUCCESS)
			{
				AddIsoField (nFldNbr, sMyIsoInfo, szWork, nLen);
				PutIsoField (ISO_ACQR_COUNTRY, sMyIsoInfo, szWork, 3);
			}
			break;
		case ISO_ADTNL_DATA_PRIV :
			if ((nLen = ToIsoAdtnlDataPriv(nIndexCtx, sNwInfo, szWork)) > 0)
				AddIsoField (ISO_ADTNL_DATA_PRIV, sMyIsoInfo, szWork, nLen);

			break;

		case ISO_TRANS_CRNCY :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szTransCrncy, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo, szTransCrncy, 3);
			break;

		case ISO_STLMNT_CRNCY :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szSettlCrncy, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo, szSettlCrncy, 3);
			else if (strlen(szTransCrncy) != 0)
				AddIsoField (nFldNbr, sMyIsoInfo, szTransCrncy, 3);
			break;

		case ISO_BILLING_CRNCY :
			if (strlen(szTransCrncy) != 0)
				AddIsoField (nFldNbr, sMyIsoInfo, szTransCrncy, 3);
			break;

		case ISO_SECURITY_DATA :
			if (GetUaeSwitchField(nFldNbr, sNwInfo, szWork, &nLen ) == SUCCES ){
				nLen = ToIsoScrtyData (szWork, szScrtyData);
				AddIsoField (ISO_SECURITY_DATA, sMyIsoInfo, szScrtyData, nLen);
			} else {
				/*BEGIN AHM 05/01/2006 Utile dans le host interface pour alimnter le champs 48*/
				/* (53) SECURITY_DATA */
				char         sPid[10];
				memset(tmpField, 0, sizeof(tmpField));
				sprintf(sPid, "%.8d", getpid());
				PutSecurityData( SECURITY_FORMAT         , tmpField,  "02"      , 2);
				PutSecurityData( PIN_BLOCK_FORMAT        , tmpField,  "01"      , 2);
				PutSecurityData( PIN_ENCRYPTION_KEY_INDEX, tmpField,  "000"     , 3);
				PutSecurityData( MAC_KEY_INDEX           , tmpField,  "000"     , 3);
				PutSecurityData( SOURCE_PID              , tmpField,  sPid    , 8);
				PutSecurityData( SOURCE_RESOURCE         , tmpField,   G_sUaeSwitch.F_szRsrcId, 6);
				PutSecurityData( DESTINATION_PID         , tmpField,  "00000000" , 8);
				PutSecurityData( DESTINATION_RESOURCE    , tmpField,  "HT0001"  , 6);
				PutSecurityData( USER                    , tmpField,  "UAESWITCH      ", 15);
				PutSecurityData( PURGE_TIME              , tmpField,  "000000000",  9);
				InsertIsoField( ISO_SECURITY_DATA, sMyIsoInfo, tmpField, strlen(tmpField));
				/*END AHM*/
			}
			break;

		case ISO_ADTNL_AMOUNTS :
			if (GetUaeSwitchField(UAESWITCH_REPLACEMENT_AMOUNT, sNwInfo, szWork, &nLen ) == SUCCES ){
				AddIsoField (nFldNbr, sMyIsoInfo, szWork, nLen);
			}
			break;
			/*** EMV UPGRADE OBE05082009*/
		case ISO_ICC_DATA :
			if (sNwInfo->nMsgType != 420)
			{
				if (GetUaeSwitchField (UAESWITCH_ICC_DATA, sNwInfo,szWork, &nLen) == SUCCESS)
				{
					dump_buffer(szWork, nLen, 'A','X', 'X');
					AddIsoField (ISO_ICC_DATA, sMyIsoInfo, szWork, nLen);
				}

				if (GetUaeSwitchField (UAESWITCH_AUTH_LIFE_CYCLE_CODE, sNwInfo,szWork, &nLen) == SUCCESS)
				{
					dump_buffer(szWork, nLen, 'A','X', 'X');
					AddIsoField (ISO_AUTHORIZATION_CYCLE_CODE, sMyIsoInfo, szWork, nLen);
				}
			}
			/*** EMV UPGRADE OBE05082009*/
		case ISO_ORIG_DATA :
			if (GetUaeSwitchField(UAESWITCH_ORIG_DATA, sNwInfo, szWork, &nLen ) == SUCCES ){
				nLen = ToIsoOrgnlData(szWork, szTransDate, szWork2);
				AddIsoField (nFldNbr, sMyIsoInfo, szWork2 , nLen);
			}
			break;

		default :
			if (GetUaeSwitchField (nFldNbr, sNwInfo, szWork, &nLen) == SUCCESS)
				AddIsoField (nFldNbr, sMyIsoInfo,  szWork,  nLen);
		}
	}

	switch (sNwInfo->nMsgType){
	case 400:
	case 401:
	case 420:
	case 421: 
		/* Field 25: Message Reason, 4001 = Unspecified */
		PutIsoField (ISO_MSG_REASON , sMyIsoInfo, "4001", 4);

		/* Partial Reversal Management */
		/* Field 30: replacement trx amount + replacement billing amount */
		/* in case of a partial reversal */
		if (GetUaeSwitchField (UAESWITCH_REPLACEMENT_AMOUNT, sNwInfo, szWork, &nLen) == SUCCESS)
		{
			PutIsoField (ISO_FUNCTION_CODE, sMyIsoInfo, "402", 3);
			memset(tmpField, 0, sizeof(tmpField));
			memset(tmpField, '0', 24);
			memcpy(tmpField, szWork,12);
			memcpy(tmpField + 12, szWork,12);
			PutIsoField (ISO_ORIGINAL_AMOUNTS, sMyIsoInfo, tmpField, 24);
		}
		break;
	}


	dump_p7_struct(sMyIsoInfo);

	return_OK(szFuncName);
}


/***********************************************************************
**  name      : BuildRepToNw()
**  desc      : convert a PowerSEVEN Reply into a Network Reply
**  in        : nIndexCtx  : index of Oracle connection context
**              sMsgBitMap : BitMap of Network Reply. It will be used to know
**                           which fileds should be present into the Reply
**              sOrgNwInfo : Network Original Request. It will be used to build
**                           most of the message.
**              sMyIsoInfo : PowerSEVEN Reply. It will be used to build response
**                           fields in the reply.
**  out       : sNwInfo    : Network Reply
**  return    : OK         : completed successfuly
**              NOK        : error while processing
***********************************************************************/
int BuildRepToNw(
	int                  nIndexCtx,
	TSMsgBitMap          sMsgBitMap[],
	TSUaeSwitchInfo     *sOrgNwInfo,
	TSIsoInfo           *sMyIsoInfo,
	TSUaeSwitchInfo     *sNewNwInfo)
{

	TSTagUaeSwitch     	UaeSwitchIccInfo; 
	TSTagIso      	    IsoIccInfo; 
	char                 szNwRespCode[3+1];
	char                 szNwAdtnlAmnt[128];
	char                 szWork[256];
	char                 sLine[MAX_LINE_TRC]; 
	char                 sIccBuffer[LG_MAX];
	char                 FieldData[256]; 
	char                 tmpField [LG_MAX];
	int                  nLen; 
	int                  nFldNbr;
	int                  nStatus;
	int    	            FieldLen;        
	int    	            tmpFieldLen;     
	int 		            nChip = 0; 



	trace_event("Start BuildRepToNw()",PROCESSING);

	memset( szNwRespCode,     '\0', sizeof(szNwRespCode));
	memset( szNwAdtnlAmnt,    '\0', sizeof(szNwAdtnlAmnt));
	memset( szWork,           '\0', sizeof(szWork));
	memset( tmpField, 	     '\0', sizeof(tmpField));
	memset( sIccBuffer, 	     '\0', sizeof(sIccBuffer));
	memset( FieldData, 	     '\0', sizeof(FieldData));
	memset( sLine, 	         '\0', sizeof(sLine));



	/* Initialize Iso structure */
	InitUaeSwitchInfo (sNewNwInfo);

	/* Message Type */
	sNewNwInfo->nMsgType = sOrgNwInfo->nMsgType + 10 - sOrgNwInfo->nMsgType%2;

	for (nFldNbr = 0; nFldNbr < 128; nFldNbr ++)
	{
		if ( M  == sMsgBitMap[nFldNbr].nPresenceFlag
			|| C  == sMsgBitMap[nFldNbr].nPresenceFlag
			|| MR == sMsgBitMap[nFldNbr].nPresenceFlag
			|| CR == sMsgBitMap[nFldNbr].nPresenceFlag
			)
		{ /* Only Mandatory and Conditional Fields */
			switch (nFldNbr)
			{
			case ISO_AUTHOR_ID :
				if ( GetIsoField  (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCES)
					AddUaeSwitchField (nFldNbr, sNewNwInfo, szWork, nLen);
				break;

			case ISO_RESPONSE_CODE :
				if ( GetIsoField  (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCES ){
					TranslateRespCode (nIndexCtx, G_sUaeSwitch.F_szRsrcId, szWork, "PR", szNwRespCode);
					if (memcmp(szNwRespCode,"  ",2)==0)
						memcpy(szNwRespCode,"96",2);

					AddUaeSwitchField (nFldNbr, sNewNwInfo, szNwRespCode, 2);

				}
				break;

			case ISO_ADTNL_AMOUNTS :
				memset(szWork, 0, sizeof(szWork));
				if ( GetIsoField  (nFldNbr, sMyIsoInfo, szWork, &nLen) == SUCCES ){
					memset (szNwAdtnlAmnt, 0 , sizeof (szNwAdtnlAmnt));
					nLen = ToNwAdtnlAmnt(nLen, szWork, szNwAdtnlAmnt);
					PutUaeSwitchField (UAESWITCH_REMAINING_OPEN_TO_USE, sNewNwInfo, szNwAdtnlAmnt, strlen (szNwAdtnlAmnt));
				}
				break;

			case ISO_STLMNT_CODE :
				PutUaeSwitchField (nFldNbr, sNewNwInfo, "1", 1); /* in balance */
				break;

			case ISO_ICC_DATA :            
				if (GetIsoField (ISO_ICC_DATA, sMyIsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					InitIsoIcTag ( &IsoIccInfo );
					AnalyseIsoIc  ( tmpField, tmpFieldLen , &IsoIccInfo );
					InitUaeSwitchIcTag ( &UaeSwitchIccInfo );

					IsoDumpIccFields(tmpField,tmpFieldLen);

					/** TAG 91 **/
					memset(FieldData, '\0', sizeof(FieldData));
					nChip = 1; 
					if ( GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &IsoIccInfo, FieldData,  &nLen ) == SUCCES )
					{    
						if(nLen == 20)	
						{
							if(memcmp(szNwRespCode, "00", 2) == 0)
								memcpy(FieldData+16, "3030", 4);
							else
								memcpy(FieldData+16, "3035", 4);

							sprintf(sLine,"uaeswitch ARPC= %s",FieldData);
							trace_event(sLine,TRACE);
						}
						AddUaeSwitchIcTag (UAESWITCH_TAG_ISS_AUTHEN_DATA , &UaeSwitchIccInfo, FieldData,  nLen );
						nChip = 1;
					}
					else
						AddUaeSwitchIcTag (UAESWITCH_TAG_ISS_AUTHEN_DATA , &UaeSwitchIccInfo, "9100" ,  4 );

					FieldLen = UaeSwitchBuildIcFld ( sIccBuffer, &UaeSwitchIccInfo);
					IsoDumpIccFields(sIccBuffer,FieldLen);
					PutUaeSwitchField (UAESWITCH_ICC_DATA, sNewNwInfo, sIccBuffer, FieldLen); 

					if (GetUaeSwitchField (UAESWITCH_CARD_SEQUENCE_NBR, sOrgNwInfo, szWork, &nLen) == SUCCESS )
						PutUaeSwitchField (UAESWITCH_CARD_SEQUENCE_NBR, sNewNwInfo, szWork, nLen);              
					else
						trace_event ("Card SEQ NOT RECEIVED", FATAL);

				}
				else if ((tmpFieldLen  != 0)&& (GetIsoField (ISO_ICC_DATA, sMyIsoInfo, tmpField, &tmpFieldLen) != SUCCESS)) 
				{
					memset(FieldData, 0, sizeof(FieldData)); 
					sprintf(sLine, "FieldData =[%d] Defaulted to 910A00000000000000003030", nLen);
					trace_event(sLine, PROCESSING); 
					memcpy(FieldData,"910A00000000000000003030", 24); 
					AddUaeSwitchIcTag (UAESWITCH_TAG_ISS_AUTHEN_DATA , &UaeSwitchIccInfo, FieldData,  nLen );
					nChip = 1;	 

					FieldLen = UaeSwitchBuildIcFld ( sIccBuffer, &UaeSwitchIccInfo);
					IsoDumpIccFields(sIccBuffer,FieldLen);
					PutUaeSwitchField (UAESWITCH_ICC_DATA, sNewNwInfo, sIccBuffer, FieldLen);

				}
				break;

			case 56:
				if (GetUaeSwitchField (nFldNbr, sOrgNwInfo, szWork, &nLen) == SUCCESS)
				{     memset(FieldData, 0, sizeof(FieldData));
				memcpy(FieldData,szWork,5);
				memcpy(FieldData+5,"1",1);
				if (GetIsoField (ISO_ICC_DATA, sMyIsoInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{  if (nChip == 1)
				memcpy(FieldData+5,"2",1);
				else 
					memcpy(FieldData+5,"1",1);
				}	
				else
					memcpy(FieldData+5, "0",1);  	
				FieldData[4] = '1';
				PutUaeSwitchField (nFldNbr, sNewNwInfo,  FieldData,  6);
				}
				break;
			default :
				if (GetUaeSwitchField (nFldNbr, sOrgNwInfo, szWork, &nLen) == SUCCESS)
					PutUaeSwitchField (nFldNbr, sNewNwInfo,  szWork,  nLen);
			}
		}
	}
	sprintf(sLine,"Response Code is %s",szNwRespCode);
	trace_event(sLine,TRACE);
	if (!memcmp(szNwRespCode,"61",2))
	{
		memset(szNwAdtnlAmnt, 0, sizeof(szNwAdtnlAmnt));
		memset(szNwAdtnlAmnt, '0', 25);
		memcpy(szNwAdtnlAmnt, "3",1);
		PutUaeSwitchField (UAESWITCH_REMAINING_OPEN_TO_USE, sNewNwInfo, szNwAdtnlAmnt, 25);
		sprintf(sLine,"BALANCE %s",szNwAdtnlAmnt);
		trace_event(sLine,TRACE);
	}

	trace_event("End   BuildRepToNw(OK)",PROCESSING);
	return(OK);

}

/***********************************************************************
**  name      : BuildRepFromNw()
**  desc      : convert a PowerSEVEN Reply into a Network Reply
**  in        : nIndexCtx  : index of Oracle connection context
**              sMsgBitMap : BitMap of Network Reply. It will be used to know
**                           which fileds should be present into the Reply
**              sOrgNwInfo : Network Original Request. It will be used to build
**                           most of the message.
**              sMyIsoInfo : PowerSEVEN Reply. It will be used to build response
**                           fields in the reply.
**  out       : sNwInfo    : Network Reply
**  return    : OK         : completed successfuly
**              NOK        : error while processing
**  author    : 
***********************************************************************/
int BuildRepFromNw(
	int                  nIndexCtx,
	TSMsgBitMap          sMsgBitMap[],
	TSUaeSwitchInfo     *sNwInfo,
	TSIsoInfo           *sOrgMyIsoInfo,
	TSIsoInfo           *sNewMyIsoInfo)
{
	char         sLine[MAX_LINE_TRC];
	char         szMyRespCode[3+1]; 
	char         szMyProcCode[6+1]; 
	char         szIsoAdtnlAmnt[128];
	char         szWork[256]; 
	char         szUaeRespCode [ 2 + 1 ];
	int          nLen; 
	int          nFldNbr; 
	int          nStatus;


	memset(sLine,          '\0', sizeof(sLine));
	memset(szMyRespCode,   '\0', sizeof(szMyRespCode));
	memset(szMyProcCode,   '\0', sizeof(szMyProcCode));
	memset(szIsoAdtnlAmnt, '\0', sizeof(szIsoAdtnlAmnt));
	memset(szWork,         '\0', sizeof(szWork));
	memset(szUaeRespCode,  '\0', sizeof(szUaeRespCode));



	sprintf(sLine,"Start BuildRepFromNw(%d)",nIndexCtx);
	trace_event(sLine,PROCESSING);


	/* Initialize Iso structure */
	InitIsoInfo (sNewMyIsoInfo);

	/* Message Type */
	sNewMyIsoInfo->nMsgType = sOrgMyIsoInfo->nMsgType + 10 - sOrgMyIsoInfo->nMsgType%2;

	/* original request */
	for (nFldNbr = 0; nFldNbr < 128; nFldNbr ++)
		if (GetIsoField (nFldNbr, sOrgMyIsoInfo, szWork, &nLen) == SUCCESS)
			AddIsoField (nFldNbr, sNewMyIsoInfo, szWork,  nLen);

	/* response code & author id */
	if (GetUaeSwitchField(UAESWITCH_RESPONSE_CODE, sNwInfo, szUaeRespCode, &nLen) == SUCCES )
	{
		TranslateRespCode (nIndexCtx, G_sUaeSwitch.F_szRsrcId, szUaeRespCode, "RP", szMyRespCode);
		if(PutIsoField    (ISO_RESPONSE_CODE, sNewMyIsoInfo, szMyRespCode, 3) != SUCCESS)
			InsertIsoField (ISO_RESPONSE_CODE, sNewMyIsoInfo, szMyRespCode, 3);

		if ( (atoi(szMyRespCode) == 0 || atoi(szMyRespCode) == 4)
			&& GetUaeSwitchField  (UAESWITCH_AUTHOR_ID, sNwInfo, szWork, &nLen) == SUCCESS
			)
			if(PutIsoField    (ISO_AUTHOR_ID, sNewMyIsoInfo, szWork, 6) != SUCCESS)
				InsertIsoField (ISO_AUTHOR_ID, sNewMyIsoInfo, szWork, 6);
	}
	if (GetUaeSwitchField (UAESWITCH_ICC_DATA , sNwInfo, szWork, &nLen) == SUCCESS)
	{ 
		if(PutIsoField    (ISO_ICC_DATA, sNewMyIsoInfo, szWork, nLen) != SUCCESS)
			InsertIsoField (ISO_ICC_DATA, sNewMyIsoInfo, szWork, nLen);
	}
	/* additionnal amounts */
	if ( GetUaeSwitchField(UAESWITCH_REMAINING_OPEN_TO_USE, sNwInfo, szWork, &nLen) == SUCCES) {
		nLen = ToIsoAdtnlAmnt(nLen, szWork, szIsoAdtnlAmnt);
		if (nLen > 0)
			if(PutIsoField    (ISO_ADTNL_AMOUNTS, sNewMyIsoInfo, szIsoAdtnlAmnt, nLen) != SUCCESS)
				InsertIsoField (ISO_ADTNL_AMOUNTS, sNewMyIsoInfo, szIsoAdtnlAmnt, nLen);
	}

	/* Field 061: ISO_NET_POS_DATA */
	if (strlen(szUaeRespCode) != 0) {
		TSTlvPrivate   tlvInfo;
		InitTlvPrivate(&tlvInfo);
		memset (szWork, 0, sizeof(szWork));
		InitTlvPrivate (&tlvInfo);

		PutTlvPrivate(RESPONSE_CODE_TAG, &tlvInfo, szUaeRespCode, 2);

		nLen = PrivateBuildTlv (szWork, &tlvInfo);
		if ( nLen > 0 )
			PutIsoField (ISO_NET_POS_DATA, sNewMyIsoInfo, szWork, nLen);
	}


	/* Settlement Date */
	if ( GetUaeSwitchField(UAESWITCH_STLMNT_DATE, sNwInfo, szWork, &nLen) == SUCCES) 
		PutIsoField    (ISO_STLMNT_DATE, sNewMyIsoInfo, szWork, nLen);

	dump_p7_struct(sNewMyIsoInfo);

	trace_event("End BuildRepFromNw()",PROCESSING);
	return(OK);

}

/***********************************************************************
**  name      : ToIsoProcCode()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToIsoProcCode(char * szNwProcCode, char * szIsoProcCode)
{
	char szFuncName[] = "ToIsoProcCode", sLine[MAX_LINE_TRC];
	int  nCntr, nStatus;

	START_TRACE_PAR1(szFuncName, "%.6s", szNwProcCode);

	memcpy(szIsoProcCode, "XX0000", 6);

	/* trans type */
	for (nCntr = 0; nCntr < sizeof(sTransType)/sizeof(TSTransType); nCntr++)
		if (memcmp(sTransType[nCntr].szNwTransType, szNwProcCode, 2) == 0)
		{
			memcpy(szIsoProcCode, sTransType[nCntr].szIsoTransType, 2);
			break;
		}

		/* from accnt type */
		for (nCntr = 0; nCntr < sizeof(sAccntType)/sizeof(TSAccntType); nCntr++)
			if (memcmp(sAccntType[nCntr].szNwAccntType, szNwProcCode + 2, 2) == 0)
			{
				memcpy(szIsoProcCode + 2, sAccntType[nCntr].szIsoAccntType, 2);
				break;
			}

			/* to accnt type */
			for (nCntr = 0; nCntr < sizeof(sAccntType)/sizeof(TSAccntType); nCntr++)
				if (memcmp(sAccntType[nCntr].szNwAccntType, szNwProcCode + 4, 2) == 0)
				{
					memcpy(szIsoProcCode + 4, sAccntType[nCntr].szIsoAccntType, 2);
					break;
				}

				return_INT_PAR1(szFuncName, 6, "%.6s", szIsoProcCode);
}

/***********************************************************************
**  name      : ToIsoFuncCode()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToIsoFuncCode(int nNwMsgType, char * szNwProcCode, char * szIsoFuncCode)
{
	char szFuncName[] = "ToIsoFuncCode", sLine[MAX_LINE_TRC];
	int  nCntr, nStatus;

	START_TRACE_PAR2(szFuncName, "%d, %.6s", nNwMsgType, szNwProcCode);

	memcpy(szIsoFuncCode, "XXX", 3);

	/* func code*/
	for (nCntr = 0; nCntr < sizeof(sTransType)/sizeof(TSTransType); nCntr++)
		if ( sTransType[nCntr].nNwMsgType ==  nNwMsgType
			&& memcmp(sTransType[nCntr].szNwTransType, szNwProcCode, 2) == 0
			)
		{
			memcpy(szIsoFuncCode, sTransType[nCntr].szIsoFuncCode, 3);
			break;
		}

		return_INT_PAR1(szFuncName, 3, "%.3s", szIsoFuncCode);
}

/***********************************************************************
**  name      : ToNwAdtnlAmnt()
**  desc      : convert the PowerSEVEN field ADDITIONA AMOUNTS to
**              the Network field UAESWITCH_REMAINING_OPEN_TO_USE
**  in        : nLen          : length of PowerSEVEN field ADDITIONA AMOUNTS
**              szIsoAdtnlAmnt: the PowerSEVEN field ADDITIONA AMOUNTS
**  out       : szNwAdtnlAmnt : the Network field UAESWITCH_REMAINING_OPEN_TO_USE
**  return    : OK         : length of the Network field UAESWITCH_REMAINING_OPEN_TO_USE
***********************************************************************/
static int ToNwAdtnlAmnt_old(
	int   nLen,
	char *szIsoAdtnlAmnt,
	char *szNwAdtnlAmnt)
{
	char         sLine[MAX_LINE_TRC];
	int          nCrntBalPresence = NOK; 
	int          nAvailBalPresence = NOK;
	int          nOffset;
	int          nStatus;

	trace_event("Start ToNwAdtnlAmnt()",PROCESSING);

	for (nOffset = 0; nOffset < nLen; nOffset+=20) 
	{
		if (memcmp(szIsoAdtnlAmnt + nOffset + 2, "02", 2) == 0) 
		{
			nCrntBalPresence    = OK;
			szNwAdtnlAmnt[1]    = szIsoAdtnlAmnt[nOffset + (2+2+3)] == 'D' ? '-' : '0';
			memcpy(szNwAdtnlAmnt + (1+1), szIsoAdtnlAmnt + nOffset + (2+2+3+1+1), 11);
		}  
		if (memcmp(szIsoAdtnlAmnt + nOffset + 2, "01", 2) == 0) 
		{
			nAvailBalPresence   = OK;
			szNwAdtnlAmnt[1+12] = szIsoAdtnlAmnt[nOffset + (2+2+3)] == 'D' ? '-' : '0';
			memcpy(szNwAdtnlAmnt + (1+12+1), szIsoAdtnlAmnt + nOffset + (2+2+3+1+1), 11);
		}
	}

	if (nCrntBalPresence == OK && nAvailBalPresence == OK)
		szNwAdtnlAmnt[0] = '4'; /* both balances are presence, use available balance */
	else if (nCrntBalPresence  == OK )
		szNwAdtnlAmnt[0] = '2';  /* only current balance is present */
	else if (nAvailBalPresence == OK )
		szNwAdtnlAmnt[0] = '2';  /* only available balance is present */
	else
		szNwAdtnlAmnt[0] = '3';  /* any value that is not specified by the network */



	replace(szNwAdtnlAmnt,' ','0',25);

	trace_event("End   ToNwAdtnlAmnt(OK)",PROCESSING);
	return (OK); 

}
static int ToNwAdtnlAmnt(
	int   nLen,
	char *szIsoAdtnlAmnt,
	char *szNwAdtnlAmnt)
{
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToNwAdtnlAmnt";
	int          nCrntBalPresence = NOK, nAvailBalPresence = NOK;
	int          nOffset, nStatus;

	START_TRACE_PAR2(szFuncName, "%d, %.80s", nLen, szIsoAdtnlAmnt);

	for (nOffset = 0; nOffset < nLen; nOffset+=20) {
		if (memcmp(szIsoAdtnlAmnt + nOffset + 2, "02", 2) == 0) {
			nCrntBalPresence    = OK;
			szNwAdtnlAmnt[1]    = szIsoAdtnlAmnt[nOffset + (2+2+3)] == 'D' ? '-' : '0';
			memcpy(szNwAdtnlAmnt + (1+1+12), szIsoAdtnlAmnt + nOffset + (2+2+3+1+1), 11);
		}
		if (memcmp(szIsoAdtnlAmnt + nOffset + 2, "01", 2) == 0) {
			nAvailBalPresence   = OK;
			szNwAdtnlAmnt[1+12] = szIsoAdtnlAmnt[nOffset + (2+2+3)] == 'D' ? '-' : '0';
			memcpy(szNwAdtnlAmnt + (1+1), szIsoAdtnlAmnt + nOffset + (2+2+3+1+1), 11);
		}
	}

	if (nCrntBalPresence == OK && nAvailBalPresence == OK)
		szNwAdtnlAmnt[0] = '4'; /* both balances are presence, use available balance */
	else if (nCrntBalPresence  == OK )
		szNwAdtnlAmnt[0] = '2';  /* only current balance is present */
	else if (nAvailBalPresence == OK )
		szNwAdtnlAmnt[0] = '2';  /* only available balance is present */
	else
		szNwAdtnlAmnt[0] = '3';  /* any value that is not specified by the network */

	replace(szNwAdtnlAmnt,' ','0',25);

	trace_event("End   ToNwAdtnlAmnt(New OK)",TRACE);


	return_INT_PAR1(szFuncName, 25, "%.25s", szNwAdtnlAmnt);
}
/***********************************************************************
**  name      : ToIsoAdtnlAmnt()
**  desc      : convert the Network field UAESWITCH_REMAINING_OPEN_TO_USE to
**              the PowerSEVEN field ADDITIONA AMOUNTS
**  in        : nLen          : length of Network field UAESWITCH_REMAINING_OPEN_TO_USE
**              szNwAdtnlAmnt : the Network field UAESWITCH_REMAINING_OPEN_TO_USE
**  out       : szIsoAdtnlAmnt: the PowerSEVEN field ADDITIONA AMOUNTS
**  return    : OK         : length of PowerSEVEN field ADDITIONA AMOUNTS
**  author    : 
***********************************************************************/
static int ToIsoAdtnlAmnt(
	int   nLen,
	char *szNwAdtnlAmnt,
	char *szIsoAdtnlAmnt)
{
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToIsoAdtnlAmnt";
	int          nCrntBalPresence = NOK, nAvailBalPresence = NOK;
	int          nOffset, nStatus;

	START_TRACE_PAR2(szFuncName, "%d, %.25s", nLen, szNwAdtnlAmnt);
	if (memcmp (szNwAdtnlAmnt + 1 , "*", 1) == 0)
		return (0);

	nOffset = 0;

	/* current balance */
	if (szNwAdtnlAmnt[0] == '1' /* only current balance is present */
		|| szNwAdtnlAmnt[0] == '3' /* both balances arepresent, use current balance */
		|| szNwAdtnlAmnt[0] == '4' /* both balances arepresent, use available balance */
		){
			memcpy(szIsoAdtnlAmnt + nOffset, UAESWITCH_DFLT_ACCNT_TYPE, 2);
			nOffset+=2;
			memcpy(szIsoAdtnlAmnt + nOffset, BAL_CURRENT, 2);
			nOffset+=2;
			memcpy(szIsoAdtnlAmnt + nOffset, UAESWITCH_DFLT_CRNCY, 3);
			nOffset+=3;
			*(szIsoAdtnlAmnt + nOffset) = (szNwAdtnlAmnt[1] == '-' ? 'D' : 'C');
			nOffset+=1;
			memcpy(szIsoAdtnlAmnt + nOffset, "0", 1);
			nOffset+=1;
			memcpy(szIsoAdtnlAmnt + nOffset, szNwAdtnlAmnt + 1 + 1, 11);
			nOffset+=11;
	}

	/* available balance */
	if (szNwAdtnlAmnt[0] == '2' /* only available balance is present */
		|| szNwAdtnlAmnt[0] == '3' /* both balances arepresent, use current balance */
		|| szNwAdtnlAmnt[0] == '4' /* both balances arepresent, use available balance */
		){
			memcpy(szIsoAdtnlAmnt + nOffset, UAESWITCH_DFLT_ACCNT_TYPE, 2);
			nOffset+=2;
			memcpy(szIsoAdtnlAmnt + nOffset, BAL_AVAILABLE, 2);
			nOffset+=2;
			memcpy(szIsoAdtnlAmnt + nOffset, UAESWITCH_DFLT_CRNCY, 3);
			nOffset+=3;
			*(szIsoAdtnlAmnt + nOffset) = (szNwAdtnlAmnt[1+12] == '-' ? 'D' : 'C');
			nOffset+=1;
			memcpy(szIsoAdtnlAmnt + nOffset, "0", 1);
			nOffset+=1;
			memcpy(szIsoAdtnlAmnt + nOffset, szNwAdtnlAmnt + 1 + 12 + 1, 11);
			nOffset+=11;
	}

	return_INT_PAR1(szFuncName, nOffset, "%.40s", szIsoAdtnlAmnt);
}

/***********************************************************************
**  name      : ToIsoPosData()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
/* static int ToIsoPosData ( */
int ToIsoPosData (
	char *szMerchType,
	char *szNwPosEntryMode,
	char *szIsoPosData)
{
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToIsoPosData";
	int          nStatus;

	START_TRACE_PAR2(szFuncName, "%.4s, %.3s", szMerchType, szNwPosEntryMode);

	memset(szIsoPosData, ' ', 12);

	/*-----------------------------------------------------------------------*/
	/* SubField 1 : Card Data Entry  Capability                              */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szNwPosEntryMode, "02", 2) == 0)
		szIsoPosData[0] = '2'; /* Magnetic stripe read */
	else
		szIsoPosData[0] = '0'; /* Not specified */

	/*-----------------------------------------------------------------------*/
	/* SubField 2 : Cardholder Auth Capabilities                             */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szNwPosEntryMode + 2, "1", 1) == 0)
		szIsoPosData[1] = '1'; /* PIN */
	else
		szIsoPosData[1] = '0'; /* No authentication */

	/*-----------------------------------------------------------------------*/
	/* SubField 3 : Card Retention Capability                                */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[2] = '1'; /* YES */
	else
		szIsoPosData[2] = '9'; /* Unknown */

	/*-----------------------------------------------------------------------*/
	/* SubField 4 : Operating Environnement                                  */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[3] = '2'; /* On premises of card acceptor, Unattended */
	else
		szIsoPosData[3] = '9'; /* Unknown */

	/*-----------------------------------------------------------------------*/
	/* SubField 5 : Cardholder Presence   Indicator                          */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[4] = '0'; /* Cardholder present */
	else
		szIsoPosData[4] = '1'; /* Cardholder not present - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 6 : Card  Presence   Indicator                               */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[5] = '1'; /* Card present */
	else
		szIsoPosData[5] = '0'; /* Card not present - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 7 : Card Data Input Mode                                     */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szNwPosEntryMode , "8", 1) == 0)
		szIsoPosData[6] = '8'; /* Fall Back */
	else if (memcmp(szNwPosEntryMode, "05", 2) == 0)
		szIsoPosData[6] = '5'; /* FChip card read */
	else if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[6] = '2'; /* Magnetic stripe read ( track2 dominant ) */
	else
		szIsoPosData[6] = '0'; /* Unspecified - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 8 : Cardholder Authentication Method                         */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[7] = '1'; /* Magnetic stripe read ( track2 dominant ) */
	else
		szIsoPosData[7] = '0'; /* Not authenticated - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 9 : Cardholder Authentication Entity                         */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[8] = '2'; /* Card acceptor center or terminal */
	else
		szIsoPosData[8] = '0'; /* Not authenticated - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 10 : Rewrite Capability                                      */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szMerchType, "6011", 4) == 0)
		szIsoPosData[9] = '1'; /* None */
	else
		szIsoPosData[9] = '0'; /* Unspecified - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 11 : Terminal Output Capability                              */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szNwPosEntryMode + 2, "1", 1) == 0)
		szIsoPosData[10] = '4'; /* Both Display & Printing capabilities */
	else
		szIsoPosData[10] = '1'; /* Unspecified - assuemed */

	/*-----------------------------------------------------------------------*/
	/* SubField 12 : Pin Entry Capability                                    */
	/*-----------------------------------------------------------------------*/
	if (memcmp(szNwPosEntryMode + 2, "1", 1) == 0)
		szIsoPosData[11] = '4'; /* Four characters */
	else
		szIsoPosData[11] = '1'; /* Device PIN capture capability unknown */

	return_INT_PAR1(szFuncName, 12, "%.12s", szIsoPosData);
}


/***********************************************************************
**  name      : ToIsoAdtnlDataPriv()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToIsoAdtnlDataPriv (
	int              nIndexCtx,
	TSUaeSwitchInfo *sNwInfo,
	char            *szIsoAdtnlDataPriv)
{
	TSTlvPrivate sTlvInfo;
	char         szZpk[256];
	char         szWork2[256], szWork[256], sLine[MAX_LINE_TRC], szFuncName[] = "ToIsoAdtnlDataPriv";
	int          nKeyIndex;
	int          nLen, nStatus;

	START_TRACE(szFuncName);

	memset(szZpk, 0, sizeof(szZpk));
	memset(szIsoAdtnlDataPriv, 0, 64);

	InitTlvPrivate (&sTlvInfo);

	/* acquirer id */
	if (GetUaeSwitchField(UAESWITCH_ACQR_ID, sNwInfo, szWork, &nLen) == SUCCESS)
	{
		/* BEGIN TBE: 08-08-2007 */
		/* sprintf(szWork2, "%.6s", szWork); */
		/* END TBE: 08-08-2007 */

		sprintf(szWork2, "%06s", szWork);
		AddTlvPrivate (HPS_ACQUIRER_BANK_TAG, &sTlvInfo, szWork2, 6);
	}

	/* external audit number */
	if(GetUaeSwitchField(UAESWITCH_AUDIT_NBR, sNwInfo, szWork, &nLen)==SUCCESS)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &sTlvInfo, szWork, 6);

	/* encryption key */
	if ( GetUaeSwitchField (UAESWITCH_PIN_DATA,      sNwInfo, szWork,  &nLen) == SUCCESS
		&& GetUaeSwitchField (UAESWITCH_SECURITY_DATA, sNwInfo, szWork2, &nLen) == SUCCESS
		){
			nKeyIndex = szWork2[7] - '0';
			if (GetKey(nIndexCtx, G_sUaeSwitch.F_szRsrcId, IWK, &nKeyIndex, szZpk) == OK)
				/*AddTlvPrivate (ENCR_PIN_BLOCK_KEY_TAG, &sTlvInfo, szZpk , 16);*/
				AddTlvPrivate (ENCR_PIN_BLOCK_KEY_TAG, &sTlvInfo, szZpk , strlen(szZpk));/*OBE14082009*/
	}

	nLen = PrivateBuildTlv (szIsoAdtnlDataPriv, &sTlvInfo);

	return_INT_PAR1(szFuncName, nLen, "%.64s", szIsoAdtnlDataPriv);
}


/***********************************************************************
**  name      : ToIsoScrtyData()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToIsoScrtyData (
	char *szNwScrtyData,
	char *szIsoScrtyData)
{
	char         sPid[10];
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToIsoScrtyData";
	int          nStatus;

	START_TRACE_PAR1(szFuncName, "%.16s", szNwScrtyData);

	memset(szIsoScrtyData, 0, 64);

	sprintf(sPid, "%08d", getpid());


	PutSecurityData(SECURITY_FORMAT,          szIsoScrtyData,  "02",                   2);
	PutSecurityData(PIN_BLOCK_FORMAT,         szIsoScrtyData,  "01",                   2);
	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX, szIsoScrtyData, "000",                   3);
	PutSecurityData(MAC_KEY_INDEX,            szIsoScrtyData, "000",                   3);
	PutSecurityData(SOURCE_PID,               szIsoScrtyData, sPid,                    8);
	PutSecurityData(SOURCE_RESOURCE,          szIsoScrtyData, G_sUaeSwitch.F_szRsrcId, 6);
	PutSecurityData(DESTINATION_PID,          szIsoScrtyData, "00000000",              8);
	PutSecurityData(DESTINATION_RESOURCE,     szIsoScrtyData, "000000",                6);
	PutSecurityData(USER,                     szIsoScrtyData, "UAESWITCH      ", 15);
	PutSecurityData(PURGE_TIME,               szIsoScrtyData, "000000000",             9);

	return_INT_PAR1(szFuncName, 58, "%.64s", szIsoScrtyData);
}

/***********************************************************************
**  name      : ToNwOrgnlData()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToNwOrgnlData (
	char *szIsoOrgnlData,
	char *szNwOrgnlData)
{
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToNwOrgnlData";
	int          nOffset, nStatus;
	char         szWork[256];

	START_TRACE_PAR1(szFuncName, "%.42s", szIsoOrgnlData);

	nOffset = 0;
	memset(szNwOrgnlData, '0', 42);

	/* original message type */
	memcpy(szNwOrgnlData + nOffset, "0200", 4);
	nOffset += 4;

	/* original trace audit number */
	memcpy(szNwOrgnlData + nOffset, szIsoOrgnlData + 4, 6);
	nOffset += 6;

	/* original trasmission date&time */
	memcpy(szNwOrgnlData + nOffset, szIsoOrgnlData + 12, 10);
	nOffset += 10;

	/* original acquirer id */
	/*
	memcpy(szNwOrgnlData + nOffset, szIsoOrgnlData + 22, 11);
	nOffset += 11;
	*/
	memset(szWork, 0, sizeof(szWork));
	memset(szWork, '0', 11);
	memcpy(szWork + (11 - strlen(UAESWITCH_INST_ID)) , UAESWITCH_INST_ID , strlen(UAESWITCH_INST_ID));
	sprintf(sLine,"SZWORK is %s", szWork);
	trace_event(sLine,TRACE);
	sprintf(sLine,"UAEID  is %s", UAESWITCH_INST_ID);
	trace_event(sLine,TRACE);
	memcpy(szNwOrgnlData + nOffset, szWork , 11);
	nOffset += 11;

	/* original forwarding id */
	memcpy(szNwOrgnlData + nOffset,"00000000000", 11);
	nOffset += 11;

	return_INT_PAR1(szFuncName, 42, "%.42s", szNwOrgnlData);
}


/***********************************************************************
**  name      : ToIsoOrgnlData()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/
static int ToIsoOrgnlData (
	char *szNwOrgnlData,
	char *szTransDate,
	char *szIsoOrgnlData)
{
	char         sLine[MAX_LINE_TRC], szFuncName[] = "ToIsoOrgnlData";
	int          nOffset, nStatus;

	START_TRACE_PAR2(szFuncName, "%.42s, %.12s", szNwOrgnlData, szTransDate);

	nOffset = 0;
	memset(szIsoOrgnlData, ' ', 35);

	/* Original Message Type */
	memcpy(szIsoOrgnlData + nOffset, "1",           1);
	memcpy(szIsoOrgnlData + nOffset, szNwOrgnlData, 4);
	nOffset += 4 ;

	/* Original Stan */
	memcpy(szIsoOrgnlData + nOffset, szNwOrgnlData + 4, 6);
	nOffset += 6;

	/* orgnl trans local date */
	memcpy(szIsoOrgnlData + nOffset,     szTransDate ,        2);
	memcpy(szIsoOrgnlData + nOffset + 2, szNwOrgnlData + 10, 10);
	nOffset += 12;

	/* Original Acquirer Id */
	memcpy (szIsoOrgnlData + nOffset, szNwOrgnlData + 20, 11);
	nOffset += 11;

	return_INT_PAR1(szFuncName, nOffset, "%.35s", szIsoOrgnlData);
}

/***********************************************************************
**  name      : GetTableInd()
**  desc      :
**  in        :
**  out       :
**  return    :
**  author    : 
***********************************************************************/

int GetTableInd( TSIsoInfo *IsoInfo, char *sAutTabInd)
{

	char            slProcCode         [ 6 + 1 ] ;
	char            sMerchActv         [ 4 + 1 ] ;
	int             nLength                      ;
	char            sLine       [ MAX_LINE_TRC ] ;


	trace_event("Start GetTableInd()", PROCESSING);

	memset( sAutTabInd, 0, sizeof(sAutTabInd));

	GetIsoField( ISO_PROC_CODE       , IsoInfo , slProcCode      , &nLength );


	GetIsoField( ISO_MERCHANT_TYPE   , IsoInfo , sMerchActv      , &nLength ) ;


	sprintf(sLine, "slProcCode  = [%s],sMerchActv  = [%s]", slProcCode,sMerchActv);
	trace_event(sLine, PROCESSING);

	memcpy(sAutTabInd, "ASM", 3); /** For Pos Trx **/
	if( !memcmp(sMerchActv, "6011", 4) || !strlen(sMerchActv) )
	{
		memcpy(sAutTabInd, "ASM", 3);
		if( memcmp(slProcCode, "17", 2) && memcmp(slProcCode, "01", 2) )
			memcpy(sAutTabInd, "MSC", 3);
	}

	sprintf(sLine, "End GetTableInd(%s)", sAutTabInd);
	trace_event(sLine, PROCESSING);
	return (OK);

}


/*****************************************************************************************/
/** load_uaeswitch_add_data                                                                  **/
/*****************************************************************************************/
int     load_uaeswitch_add_data(pTarget, pMCC)
	char    *pTarget;
char    *pMCC;
{
	int     lg;
	int     offset;
	char    sLine [ MAX_LINE_TRC ];

	sprintf(sLine, "Start load_uaeswitch_add_data(%s)", pMCC);
	trace_event(sLine, PROCESSING);

	if ( !memcmp(pMCC, "6011", 4) )  /** ATM Trx **/
	{
		offset = 0;

		memcpy(pTarget  + offset, "000000000000000000000000", 24);
		offset += 24;
		memcpy(pTarget  + offset, "0",                         1);
		offset += 1;
		memcpy(pTarget  + offset, "00",                        2);
		offset += 2;
		memcpy(pTarget  + offset, "000",                       3);
		offset += 3;
		memcpy(pTarget  + offset, "000",                       3);
		offset += 3;
		memcpy(pTarget  + offset, "00000000000",              11);
		offset += 11;

	}
	else
		if ( strlen(pMCC) ) /** Pos Trx **/
		{
			offset = 0;

			memcpy(pTarget + offset, "0000000000000000000", 19);
			offset += 19;
			memcpy(pTarget + offset, "0000", 4);
			offset += 4;
			memcpy(pTarget + offset, "0000", 4);
			offset += 4;
		}
		else /** Net Mng **/
		{
			offset = 0;

			memcpy(pTarget  + offset, "01",       2);
			offset += 2;
			memcpy(pTarget  + offset, "1",        1);
			offset += 1;
			memcpy(pTarget + offset,  "1",        1);
			offset += 1;
			memcpy(pTarget + offset,  "0",        1);
			offset += 1;
			memcpy(pTarget + offset,  "0",        1);
			offset += 1;
			memcpy(pTarget + offset,  "2",        1);
			offset += 1;
			memcpy(pTarget + offset,  "1",        1);
			offset += 1;

			memcpy(pTarget + offset, "11N00634",  8);
			offset += 8;

		}

		sprintf(sLine, "End   load_uaeswitch_add_data(%d)", offset);
		trace_event(sLine, PROCESSING);
		return(offset);
}

