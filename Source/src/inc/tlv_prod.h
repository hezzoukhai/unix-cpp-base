#ifndef HPS_TLV_PROD_H
#define HPS_TLV_PROD_H

#include <tlv_com.h>
#include <tlv_prodfields.h>

/**************************************/
#define   PROD_TLV_FIX           0
#define   PROD_TLV_VAR           1
/**************************************/

#define  MAX_PROD_TLV       999
#define  MAX_PROD_TLV_LEN  4098

typedef struct STlvProd
{
   int  nPresent  [ MAX_PROD_TLV ];
   int  nPosTlv   [ MAX_PROD_TLV ];
   int  nLength;
   char sTlvData  [ MAX_PROD_TLV_LEN ];
}
TSTlvProd;

void InitTlvProd(TSTlvProd * tlvInfo);
int  AnalyseTlvProd(char *buffer,TSTlvProd *tlvInfo);
int  GetTlvProd (char *tlv_name, TSTlvProd *tlvInfo,char *data, int *length);
int  AddTlvProd (char *tlv_name, TSTlvProd *tlvInfo, char *data, int length);
int  PutTlvProd (char *tlv_name, TSTlvProd *tlvInfo, char *data, int length);
int  ProdBuildTlv (char * buffer_snd, TSTlvProd *tlvInfo);
int  PutProdTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length);


#endif /** HPS_TLV_PROD_H **/
