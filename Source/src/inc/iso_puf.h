#ifndef HPS_ISO_PUF_H
#define HPS_ISO_PUF_H

#include <puf_fields.h>
#include <iso_com.h>
/***** Types de Champs  ******/
#define   PUF_FIX_BCD       0
#define   PUF_FIX_ALPHA     1
#define   PUF_FIX_BIN       2
#define   PUF_VAR_BCD       3
#define   PUF_VAR_SBCD      4
#define   PUF_VAR_ALPHA     5
#define   PUF_VAR_BIN       6
#define   PUF_VAR_BITMAP    7
#define   PUF_FIX_BITMAP    8
#define   PUF_FIX_HEX       9
#define   PUF_VAR_HEX      10

/**** Longueur de champs  ****/
#define PUF_BITMAP_LEN        3
#define MAX_PUF_FIELDS       64
#define MAX_PUF_DATA_LEN    512

typedef struct SPufInfo
{
   int           nFieldPos    [ MAX_PUF_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ PUF_BITMAP_LEN   ];
   char          sData        [ MAX_PUF_DATA_LEN ];
}TSPufInfo;

void  InitPufInfo(TSPufInfo * pufInfo);

int   AnalysePuf     (char * buffer_rec, TSPufInfo * pufInfo);
int   GetPufSubField (int    field_no,   TSPufInfo * pufInfo, 
                                                   char * data, int *length);
int   AddPufSubField (int    field_no,   TSPufInfo * pufInfo, 
                                                   char * data, int length);
int   PufBuildMsg    ( char * buffer_snd,   TSPufInfo * pufInfo);

#endif /** HPS_ISO_PUF_H **/
