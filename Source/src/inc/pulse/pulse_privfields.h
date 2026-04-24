#ifndef HPS_ISO_PD_H
#define HPS_ISO_PD_H

#include <iso_com.h>
/***** Types de Champs  ******/
#define   PD_FIX_BCD       0
#define   PD_FIX_ALPHA     1
#define   PD_FIX_BIN       2
#define   PD_VAR_BCD       3
#define   PD_VAR_SBCD      4
#define   PD_VAR_ALPHA     5
#define   PD_VAR_BIN       6
#define   PD_VAR_BITMAP    7
#define   PD_FIX_BITMAP    8
#define   PD_FIX_HEX       9
#define   PD_VAR_HEX      10

#define PD_PSEUDO_TERM    0
#define PD_ADVICE_REASON	1
#define PD_REVERSAL_REASON	0

/**** Longueur de champs  ****/
#define PD_BITMAP_LEN        8
#define MAX_PD_FIELDS       64
#define MAX_PD_DATA_LEN    512

typedef struct SPdInfo
{
   int           nFieldPos    [ MAX_PD_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ PD_BITMAP_LEN   ];
   char          sData        [ MAX_PD_DATA_LEN ];
}TSPdInfo;

void  InitPdInfo(TSPdInfo * pdInfo);

int   AnalysePd     (char * buffer_rec, TSPdInfo * pdInfo);
int   GetPdSubField (int    field_no,   TSPdInfo * pdInfo, 
                                                   char * data, int *length);
int   AddPdSubField (int    field_no,   TSPdInfo * pdInfo, 
                                                   char * data, int length);

int   PdBuildMsg    ( char * buffer_snd,   TSPdInfo * msgInfo);

#endif /** HPS_ISO_PD_H **/
