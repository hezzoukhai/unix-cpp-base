#ifndef HPS_ISO_CPS_H
#define HPS_ISO_CPS_H

#include <cps_fields.h>
#include <iso_com.h>
/***** Types de Champs  ******/
#define   CPS_FIX_BCD       0
#define   CPS_FIX_ALPHA     1
#define   CPS_FIX_BIN       2
#define   CPS_VAR_BCD       3
#define   CPS_VAR_SBCD      4
#define   CPS_VAR_ALPHA     5
#define   CPS_VAR_BIN       6
#define   CPS_VAR_BITMAP    7
#define   CPS_FIX_BITMAP    8
#define   CPS_FIX_HEX       9
#define   CPS_VAR_HEX      10

/**** Longueur de champs  ****/
#define CPS_BITMAP_LEN        8
#define MAX_CPS_FIELDS       64
#define MAX_CPS_DATA_LEN    512

typedef struct SCpsInfo
{
   int           nFieldPos    [ MAX_CPS_FIELDS  +1 ];
   int           nLength;
   char          sBitMap      [ CPS_BITMAP_LEN   ];
   char          sData        [ MAX_CPS_DATA_LEN ];
}TSCpsInfo;

void  InitCpsInfo(TSCpsInfo * cpsInfo);

int   AnalyseCps     (char * buffer_rec, TSCpsInfo * cpsInfo);
int   GetCpsSubField (int    field_no,   TSCpsInfo * cpsInfo, 
                                                   char * data, int *length);
int   AddCpsSubField (int    field_no,   TSCpsInfo * cpsInfo, 
                                                   char * data, int length);

int   CpsBuildMsg    ( char * buffer_snd,   TSCpsInfo * msgInfo);

#endif /** HPS_ISO_CPS_H **/
