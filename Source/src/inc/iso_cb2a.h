#ifndef HPS_CB2A_HPS_H
#define HPS_CB2A_HPS_H

#include <iso_com.h>
#include <cb2a_fields.h>
#include <cb2a_aut_fields.h>

/***** Types de Champs  ******/
#define   CB2A_FIX_HEX       0
#define   CB2A_FIX_ALPHA     1
#define   CB2A_FIX_BIN       2
#define   CB2A_FIX_BCD       3
#define   CB2A_LL_HEX        4
#define   CB2A_LL_BCD        5
#define   CB2A_LL_ALPHA      6
#define   CB2A_LL_BIN        7
#define   CB2A_LLL_BCD       8
#define   CB2A_LLL_HEX       9
#define   CB2A_LLL_ALPHA    10
#define   CB2A_LLL_BIN      11
#define   CB2A_FIX_EBCDIC   12

/**** Longueur de champs  ****/
#define CB2A_BITMAP_LEN           16
#define MAX_CB2A_FIELDS          128
#define MAX_CB2A_DATA_LEN       4096

/**** CHIP DATA   ****/
#define  MAX_CB2A_CHIP_TAG       100
#define  MAX_CB2A_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   CB2A_TAG_FIX_BIN       0
#define   CB2A_TAG_VAR_BIN       1
#define   CB2A_TAG_FIX_BCD       2
#define   CB2A_TAG_VAR_BCD       3
#define   CB2A_TAG_FIX_ALPHA     4
#define   CB2A_TAG_VAR_ALPHA     5
#define   CB2A_TAG_VAR_HEX       6

#define CB2A_PROTO_TTN			0		/* TLP/TLC/Network managemnt */
#define CB2A_PROTO_AUT			1


typedef struct SCb2aInfo
{
   int           nFieldPos    [ MAX_CB2A_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ CB2A_BITMAP_LEN   ];
   char          sData        [ MAX_CB2A_DATA_LEN ];
   int			 nProtocol;
}TSCb2aInfo;

typedef struct STagCb2a
{
   int  nPresent  [ MAX_CB2A_CHIP_TAG ];
   int  nPosTag   [ MAX_CB2A_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_CB2A_CHIP_LEN ];
}TSTagCb2a;

extern void  InitCb2aInfo(TSCb2aInfo * msgInfo);
int   AnalyseCb2a    (char * buffer_rec, TSCb2aInfo * msgInfo );
int   GetCb2aField   (int    field_no,   TSCb2aInfo * msgInfo, 
                                                  char * data, int *length);
int   AddCb2aField   (int    field_no,   TSCb2aInfo * msgInfo, 
                                                  char * data, int length);
int   InsertCb2aField   (int    field_no,   TSCb2aInfo * msgInfo, 
                                                  char * data, int length);
int   PutCb2aField   (int    field_no,   TSCb2aInfo * msgInfo, 
                                                  char * data, int length);
int   Cb2aBuildMsg (char * buffer_snd, TSCb2aInfo *msgInfo);

void  InitCb2aIcTag(TSTagCb2a * msgInfo);
int   AnalyseCb2aIc(char * buffer, int nLength, TSTagCb2a * msgInfo);
int   GetCb2aIcTag (char *tag_name, TSTagCb2a * msgInfo, char *data, int *length);
int   AddCb2aIcTag (char *tag_name, TSTagCb2a * msgInfo, char *data, int length);
int   PutCb2aIcTag (char *tag_name, TSTagCb2a * msgInfo, char *data, int length);
#endif /** HPS_CB2A_HPS_H **/
