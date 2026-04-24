#ifndef P7_FIELDS_TYPES__H
#define P7_FIELDS_TYPES__H


typedef enum {

	FT_UNDEF		= 0		,
	FT_SINGLE				,
	FT_STRUCT_TLV			,
	FT_STRUCT_STATIC		,
	FT_STRUCT_BER			,
	FT_STRUCT_BITMAPPED		,

	FT_QTY
} E_FIELD_TYPE;


typedef enum {
	FF_HEX		= 0	,
	FF_BIN			,
	FF_BCD			,
	FF_ALPHA_A		,
	FF_ALPHA_N		,
	FF_ALPHA_AN		,
	FF_ALPHA_ANS	,
	FF_ALPHA_NS		,
	FF_Z			,
	FF_X_BCD		,
	FF_X_ALPHA_N	,
	FF_CBCD			,

	FF_QTY
} E_FIELD_FORMAT;

typedef enum {

	FL_FIX		= 0	,
	FL_L			,
	FL_LL			,
	FL_LLL			,
	FL_B			,
	FL_BB			,
	FL_D			,
	FL_DD			,
	FL_QTY
} E_FIELD_LENGTH;

typedef enum {
	LU_BYTE		= 0	,
	LU_NIBBLES		,

	LU_QTY
} E_LENGTH_UNIT;

typedef enum {
	PP_PRINT		= 0	,
	PP_ENCRYPT			,
	PP_HIDE				,

	PP_QTY
} E_PRINT_POLICY;


typedef enum {
	AF_ASCII	= 0	,
	AF_EBCDIC		,

	AF_QTY
} E_ALPHA_FORMAT;




#endif

