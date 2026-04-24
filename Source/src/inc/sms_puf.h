#ifndef HPS_SMS_PUF_H
#define HPS_SMS_PUF_H

#include <iso_com.h>
/***** Types de Champs  ******/
#define   SMS_PUF_FIX_BCD       0
#define   SMS_PUF_FIX_ALPHA     1
#define   SMS_PUF_FIX_BIN       2
#define   SMS_PUF_VAR_BCD       3
#define   SMS_PUF_VAR_SBCD      4
#define   SMS_PUF_VAR_ALPHA     5
#define   SMS_PUF_VAR_BIN       6
#define   SMS_PUF_VAR_BITMAP    7
#define   SMS_PUF_FIX_BITMAP    8
#define   SMS_PUF_FIX_HEX       9
#define   SMS_PUF_VAR_HEX      10

/**** Longueur de champs  ****/
#define SMS_PUF_BITMAP_LEN        8
#define MAX_SMS_PUF_FIELDS       64
#define MAX_SMS_PUF_DATA_LEN    512

typedef struct SSmsPufInfo
{
   int           nFieldPos    [ MAX_SMS_PUF_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ SMS_PUF_BITMAP_LEN   ];
   char          sData        [ MAX_SMS_PUF_DATA_LEN ];
}TSSmsPufInfo;

void  InitSmsPufInfo(TSSmsPufInfo * pufInfo);

int   AnalyseSmsPuf     (char * buffer_rec, TSSmsPufInfo * pufInfo);
int   GetSmsPufSubField (int    field_no,   TSSmsPufInfo * pufInfo, 
                                                   char * data, int *length);
int   AddSmsPufSubField (int    field_no,   TSSmsPufInfo * pufInfo, 
                                                   char * data, int length);
int   SmsPufBuildMsg    ( char * buffer_snd,   TSSmsPufInfo * pufInfo);

#endif /** HPS_SMS_PUF_H **/
