#include <p7_common.h>
#include <iso8583/iso8583_define.h>
#include <iso8583/conv_module.h>

/******************************************************************************/
void InitConversionModule(TSConversionModule* pkConversionModule)
{
	pkConversionModule->nProtocolId = 0;
	pkConversionModule->nSize = 0;
}
/******************************************************************************/
const TSFieldMapping* GetMappingLogicBySrc(const TSConversionModule* pkConversionModule, int nFieldNo)
{

	int i;
	for(i=0; i < pkConversionModule->nSize; i++ )
	{
		if( pkConversionModule->tab_MappingLogic[i].nSrcFieldNo == nFieldNo )
		{
			return &pkConversionModule->tab_MappingLogic[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const TSFieldMapping* GetMappingLogicByDst(const TSConversionModule* pkConversionModule, int nFieldNo)
{
	int i;
	for(i=0; i < pkConversionModule->nSize; i++ )
	{
		if( pkConversionModule->tab_MappingLogic[i].nDstFieldNo == nFieldNo )
		{
			return &pkConversionModule->tab_MappingLogic[i];
		}
	}
	return NULL;
}
/******************************************************************************/
