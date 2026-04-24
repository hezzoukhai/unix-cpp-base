#ifndef ISO_BASE24_H
#define ISO_BASE24_H

#include <tlv_switch_header.h>

/* Field types */
#define   BASE24_FIX_BCD       0
#define   BASE24_FIX_ALPHA     1
#define   BASE24_FIX_BIN       2
#define   BASE24_FIX_BITMAP    3
#define   BASE24_FIX_HEX       4
#define   BASE24_VAR_BCD       5
#define   BASE24_VAR_SBCD      6
#define   BASE24_VAR_ALPHA     7
#define   BASE24_VAR_BIN       8
#define   BASE24_VAR_BITMAP    9
#define   BASE24_VAR_HEX      10
#define   BASE24_LL_ALPHA     11
#define   BASE24_LL_BIN       12
#define   BASE24_LL_HEX       13
#define   BASE24_LLL_ALPHA    14
#define   BASE24_LLL_BIN      15
#define   BASE24_LLL_HEX      16
#define   BASE24_UNDEF        99

/* Lengths */
#define BASE24_BITMAP_LEN				24
#define BASE24_HEADER_LEN				9
#define MAX_BASE24_FIELDS				128
#define MAX_BASE24_DATA_LEN			    2048


#define MAX_SWITCH_HEADER_LEN			256

/* ICC fields types */
/*
#define   BASE24_TAG_FIX_BIN       0
#define   BASE24_TAG_VAR_BIN       1
#define   BASE24_TAG_FIX_BCD       2
#define   BASE24_TAG_VAR_BCD       3
#define   BASE24_TAG_FIX_ALPHA     4
#define   BASE24_TAG_VAR_ALPHA     5
#define   BASE24_TAG_VAR_HEX       6
*/

/**** CHIP DATA   ****/
/*
#define  MAX_BASE24_CHIP_TAG       100
#define  MAX_BASE24_CHIP_LEN       999
*/
/********************NAB08102020 deplace it from base24******************/
enum {
        CSM_F_MCL       = 0     ,
        CSM_F_RCV               ,
        CSM_F_ORG               ,
        CSM_F_SVR               ,
        CSM_F_KD                ,
        CSM_F_CTP               ,
        CSM_F_ERF               ,

        CFS_F_QTY
};



typedef struct {
        char    sData[156];
        int             nPos[CFS_F_QTY];
        int             nLength[CFS_F_QTY];
        int             nPresent[CFS_F_QTY];
        int             nDataLength;
} TSCSMData;

int InitCMSData(TSCSMData*	CSMData);
int AnalyseCSM(char* sData, TSCSMData*	CSMData);
int GetCSMField(int nField, TSCSMData*	CSMData, char* data, int* length);
int AddCSMField(int nField, TSCSMData*	CSMData, char* data, int length);
int PutCSMField(int nField, TSCSMData*	CSMData, char* data, int length);
void DumpCSM(TSCSMData*	CSMData);
int BuildCSM(char* sData, TSCSMData*	CSMData);

#define LBL_CSM_F_MCL			"MCL"
#define LBL_CSM_F_RCV			"RCV"
#define LBL_CSM_F_ORG			"ORG"
#define LBL_CSM_F_SVR			"SVR"
#define LBL_CSM_F_KD			"KD"
#define LBL_CSM_F_CTP			"CTP"
#define LBL_CSM_F_ERF			"ERF"



#define LBL_CSM_F_MCL_LEN		3
#define LBL_CSM_F_RCV_LEN		3
#define LBL_CSM_F_ORG_LEN		3
#define LBL_CSM_F_SVR_LEN		3
#define LBL_CSM_F_KD_LEN		2
#define LBL_CSM_F_CTP_LEN		3
#define LBL_CSM_F_ERF_LEN		3


/********************NAB08102020******************/

typedef struct SBase24Header
{
    char    sProductIndicator   [2];
    char    sReleaseNumber      [2];
    char    sStatus             [3];
    char    sOriginatorCode;
    char    sResponderCode;

} TSBase24Header;

typedef struct SBase24Info
{
   int              nFieldPos    [ MAX_BASE24_FIELDS  +1 ];
   int              nMsgType;
   int              nLength;
   TSBase24Header   sHeader;
   int              nProductIndicator;
   char             sBitMap      [ BASE24_BITMAP_LEN   ];
   unsigned char    sData        [ MAX_BASE24_DATA_LEN ];
   TSTlvSwitchHeader	SwitchHeader;/*EBE131005*/
}TSBase24Info;

/*
typedef struct STagBase24
{
   int  nPresent  [ MAX_BASE24_CHIP_TAG ];
   int  nPosTag   [ MAX_BASE24_CHIP_TAG ];
   int  nLength;
   unsigned char sChipData [ MAX_BASE24_CHIP_LEN ];
}TSTagBase24;
*/


#endif

