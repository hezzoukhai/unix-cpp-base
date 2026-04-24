#ifndef tlv_fund_transfer_data_H
#define tlv_fund_transfer_data_H
/*This file is generated and should not be modified manually*/




/*typedef struct 
{
 int nPresent [ F112_NB_FIELDS + 1];
 int nPosTlv [ F112_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F112_BUF_LEN ];
 
}
 TSTlvFundTransferData;*/
 
typedef struct 	/*AMER20190228 PROD00066569: Enh5.0 1062*/
{
 int nPresent [ F118_NB_FIELDS + 1];
 int nPosTlv  [ F118_NB_FIELDS + 1];
 int nLength;
 char sTlvData[ F118_BUF_LEN ];
 
}
 TSTlvFundTransferData;
 
 void InitTlvFundTransferData(TSTlvFundTransferData * tlvInfo);
 int AnalyseTlvFundTransferData(char *buffer,TSTlvFundTransferData *tlvInfo);
 int GetTlvFundTransferData (char *tlv_name, TSTlvFundTransferData *tlvInfo,char *data, int *length);
 int AddTlvFundTransferData (char *tlv_name, TSTlvFundTransferData *tlvInfo, char *data, int length);
 int PutTlvFundTransferData (char *tlv_name, TSTlvFundTransferData *tlvInfo, char *data, int length);
 void DumpFundTransferData(TSTlvFundTransferData *tlvInfo);
 int FundTransferDataBuildTlv (char * buffer_snd, TSTlvFundTransferData *tlvInfo);

#endif
