#ifndef P7_CONV_MODULE__H
#define P7_CONV_MODULE__H


typedef enum {
	ML_NO_MAPPING = 0,
	ML_F_TO_F,
	ML_MAP_TABLE,
	ML_CUSTOM,
	
	ML_QTY
} E_MAPPING_LOGIC;


typedef struct {

	int					nSrcFieldNo;
	int					nDstFieldNo;
	E_MAPPING_LOGIC		eMappingLogic;
	/*MapTable*			m_pkMapTable;
	std::string			m_kDefaultValue;
	std::string			m_szMapTableName;*/

} TSFieldMapping;

typedef struct {
	int					nProtocolId;
	TSFieldMapping		tab_MappingLogic[MAX_STRUCT_FIELDS];
	int					nSize;
} TSConversionModule;

extern void InitConversionModule(TSConversionModule* pkConversionModule);
extern const TSFieldMapping* GetMappingLogicBySrc(const TSConversionModule* pkConversionModule, int nFieldNo);
extern const TSFieldMapping* GetMappingLogicByDst(const TSConversionModule* pkConversionModule, int nFieldNo);



#endif

