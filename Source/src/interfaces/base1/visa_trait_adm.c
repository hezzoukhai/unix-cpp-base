#include <stdio.h>
#include <define.h>
#include <p7_sid.h>
#include <queue.h>
#include <define.h>
#include <queue.h>
#include <iso_hps.h>
#include <tlv_private.h>

void 
AdmProcMsg(int nIndexCtx, TSBalExchange * sBalMsg)
{
	TSIsoInfo       IsoInfo;
	char            sLine[MAX_LINE_TRC];
	char            FieldData[256];
	int             FieldLen;
	char			szFieldData[1024];
    int			    nFieldLen;
    int 			periode;
   TSTlvPrivate	TlvPrivateInfo;

	InitIsoInfo(&IsoInfo);
	AnalyseIso(sBalMsg->sBody, &IsoInfo);
	InitTlvPrivate(&TlvPrivateInfo);
   
    memset(szFieldData,'\0',sizeof(szFieldData));

	trace_event("Service Message Received ", PROCESSING);
	dump_p7_struct(&IsoInfo);

	switch (IsoInfo.nMsgType) {
	case 1606:		/* Service Message */
		if( GetIsoField(ISO_FUNCTION_CODE, &IsoInfo, FieldData, &FieldLen) != SUCCESS )
		{
			trace_event ( "Function code missing" , PROCESSING );
			return;
		}
		switch (atoi(FieldData)) {
		case P7_F024_N_STOP_INTERFACE:	/* Stop interface */
			FCT_STOP_INTERFACE(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_PARAM_REFRESH:	/* Refresh parameters */
			FCT_REFRESH_PARAMS(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_PING_RESOURCE:	/* Indicate resource status */
			FCT_REPORT_STATUS(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_STATISTICS:	/* Stats */
			FCT_STATISTICS(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_SIGNON_BIN:	/* signon */
		case P7_F024_N_SIGNOFF_BIN:	/* signoff */
		case P7_F024_N_START_SAF:	/* start SAF */
		case P7_F024_N_END_SAF:	/* end SAF */
		case P7_F024_N_ECHO_TEST:	/* echo test */
			FCT_NW_MNG_REQ_TO_NW(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_LINE_DOWN_IND:	/* Line Down  */
			FCT_LINE_DOWN(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_LINE_UP_IND:	/* Line Up  */
			FCT_LINE_UP(nIndexCtx, &IsoInfo);
			break;

		case P7_F024_N_KEY_REQUEST: /* Key Request  FZL20220905 PLUTONL-4979*/
		    FCT_KEY_REQUEST(nIndexCtx, &IsoInfo, 'I');
		    FCT_KEY_REQUEST(nIndexCtx, &IsoInfo, 'A');
		    break;

		case p7_F024_N_DISABLE_MODE_PCI: /* change mode PCI for a periode */
			if( GetIsoField(ISO_ADTNL_DATA_PRIV, &IsoInfo, szFieldData, &nFieldLen) == SUCCESS )
			{
				AnalyseTlvPrivate(szFieldData,nFieldLen,&TlvPrivateInfo);
			}
			if( GetTlvPrivate(HPS_DURATION_TAG,&TlvPrivateInfo,szFieldData,&nFieldLen) == SUCCESS )
			{
				periode=atoi(szFieldData);
				disable_pci(periode);
			}
			break;

		default:	/* Network Management Message */
			sprintf(sLine, "Function Code %.3s not supported by Base1", FieldData);
			trace_event(sLine, PROCESSING);
			break;
		}
	}
}