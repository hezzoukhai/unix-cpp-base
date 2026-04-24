#ifndef P7_MSG_LAYOUT__H
#define P7_MSG_LAYOUT__H


typedef enum {

	FP_UNDEF			=0	,
	FP_MANDATORY			,
	FP_CONDITIONAL			,
	FP_OPTIONAL				,
	FP_FORBIDDEN			,

	FP_QTY
} E_FIELD_PRESENCE;

typedef enum {

	FO_UNDEF			=0	,
	FO_REQ_MSG				,
	FO_INIT_REQ_MSG			,
	FO_INIT_REP_MSG			,
	FO_GENERATED			,

	FO_QTY
} E_FIELD_ORIGIN;


#define FIELD_PRESENCE(c)		((c)&0x0F)
#define FIELD_ORIGIN(c)			(((c)&0xF0)>>4)
#define FIELD_MAP(p,o)			(((o)<<4)|(p))

#define P7_B_UN					FIELD_MAP(FP_UNDEF,FO_UNDEF)
#define P7_B_Z					FIELD_MAP(FP_FORBIDDEN,FO_UNDEF)

#define P7_B_X					FIELD_MAP(FP_MANDATORY,FO_GENERATED)
#define P7_B_XQ					FIELD_MAP(FP_MANDATORY,FO_REQ_MSG)
#define P7_B_XQI				FIELD_MAP(FP_MANDATORY,FO_INIT_REQ_MSG)
#define P7_B_XRI				FIELD_MAP(FP_MANDATORY,FO_INIT_REP_MSG)

#define P7_B_C					FIELD_MAP(FP_CONDITIONAL,FO_GENERATED)
#define P7_B_CQ					FIELD_MAP(FP_CONDITIONAL,FO_REQ_MSG)
#define P7_B_CQI				FIELD_MAP(FP_CONDITIONAL,FO_INIT_REQ_MSG)
#define P7_B_CRI				FIELD_MAP(FP_CONDITIONAL,FO_INIT_REP_MSG)

#define P7_B_O					FIELD_MAP(FP_OPTIONAL,FO_GENERATED)
#define P7_B_OQ					FIELD_MAP(FP_OPTIONAL,FO_REQ_MSG)
#define P7_B_OQI				FIELD_MAP(FP_OPTIONAL,FO_INIT_REQ_MSG)
#define P7_B_ORI				FIELD_MAP(FP_OPTIONAL,FO_INIT_REP_MSG)


typedef struct {
	int		nFieldNo;
	char	presence_info;
	int		nConditionNo;
} TSFieldPresence;



typedef unsigned int msg_layout_id_t;


typedef struct 
{
	TSFieldPresence		tab_msgFields[MAX_STRUCT_FIELDS];
	int					nProtocolId;
	char				szMsgType[MAX_MSG_TYPE_LEN + 1];
	char				szName;
	msg_layout_id_t		id;
} TSMsgLayout;

extern void InitMsgLayout(TSMsgLayout* pkMsgLayout);
extern int	RegisterMsgLayout(const char* szName, TSMsgLayout* pkMsgLayout);
extern int	GetMsgLayout(const char* szName);
extern int	GetFieldLayout(msg_layout_id_t msgLayoutId, int nFieldNo, int nStartSearchPos, int* nFieldIndex, const TSFieldPresence** pkFieldPresence);
extern int	GetLayoutSize(msg_layout_id_t msgLayoutId) { return MAX_STRUCT_FIELDS; }
/*extern const TSFieldInfo*	GetFieldLayoutByIndex(msg_layout_id_t msgLayoutId, int nFieldIndex);*/



#endif

