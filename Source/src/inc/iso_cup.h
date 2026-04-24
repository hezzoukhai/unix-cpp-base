#ifndef HPS_CUP_HPS_H
#define HPS_CUP_HPS_H

#include <iso_com.h>
#include <cup_fields.h>
#include <iso_ictag.h>
#include <header_cup.h>

/***** Types de Champs  ******/
#define   CUP_FIX_HEX       0
#define   CUP_FIX_ALPHA     1
#define   CUP_FIX_BIN       2
#define   CUP_LL_HEX        3
#define   CUP_LL_ALPHA      4
#define   CUP_LL_BIN        5
#define   CUP_LLL_HEX       6
#define   CUP_LLL_ALPHA     7
#define   CUP_FIX_BITMAP    8
#define   CUP_LLL_BIN       9
#define   CUP_CD_ALPHA       10
/**** Longueur de champs  ****/
#define CUP_BITMAP_LEN           24
#define MAX_CUP_FIELDS          128
#define MAX_CUP_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_CUP_CHIP_TAG       100
#define  MAX_CUP_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   CUP_TAG_FIX_BIN       0
#define   CUP_TAG_VAR_BIN       1
#define   CUP_TAG_FIX_BCD       2
#define   CUP_TAG_VAR_BCD       3
#define   CUP_TAG_FIX_ALPHA     4
#define   CUP_TAG_VAR_ALPHA     5
#define   CUP_TAG_VAR_HEX       6


typedef struct SCupInfo
{
   int            nFieldPos    [ MAX_CUP_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   TSCupHeader    sHeader;
   char           sBitMap      [ CUP_BITMAP_LEN   ];
   unsigned char           sData        [ MAX_CUP_DATA_LEN ]; /*OBE121106*FIELD 55 MANAGEMENT***/
   msg_id_t       msgId;/*PLUTONL-3762*/
}TSCupInfo;


typedef struct STagCup
{
   int  nPresent  [ MAX_CUP_CHIP_TAG ];
   int  nPosTag   [ MAX_CUP_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_CUP_CHIP_LEN ];
}TSTagCup;

void  InitCupInfo(TSCupInfo * msgInfo);

int   AnalyseCup    (unsigned char * buffer_rec, TSCupInfo * msgInfo);
int   GetCupField   (int    field_no,   TSCupInfo * msgInfo, 
                                                  char * data, int *length);
int   AddCupField   (int    field_no,   TSCupInfo * msgInfo, 
                                                  char * data, int length);
int   InsertCupField   (int    field_no,   TSCupInfo * msgInfo, 
                                                  char * data, int length);
int   PutCupField   (int    field_no,   TSCupInfo * msgInfo, 
                                                  char * data, int length);
int   CupBuildMsg (char * buffer_snd, TSCupInfo *msgInfo);

void  InitCupIcTag(TSTagCup * msgInfo);
int   AnalyseCupIc(char * buffer, int nLength, TSTagCup * msgInfo);
int   GetCupIcTag (char *tag_name, TSTagCup * msgInfo, char *data, int *length);
int   AddCupIcTag (char *tag_name, TSTagCup * msgInfo, char *data, int length);
int   PutCupIcTag (char *tag_name, TSTagCup * msgInfo, char *data, int length);

#endif /** HPS_CUP_HPS_H **/
