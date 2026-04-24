#ifndef HPS_TLV_AUTHO_H
#define HPS_TLV_AUTHO_H
#include <tlv_com.h>
#include <tlv_fields.h>

/**************************************/
#define   AUTHO_TLV_FIX           0
#define   AUTHO_TLV_VAR           1
/**************************************/

#define  MAX_AUTHO_TLV       999
#define  MAX_AUTHO_TLV_LEN  4098

typedef struct STlvAutho
{
   int  nPresent  [ MAX_AUTHO_TLV ];
   int  nPosTlv   [ MAX_AUTHO_TLV ];
   int  nLength;
   char sTlvData  [ MAX_AUTHO_TLV_LEN ];
}
TSTlvAutho;

void InitTlvAutho(TSTlvAutho * tlvInfo);
int  AnalyseTlvAutho(char *buffer,TSTlvAutho *tlvInfo);
int  GetTlvAutho (char *tlv_name, TSTlvAutho *tlvInfo,char *data, int *length);
int  AddTlvAutho (char *tlv_name, TSTlvAutho *tlvInfo, char *data, int length);
int  PutTlvAutho (char *tlv_name, TSTlvAutho *tlvInfo, char *data, int length);
int  AuthoBuildTlv (char * buffer_snd, TSTlvAutho *tlvInfo);
int  PutTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length);
int  PutEvent(char *sTlvData, char *sEvent, char *sMsg);



#endif /** HPS_TLV_AUTHO_H **/
