#ifndef P7_BER_INFO_TYPE_H
#define P7_BER_INFO_TYPE_H

typedef struct {
	char				szTag[ISO3_MAX_TLV_TAG_LEN+1];
	field_info_t		field_info;
	E_REPEATABLE		eIsRepeatable;
} ber_field_t;

typedef struct {
	int					nNbFields;
	ber_field_t			tabFieldsInfo[ MAX_STRUCT_FIELDS ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} BerInfoProperties;

typedef struct {
	int				nLength;
	char			        sData				[ MAX_STRUCT_DATA_LEN ];
	int				nPresent			[ MAX_STRUCT_FIELDS + 1];
	int				nPosTlv				[ MAX_STRUCT_FIELDS + 1];
	int				nRepeatedFieldPos	[ MAX_NB_MULTIFIELDS ][2];
	int				nNextFreeRFPos;
	
	const BerInfoProperties* pkProperties;
} BerInfo;


extern void InitBerInfo(const char* szPropertiesName, BerInfo* pkBerInfo);
extern void ResetBerInfo(BerInfo* pkBerInfo);
extern int  AnalyseBerInfo(const char *buffer_in, int nLength, BerInfo* pkBerInfo);
extern int  GetBerInfoField (const char *tlv_name, const BerInfo* pkBerInfo, char *data, int* nLength);
extern int  GetBerInfoNextField (const char *tlv_name, const BerInfo* pkBerInfo, char *data, int* nLength, int nIter);
extern int  AddBerInfoField (const char *tlv_name, BerInfo* pkBerInfo, const char *data, int nLength);
extern int  PutBerInfoField (const char *tlv_name, BerInfo* pkBerInfo, const char *data, int nLength);
extern int  BuildBerInfoBuffer (char * buffer_snd, BerInfo* pkBerInfo);
extern void DumpBerInfo(const BerInfo* pkBerInfo);


extern void InitBerInfoProperties(BerInfoProperties* pkBerInfoProperties);


#endif

