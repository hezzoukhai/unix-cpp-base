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

#ifndef PWC_ISO_UAE_SWI__H
#define PWC_ISO_UAE_SWI__H


#include <iso_com.h>


#define   UAESWITCH_FIX_BCD       0
#define   UAESWITCH_FIX_ALPHA     1
#define   UAESWITCH_FIX_BIN       2
#define   UAESWITCH_VAR_BCD       3
#define   UAESWITCH_VAR_SBCD      4
#define   UAESWITCH_VAR_ALPHA     5
#define   UAESWITCH_VAR_BIN       6
#define   UAESWITCH_VAR_BITMAP    7
#define   UAESWITCH_FIX_BITMAP    8
#define   UAESWITCH_FIX_HEX       9
#define   UAESWITCH_VAR_HEX      10


#define UAESWITCH_BITMAP_LEN           24
#define UAESWITCH_HEADER_LEN           21
#define MAX_UAESWITCH_FIELDS          192
#define MAX_UAESWITCH_DATA_LEN       2048


#define   UAESWITCH_TAG_FIX_BIN       0
#define   UAESWITCH_TAG_VAR_BIN       1
#define   UAESWITCH_TAG_FIX_BCD       2
#define   UAESWITCH_TAG_VAR_BCD       3
#define   UAESWITCH_TAG_FIX_ALPHA     4
#define   UAESWITCH_TAG_VAR_ALPHA     5
#define   UAESWITCH_TAG_VAR_HEX       6

#define  MAX_UAESWITCH_CHIP_TAG       100
#define  MAX_UAESWITCH_CHIP_LEN       999


typedef struct SUaeSwitchInfo
{
   int            nFieldPos    [ MAX_UAESWITCH_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ UAESWITCH_HEADER_LEN + 1 ];
   char           sBitMap      [ UAESWITCH_BITMAP_LEN   ];
   char           sData        [ MAX_UAESWITCH_DATA_LEN ];
}TSUaeSwitchInfo;


typedef struct STagUaeSwitch
{
   int  nPresent  [ MAX_UAESWITCH_CHIP_TAG ];
   int  nPosTag   [ MAX_UAESWITCH_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_UAESWITCH_CHIP_LEN ];
}TSTagUaeSwitch;

void  InitUaeSwitchInfo(TSUaeSwitchInfo * msgInfo);


#endif


