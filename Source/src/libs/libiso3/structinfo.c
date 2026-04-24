#include <p7_common.h>
#include <p7_globals.h>
#include <p7_tools.h>
#include <p7_trace.h>
#include <iso8583/structinfo.h>
#include <iso8583.h>
#include <iso8583/propmng.h>

/******************************************************************************/
void InitStructInfo(E_FIELD_TYPE eFieldType, const char* szPropertiesName, StructInfo* kStructInfo)
{
	kStructInfo->eType = eFieldType;
	
	switch(eFieldType)
	{
	case FT_STRUCT_TLV:
		InitTlvInfo(szPropertiesName);
		break;
	case FT_STRUCT_STATIC:
		kStructInfo->kStaticInfo.pkProperties = GetStaticInfoProperties(szPropertiesName);
		break;
	case FT_STRUCT_BER:
		kStructInfo->kBerInfo.pkProperties = GetBerInfoProperties(szPropertiesName);
		break;
	case FT_STRUCT_BITMAPPED:
		kStructInfo->kBitMappedInfo.pkProperties = GetBitMappedProperties(szPropertiesName);
		break;
	default:
		break;
	}
}
/******************************************************************************/
void ResetStruct(StructInfo* kStructInfo)
{
	/*kStructInfo.pkOps->Reset(&kStructInfo.);*/
}
/******************************************************************************/
int  AnalyseStruct(const char *buffer_in, int nLength, StructInfo* kStructInfo)
{

}
/******************************************************************************/
int  GetStructField (const char *tlv_name, const StructInfo* kStructInfo, char *data, int* nLength)
{

}
/******************************************************************************/
int  GetNextStructField (const char *tlv_name, const StructInfo* kStructInfo, char *data, int* nLength, int nIter)
{

}
/******************************************************************************/
int  AddStructField (const char *tlv_name, StructInfo* kStructInfo, const char *data, int nLength)
{

}
/******************************************************************************/
int  PutStructField (const char *tlv_name, StructInfo* kStructInfo, const char *data, int nLength)
{

}
/******************************************************************************/
int  BuildStructBuffer (char * buffer_snd, StructInfo* kStructInfo)
{

}
/******************************************************************************/
void DumpStruct(const StructInfo* kStructInfo)
{

}
/******************************************************************************/

