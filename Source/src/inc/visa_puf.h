#ifndef HPS_VISA_PUF_H
#define HPS_VISA_PUF_H

#include <iso_com.h>
/***** Types de Champs  ******/
#define   VISA_PUF_FIX_BCD       0
#define   VISA_PUF_FIX_ALPHA     1
#define   VISA_PUF_FIX_BIN       2
#define   VISA_PUF_VAR_BCD       3
#define   VISA_PUF_VAR_SBCD      4
#define   VISA_PUF_VAR_ALPHA     5
#define   VISA_PUF_VAR_BIN       6
#define   VISA_PUF_VAR_BITMAP    7
#define   VISA_PUF_FIX_BITMAP    8
#define   VISA_PUF_FIX_HEX       9
#define   VISA_PUF_VAR_HEX      10

/**** Longueur de champs  ****/
#define VISA_PUF_BITMAP_LEN        8
#define MAX_VISA_PUF_FIELDS       64
#define MAX_VISA_PUF_DATA_LEN    512

typedef struct SVisaPufInfo
{
   int           nFieldPos    [ MAX_VISA_PUF_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ VISA_PUF_BITMAP_LEN   ];
   char          sData        [ MAX_VISA_PUF_DATA_LEN ];
}TSVisaPufInfo;

void  InitVisaPufInfo(TSVisaPufInfo * pufInfo);

int   AnalyseVisaPuf     (char * buffer_rec, TSVisaPufInfo * pufInfo);
int   GetVisaPufSubField (int    field_no,   TSVisaPufInfo * pufInfo, 
                                                   char * data, int *length);
int   AddVisaPufSubField (int    field_no,   TSVisaPufInfo * pufInfo, 
                                                   char * data, int length);
int   VisaPufBuildMsg    ( char * buffer_snd,   TSVisaPufInfo * pufInfo);

#endif /** HPS_VISA_PUF_H **/
