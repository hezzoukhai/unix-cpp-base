#ifndef HPS_RANGE_SWITCH_H
#define HPS_RANGE_SWITCH_H

typedef struct SRangeSwitch
{
   char     card    [ 22];
   char     mti             [ 4];
   char     processing_code [ 2];
   char     function_code   [ 3];
   char     source_account  [ 2];
   char     destination_account  [ 2];

   char  routing_debit   [ 6];
   char  alternate_debit [ 6];
   char  routing_credit  [ 6];
   char  alternate_credit[ 6];

}TSRangeSwitch;

#endif /** HPS_RANGE_SWITCH_H **/
