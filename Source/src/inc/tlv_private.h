#ifndef HPS_TLV_PRIVATE_H
#define HPS_TLV_PRIVATE_H
#include <tlv_com.h>
#include <tlv_privfields.h>

/**************************************/
#define   PRIVATE_TLV_FIX           0
#define   PRIVATE_TLV_VAR           1

/********************/
#define  MAX_PRIVATE_TLV		999
#define  MAX_PRIVATE_TLV_LEN	2048

#define	MAX_USER_PRIVATE_TLV	256
#define MAX_TLV_TAG_LEN			6

typedef struct STlvPrivate
{
   int  nPresent  [ MAX_PRIVATE_TLV ];
   int  nPosTlv   [ MAX_PRIVATE_TLV ];
   int  nLength;
   char sTlvData  [ MAX_PRIVATE_TLV_LEN ];
}TSTlvPrivate;


void InitTlvPrivate(TSTlvPrivate * tlvInfo);
int  AnalyseTlvPrivate(char *buffer,int nLength,TSTlvPrivate *tlvInfo);
int  GetTlvPrivate (char *tlv_name, TSTlvPrivate *tlvInfo,char *data, int *length);
int  AddTlvPrivate (char *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length);
int  PutTlvPrivate (char *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length);
int  PrivateBuildTlv (char * buffer_snd, TSTlvPrivate *tlvInfo);

#endif /** HPS_TLV_PRIVATE_H **/
