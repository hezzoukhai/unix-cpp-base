#ifndef tlv_nat_data_H
#define tlv_nat_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F058_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F058_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F058_BUF_LEN*2 ];
 
}
 TSTlvNatData;
 void InitTlvNatData(TSTlvNatData * tlvInfo);
 int AnalyseTlvNatData(char *buffer, int nLength, TSTlvNatData *tlvInfo);
 int GetTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo,char *data, int *length);
 int AddTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo, char *data, int length);
 int PutTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo, char *data, int length);
 int ConcatTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo, char *data, int length);
 int GetTlvNatDataType ( int nIndice );
 int GetTlvNatDataLength ( int nIndice );
 const char* GetTlvNatDataName ( int nIndice );
 void DumpNatData(TSTlvNatData *tlvInfo);
 int NatDataBuildTlv (char * buffer_snd, TSTlvNatData *tlvInfo);

#endif
