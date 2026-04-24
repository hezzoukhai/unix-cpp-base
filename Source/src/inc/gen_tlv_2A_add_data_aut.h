#ifndef tlv_add_data_aut_H
#define tlv_add_data_aut_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_AUT_F047_NB_FIELDS + 1];
 int nPosTlv [ CB2A_AUT_F047_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_AUT_F047_BUF_LEN ];
 
}
 TSTlvAddDataAut;
 void InitTlvAddDataAut(TSTlvAddDataAut * tlvInfo);
 int AnalyseTlvAddDataAut(char *buffer, int nLength, TSTlvAddDataAut *tlvInfo);
 int GetTlvAddDataAut (char *tlv_name, TSTlvAddDataAut *tlvInfo,char *data, int *length);
 int AddTlvAddDataAut (char *tlv_name, TSTlvAddDataAut *tlvInfo, char *data, int length);
 int PutTlvAddDataAut (char *tlv_name, TSTlvAddDataAut *tlvInfo, char *data, int length);
 int GetTlvAddDataAutType ( int nIndice );
 int GetTlvAddDataAutLength ( int nIndice );
 const char* GetTlvAddDataAutName ( int nIndice );
 void DumpAddDataAut(TSTlvAddDataAut *tlvInfo);
 int AddDataAutBuildTlv (char * buffer_snd, TSTlvAddDataAut *tlvInfo);

#endif
