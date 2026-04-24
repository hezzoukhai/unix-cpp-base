#ifndef tlv_uf_data_H
#define tlv_uf_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F117_NB_FIELDS + 1];
 int nPosTlv [ F117_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F117_BUF_LEN ];
 
}
 TSTlvUFData;
 void InitTlvUFData(TSTlvUFData * tlvInfo);
 int AnalyseTlvUFData(char *buffer,TSTlvUFData *tlvInfo);
 int GetTlvUFData (char *tlv_name, TSTlvUFData *tlvInfo,char *data, int *length);
 int AddTlvUFData (char *tlv_name, TSTlvUFData *tlvInfo, char *data, int length);
 int PutTlvUFData (char *tlv_name, TSTlvUFData *tlvInfo, char *data, int length);
 void DumpUFData(TSTlvUFData *tlvInfo);
 int UFDataBuildTlv (char * buffer_snd, TSTlvUFData *tlvInfo);

#endif
