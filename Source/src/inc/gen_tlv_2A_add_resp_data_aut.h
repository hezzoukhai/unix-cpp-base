#ifndef tlv_add_resp_data_aut_H
#define tlv_add_resp_data_aut_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_AUT_F044_NB_FIELDS + 1];
 int nPosTlv [ CB2A_AUT_F044_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_AUT_F044_BUF_LEN ];
 
}
 TSTlvAddRespDataAut;
 void InitTlvAddRespDataAut(TSTlvAddRespDataAut * tlvInfo);
 int AnalyseTlvAddRespDataAut(char *buffer, int nLength, TSTlvAddRespDataAut *tlvInfo);
 int GetTlvAddRespDataAut (char *tlv_name, TSTlvAddRespDataAut *tlvInfo,char *data, int *length);
 int AddTlvAddRespDataAut (char *tlv_name, TSTlvAddRespDataAut *tlvInfo, char *data, int length);
 int PutTlvAddRespDataAut (char *tlv_name, TSTlvAddRespDataAut *tlvInfo, char *data, int length);
 int GetTlvAddRespDataAutType ( int nIndice );
 int GetTlvAddRespDataAutLength ( int nIndice );
 const char* GetTlvAddRespDataAutName ( int nIndice );
 void DumpAddRespDataAut(TSTlvAddRespDataAut *tlvInfo);
 int AddRespDataAutBuildTlv (char * buffer_snd, TSTlvAddRespDataAut *tlvInfo);

#endif
