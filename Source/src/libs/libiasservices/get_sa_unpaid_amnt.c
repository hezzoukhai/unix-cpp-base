#include <define.h>
#include <tlv_autho.h>
#include <br_cmd_fields.h>
#include <ias_services.h>

int IASGetSAUnpaidAmnt (int		nIndexCtx, 
                        char*	sAccountNumber,
                        char*	pAmount,
						char*	pCurrency)
{

	char            sLine[MAX_LINE_TRC];
	char			sBufferSnd[BUF_LG_MAX];
	char			sBufferRcv[BUF_LG_MAX];
	char			sResult[BRCMD_RES_LEN + 1];
	char			sRequestorId[24];
	char			sEvent[12];
	char			sData[512];
	int				nLength;
	int				nResult;
	int				i;
	TSTlvBrCmd		kTlvBrCmd;


	trace_event("Start  IASGetSAUnpaidAmnt()", PROCESSING);
	
	InitTlvBrCmd(&kTlvBrCmd);
	
	PutTlvBrCmd   ( BRCMD_ACCOUNT_ID_1 , &kTlvBrCmd, sAccountNumber , strlen (sAccountNumber));
                 
	PutTlvBrCmd   ( BRCMD_COMMAND , &kTlvBrCmd, GET_SA_UNPAID_AMOUNT , IAS_SERVICE_LEN);
	
	/*
	sprintf(sRequestorId,"%0.8d",p7_thread_id());
	PutTlvBrCmd   ( BRCMD_REQUESTOR_ID , &kTlvBrCmd, sRequestorId , strlen(sRequestorId));
	*/

	nLength = BrCmdBuildTlv(sBufferSnd, &kTlvBrCmd);

	nResult = BridgeIASQuery(nIndexCtx,sBufferSnd,sBufferRcv);
	if( nResult != OK )
	{
		trace_event("End    IASGetSAUnpaidAmnt(NOK): ERROR on Get SA unpaid amount", ERROR);
		return NOK;
	}
	

	AnalyseTlvBrCmd(sBufferRcv,&kTlvBrCmd);
	GetTlvBrCmd( BRCMD_RESULT , &kTlvBrCmd, sResult , &nLength);
	if( memcmp(sResult,BRCMD_RES_OK,BRCMD_RES_LEN) != 0 )
	{
		GetTlvBrCmd( BRCMD_EVENT_CODE , &kTlvBrCmd, sEvent , &nLength);
		GetTlvBrCmd( BRCMD_MESSAGE , &kTlvBrCmd, sData , &nLength);
		sprintf(sLine,">>> Err: [%s,%s,%s]", sResult,sEvent,sData);
		trace_event(sLine, ERROR);
		trace_event("End    IASGetSAUnpaidAmnt(NOK)", ERROR);
		return (NOK);

	}

	if ( GetTlvBrCmd( BRCMD_ADD_AMOUNTS , &kTlvBrCmd, sBufferSnd , &nLength) != SUCCES )
	{
		trace_event("End    IASGetSAUnpaidAmnt(NOK): Missing SA unpaid amount", ERROR);
		return (NOK);
	}
               
	for(i=0; i < nLength; i+=20 )
	{
		if( memcmp(sBufferSnd + i + 2, "04",2) == 0 )
		{
			strncpy(pAmount,sBufferSnd + i + 2 + 2 + 3 + 1,12);
			pAmount[12]='\0';
			strncpy(pCurrency,sBufferSnd + i + 2 + 2,3);
			pCurrency[3]='\0';
			trace_event("End  IASGetSAUnpaidAmnt(OK)", PROCESSING);
			return OK;
		}
	}

	trace_event("End  IASGetSAUnpaidAmnt(NOK): Missing due amount", PROCESSING);
	return (NOK);
}
