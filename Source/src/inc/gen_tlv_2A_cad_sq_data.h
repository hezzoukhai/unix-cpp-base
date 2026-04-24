#ifndef tlv_cad_sq_data_H
#define tlv_cad_sq_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ CB2A_F046_NB_FIELDS + 1];
 int nPosTlv [ CB2A_F046_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ CB2A_F046_BUF_LEN*2 ];
 
}
 TSTlvCADSQData;
 void InitTlvCADSQData(TSTlvCADSQData * tlvInfo);
 int AnalyseTlvCADSQData(char *buffer, int nLength, TSTlvCADSQData *tlvInfo);
 int GetTlvCADSQData (char *tlv_name, TSTlvCADSQData *tlvInfo,char *data, int *length);
 int AddTlvCADSQData (char *tlv_name, TSTlvCADSQData *tlvInfo, char *data, int length);
 int PutTlvCADSQData (char *tlv_name, TSTlvCADSQData *tlvInfo, char *data, int length);
 int ConcatTlvCADSQData (char *tlv_name, TSTlvCADSQData *tlvInfo, char *data, int length);
 int GetTlvCADSQDataType ( int nIndice );
 int GetTlvCADSQDataLength ( int nIndice );
 const char* GetTlvCADSQDataName ( int nIndice );
 void DumpCADSQData(TSTlvCADSQData *tlvInfo);
 int CADSQDataBuildTlv (char * buffer_snd, TSTlvCADSQData *tlvInfo);

#endif
