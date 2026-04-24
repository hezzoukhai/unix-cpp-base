#ifndef STATIC_INFO_TYPE_H
#define STATIC_INFO_TYPE_H


typedef struct {
	int					nPos;
	field_info_t		field_info;
} static_field_t;

typedef struct {
	int					nNbFields;
	static_field_t		tabFieldsInfo[ MAX_STRUCT_FIELDS ];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
} StaticInfoProperties;

typedef struct {
	int				nLength;
	char			sData		[ MAX_STRUCT_DATA_LEN ];	
	const StaticInfoProperties* pkProperties;
} StaticInfo;

extern void InitStaticInfo(const char* szPropertiesName, StaticInfo* kStaticInfo);
extern void ResetStaticInfo(StaticInfo* kStaticInfo);
extern int  AnalyseStaticInfo(const char *buffer_in, int nLength, StaticInfo* kStaticInfo);
extern int  GetStaticInfoField (int nPos, const StaticInfo* kStaticInfo, char *data, int* nLength);
extern int  AddStaticInfoField (int nPos, StaticInfo* kStaticInfo, const char *data, int nLength);
extern int  PutStaticInfoField (int nPos, StaticInfo* kStaticInfo, const char *data, int nLength);
extern int  BuildStaticInfoBuffer (char * buffer_snd, StaticInfo* kStaticInfo);
extern void DumpStaticInfo(const StaticInfo* kStaticInfo);


extern void InitStaticInfoProperties(StaticInfoProperties* kStaticInfoProperties);
/*
extern void LoadStaticInfoProperties( TiXmlElement* pkItemElement, StaticInfoProperties* kStaticInfoProperties);
extern void SaveStaticInfoProperties( TiXmlElement* pkItemElement, const StaticInfoProperties* kStaticInfoProperties );
*/

#endif
