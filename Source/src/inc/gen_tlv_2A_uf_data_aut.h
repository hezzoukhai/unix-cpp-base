#ifndef tlv_uf_data_aut_H
#define tlv_uf_data_aut_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_AUT_F117_NB_FIELDS + 1];
 int nPosTlv [ CB2A_AUT_F117_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_AUT_F117_BUF_LEN ];
 
}
 TSTlvUFDataAut;
 void InitTlvUFDataAut(TSTlvUFDataAut * tlvInfo);
 int AnalyseTlvUFDataAut(char *buffer, int nLength, TSTlvUFDataAut *tlvInfo);
 int GetTlvUFDataAut (char *tlv_name, TSTlvUFDataAut *tlvInfo,char *data, int *length);
 int AddTlvUFDataAut (char *tlv_name, TSTlvUFDataAut *tlvInfo, char *data, int length);
 int PutTlvUFDataAut (char *tlv_name, TSTlvUFDataAut *tlvInfo, char *data, int length);
 int GetTlvUFDataAutType ( int nIndice );
 int GetTlvUFDataAutLength ( int nIndice );
 const char* GetTlvUFDataAutName ( int nIndice );
 void DumpUFDataAut(TSTlvUFDataAut *tlvInfo);
 int UFDataAutBuildTlv (char * buffer_snd, TSTlvUFDataAut *tlvInfo);

#endif
