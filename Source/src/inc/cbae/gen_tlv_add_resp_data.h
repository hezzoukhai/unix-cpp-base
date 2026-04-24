#ifndef tlv_add_resp_data_H
#define tlv_add_resp_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F044_NB_FIELDS + 1];
 int nPosTlv [ F044_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F044_BUF_LEN ];
 
}
 TSTlvAddRespData;
 void InitTlvAddRespData(TSTlvAddRespData * tlvInfo);
 int AnalyseTlvAddRespData(char *buffer,TSTlvAddRespData *tlvInfo);
 int GetTlvAddRespData (char *tlv_name, TSTlvAddRespData *tlvInfo,char *data, int *length);
 int AddTlvAddRespData (char *tlv_name, TSTlvAddRespData *tlvInfo, char *data, int length);
 int PutTlvAddRespData (char *tlv_name, TSTlvAddRespData *tlvInfo, char *data, int length);
 void DumpAddRespData(TSTlvAddRespData *tlvInfo);
 int AddRespDataBuildTlv (char * buffer_snd, TSTlvAddRespData *tlvInfo);

#endif
