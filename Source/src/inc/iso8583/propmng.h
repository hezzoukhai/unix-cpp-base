#ifndef P7_PROP_MNG__H
#define P7_PROP_MNG__H

int		InitIsoDataProperties();
/*
int SetNetworkProtocolId(E_ISO8583_PROTO eNwProtocolId);
*/

int	RegisterMsgInfoProp(const MsgInfoProperties* pkMsgInfoProperties);
int	RegisterBerInfoProp(const BerInfoProperties* pkBerInfoProperties);
int	RegisterBitmappedInfoProp(const BitMappedProperties* pkBitMappedProperties);
int	RegisterStaticInfoProp(const StaticInfoProperties* pkStaticInfoProperties);
int	RegisterTlvInfoProp(const TlvInfoProperties* pkTlvInfoProperties);


const MsgInfoProperties*	GetTabMsgInfoProperties			( int* nSize);
const BerInfoProperties*	GetTabBerInfoProperties			( int* nSize);
const BitMappedProperties*	GetTabBitMappedProperties		( int* nSize);
const StaticInfoProperties* GetTabStaticInfoProperties		( int* nSize);
const TlvInfoProperties*	GetTabTlvInfoProperties			( int* nSize);


extern const TlvInfoProperties*const  GetTlvInfoProperties(const char* szPropertiesName);
extern const StaticInfoProperties*const GetStaticInfoProperties(const char* szPropertiesName);
extern const MsgInfoProperties*const GetMsgInfoProperties(const char* szPropertiesName);
extern const BerInfoProperties*const GetBerInfoProperties(const char* szPropertiesName);
extern const BitMappedProperties*const GetBitMappedProperties(const char* szPropertiesName);

extern int	FreeAllProperties();



#endif
