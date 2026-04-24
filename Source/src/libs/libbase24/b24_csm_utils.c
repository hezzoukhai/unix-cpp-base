/*#include <base24.h>*/
#include <stdio.h>
#include <stdlib.h>

#include <base24/iso_base24.h>
#include <tools.h>
#include <define.h>

/******************************************************************************/
static char* gs_csm_labels[]=
{
	LBL_CSM_F_MCL,
	LBL_CSM_F_RCV,
	LBL_CSM_F_ORG,
	LBL_CSM_F_SVR,
	LBL_CSM_F_KD,
	LBL_CSM_F_CTP,
	LBL_CSM_F_ERF
};

/******************************************************************************/
int InitCMSData(TSCSMData*	CSMData)
{
	memset((char*)CSMData,0,sizeof(TSCSMData));
	return OK;
}
/******************************************************************************/
int AnalyseCSM(char* sData, TSCSMData*	CSMData)
{
	int		nOffset;
	int		nLastOffset;
	int		nFieldFound;

	InitCMSData(CSMData);

	if( memcmp(sData,"CSM(",4) != 0 )
		return NOK;

	
	
	CSMData->nDataLength = strlen(sData);
	if( sData[CSMData->nDataLength - 1] != ')' )
		return NOK;

	sData += 4;					/*Skip "CSM(" */

	CSMData->nDataLength -= 5;	/* Ignore "CSM(" and ")"	*/

	if( CSMData->nDataLength > sizeof(CSMData->sData) - 1 )
	{
		return NOK;
	}

	memcpy(CSMData->sData,sData,CSMData->nDataLength);

	nOffset = 0;
	while( nOffset < CSMData->nDataLength )
	{
		nFieldFound = CFS_F_QTY;
		
		if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_MCL,LBL_CSM_F_MCL_LEN) == 0 )
		{
			nFieldFound = CSM_F_MCL;
			nOffset+=LBL_CSM_F_MCL_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_RCV,LBL_CSM_F_RCV_LEN) == 0 )
		{
			nFieldFound = CSM_F_RCV;
			nOffset+=LBL_CSM_F_RCV_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_ORG,LBL_CSM_F_ORG_LEN) == 0 )
		{
			nFieldFound = CSM_F_ORG;
			nOffset+=LBL_CSM_F_ORG_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_SVR,LBL_CSM_F_SVR_LEN) == 0 )
		{
			nFieldFound = CSM_F_SVR;
			nOffset+=LBL_CSM_F_SVR_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_KD,LBL_CSM_F_KD_LEN) == 0 )
		{
			nFieldFound = CSM_F_KD;
			nOffset+=LBL_CSM_F_KD_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_CTP,LBL_CSM_F_CTP_LEN) == 0 )
		{
			nFieldFound = CSM_F_CTP;
			nOffset+=LBL_CSM_F_CTP_LEN + 1;
		}
		else if( memcmp(CSMData->sData + nOffset,LBL_CSM_F_ERF,LBL_CSM_F_ERF_LEN) == 0 )
		{
			nFieldFound = CSM_F_ERF;
			nOffset+=LBL_CSM_F_ERF_LEN + 1;
		}
		else
		{
			trace_message(ERROR,">>> Error AnalyseCSM [%s]",CSMData->sData);
			return NOK;
		}
		
		CSMData->nPresent[nFieldFound] = 1;

		nLastOffset = nOffset;
		if( nOffset == CSMData->nDataLength )
			break;
		while( CSMData->sData[nOffset++] != ' ' && nOffset < CSMData->nDataLength );
		if( nOffset - nLastOffset > 0 )
		{
			CSMData->nPos[nFieldFound] = nLastOffset;
			CSMData->nLength[nFieldFound] = nOffset - nLastOffset - 1;
			
			trace_message(PROCESSING,"Field %s, Pos %d, Len %d",gs_csm_labels[nFieldFound],CSMData->nPos[nFieldFound],CSMData->nLength[nFieldFound]);
		}
	}
	return OK;
}
/******************************************************************************/
int GetCSMField(int nField, TSCSMData*	CSMData, char* data, int* length)
{
	if( CSMData->nLength[nField] == 0 )
		return NOK;

	(*length) = CSMData->nLength[nField];
	memcpy(data, CSMData->sData + CSMData->nPos[nField],(*length));
	data[(*length)]='\0';
	return OK;
}
/******************************************************************************/
int AddCSMField(int nField, TSCSMData*	CSMData, char* data, int length)
{
	int		nOffset;
	if( CSMData->nPresent[nField] != 0 )
		return NOK;
	
	nOffset = CSMData->nDataLength;
	strcpy(CSMData->sData + CSMData->nDataLength, gs_csm_labels[nField]);
	CSMData->nDataLength+= strlen(gs_csm_labels[nField]);
	CSMData->sData[CSMData->nDataLength++]='/';

	CSMData->nPresent[nField] = 1;

	if( length > 0 )
	{
		memcpy(CSMData->sData + CSMData->nDataLength,data,length);
		CSMData->nDataLength+= length;
		CSMData->sData[CSMData->nDataLength++]=' ';
		CSMData->nPos[nField] =  nOffset;
		CSMData->nLength[nField] = length;
	}
	return OK;
}
/******************************************************************************/
int PutCSMField(int nField, TSCSMData*	CSMData, char* data, int length)
{
	char	sRightData[256];
	int		nRightDataLen;
	int		nOffset;
	int		nDiff;
	int		i;

	if( CSMData->nPresent[nField] == 0 )
	{
		return AddCSMField(nField,CSMData,data,length);
	}

	if( CSMData->nLength[nField] == length )
	{
		memcpy(CSMData->sData + CSMData->nPos[nField],data,length);
		return OK;
	}

	if( length == 0 )
		return OK;
	
	nRightDataLen = CSMData->nDataLength - (CSMData->nPos[nField] + CSMData->nLength[nField]);
	if( nRightDataLen > 0 )
		memcpy(sRightData, CSMData->sData + CSMData->nPos[nField] + CSMData->nLength[nField], nRightDataLen);

	memcpy(CSMData->sData + CSMData->nPos[nField],data,length);
	*(CSMData->sData + CSMData->nPos[nField] + length)=' ';
	nDiff = length - CSMData->nLength[nField];
	CSMData->nDataLength+= nDiff;

	CSMData->nLength[nField] = length;

	if( nRightDataLen > 0 )
	{
		nOffset = CSMData->nPos[nField] +length + 1;
		memcpy(CSMData->sData + nOffset,sRightData,nRightDataLen);
	
		for(i=0; i < CFS_F_QTY; i++ )
		{
			if( CSMData->nPos[i] > CSMData->nPos[nField] )
			{
				CSMData->nPos[i]+=nDiff;
			}
		}
	}

	return OK;
}
/******************************************************************************/
void DumpCSM(TSCSMData*	CSMData)
{
	int		i;
	int		nLength;
	char	data[256];

	memset(data,'\0',sizeof(data));
	for(i=0; i < CFS_F_QTY; i++ )
	{
		if( GetCSMField(i,CSMData,data,&nLength) == OK )
		{
			trace_message(TRACE,"Field %s: [%s]",gs_csm_labels[i],data);
		}
	}
}

/******************************************************************************/
int BuildCSM(char* sData, TSCSMData*	CSMData)
{
	int		nOffset = 0;
	int		i;
	int		nLen;

	memcpy(sData + nOffset,"CSM(",4); nOffset+=4;
	memcpy(sData + nOffset,CSMData->sData,CSMData->nDataLength); nOffset+=CSMData->nDataLength;
	sData[nOffset++]=')';
	sData[nOffset]='\0';
	return nOffset;
}
