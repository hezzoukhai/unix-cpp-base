#ifndef HPS_NETWORK_SWITCH_H
#define HPS_NETWORK_SWITCH_H

typedef struct SNetworkSwitch
{
   char  network          [ 2];
   char  resource         [ 6];
   char  mti              [ 4];
   char  alternate_debit  [ 6];
   char  alternate_credit [ 6];

} TSNetworkSwitch;

#endif  /**  HPS_NETWORK_SWITCH_H **/
