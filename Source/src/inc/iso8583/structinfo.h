#ifndef P7_STRUCT_INFO_TYPE_H
#define P7_STRUCT_INFO_TYPE_H

#include <p7_common.h>
#include <p7_types.h>
#include <iso8583/berinfo.h>
#include <iso8583/bitmapinfo.h>
#include <iso8583/staticinfo.h>
#include <iso8583/tlvinfo.h>
#include <iso8583/tlvbuffer.h>

typedef struct {
	union {
		TlvInfo			kTlvInfo;
		StaticInfo		kStaticInfo;
		BerInfo			kBerInfo;
		BitMappedInfo	kBitMappedInfo;
	} ;
	E_FIELD_TYPE	eType;
	/*structops*		pkOps;*/
} StructInfo;


void InitStructInfo(E_FIELD_TYPE eFieldType, const char* szPropertiesName, StructInfo* kStructInfo);

extern void ResetStruct(StructInfo* kStructInfo);
extern int  AnalyseStruct(const unsigned char *buffer_in, int nLength, StructInfo* kStructInfo);
extern int  GetStructField (const char *tlv_name, const StructInfo* kStructInfo, char *data, int* nLength);
extern int  GetNextStructField (const char *tlv_name, const StructInfo* kStructInfo, char *data, int* nLength, int nIter);
extern int  AddStructField (const char *tlv_name, StructInfo* kStructInfo, const char *data, int nLength);
extern int  PutStructField (const char *tlv_name, StructInfo* kStructInfo, const char *data, int nLength);
extern int  BuildStructBuffer (char * buffer_snd, StructInfo* kStructInfo);
extern void DumpStruct(const StructInfo* kStructInfo);


#endif

