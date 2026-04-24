#ifndef HPS_ISO_T24_H
#define HPS_ISO_T24_H

#include <iso_com.h>
#include <bank_fields.h>
#include <bank_ictag.h>

/***** Types de Champs  ******/
#define   T24_FIX_HEX       0
#define   T24_FIX_ALPHA     1
#define   T24_FIX_BIN       2
#define   T24_LL_HEX        3
#define   T24_LL_ALPHA      4
#define   T24_LL_BIN        5
#define   T24_LLL_HEX       6
#define   T24_LLL_ALPHA     7
#define   T24_LLL_BIN       8

/**** Longueur de champs  ****/
#define T24_BITMAP_LEN           16
#define T24_HDR_LEN              15
#define MAX_T24_FIELDS          128
#define MAX_T24_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_T24_CHIP_TAG       100
#define  MAX_T24_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   T24_TAG_FIX_BIN       0
#define   T24_TAG_VAR_BIN       1
#define   T24_TAG_FIX_BCD       2
#define   T24_TAG_VAR_BCD       3
#define   T24_TAG_FIX_ALPHA     4
#define   T24_TAG_VAR_ALPHA     5
#define   T24_TAG_VAR_HEX       6

typedef struct ST24Info
{
   int           nFieldPos    [ MAX_T24_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sHeader      [ T24_HDR_LEN + 1 ]; 
   char          sBitMap      [ T24_BITMAP_LEN   ];
   char          sData        [ MAX_T24_DATA_LEN ];
}TST24Info;

typedef struct STagT24
{
   int  nPresent  [ MAX_T24_CHIP_TAG ];
   int  nPosTag   [ MAX_T24_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_T24_CHIP_LEN ];
}TSTagT24;

void  InitT24Info(TST24Info * msgInfo);

int   AnalyseT24    (char * buffer_rec, TST24Info * msgInfo);
int   GetT24Field   (int    field_no,   TST24Info * msgInfo, 
                                                  char * data, int *length);
int   AddT24Field   (int    field_no,   TST24Info * msgInfo, 
                                                  char * data, int length);
int   InsertT24Field   (int    field_no,   TST24Info * msgInfo, 
                                                  char * data, int length);
int   PutT24Field   (int    field_no,   TST24Info * msgInfo, 
                                                  char * data, int length);
int   T24BuildMsg (char * buffer_snd, TST24Info *msgInfo);

int   T24BuildHeader(char*   pNewHeader,int sLen , TST24Info   *T24Info);

void  InitT24IcTag(TSTagT24 * msgInfo);
int   AnalyseT24Ic(char * buffer, int nLength, TSTagT24 * msgInfo);
int   GetT24IcTag (char *tag_name, TSTagT24 * msgInfo, char *data, int *length);
int   AddT24IcTag (char *tag_name, TSTagT24 * msgInfo, char *data, int length);
int   PutT24IcTag (char *tag_name, TSTagT24 * msgInfo, char *data, int length);

#endif /** HPS_ISO_T24_H **/
