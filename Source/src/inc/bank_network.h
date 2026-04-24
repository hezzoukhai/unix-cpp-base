#ifndef HPS_BANK_NETWORK_H
#define HPS_BANK_NETWORK_H

typedef struct SBankNetwork
{

 char     bank_code	[ 6];
 char     network_code	[ 2];
 char     acquirer_bin	[ 11];
 char     acquirer_ica	[ 4];
 char     settlement_currency	[ 3];
 char     billing_currency	[ 3];
 char     reconciliation_currency	[ 3];
 char     virtual_merchant_number	[ 15];
 char     virtual_outlet_number	[ 15];
 char     virtual_abrev_location	[ 25];
 char     virtual_activity_code	[ 4];
 char     virtual_city_code	[ 3];
 char     virtual_region_code	[ 3];
 char     virtual_country_code	[ 3];

} TSBankNetwork;

#endif /* HPS_BANK_NETWORK_H */
