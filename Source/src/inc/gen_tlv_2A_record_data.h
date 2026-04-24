#ifndef tlv_record_data_H
#define tlv_record_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F072_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F072_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F072_BUF_LEN*2 ];
 
}
 TSTlvRecordData;
 void InitTlvRecordData(TSTlvRecordData * tlvInfo);
 int AnalyseTlvRecordData(char *buffer, int nLength, TSTlvRecordData *tlvInfo);
 int GetTlvRecordData (char *tlv_name, TSTlvRecordData *tlvInfo,char *data, int *length);
 int AddTlvRecordData (char *tlv_name, TSTlvRecordData *tlvInfo, char *data, int length);
 int PutTlvRecordData (char *tlv_name, TSTlvRecordData *tlvInfo, char *data, int length);
 int ConcatTlvRecordData (char *tlv_name, TSTlvRecordData *tlvInfo, char *data, int length);
 int GetTlvRecordDataType ( int nIndice );
 int GetTlvRecordDataLength ( int nIndice );
 const char* GetTlvRecordDataName ( int nIndice );
 void DumpRecordData(TSTlvRecordData *tlvInfo);
 int RecordDataBuildTlv (char * buffer_snd, TSTlvRecordData *tlvInfo);

#endif
