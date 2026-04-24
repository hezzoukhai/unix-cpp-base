#ifndef tlv_nat_data_aut_H
#define tlv_nat_data_aut_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_AUT_F059_NB_FIELDS + 1];
 int nPosTlv [ CB2A_AUT_F059_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_AUT_F059_BUF_LEN*2 ];
 
}
 TSTlvNatDataAut;
 void InitTlvNatDataAut(TSTlvNatDataAut * tlvInfo);
 int AnalyseTlvNatDataAut(char *buffer, int nLength, TSTlvNatDataAut *tlvInfo);
 int GetTlvNatDataAut (char *tlv_name, TSTlvNatDataAut *tlvInfo,char *data, int *length);
 int AddTlvNatDataAut (char *tlv_name, TSTlvNatDataAut *tlvInfo, char *data, int length);
 int PutTlvNatDataAut (char *tlv_name, TSTlvNatDataAut *tlvInfo, char *data, int length);
 int ConcatTlvNatDataAut (char *tlv_name, TSTlvNatDataAut *tlvInfo, char *data, int length);
 int GetTlvNatDataAutType ( int nIndice );
 int GetTlvNatDataAutLength ( int nIndice );
 const char* GetTlvNatDataAutName ( int nIndice );
 void DumpNatDataAut(TSTlvNatDataAut *tlvInfo);
 int NatDataAutBuildTlv (char * buffer_snd, TSTlvNatDataAut *tlvInfo);

#endif
