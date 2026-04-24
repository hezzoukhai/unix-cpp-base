#ifndef HPS_ISO_VISA_H
#define HPS_ISO_VISA_H

#include <header_visa.h>
#include <visa_fields.h>
#include <iso_com.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <iso8583.h>
/***** Types de Champs  ******/
#define   VISA_FIX_BCD       0
#define   VISA_FIX_ALPHA     1
#define   VISA_FIX_BIN       2
#define   VISA_VAR_BCD       3
#define   VISA_VAR_SBCD      4
#define   VISA_VAR_ALPHA     5
#define   VISA_VAR_BIN       6
#define   VISA_VAR_BITMAP    7
#define   VISA_FIX_BITMAP    8
#define   VISA_FIX_HEX       9
#define   VISA_VAR_HEX      10
#define   VISA_VAR2_BIN     11     /*IBO20190723 PROD00070403 Enh19.Q4 Art 9.1.3*/

/**** Longueur de champs  ****/
#define VISA_BITMAP_LEN           24
#define MAX_VISA_FIELDS          192
#define MAX_VISA_DATA_LEN       2048

/*Start MKB221208 CCD*/
#include <visa_ictag.h>

#define   VISA_TAG_FIX_BIN       0
#define   VISA_TAG_VAR_BIN       1
#define   VISA_TAG_FIX_BCD       2
#define   VISA_TAG_VAR_BCD       3
#define   VISA_TAG_FIX_ALPHA     4
#define   VISA_TAG_VAR_ALPHA     5
#define   VISA_TAG_VAR_HEX               6


#define  MAX_VISA_CHIP_TAG       100
#define  MAX_VISA_CHIP_LEN       999
/*End MKB221208 CCD*/

#define   VAR2_LEN_LEN		 2 				/*IBO20190723 PROD00070403 Enh19.Q4 Art 9.1.3*/

extern char  BASE1_REJ_NON3DS;  /*NAB07042020 PLUTONL-1477*/

typedef struct SVisaInfo
{
   int            nFieldPos    [ MAX_VISA_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   TSVisaHeader   sHeader;
   char           sBitMap      [ VISA_BITMAP_LEN   ];
   unsigned char  sData        [ MAX_VISA_DATA_LEN ]; /*OBE121106*FIELD 55 MANAGEMENT***/
   msg_id_t       msgId;/*PLUTONL-3762*/
   struct timeval kCtime; /*PLUTONL-4411*/
}TSVisaInfo;

/*Start MKB221208 CCD*/
typedef struct STagVisa
{
   int  nPresent  [ MAX_VISA_CHIP_TAG ];
   int  nPosTag   [ MAX_VISA_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_VISA_CHIP_LEN ];
}TSTagVisa;


void  InitVisaIcTag(TSTagVisa * tagInfo);
int   AnalyseVisaIc(char * buffer, int nLength, TSTagVisa * tagInfo);
int   GetVisaIcTag (char *tag_name, TSTagVisa *tagInfo,char *data, int *length);
int   AddVisaIcTag (char *tag_name, TSTagVisa *tagInfo, char *data, int length);
int   PutVisaIcTag (char *tag_name, TSTagVisa *tagInfo, char *data, int length);
int   VisaBuildIcFld (char * buffer_snd, TSTagVisa *tagInfo);

/*End MKB221208 CCD*/

void  InitVisaInfo(TSVisaInfo * msgInfo);

/*OBE121106*FIELD 55 MANAGEMENT***
int   AnalyseVisa    (char * buffer_rec, TSVisaInfo * msgInfo);
int   GetVisaField   (int    field_no,   TSVisaInfo * msgInfo, 
                                                   char * data, int *length);
*/
int   AnalyseVisa    (unsigned char * buffer_rec, TSVisaInfo * msgInfo);
int   GetVisaField   (int    field_no,   TSVisaInfo * msgInfo, 
                                                   unsigned char * data, int *length);

int   InsertVisaField(int    field_no,   TSVisaInfo * msgInfo, 
                                                   char * data, int length);
int   PutVisaField   (int    field_no,   TSVisaInfo * msgInfo, 
                                                   char * data, int length);
int   AddVisaField   (int    field_no,   TSVisaInfo * msgInfo, 
                                                   char * data, int length);
int   VisaBuildMsg   ( char * buffer_snd,   TSVisaInfo * msgInfo);

void VisaBuildHeader(int      nReqResp,
                     TSVisaHeader*   pNewHeader,
                     TSVisaHeader*   pIncomingHeader,
                     int nVisaMsgLength,
                     char * szStationId);



void InitVisaTransSpecDataTlv(TlvInfo* pkTlvInfo);
void InitVisaAppIdentDataTlv(TlvInfo* pkTlvInfo);
void InitVisaSenderDataTlv(TlvInfo* pkTlvInfo);
void InitB1VerifDataTlvInfo(TlvInfo* pkTlvInfo);
void InitB1AddrVerifDataTlvInfo(TlvInfo* pkTlvInfo);
void InitB1TokenDataTlvInfo(TlvInfo* pkTlvInfo);



#endif /** HPS_ISO_VISA_H **/
