#ifndef tlv_cb_reserved_data_H
#define tlv_cb_reserved_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F116_NB_FIELDS + 1];
 int nPosTlv [ F116_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F116_BUF_LEN ];
 
}
 TSTlvCBReservedData;
 void InitTlvCBReservedData(TSTlvCBReservedData * tlvInfo);
 int AnalyseTlvCBReservedData(char *buffer,TSTlvCBReservedData *tlvInfo);
 int GetTlvCBReservedData (char *tlv_name, TSTlvCBReservedData *tlvInfo,char *data, int *length);
 int AddTlvCBReservedData (char *tlv_name, TSTlvCBReservedData *tlvInfo, char *data, int length);
 int PutTlvCBReservedData (char *tlv_name, TSTlvCBReservedData *tlvInfo, char *data, int length);
 void DumpCBReservedData(TSTlvCBReservedData *tlvInfo);
 int CBReservedDataBuildTlv (char * buffer_snd, TSTlvCBReservedData *tlvInfo);

#endif
