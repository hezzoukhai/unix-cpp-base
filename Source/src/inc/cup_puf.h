#ifndef HPS_CUP_PUF_H
#define HPS_CUP_PUF_H

#include <iso_com.h>
/***** Types de Champs  ******/
#define   CUP_PUF_FIX_BCD       0
#define   CUP_PUF_FIX_ALPHA     1
#define   CUP_PUF_FIX_BIN       2
#define   CUP_PUF_VAR_BCD       3
#define   CUP_PUF_VAR_SBCD      4
#define   CUP_PUF_VAR_ALPHA     5
#define   CUP_PUF_VAR_BIN       6
#define   CUP_PUF_VAR_BITMAP    7
#define   CUP_PUF_FIX_BITMAP    8
#define   CUP_PUF_FIX_HEX       9
#define   CUP_PUF_VAR_HEX      10

/**** Longueur de champs  ****/
#define CUP_PUF_BITMAP_LEN        8
#define MAX_CUP_PUF_FIELDS       64
#define MAX_CUP_PUF_DATA_LEN    512

typedef struct SCupPufInfo
{
   int           nFieldPos    [ MAX_CUP_PUF_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ CUP_PUF_BITMAP_LEN   ];
   char          sData        [ MAX_CUP_PUF_DATA_LEN ];
}TSCupPufInfo;

void  InitCupPufInfo(TSCupPufInfo * pufInfo);
int   AnalyseCupPuf     (char * buffer_rec, TSCupPufInfo * pufInfo);
int   GetCupPufSubField (int    field_no,   TSCupPufInfo * pufInfo, char * data, int *length);
int   AddCupPufSubField (int    field_no,   TSCupPufInfo * pufInfo, char * data, int length);
int   CupPufBuildMsg    ( char * buffer_snd,   TSCupPufInfo * pufInfo);

#endif /** HPS_CUP_PUF_H **/
