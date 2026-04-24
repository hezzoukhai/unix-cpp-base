#ifndef HPS_ISO_GB_H
#define HPS_ISO_GB_H

#include <iso_com.h>
#include <header_gb.h>
#include <gb/gb_fields.h>

/***** Types de Champs  ******/

#define   GB_FIX_ALPHA     0
#define   GB_FIX_BIN       1
#define   GB_LL_ALPHA      2
#define   GB_LL_BIN        3
#define   GB_LLL_ALPHA     4
#define   GB_LLLL_ALPHA    5 
#define   GB_LLL_BIN       5
#define   GB_VAR_BITMAP    6
#define   GB_FIX_BITMAP    7
#define   GB_FIX_HEX       8
#define   GB_LL_HEX        9
#define   GB_LLL_HEX       10
/** HMA - 22052013 **/
#define   GB_LL_ALPHA_N    11
#define   GB_L_ALPHA       12
#define   GB_FIX_ALPHA_N   13

/**** Longueur de champs  ****/
#define GB_BITMAP_LEN      16
#define MAX_GB_FIELDS      128
#define MAX_GB_DATA_LEN    2048


typedef struct SGBInfo
{
   int           nFieldPos    [ MAX_GB_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   TSGBHeader    sHeader;
   char          sBitMap      [ GB_BITMAP_LEN   ];
   char          sData        [ MAX_GB_DATA_LEN ];
}TSGBInfo;

void  BuildPosGBMode ( char *IsoPosMode , char *sFunctionCode, char *gbPosMode );

void  InitGBInfo(TSGBInfo * msgInfo);

int   AnalyseGB    	(char * buffer_rec, TSGBInfo * msgInfo);
int   GetGBField   	(int    field_no,   TSGBInfo * msgInfo, char * data, int *length);
int   AddGBField   	(int    field_no,   TSGBInfo * msgInfo, char * data, int length, int nCompress); /** HMA - 21052013 **/
int   InsertGBField   	(int    field_no,   TSGBInfo * msgInfo, char * data, int length);
int   PutGBField   	(int    field_no,   TSGBInfo * msgInfo, char * data, int length);
int   GBBuildMsg 	(char * buffer_snd, TSGBInfo *msgInfo);

#endif /** HPS_ISO_GB_H **/
