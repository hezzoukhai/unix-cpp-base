#ifndef tlv_services_data_H
#define tlv_services_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F114_NB_FIELDS + 1];
 int nPosTlv [ F114_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F114_BUF_LEN ];
 
}
 TSTlvServicesData;
 void InitTlvServicesData(TSTlvServicesData * tlvInfo);
 int AnalyseTlvServicesData(char *buffer,TSTlvServicesData *tlvInfo);
 int GetTlvServicesData (char *tlv_name, TSTlvServicesData *tlvInfo,char *data, int *length);
 int AddTlvServicesData (char *tlv_name, TSTlvServicesData *tlvInfo, char *data, int length);
 int PutTlvServicesData (char *tlv_name, TSTlvServicesData *tlvInfo, char *data, int length);
 void DumpServicesData(TSTlvServicesData *tlvInfo);
 int ServicesDataBuildTlv (char * buffer_snd, TSTlvServicesData *tlvInfo);

#endif
