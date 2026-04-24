#ifndef P7_BITMAP_INFO_TYPE_H
#define P7_BITMAP_INFO_TYPE_H

typedef struct {
	int					nFieldNo;
	field_info_t		field_info;
} bm_field_t;

typedef struct {
	int					nNbFields;
	int					nNbBitmaps;
	int					nNbConsBitmaps;
	int					nBitmapLen;
	E_FIELD_FORMAT		eBitmapFmt;
	bm_field_t			tabFieldsInfo[MAX_STRUCT_FIELDS];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} BitMappedProperties;

typedef struct {
	int				nLength;
	char			sBitMap			[ MAX_MSG_BITMAP_LEN ];
	char			sData			[ MAX_STRUCT_DATA_LEN ];
	int				nPresent		[ MAX_STRUCT_FIELDS + 1];
	int				nPos			[ MAX_STRUCT_FIELDS + 1];
	
	const BitMappedProperties* pkProperties;
} BitMappedInfo;


extern void InitBitMappedInfo(const char* szPropertiesName, BitMappedInfo* kBitMappedInfo);
extern void ResetBitMappedInfo(BitMappedInfo* kBitMappedInfo);
extern int  AnalyseBitMappedInfo(const char *buffer_in, int nLength, BitMappedInfo* kBitMappedInfo);
extern int  GetBitMappedInfoField (int nFieldNo, const BitMappedInfo* kBitMappedInfo, char *data, int* nLength);
extern int  AddBitMappedInfoField (int nFieldNo, BitMappedInfo* kBitMappedInfo, const char *data, int nLength);
extern int  PutBitMappedInfoField (int nFieldNo, BitMappedInfo* kBitMappedInfo, const char *data, int nLength);
extern int  BuildBitMappedInfoBuffer (char * buffer_snd, const BitMappedInfo* kBitMappedInfo);
extern void DumpBitMappedInfo(const BitMappedInfo* kBitMappedInfo);


extern void InitBitMappedInfoProperties(BitMappedProperties* kBitMappedProperties);

/*
extern void LoadBitMappedInfoProperties( TiXmlElement* pkItemElement, BitMappedProperties& kBitMappedProperties);
extern void SaveBitMappedInfoProperties( TiXmlElement* pkItemElement, const BitMappedProperties& kBitMappedProperties );
*/


#endif
