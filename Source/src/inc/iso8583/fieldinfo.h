#ifndef P7_FIELD_INFO__H
#define P7_FIELD_INFO__H


typedef enum {
	NOT_REPEATABLE	= 0,
	IS_REPEATABLE	= 1
} E_REPEATABLE;


typedef struct field_info_s {
	E_FIELD_TYPE		eFieldType;
	E_FIELD_FORMAT		eFormatType;
	E_ALPHA_FORMAT		eAlphaFormat;
	E_FIELD_LENGTH		eLengthType;
	E_LENGTH_UNIT		eLengthUnit;
	int					nLength;
	int					nPrintPolicy;
	char				szPattern[MAX_PATTERN_LEN + 1];
	char				szLabel[MAX_FIELD_LABEL_LEN + 1];
	char				szPropertiesName[MAX_PROP_NAME_LEN + 1];
	
} field_info_t;



extern void InitFieldInfo(field_info_t* stFieldInfo);

#endif

