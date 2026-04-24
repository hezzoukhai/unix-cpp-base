#ifndef tlv_nat_data_H
#define tlv_nat_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F059_NB_FIELDS + 1];
 int nPosTlv [ F059_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F059_BUF_LEN ];
 
}
 TSTlvNatData;
 void InitTlvNatData(TSTlvNatData * tlvInfo);
 int AnalyseTlvNatData(char *buffer,TSTlvNatData *tlvInfo);
 int GetTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo,char *data, int *length);
 int AddTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo, char *data, int length);
 int PutTlvNatData (char *tlv_name, TSTlvNatData *tlvInfo, char *data, int length);
 void DumpNatData(TSTlvNatData *tlvInfo);
 int NatDataBuildTlv (char * buffer_snd, TSTlvNatData *tlvInfo);

#endif
