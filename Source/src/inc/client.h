#ifndef HPS_CLIENT_H
#define HPS_CLIENT_H

typedef struct SClient
{

 char     bank_code	[ 6];
 char     client_code	[ 24];
 char     branch_code	[ 6];
 char     status_code	[ 1];
 char     bank_account_opening_date	[ 8];
 char     bank_account_code	[ 6];
 char     bank_account_number	[ 24];
 char     currency_code	[ 3];
 double   credit_limit;
 double   cash_transfer_limit;
 double   loan_limit;
 char     sensitive_operation_record	[ 1];
 char     client_addendum_flag	[ 1];
 char     rowid_record	[ 18];

} TSClient;

#endif /** HPS_CLIENT_H **/
