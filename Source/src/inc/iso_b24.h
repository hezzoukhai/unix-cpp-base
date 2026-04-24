#ifndef HPS_ISO_B24_H
#define HPS_ISO_B24_H

#include <iso_com.h>
#include <b24_header.h>
/***** Types de Champs  ******/

/***** Types de Champs  ******/
#define   B24_FIX_HEX       0
#define   B24_FIX_ALPHA     1
#define   B24_FIX_BIN       2
#define   B24_LL_HEX        3
#define   B24_LL_ALPHA      4
#define   B24_LL_BIN        5
#define   B24_LLL_HEX       6
#define   B24_LLL_ALPHA     7
#define   B24_LLL_BIN       8
#define   B24_VAR_ALPHA     9


/**** Longueur de champs  ****/
#define B24_BITMAP_LEN           16
#define MAX_B24_FIELDS          128
#define MAX_B24_DATA_LEN       2048

typedef struct SB24Info
{
   int            nFieldPos    [ MAX_B24_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   TSB24Header    sHeader;
   char           sBitMap      [ B24_BITMAP_LEN   ];
   char           sData        [ MAX_B24_DATA_LEN ];
}TSB24Info;

void  InitB24Info(TSB24Info * msgInfo);

int   AnalyseB24    (char * buffer_rec, TSB24Info * msgInfo);
int   GetB24Field   (int    field_no,   TSB24Info * msgInfo, 
                                                   char * data, int *length);
int   InsertB24Field(int    field_no,   TSB24Info * msgInfo, 
                                                   char * data, int length);
int   PutB24Field   (int    field_no,   TSB24Info * msgInfo, 
                                                   char * data, int length);
int   AddB24Field   (int    field_no,   TSB24Info * msgInfo, 
                                                   char * data, int length);
int   B24BuildMsg   ( char * buffer_snd,   TSB24Info * msgInfo);

#endif /** HPS_ISO_B24_H **/
