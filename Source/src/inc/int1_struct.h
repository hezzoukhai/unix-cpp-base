#ifndef P7_INT1_STRUCT___H
#define P7_INT1_STRUCT___H

#define MSG_TYPE_UNDEF			0
#define MSG_TYPE_REQ			1
#define MSG_TYPE_RESP			2



#define RESP_MSG_TYPE(m)	((m)&0xFFFFFFFE)+10
#define ORIG_MSG_TYPE(m)	((m)&0xFFFFFFFE)
#define REQ_MSG_TYPE(m)		(m)-10


/* Field presence flags */
#define FP_UNDEF				0
#define FP_MANDATORY			1
#define FP_CONDITIONAL			2
#define FP_OPTIONAL				3
#define FP_FORBIDDEN			4

/* Field origin flags */
#define FO_UNDEF				0
#define FO_REQ_MSG				1
#define FO_INIT_REQ_MSG			3
#define FO_INIT_REP_MSG			4
#define FO_GENERATED			5

/* macros to retreive presence/origin and to build a field indicator from presence/indicator */
#define FIELD_PRESENCE(c)		((c)&0x0F)
#define FIELD_ORIGIN(c)			(((c)&0xF0)>>4)
#define FIELD_MAP(p,o)			(((o)<<4)|(p))

#define B_UN					FIELD_MAP(FP_UNDEF,FO_UNDEF)
#define B_Z						FIELD_MAP(FP_FORBIDDEN,FO_UNDEF)

#define B_X						FIELD_MAP(FP_MANDATORY,FO_GENERATED)
#define B_XQ					FIELD_MAP(FP_MANDATORY,FO_REQ_MSG)
#define B_XQI					FIELD_MAP(FP_MANDATORY,FO_INIT_REQ_MSG)
#define B_XRI					FIELD_MAP(FP_MANDATORY,FO_INIT_REP_MSG)

#define B_C						FIELD_MAP(FP_CONDITIONAL,FO_GENERATED)
#define B_CQ					FIELD_MAP(FP_CONDITIONAL,FO_REQ_MSG)
#define B_CQI					FIELD_MAP(FP_CONDITIONAL,FO_INIT_REQ_MSG)
#define B_CRI					FIELD_MAP(FP_CONDITIONAL,FO_INIT_REP_MSG)

#define B_F						FIELD_MAP(FP_OPTIONAL,FO_GENERATED)
#define B_FQ					FIELD_MAP(FP_OPTIONAL,FO_REQ_MSG)
#define B_FQI					FIELD_MAP(FP_OPTIONAL,FO_INIT_REQ_MSG)
#define B_FRI					FIELD_MAP(FP_OPTIONAL,FO_INIT_REP_MSG)





typedef struct msg_map_st {
	int		nFieldNo;
	char	info;
	int		nCondition;
} msg_map_t;





#endif


