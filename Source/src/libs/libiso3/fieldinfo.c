#include <p7_common.h>
#include <p7_types.h>
#include <iso8583/iso8583_define.h>
#include <iso8583/fieldinfo.h>
/******************************************************************************/
void InitFieldInfo(field_info_t* stFieldInfo)
{

	stFieldInfo->eFieldType		= FT_UNDEF;
	stFieldInfo->eFormatType	= FF_HEX;
	stFieldInfo->eLengthType	= FL_FIX;
	stFieldInfo->eLengthUnit	= LU_BYTE;
	stFieldInfo->nLength		= 0;
	stFieldInfo->nPrintPolicy = PP_HIDE;
	memset(stFieldInfo->szPattern,'\0',sizeof(stFieldInfo->szPattern));
	memset(stFieldInfo->szLabel,'\0',sizeof(stFieldInfo->szLabel));
	memset(stFieldInfo->szPropertiesName,'\0',sizeof(stFieldInfo->szPropertiesName));
}
/******************************************************************************/
/*Start PROD00070301*/
int ControlDataType(char* data, E_FIELD_FORMAT format, int nLen)
{
	int			j;
	int			nIsFormatError = 0;
	
	if(nLen == 0) 
		return OK;		
		
	switch(format)
	{
	case FF_HEX:
		for( j=0; j < nLen; ++j )
		{
			if( !isxdigit(data[j]) )
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;	
	case FF_ALPHA_A:
		for( j=0; j < nLen; ++j )
		{
			if( !isalpha(data[j]) )
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;	
	case FF_ALPHA_N:
		for( j=0; j < nLen; ++j )
		{
			if( !isdigit(data[j]) )
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;
	case FF_ALPHA_AN:
		for( j=0; j < nLen; ++j )
		{
			if( !isalnum(data[j]) )
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;
	case FF_ALPHA_ANS:
		for( j=0; j < nLen; ++j )
		{
			/*if( !isalnum(data[j]) && data[j] != ' ')*/
			if( !isprint(data[j]))	/*AMER20190828: 'S' means special caracters not space*/
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;
	case FF_ALPHA_NS:
		for( j=0; j < nLen; ++j )
		{
			/*if( !isdigit(data[j]) && data[j] != ' ')*/
			if( isalpha(data[j] && !isprint(data[j])))	/*AMER20190828: 'S' means special caracters not space*/
			{
				nIsFormatError = 1;
				break;
			}
		}
		break;
	}
	if( nIsFormatError != 0 )
	{
		return NOK;
	}
	
	return OK;
}
/*End PROD00070301*/
/******************************************************************************/					  																				