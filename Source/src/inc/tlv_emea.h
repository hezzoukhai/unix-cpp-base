#ifndef HPS_TLV_EMEA_H
#define HPS_TLV_EMEA_H

#include <tlv_com.h>
#include <tlv_emeafields.h>

/**************************************/
#define   EMEA_TLV_FIX           0
#define   EMEA_TLV_VAR           1
/**************************************/

#define  MAX_EMEA_TLV       999
#define  MAX_EMEA_TLV_LEN  4098

typedef struct STlvEmea
{
   int  nPresent  [ MAX_EMEA_TLV ];
   int  nPosTlv   [ MAX_EMEA_TLV ];
   int  nLength;
   char sTlvData  [ MAX_EMEA_TLV_LEN ];
}
TSTlvEmea;

void InitTlvEmea(TSTlvEmea * tlvInfo);
int  AnalyseTlvEmea(char *buffer,TSTlvEmea *tlvInfo);
int  GetTlvEmea (char *tlv_name, TSTlvEmea *tlvInfo,char *data, int *length);
int  AddTlvEmea (char *tlv_name, TSTlvEmea *tlvInfo, char *data, int length);
int  PutTlvEmea (char *tlv_name, TSTlvEmea *tlvInfo, char *data, int length);
int  EmeaBuildTlv (char * buffer_snd, TSTlvEmea *tlvInfo);
int  PutTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length);


#endif /** HPS_TLV_EMEA_H **/
