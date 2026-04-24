#include <stdio.h>
#include <iso_hps.h>
#include <list_thr.h>
#include <tlv_autho.h>

#include <int1_struct.h>
#include <int1.h>


int Int1BalProcMsg(int nIndexCtx, TSBalExchange * sBalMsg)
{
	TSIsoInfo       IsoInfo;
	TSTlvAutho		sTlvInfo;
	char            FieldData[256];
	int             FieldLen;
	int             result;


	InitIsoInfo(&IsoInfo);

	if( sBalMsg->sHeader.nCommand != 4100 )
	{
		AnalyseIso(sBalMsg->sBody, &IsoInfo);
	}
	else
	{
		InitTlvAutho    ( &sTlvInfo );
		result = AnalyseTlvAutho ( sBalMsg->sBody , &sTlvInfo );
		result = SafTlvToIso        ( nIndexCtx , &sTlvInfo   , &IsoInfo );
	}

	

	IsoInfo.msgId = sBalMsg->sHeader.msgId;
	dump_p7_struct(&IsoInfo);

	switch (IsoInfo.nMsgType) {
	case 1100:
	case 1200:
	case 1101:
	case 1201:
		if (ResourceStatusSignOn(ResourceStatus) != OK) 
		{
			IsoInfo.nMsgType = 1110;
			ReplyIsoMsgToBal(nBalMsg, &IsoInfo, 0, P7_F039_ISS_UNV);
			MsgFwdRepReplied(sBalMsg->sHeader.msgId);
		} 
		else
			INT1_FCT_AUT_REQ_TO_NW(nIndexCtx, &IsoInfo);
		break;

	case 1110:
	case 1210:
		INT1_FCT_AUT_REP_TO_NW(nIndexCtx, &IsoInfo);
		break;
	case 1420:
	case 1421:
		INT1_FCT_REV_REQ_TO_NW(nIndexCtx,  &IsoInfo, sBalMsg);
		break;

	case 1606:		/* Service Message */
		if( GetIsoField(ISO_FUNCTION_CODE, &IsoInfo, FieldData, &FieldLen) != SUCCESS )
		{
			trace_event ( "Function code missing" , PROCESSING );
			return;
		}
		switch (atoi(FieldData)) {
		case P7_F024_N_STOP_INTERFACE:	/* Stop interface */
			break;
		case P7_F024_N_PARAM_REFRESH:	/* Refresh parameters */
			break;
		case P7_F024_N_PING_RESOURCE:	/* Indicate resource status */
			break;
		case P7_F024_N_STATISTICS:	/* Stats */
			break;
		default:	/* Network Management Message */
			INT1_FCT_NW_MNG_REQ_TO_NW(nIndexCtx, &IsoInfo);
			break;
		}
	case 1700:		/* ShutDown Message To Line Threads */
		result = INT1_FCT_SHUTDOWN_BAL(nIndexCtx);
		break;

	}

	return OK;
}
