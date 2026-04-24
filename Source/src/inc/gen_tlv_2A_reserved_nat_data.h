#ifndef tlv_reserved_nat_data_H
#define tlv_reserved_nat_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F118_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F118_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F118_BUF_LEN ];
 
}
 TSTlvReservedNatData;
 void InitTlvReservedNatData(TSTlvReservedNatData * tlvInfo);
 int AnalyseTlvReservedNatData(char *buffer, int nLength, TSTlvReservedNatData *tlvInfo);
 int GetTlvReservedNatData (char *tlv_name, TSTlvReservedNatData *tlvInfo,char *data, int *length);
 int AddTlvReservedNatData (char *tlv_name, TSTlvReservedNatData *tlvInfo, char *data, int length);
 int PutTlvReservedNatData (char *tlv_name, TSTlvReservedNatData *tlvInfo, char *data, int length);
 int GetTlvReservedNatDataType ( int nIndice );
 int GetTlvReservedNatDataLength ( int nIndice );
 const char* GetTlvReservedNatDataName ( int nIndice );
 void DumpReservedNatData(TSTlvReservedNatData *tlvInfo);
 int ReservedNatDataBuildTlv (char * buffer_snd, TSTlvReservedNatData *tlvInfo);

#endif
