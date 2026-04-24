#ifndef HPS_ISO_SMS_H
#define HPS_ISO_SMS_H

#include <header_sms.h>
#include <sms_fields.h>
#include <iso_com.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <iso8583.h>
/***** Types de Champs  ******/
#define   SMS_FIX_BCD       0
#define   SMS_FIX_ALPHA     1
#define   SMS_FIX_BIN       2
#define   SMS_VAR_BCD       3
#define   SMS_VAR_SBCD      4
#define   SMS_VAR_ALPHA     5
#define   SMS_VAR_BIN       6
#define   SMS_VAR_BITMAP    7
#define   SMS_FIX_BITMAP    8
#define   SMS_FIX_HEX       9
#define   SMS_VAR_HEX      10
#define   SMS_VAR_SHEX      11
#define   SMS_VAR2_BIN     12     /*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/

/***** Types des TAG (Champ 55)  ******/
#define   SMS_TAG_FIX_BIN       0
#define   SMS_TAG_VAR_BIN       1
#define   SMS_TAG_FIX_BCD       2
#define   SMS_TAG_VAR_BCD       3
#define   SMS_TAG_FIX_ALPHA     4
#define   SMS_TAG_VAR_ALPHA     5
#define   SMS_TAG_VAR_HEX       6




/**** Longueur de champs  ****/
#define SMS_BITMAP_LEN           16
#define MAX_SMS_FIELDS          128
#define MAX_SMS_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_SMS_CHIP_TAG       100
#define  MAX_SMS_CHIP_LEN       999

#define   VAR2_LEN_LEN		 2 				/*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/

typedef struct SSmsInfo
{
   int           nFieldPos    [ MAX_SMS_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   TSSmsHeader   sHeader;
   char          sBitMap      [ SMS_BITMAP_LEN   ];
   char          sData        [ MAX_SMS_DATA_LEN ];
   msg_id_t      msgId;/*PLUTONL-3762*/
   struct timeval kCtime; /*PLUTONL-4411*/
}TSSmsInfo;

typedef struct STagSms
{
   int  nPresent  [ MAX_SMS_CHIP_TAG ];
   int  nPosTag   [ MAX_SMS_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_SMS_CHIP_LEN ];
}TSTagSms;


void  InitSmsInfo(TSSmsInfo * msgInfo);

int   AnalyseSms    (char * buffer_rec, TSSmsInfo * msgInfo);
int   GetSmsField   (int    field_no,   TSSmsInfo * msgInfo, 
                                                   char * data, int *length);
int   AddSmsField   (int    field_no,   TSSmsInfo * msgInfo, 
                                                   char * data, int length);
int   InsertSmsField   (int    field_no,   TSSmsInfo * msgInfo, 
                                                   char * data, int length);
int   PutSmsField   (int    field_no,   TSSmsInfo * msgInfo, 
                                                   char * data, int length);
int   SmsBuildMsg    ( char * buffer_snd,   TSSmsInfo * msgInfo);

void SmsBuildHeader(int      nReqResp,
                    TSSmsHeader*   pNewHeader,
                    TSSmsHeader*   pIncomingHeader,
                    int nSmsMsgLength,
                    char * szStationId);
					

void InitSmsVerifDataTlvInfo(TlvInfo* pkTlvInfo);
void InitSmsAddrVerifDataTlvInfo(TlvInfo* pkTlvInfo);
void InitSmsTokenDataTlvInfo(TlvInfo* pkTlvInfo);	
#endif /** HPS_ISO_SMS_H **/
