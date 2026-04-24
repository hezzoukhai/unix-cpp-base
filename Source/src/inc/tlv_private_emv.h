/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      tlv_private_emv.h                                                      */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   25/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#ifndef HPS_TLV_PRIVATE_H
#define HPS_TLV_PRIVATE_H
#include <tlv_com.h>
#include <tlv_privfields.h>

/**************************************/
#define   PRIVATE_TLV_FIX           0
#define   PRIVATE_TLV_VAR           1

/********************/
#define  MAX_PRIVATE_TLV       999
#define  MAX_PRIVATE_TLV_LEN  4000 

typedef struct STlvPrivate
{
   int  nPresent  [ MAX_PRIVATE_TLV ];
   int  nPosTlv   [ MAX_PRIVATE_TLV ];
   int  nLength;
   char sTlvData  [ MAX_PRIVATE_TLV_LEN ];
}TSTlvPrivate;


void InitTlvPrivateEmv(TSTlvPrivate * tlvInfo);
int  AnalyseTlvPrivateEmv(char *buffer,int nLength,TSTlvPrivate *tlvInfo);
int  GetTlvPrivateEmv (char *tlv_name, TSTlvPrivate *tlvInfo,char *data, int *length);
int  AddTlvPrivateEmv (char *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length);
int  PutTlvPrivateEmv (char *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length);
int  PrivateBuildTlvEmv (char * buffer_snd, TSTlvPrivate *tlvInfo);

#endif /** HPS_TLV_PRIVATE_H **/
