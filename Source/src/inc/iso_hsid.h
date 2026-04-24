#ifndef HPS_HSID_HPS_H
#define HPS_HSID_HPS_H

#include <iso_com.h>
#include <hsid_fields.h>
#include <iso_ictag.h>
#include <iso_sid.h>

/***** Types de Champs  ******/
#define   HSID_FIX_HEX       0
#define   HSID_FIX_ALPHA     1
#define   HSID_FIX_BIN       2
#define   HSID_LL_HEX        3
#define   HSID_LL_ALPHA      4
#define   HSID_LL_BIN        5
#define   HSID_LLL_HEX       6
#define   HSID_LLL_ALPHA     7
#define   HSID_LLL_BIN       8

/**** Longueur de champs  ****/
#define HSID_BITMAP_LEN          SID_BITMAP_LEN
#define MAX_HSID_FIELDS          MAX_SID_FIELDS
#define MAX_HSID_DATA_LEN        MAX_SID_DATA_LEN


/**** CHIP DATA   ****/
#define  MAX_HSID_CHIP_TAG       100
#define  MAX_HSID_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   HSID_TAG_FIX_BIN       0
#define   HSID_TAG_VAR_BIN       1
#define   HSID_TAG_FIX_BCD       2
#define   HSID_TAG_VAR_BCD       3
#define   HSID_TAG_FIX_ALPHA     4
#define   HSID_TAG_VAR_ALPHA     5
#define   HSID_TAG_VAR_HEX       6

/*typedef struct SHSidInfo
{
   int           nFieldPos    [ MAX_HSID_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ HSID_BITMAP_LEN   ];
   char          sData        [ MAX_HSID_DATA_LEN ];
}TSSidInfo;

typedef struct STagHSid
{
   int  nPresent  [ MAX_HSID_CHIP_TAG ];
   int  nPosTag   [ MAX_HSID_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_HSID_CHIP_LEN ];
}TSTagSid;*/

void  InitHsidInfo(TSSidInfo * msgInfo);

int   AnalyseHsid    (char * buffer_rec, TSSidInfo * msgInfo);
int   GetHsidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int *length);
int   AddHsidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   InsertHsidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   PutHsidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   HsidBuildMsg (int nIndiceCtx, char * buffer_snd, TSSidInfo *msgInfo,char *BadField);

void  InitHsidIcTag(TSTagSid * msgInfo);
int   AnalyseHsidIc(char * buffer, int nLength, TSTagSid * msgInfo);
int   GetHsidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int *length);
int   AddHsidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int length);
int   PutHsidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int length);

#endif /** HPS_HSID_HPS_H **/
