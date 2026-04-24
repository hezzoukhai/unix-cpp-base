#ifndef tlv_add_data_H
#define tlv_add_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F047_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F047_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F047_BUF_LEN ];
 
}
 TSTlvAddData;
 void InitTlvAddData(TSTlvAddData * tlvInfo);
 int AnalyseTlvAddData(char *buffer, int nLength, TSTlvAddData *tlvInfo);
 int GetTlvAddData (char *tlv_name, TSTlvAddData *tlvInfo,char *data, int *length);
 int AddTlvAddData (char *tlv_name, TSTlvAddData *tlvInfo, char *data, int length);
 int PutTlvAddData (char *tlv_name, TSTlvAddData *tlvInfo, char *data, int length);
 int GetTlvAddDataType ( int nIndice );
 int GetTlvAddDataLength ( int nIndice );
 const char* GetTlvAddDataName ( int nIndice );
 void DumpAddData(TSTlvAddData *tlvInfo);
 int AddDataBuildTlv (char * buffer_snd, TSTlvAddData *tlvInfo);

#endif
