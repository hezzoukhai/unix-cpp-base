/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef PWC_INT_IST__H
#define PWC_INT_IST__H


#include <iso_com.h>


#define   IST_FIX_BCD       0
#define   IST_FIX_ALPHA     1
#define   IST_FIX_BIN       2
#define   IST_VAR_BCD       3
#define   IST_VAR_SBCD      4
#define   IST_VAR_ALPHA     5
#define   IST_VAR_BIN       6
#define   IST_VAR_BITMAP    7
#define   IST_FIX_BITMAP    8
#define   IST_FIX_HEX       9
#define   IST_VAR_HEX      10
#define   IST_LLL_BIN      11
#define   IST_LLL_ALPHA      12


#define IST_BITMAP_LEN           24
#define IST_HEADER_LEN           21
#define MAX_IST_FIELDS          128
#define MAX_IST_DATA_LEN       2048


#define   IST_TAG_FIX_BIN       0
#define   IST_TAG_VAR_BIN       1
#define   IST_TAG_FIX_BCD       2
#define   IST_TAG_VAR_BCD       3
#define   IST_TAG_FIX_ALPHA     4
#define   IST_TAG_VAR_ALPHA     5
#define   IST_TAG_VAR_HEX       6
#define   IST_TAG_LLL_ALPHA     7

#define  MAX_IST_CHIP_TAG       100
#define  MAX_IST_CHIP_LEN       999


typedef struct SIstInfo
{
   int            nFieldPos    [ MAX_IST_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ IST_HEADER_LEN + 1 ];
   char           sBitMap      [ IST_BITMAP_LEN   ];
   char           sData        [ MAX_IST_DATA_LEN ];
} TSIstInfo;


typedef struct STagIst
{
   int  nPresent  [ MAX_IST_CHIP_TAG ];
   int  nPosTag   [ MAX_IST_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_IST_CHIP_LEN ];
} TSTagIst;

void  InitIstInfo(TSIstInfo * msgInfo);


#endif



