#ifndef HPS_ISO_PULSE_H
#define HPS_ISO_PULSE_H

#include <iso_com.h>
#include <pulse/pulse_fields.h>
#include <pulse/pulse_ictag.h>
/*#include <pulse_pdstag.h>*/

/***** Types de Champs  ******/

#define   PULSE_FIX_ALPHA     0
#define   PULSE_FIX_BIN       1
#define   PULSE_LL_ALPHA      2
#define   PULSE_LL_BIN        3
#define   PULSE_LLL_ALPHA     4
#define   PULSE_LLL_BIN       5
#define   PULSE_VAR_BITMAP      6
#define   PULSE_FIX_BITMAP    7
#define   PULSE_FIX_HEX       8
#define   PULSE_LL_HEX        9
#define   PULSE_LLL_HEX       10

/***** Types des TAG (Champ 55)  ******/
#define   PULSE_TAG_FIX_BIN       0
#define   PULSE_TAG_VAR_BIN       1
#define   PULSE_TAG_FIX_BCD       2
#define   PULSE_TAG_VAR_BCD       3
#define   PULSE_TAG_FIX_ALPHA     4
#define   PULSE_TAG_VAR_ALPHA     5
#define   PULSE_TAG_VAR_HEX       6

/***** Types des PDS (Champ 48)  ******/
#define   PULSE_PDS_FIX_ALPHA     0
#define   PULSE_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define PULSE_BITMAP_LEN           16
#define MAX_PULSE_FIELDS          128
#define MAX_PULSE_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_PULSE_CHIP_TAG       100
#define  MAX_PULSE_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_PULSE_PDS       100
#define  MAX_PULSE_PDS_LEN   999

typedef struct SPulseInfo
{
   int           nFieldPos    [ MAX_PULSE_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ PULSE_BITMAP_LEN   ];
   char          sData        [ MAX_PULSE_DATA_LEN ];
}TSPulseInfo;

typedef struct SPdsPulse
{
   int  nPresent  [ MAX_PULSE_PDS ];
   int  nPosPds   [ MAX_PULSE_PDS ];
   int  nLength;
   int  nMsgType;
   char sPdsData  [ MAX_PULSE_PDS_LEN ];
}TSPdsPulse;

typedef struct STagPulse
{
   int  nPresent  [ MAX_PULSE_CHIP_TAG ];
   int  nPosTag   [ MAX_PULSE_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_PULSE_CHIP_LEN ];
}TSTagPulse;

/*void  BuildPosPulseMode ( char *IsoPosMode , char *sFunctionCode, char *pulsePosMode );*/
void  BuildPulsePosEntryMode ( char *IsoPosData , char *sFunctionCode , char *sMerchantType , char *sNetworkCode, char *sRecurringFlag, int msgType, char *pulsePosConditionCode, char *pulsePosMode );

void  InitPulseInfo(TSPulseInfo * msgInfo);

int   AnalysePulse    (char * buffer_rec, TSPulseInfo * msgInfo);
int   GetPulseField   (int    field_no,   TSPulseInfo * msgInfo, 
                                                  char * data, int *length);
int   AddPulseField   (int    field_no,   TSPulseInfo * msgInfo, 
                                                  char * data, int length);
int   InsertPulseField   (int    field_no,   TSPulseInfo * msgInfo, 
                                                  char * data, int length);
int   PutPulseField   (int    field_no,   TSPulseInfo * msgInfo, 
                                                  char * data, int length);
int   PulseBuildMsg (char * buffer_snd, TSPulseInfo *msgInfo);

void  InitPulseIcTag(TSTagPulse * tagInfo);
int   AnalysePulseIc(char * buffer, int nLength, TSTagPulse * tagInfo);
int   GetPulseIcTag (char *tag_name, TSTagPulse *tagInfo,char *data, int *length);
int   AddPulseIcTag (char *tag_name, TSTagPulse *tagInfo, char *data, int length);
int   PutPulseIcTag (char *tag_name, TSTagPulse *tagInfo, char *data, int length);
int   PulseBuildIcFld (char * buffer_snd, TSTagPulse *tagInfo);

#endif /** HPS_ISO_PULSE_H **/
