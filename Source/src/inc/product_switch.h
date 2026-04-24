#ifndef HPS_PRODUCT_SWITCH_H
#define HPS_PRODUCT_SWITCH_H


typedef struct SProductSwitch
{
   char  product_code    [ 3];
   char  mti             [ 4];
   char  processing_code [ 2];
   char  function_code   [ 3];
   char  source_account  [ 2];
   char  destination_account  [ 2];

   char  routing_debit   [ 6];
   char  alternate_debit [ 6];
   char  routing_credit  [ 6];
   char  alternate_credit[ 6];

}TSProductSwitch;

#endif /**  HPS_PRODUCT_SWITCH_H **/
