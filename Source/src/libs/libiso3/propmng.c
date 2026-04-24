#include <p7_common.h>
#include <stdlib.h>
#include <iso8583.h>
#include <iso8583/propmng.h>
/******************************************************************************/
#define DFLT_NB_MSG_INFO_PROP		2
#define DFLT_NB_BER_INFO_PROP		4
#define DFLT_NB_BM_INFO_PROP		4
#define DFLT_NB_SC_INFO_PROP		8
#define DFLT_NB_TLV_INFO_PROP		8

#define DFLT_STEP_MSG_INFO_PROP		2
#define DFLT_STEP_BER_INFO_PROP		4
#define DFLT_STEP_BM_INFO_PROP		4
#define DFLT_STEP_SC_INFO_PROP		4
#define DFLT_STEP_TLV_INFO_PROP		4
/******************************************************************************/
static MsgInfoProperties*		gs_tab_MsgInfoProperties = NULL;
static BerInfoProperties*		gs_tab_BerInfoProperties = NULL;
static BitMappedProperties*		gs_tab_BitMappedProperties = NULL;
static StaticInfoProperties*	gs_tab_StaticInfoProperties = NULL;
static TlvInfoProperties*		gs_tab_TlvInfoProperties = NULL;

static int		gs_nMsgInfoPropertiesSize		= 0;
static int		gs_nBerInfoPropertiesSize		= 0;
static int		gs_nBitMappedPropertiesSize		= 0;
static int		gs_nStaticInfoPropertiesSize	= 0;
static int		gs_nTlvInfoPropertiesSize		= 0;


static int		gs_nCurrMsgInfoPropertiesSize		= 0;
static int		gs_nCurrBerInfoPropertiesSize		= 0;
static int		gs_nCurrBitMappedPropertiesSize		= 0;
static int		gs_nCurrStaticInfoPropertiesSize	= 0;
static int		gs_nCurrTlvInfoPropertiesSize		= 0;


static E_ISO8583_PROTO eInternalProtocol;
static E_ISO8583_PROTO eNetworkProtocol;
/******************************************************************************/
int		InitIsoDataProperties()
{

	gs_tab_MsgInfoProperties	= (MsgInfoProperties*)malloc(gs_nMsgInfoPropertiesSize*sizeof(MsgInfoProperties));
	gs_tab_BerInfoProperties	= (BerInfoProperties*)malloc(gs_nBerInfoPropertiesSize*sizeof(BerInfoProperties));
	gs_tab_BitMappedProperties	= (BitMappedProperties*)malloc(gs_nBitMappedPropertiesSize*sizeof(BitMappedProperties));
	gs_tab_StaticInfoProperties = (StaticInfoProperties*)malloc(gs_nStaticInfoPropertiesSize*sizeof(StaticInfoProperties));
	gs_tab_TlvInfoProperties	= (TlvInfoProperties*)malloc(gs_nTlvInfoPropertiesSize*sizeof(TlvInfoProperties));

	gs_nCurrMsgInfoPropertiesSize		= 0;
	gs_nCurrBerInfoPropertiesSize		= 0;
	gs_nCurrBitMappedPropertiesSize		= 0;
	gs_nCurrStaticInfoPropertiesSize	= 0;
	gs_nCurrTlvInfoPropertiesSize		= 0;


	return OK;
}
/******************************************************************************/
int SetNetworkProtocolId(E_ISO8583_PROTO eNwProtocolId)
{
	eNetworkProtocol = eNwProtocolId;
	return OK;
}
/******************************************************************************/
int	RegisterMsgInfoProp(const MsgInfoProperties* pkMsgInfoProperties)
{
	if( gs_nCurrMsgInfoPropertiesSize >= gs_nMsgInfoPropertiesSize )
	{
		
		gs_nMsgInfoPropertiesSize+=DFLT_STEP_MSG_INFO_PROP;
		gs_tab_MsgInfoProperties	= (MsgInfoProperties*)realloc(gs_tab_MsgInfoProperties, gs_nMsgInfoPropertiesSize*sizeof(MsgInfoProperties));
	}
	gs_tab_MsgInfoProperties[gs_nCurrMsgInfoPropertiesSize] = (*pkMsgInfoProperties);
	gs_nCurrMsgInfoPropertiesSize++;

	if( gs_nCurrMsgInfoPropertiesSize > 0 )
	{
		eInternalProtocol = (E_ISO8583_PROTO)gs_tab_MsgInfoProperties[0].nProtocolId;
		if( gs_nCurrMsgInfoPropertiesSize > 1 )
			eNetworkProtocol	= (E_ISO8583_PROTO)gs_tab_MsgInfoProperties[1].nProtocolId;
		else
			eNetworkProtocol	= (E_ISO8583_PROTO)gs_tab_MsgInfoProperties[0].nProtocolId;
	}

	return 0;
}
/******************************************************************************/
int	RegisterBerInfoProp(const BerInfoProperties* pkBerInfoProperties)
{
	if( gs_nCurrBerInfoPropertiesSize >= gs_nBerInfoPropertiesSize )
	{
		
		gs_nBerInfoPropertiesSize+=DFLT_STEP_BER_INFO_PROP;
		gs_tab_BerInfoProperties	= (BerInfoProperties*)realloc(gs_tab_BerInfoProperties, gs_nBerInfoPropertiesSize*sizeof(BerInfoProperties));
	}
	gs_tab_BerInfoProperties[gs_nCurrBerInfoPropertiesSize] = (*pkBerInfoProperties);
	gs_nCurrBerInfoPropertiesSize++;
	return 0;
}
/******************************************************************************/
int	RegisterBitmappedInfoProp(const BitMappedProperties* pkBitMappedProperties)
{
	if( gs_nCurrBitMappedPropertiesSize >= gs_nBitMappedPropertiesSize )
	{
		
		gs_nBitMappedPropertiesSize+=DFLT_STEP_BM_INFO_PROP;
		gs_tab_BitMappedProperties	= (BitMappedProperties*)realloc(gs_tab_BitMappedProperties, gs_nBitMappedPropertiesSize*sizeof(BitMappedProperties));
	}
	gs_tab_BitMappedProperties[gs_nCurrBitMappedPropertiesSize] = (*pkBitMappedProperties);
	gs_nCurrBitMappedPropertiesSize++;
	return 0;
}
/******************************************************************************/
int	RegisterStaticInfoProp(const StaticInfoProperties* pkStaticInfoProperties)
{
	if( gs_nCurrBitMappedPropertiesSize >= gs_nStaticInfoPropertiesSize )
	{
		
		gs_nStaticInfoPropertiesSize+=DFLT_STEP_SC_INFO_PROP;
		gs_tab_StaticInfoProperties	= (StaticInfoProperties*)realloc(gs_tab_StaticInfoProperties, gs_nStaticInfoPropertiesSize*sizeof(StaticInfoProperties));
	}
	gs_tab_StaticInfoProperties[gs_nCurrBitMappedPropertiesSize] = (*pkStaticInfoProperties);
	gs_nCurrBitMappedPropertiesSize++;
	return 0;
}
/******************************************************************************/
int	RegisterTlvInfoProp(const TlvInfoProperties* pkTlvInfoProperties)
{
	if( gs_nCurrTlvInfoPropertiesSize >= gs_nTlvInfoPropertiesSize )
	{
		
		gs_nTlvInfoPropertiesSize+=DFLT_STEP_TLV_INFO_PROP;
		gs_tab_TlvInfoProperties	= (TlvInfoProperties*)realloc(gs_tab_TlvInfoProperties, gs_nTlvInfoPropertiesSize*sizeof(TlvInfoProperties));
	}
	gs_tab_TlvInfoProperties[gs_nCurrTlvInfoPropertiesSize] = (*pkTlvInfoProperties);
	gs_nCurrTlvInfoPropertiesSize++;
	return 0;
}

/******************************************************************************/
const MsgInfoProperties*	GetTabMsgInfoProperties			( int* nSize)
{
	(*nSize) = gs_nCurrMsgInfoPropertiesSize;
	return gs_tab_MsgInfoProperties;
}

const BerInfoProperties*	GetTabBerInfoProperties			( int* nSize)
{
	(*nSize) = gs_nCurrBerInfoPropertiesSize;
	return gs_tab_BerInfoProperties;
}

const BitMappedProperties*	GetTabBitMappedProperties		( int* nSize)
{
	(*nSize) = gs_nCurrBitMappedPropertiesSize;
	return gs_tab_BitMappedProperties;
}

const StaticInfoProperties* GetTabStaticInfoProperties		( int* nSize)
{
	(*nSize) = gs_nCurrStaticInfoPropertiesSize;
	return gs_tab_StaticInfoProperties;
}
const TlvInfoProperties*	GetTabTlvInfoProperties			( int* nSize)
{
	(*nSize) = gs_nCurrTlvInfoPropertiesSize;
	return gs_tab_TlvInfoProperties;
}
/******************************************************************************/
const TlvInfoProperties*const  GetTlvInfoProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nCurrTlvInfoPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_TlvInfoProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_TlvInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const StaticInfoProperties*const GetStaticInfoProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nCurrStaticInfoPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_StaticInfoProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_StaticInfoProperties[i];
		}
	}
	return NULL;

}
/******************************************************************************/
const BerInfoProperties*const GetBerInfoProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nCurrBerInfoPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_BerInfoProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_BerInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const MsgInfoProperties*const GetMsgInfoProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nCurrMsgInfoPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_MsgInfoProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_MsgInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const BitMappedProperties*const GetBitMappedProperties(const char* szPropertiesName)
{
	int		i;
	for( i = 0; i < gs_nCurrBitMappedPropertiesSize; i++ )
	{
		if( strcmp(gs_tab_BitMappedProperties[i].szPropertiesName,szPropertiesName) == 0 )
		{
			return &gs_tab_BitMappedProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const MsgInfoProperties*const  GetNetworkMsgProperties()
{
	int		i;
	for( i = 0; i < gs_nCurrMsgInfoPropertiesSize; i++ )
	{
		if( gs_tab_MsgInfoProperties[i].nProtocolId == eNetworkProtocol )
		{
			return &gs_tab_MsgInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
const MsgInfoProperties*const  GetSwitchMsgProperties()
{
	int		i;
	for( i = 0; i < gs_nCurrMsgInfoPropertiesSize; i++ )
	{
		if( gs_tab_MsgInfoProperties[i].nProtocolId == eInternalProtocol )
		{
			return &gs_tab_MsgInfoProperties[i];
		}
	}
	return NULL;
}
/******************************************************************************/
int	FreeAllProperties()
{
	if(gs_tab_MsgInfoProperties)
	{
		free(gs_tab_MsgInfoProperties);
		gs_tab_MsgInfoProperties=NULL;
	}
	if(gs_tab_BerInfoProperties)
	{
		free(gs_tab_BerInfoProperties);
		gs_tab_BerInfoProperties=NULL;
	}
	if(gs_tab_BitMappedProperties)
	{
		free(gs_tab_BitMappedProperties);
		gs_tab_BitMappedProperties=NULL;
	}
	if(gs_tab_StaticInfoProperties)
	{
		free(gs_tab_StaticInfoProperties);
		gs_tab_StaticInfoProperties=NULL;
	}
	if(gs_tab_TlvInfoProperties)
	{
		free(gs_tab_TlvInfoProperties);
		gs_tab_TlvInfoProperties=NULL;
	}

	gs_nMsgInfoPropertiesSize=0;
	gs_nBerInfoPropertiesSize=0;
	gs_nBitMappedPropertiesSize=0;
	gs_nStaticInfoPropertiesSize=0;
	gs_nTlvInfoPropertiesSize=0;

	gs_nCurrMsgInfoPropertiesSize=0;
	gs_nCurrBerInfoPropertiesSize=0;
	gs_nCurrBitMappedPropertiesSize=0;
	gs_nCurrStaticInfoPropertiesSize=0;
	gs_nCurrTlvInfoPropertiesSize=0;

	
	return OK;
}
/******************************************************************************/

