#ifndef PWC_HSM_C2P__H
#define PWC_HSM_C2P__H

#define C2P_MAX_MSG_FIELDS		64
#define C2P_BUF_MAX_LEN			2048


typedef struct {
	int					nNbFields;
	ber_field_t			tabFieldsInfo[ C2P_FN_QTY ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} C2PCmdInfoProperties;

typedef struct {
	int					nNbFields;
	ber_field_t			tabFieldsInfo[ MAX_STRUCT_FIELDS ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} C2PCmdTagProperties;


typedef struct {
	int		nFieldId;
} TSC2PFieldProp;

typedef struct {
	E_C2P_LAYOUT_TYPE	eLayoutType;
	int					nNbFields;
	int					nSize;
	E_C2P_CMD_CODE		eCmdCode;
	TSC2PFieldProp		tab_fields[C2P_MAX_MSG_FIELDS];
} TSC2PMsgLayout;


typedef struct {
	int				nLength;
	char			sData				[ MAX_STRUCT_DATA_LEN ];
	int				nPresent			[ MAX_STRUCT_FIELDS + 1];
	int				nPosTlv				[ MAX_STRUCT_FIELDS + 1];
	int				nRepeatedFieldPos	[ MAX_NB_MULTIFIELDS ][2];
	char			sCmdTag				[64];
	int				nCmdTagLen;
	int				nNextFreeRFPos;
	const C2PCmdInfoProperties*	pkProperties;
	const C2PCmdTagProperties*	pkCmdTagProperties;
	const TSC2PMsgLayout*		pkLayout;
} TSC2PCmd;


int InitC2PCmd(TSC2PCmd* pkC2PCmd, E_C2P_LAYOUT_TYPE eCmdType);
int  AnalyseC2PCmdInfo(const char *buffer_in, TSC2PCmd* pkC2PCmd);
int  GetC2PCmdInfoNextField (E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd, char *data, int* nLength, int nIter);
int  GetC2PCmdInfoField (E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd, char *data, int* nLength);
int  AddC2PCmdInfoField (E_C2P_FIELD_ID nFieldId, TSC2PCmd* pkC2PCmd, const char *data, int nLength);
int  PutC2PCMdInfoField (E_C2P_FIELD_ID nFieldId, TSC2PCmd* pkC2PCmd, const char *data, int nLength);
int  BuildC2PCmdInfoBuffer (char * buffer_out, TSC2PCmd* pkC2PCmd);
void DumpC2PCmdInfo(const TSC2PCmd* pkC2PCmd);
int GetFieldIdLength(E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd);



#endif
