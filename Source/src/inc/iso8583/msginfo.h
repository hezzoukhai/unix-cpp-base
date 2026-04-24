#ifndef P7_MSG_INFO_H
#define P7_MSG_INFO_H

typedef enum {
	E_FIXED_LENGTH	=0,

	HDR_TYPE_QTY
} E_HDR_TYPE;

typedef struct {
	E_FIELD_FORMAT		eMsgTypeType;
	int					nMsgTypeLen;
	E_ALPHA_FORMAT		eMsgTypeAlphaFormat;
	char				szProtocolName[MAX_PROTOCOL_NAME_LEN + 1];
	char				szProtocolPrefix[MAX_PROTOCOL_PREFIX_LEN + 1];
	BitMappedProperties kDataPorperties;
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
	int					nProtocolId;
	E_HDR_TYPE			eHeaderType;
	E_FIELD_FORMAT		eHeaderFormat;
	int					nHeaderLen;
} MsgInfoProperties;


typedef struct  {
	BitMappedInfo		kData;
	int					nHeaderLen;
	char				sHeader		[ MAX_MSG_HEADER_LEN + 1];
	int					nMsgType;
	
	const MsgInfoProperties*		pkProperties;
} MsgInfo;





extern void				InitNetworkMsgInfo(MsgInfo* kMsgInfo);
extern void				InitSwitchMsgInfo(MsgInfo* kMsgInfo);
extern void				InitMsgInfoFromProp(const char* szPropertiesName, MsgInfo* kMsgInfo);
extern int				AnalyseMsg(const char* buffer, MsgInfo* kMsgInfo);
P7_INLINE extern int	GetMsgField(int nFieldNo, const MsgInfo* kMsgInfo, char *data, int* nLength);
P7_INLINE extern int	AddMsgField(int nFieldNo, MsgInfo* kMsgInfo, const char *data, int nLength);
P7_INLINE extern int	PutMsgField(int nFieldNo, MsgInfo* kMsgInfo, const char *data, int nLength);
/*extern int				RemoveMsgField(int nFieldNo, MsgInfo* kMsgInfo);*/
extern int				BuildMsgInfoBuffer (char * buffer, const MsgInfo* kMsgInfo);
extern void				DumpMsgInfo(const MsgInfo* kMsgInfo);


extern void InitMsgInfoProperties(MsgInfoProperties* kMsgInfoProperties);


/* ISO Message  type identifiers */
#define MSG_TYPE_VERSION				0
#define MSG_TYPE_CLASS					1
#define MSG_TYPE_FUNCTION				2
#define MSG_TYPE_TRX_ORIGIN				3


#define MSG_TYPE_P2_AUT					'1'			/* authorization				*/
#define MSG_TYPE_P2_FIN					'2'			/* financial					*/
#define MSG_TYPE_P2_FILE				'3'			/* file action					*/
#define MSG_TYPE_P2_REV					'4'			/* reversal/chargeback			*/
#define MSG_TYPE_P2_REC					'5'			/* reconciliation				*/
#define MSG_TYPE_P2_ADM					'6'			/* administrative				*/
#define MSG_TYPE_P2_FEE					'7'			/* fee collection				*/
#define MSG_TYPE_P2_NET					'8'			/* network management			*/


#define MSG_TYPE_P3_REQ					'0'
#define MSG_TYPE_P3_REQ_RESP			'1'
#define MSG_TYPE_P3_ADV					'2'
#define MSG_TYPE_P3_ADV_RESP			'3'
#define MSG_TYPE_P3_NOTIF				'4'

#define MSG_TYPE_P4_ACQ					'0'
#define MSG_TYPE_P4_ACQ_REP				'1'
#define MSG_TYPE_P4_ISS					'2'
#define MSG_TYPE_P4_ISS_REP				'3'
#define MSG_TYPE_P4_OTHER				'4'
#define MSG_TYPE_P4_OTHER_REP			'5'
/**********************************************************/

/**********************************************************/

#endif

