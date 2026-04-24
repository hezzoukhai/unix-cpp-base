#ifndef LINK__INT__H
#define LINK__INT__H


extern char		gInstId[];
extern char		gAcquirerId[];
extern char		gLinkId[];

#define 	LINK_ID                		   "L01"
#define 	LINK_ACQUIRER_ID               "L02"
#define 	LINK_INSTITUTIONAL_ID          "L03"



int LinkToIso(int nIndexCtx,MsgInfo* pkMsgInfo, TSIsoInfo* pkIsoInfo, TSIsoInfo* pkOrigIsoInfo);
int IsoToLink(		int				nIndexCtx,
					MsgInfo			*pkMsgInfo, 
					TSIsoInfo*		pkIsoInfo, 
					MsgInfo			*pkOrigMsgInfo,
					int				nReqRespFlag
					);

int Int1UserNwMsgFromIso(int nIndexCtx,TSIsoInfo* pkIsoInfo, MsgInfo* pkMsgInfo, MsgInfo* pkOrigMsgInfo);
int BuildLinkResp(int nIndexCtx,MsgInfo	*pkMsgInfo,TSIsoInfo *pkIsoInfo, MsgInfo *pkOrigMsgInfo);
int LinkReplyToLine(
				int				nIndexCtx,
				TSIsoInfo		*pkIsoInfo,
				MsgInfo			*pkOrigMsgInfo,
				char			*sTlvBuffer
				);
void * thread_signin(void * arg);


#endif

