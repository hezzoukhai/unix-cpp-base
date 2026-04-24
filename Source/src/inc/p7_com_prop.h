#ifndef P7_COM_PROPERTIES__H
#define P7_COM_PROPERTIES__H


typedef enum {
	E_CM_NONE	= 0	,
	E_CM_SERVER		,
	E_CM_CLIENT		,
} E_CONN_MODE;

typedef enum {
	E_CONN_TCP	= 0		,
	E_CONN_UDP			,
	E_CONN_SSL			,

	E_CONN_QTY
} E_CONN_TYPE;

typedef unsigned long conn_id_t;
enum { INVALID_CONN_ID = UINT_MAX };

typedef enum {
	CLT_CONN_FREE	= 1	,
	CLT_CONN_ONLINE		
} E_CONN_STATUS;

typedef enum {
	CLTP_HIGH_BINARY	=0	,
	CLTP_LOW_BINARY			,
	CLTP_NO_LENGTH			,
	CLTP_RFU_1				,
	CLTP_HIGH_ASCII			,
	CLTP_LOW_ASCII			,

	CLTP_QTY
} E_CONN_LENGTH_TP;

typedef enum {
	LDT_LENGTH_EXCLUDED		= 0	,
	LDT_LENGTH_INCLUDED			,

} E_LENGH_DT;

typedef struct {
	E_CONN_MODE			eConnMode;
	E_CONN_TYPE			eConnType;
	E_CONN_LENGTH_TP	ePacketLengthType;
	int					nLengthLen;
	int					nTotalLengthLen;
	int					nLengthOffset;
	E_LENGH_DT			bExcludeLength;
	int					nMaxClients;
	int					nDivFactor;
	int					bMultiConnPerClient;
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} TSComInfoProperties;


extern void		InitComInfoProperties(TSComInfoProperties* pkComInfoProperties);



#endif