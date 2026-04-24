#ifndef PWC_HSM_ATALLA__H
#define PWC_HSM_ATALLA__H

#define ATALLA_MAX_MSG_FIELDS		64
#define ATALLA_BUF_MAX_LEN			2048


typedef struct {
	int					nNbFields;
	ber_field_t			tabFieldsInfo[ ATALLA_FN_QTY ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} AtallaCmdInfoProperties;

typedef struct {
	int					nNbFields;
	ber_field_t			tabFieldsInfo[ MAX_STRUCT_FIELDS ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} AtallaCmdTagProperties;


typedef struct {
	int		nFieldId;
} TSAtallaFieldProp;

typedef struct {
	E_ATALLA_LAYOUT_TYPE	eLayoutType;
	int						nNbFields;
	int						nSize;
	E_ATALLA_CMD_CODE		eCmdCode;
	TSAtallaFieldProp		tab_fields[ATALLA_MAX_MSG_FIELDS];
} TSAtallaMsgLayout;


typedef struct {
	int				nLength;
	char			sData				[ 4096 + 1 ];
	int				nPresent			[ MAX_STRUCT_FIELDS + 1];
	int				nPosTlv				[ MAX_STRUCT_FIELDS + 1];
	char			sCmdTag				[64];
	int				nCmdTagLen;
	const AtallaCmdInfoProperties*	pkProperties;
	const AtallaCmdTagProperties*	pkCmdTagProperties;
	const TSAtallaMsgLayout*		pkLayout;
} TSAtallaCmd;


int  InitAtallaCmd(TSAtallaCmd* pkAtallaCmd, E_ATALLA_LAYOUT_TYPE eCmdType);
int  AnalyseAtallaCmdInfo(const char *buffer_in, int nLength, TSAtallaCmd* pkAtallaCmd);
int  GetAtallaCmdInfoField (E_ATALLA_FIELD_ID nFieldId, const TSAtallaCmd* pkAtallaCmd, char *data, int* nLength);
int  AddAtallaCmdInfoField (E_ATALLA_FIELD_ID nFieldId, TSAtallaCmd* pkAtallaCmd, const char *data, int nLength);
int  BuildAtallaCmdInfoBuffer (char * buffer_out, TSAtallaCmd* pkAtallaCmd);
void DumpAtallaCmdInfo(const TSAtallaCmd* pkAtallaCmd);
int  GetFieldIdLength(E_ATALLA_FIELD_ID nFieldId, const TSAtallaCmd* pkAtallaCmd);



#endif
