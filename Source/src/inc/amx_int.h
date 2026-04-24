#ifndef AMX__INT__H
#define AMX__INT__H


extern char		gInstId[];
extern char		gAcquirerId[];
extern char		gAmexId[];

#define 	AMEX_GNS_AMEX_ID                   "A01"
#define 	AMEX_GNS_ACQUIRER_ID               "A02"
#define 	AMEX_GNS_INSTITUTIONAL_ID          "A03"


int AmxToIso(int nIndexCtx,MsgInfo* pkMsgInfo, TSIsoInfo* pkIsoInfo, TSIsoInfo* pkOrigIsoInfo);
int IsoToAmx(		int				nIndexCtx, 
					MsgInfo			*pkMsgInfo, 
					TSIsoInfo*		pkIsoInfo, 
					MsgInfo			*pkOrigMsgInfo,
					int				nReqRespFlag
					);

int Int1UserNwMsgFromIso(int nIndexCtx,TSIsoInfo* pkIsoInfo, MsgInfo* pkMsgInfo, MsgInfo* pkOrigMsgInfo);
int BuildAmxResp(int nIndexCtx,MsgInfo	*pkMsgInfo,TSIsoInfo *pkIsoInfo, MsgInfo *pkOrigMsgInfo);
int BuildIsoResp(int nIndexCtx, MsgInfo* pkMsgInfo, TSIsoInfo* pkIsoInfo, TSIsoInfo* OrigIsoInfo);
int AmxReplyToLine(
				int				nIndexCtx,
				TSIsoInfo		*pkIsoInfo,
				MsgInfo			*pkOrigMsgInfo,
				char			*sTlvBuffer
				);


#endif

