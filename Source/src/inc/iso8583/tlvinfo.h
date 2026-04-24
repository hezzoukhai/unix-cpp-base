#ifndef P7_TLV_INFO_H
#define P7_TLV_INFO_H
/******************************************************************************/

typedef enum  {
	TLV_LT_ASCII	= 0,
	TLV_LT_BIN		= 1
} E_TL_TYPE;



typedef struct {
	char			szTag[ISO3_MAX_TLV_TAG_LEN+1];
	field_info_t		field_info;
	E_REPEATABLE		eIsRepeatable;
} tlv_field_t;

typedef struct {
	int					nTagLen;
	int					nLengthLen;
	E_TL_TYPE			eLengthType;
	E_TL_TYPE			eTagType;
	int					nNbFields;
	tlv_field_t			tabFieldsInfo[ MAX_STRUCT_FIELDS ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} TlvInfoProperties;

/*
* nRepeatedFieldPos[] is used as:
* first index indicates the position of the element in sData
* second index indicates the index in nRepeatedFieldPos[] which contains the next iteration of this element
*/
typedef struct {
	int				nLength;
	char			sData				[ MAX_STRUCT_DATA_LEN ];
	int				nPresent			[ MAX_STRUCT_FIELDS + 1];
	int				nPosTlv				[ MAX_STRUCT_FIELDS + 1];
	int				nRepeatedFieldPos	[ MAX_NB_MULTIFIELDS ][2];
	int				nNextFreeRFPos;

	const TlvInfoProperties* pkProperties;
} TlvInfo;

extern void InitTlvInfo(const char* szPropertiesName, TlvInfo* kTlvInfo);
extern void ResetTlvInfo(TlvInfo* kTlvInfo);
extern int  AnalyseTlvInfo(const char *buffer_in, int nLength, TlvInfo* kTlvInfo);
extern int  GetTlvInfoField (const char *tlv_name, const TlvInfo* pkTlvInfo, char *data, int* nLength);
extern int  GetTlvInfoNextField (const char *tlv_name, const TlvInfo* pkTlvInfo, char *data, int* nLength, int nIter);
extern int  AddTlvInfoField (const char *tlv_name, TlvInfo* kTlvInfo, const char *data, int nLength);
extern int  PutTlvInfoField (const char *tlv_name, TlvInfo* kTlvInfo, const char *data, int nLength);
extern int  BuildTlvInfoBuffer (char * buffer_snd, TlvInfo* kTlvInfo);
extern void DumpTlvInfo(const TlvInfo* kTlvInfo);


extern void InitTlvInfoProperties(TlvInfoProperties* kTlvInfoProperties);

/******************************************************************************/


/*
struct structops {
	void (*Reset)(void*);
	int (*Analyse)(const unsigned char *buffer_in, int nLength, void* );
	int (*Get)(const char *tlv_name, const void*, char *data, int& nLength);
	int (*Add)(const char *tlv_name, void*, const char *data, int nLength);
	int (*Put)(const char *tlv_name, void*, const char *data, int nLength);
	int (*BuildBuffer)(char * buffer_snd, void*);
	void (*Dump)(const void*);
};
*/





#endif

