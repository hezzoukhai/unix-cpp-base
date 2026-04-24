#ifndef tlv_print_display_data_H
#define tlv_print_display_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F031_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F031_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F031_BUF_LEN ];
 
}
 TSTlvPrintDisplayData;
 void InitTlvPrintDisplayData(TSTlvPrintDisplayData * tlvInfo);
 int AnalyseTlvPrintDisplayData(char *buffer, int nLength, TSTlvPrintDisplayData *tlvInfo);
 int GetTlvPrintDisplayData (char *tlv_name, TSTlvPrintDisplayData *tlvInfo,char *data, int *length);
 int AddTlvPrintDisplayData (char *tlv_name, TSTlvPrintDisplayData *tlvInfo, char *data, int length);
 int PutTlvPrintDisplayData (char *tlv_name, TSTlvPrintDisplayData *tlvInfo, char *data, int length);
 int GetTlvPrintDisplayDataType ( int nIndice );
 int GetTlvPrintDisplayDataLength ( int nIndice );
 const char* GetTlvPrintDisplayDataName ( int nIndice );
 void DumpPrintDisplayData(TSTlvPrintDisplayData *tlvInfo);
 int PrintDisplayDataBuildTlv (char * buffer_snd, TSTlvPrintDisplayData *tlvInfo);

#endif
