#ifndef HPS_NETWORK_H
#define HPS_NETWORK_H

typedef struct SNetwork
{

 char     network_code	[ 2];
 char     credit_primary_port	[ 6];
 char     credit_alternate_port	[ 6];
 char     debit_primary_port	[ 6];
 char     debit_alternate_port	[ 6];
 char     virtual_bank_code	[ 6];

} TSNetwork;


#endif  /** HPS_NETWORK_H **/
