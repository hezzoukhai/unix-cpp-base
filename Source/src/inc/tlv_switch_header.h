#ifndef tlv_switch_header_H
#define tlv_switch_header_H

#define MAX_SW_HEADER_FIELDS		12
#define MAX_SW_HEADER_DATA_LEN		256


#define ISW_MSG_ID					"001"
#define ISW_CHANNEL_ID				"002"
#define ISW_CONNEXION_ID            "003"

typedef struct 
{
	int		nPresent [ MAX_SW_HEADER_FIELDS + 1];
	int		nPosTlv [ MAX_SW_HEADER_FIELDS + 1];
	int		nLength;
	char	sTlvData [ MAX_SW_HEADER_DATA_LEN ];
} TSTlvSwitchHeader;

 void InitTlvSwitchHeader(TSTlvSwitchHeader * tlvInfo);
 int AnalyseTlvSwitchHeader(char *buffer, TSTlvSwitchHeader *tlvInfo);
 int GetTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo,char *data, int *length);
 int AddTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo, char *data, int length);
 int PutTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo, char *data, int length);
 int GetTlvSwitchHeaderIndice ( char *tlv_name );
 int GetTlvSwitchHeaderLength ( int nIndice );
 const char* GetTlvSwitchHeaderName ( int nIndice );
 void DumpSwitchHeader(TSTlvSwitchHeader *tlvInfo);
 int BuildTlvSwitchHeader (char * buffer_snd, TSTlvSwitchHeader *tlvInfo);

#endif
