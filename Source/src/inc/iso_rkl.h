#ifndef HPS_ISO_RKL_H
#define HPS_ISO_RKL_H

/*#include <header_rkl.h>*/
#include <rkl_fields.h>
#include <iso_com.h>
#include <iso_cps.h>
#include <iso_puf.h>
/***** Types de Champs  ******/
#define   RKL_FIX_BCD       0
#define   RKL_FIX_ALPHA     1
#define   RKL_FIX_BIN       2
#define   RKL_VAR_BCD       3
#define   RKL_VAR_SBCD      4
#define   RKL_VAR_ALPHA     5
#define   RKL_VAR_BIN       6
#define   RKL_VAR_BITMAP    7
#define   RKL_FIX_BITMAP    8
#define   RKL_FIX_HEX       9
#define   RKL_VAR_HEX      10

/**** Longueur de champs  ****/
#define RKL_BITMAP_LEN           24
#define MAX_RKL_FIELDS          192
#define MAX_RKL_DATA_LEN       2048

typedef struct SRklInfo
{
   int            nFieldPos    [ MAX_RKL_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   /*TSRklHeader   sHeader;*/
   char           sBitMap      [ RKL_BITMAP_LEN   ];
   char           sData        [ MAX_RKL_DATA_LEN ];
}TSRklInfo;

void  InitRklInfo(TSRklInfo * msgInfo);

int   AnalyseRkl    (char * buffer_rec, TSRklInfo * msgInfo);
int   GetRklField   (int    field_no,   TSRklInfo * msgInfo, 
                                                   char * data, int *length);
int   InsertRklField(int    field_no,   TSRklInfo * msgInfo, 
                                                   char * data, int length);
int   PutRklField   (int    field_no,   TSRklInfo * msgInfo, 
                                                   char * data, int length);
int   AddRklField   (int    field_no,   TSRklInfo * msgInfo, 
                                                   char * data, int length);
int   RklBuildMsg   ( char * buffer_snd,   TSRklInfo * msgInfo);

/*void RklBuildHeader(int      nReqResp,
                     TSRklHeader*   pNewHeader,
                     TSRklHeader*   pIncomingHeader,
                     int nRklMsgLength,
                     char * szStationId);*/

#endif /** HPS_ISO_RKL_H **/
