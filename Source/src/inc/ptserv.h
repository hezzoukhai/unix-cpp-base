#ifndef HPS_PTSERV_H
#define HPS_PTSERV_H

typedef  struct SPtserv 
{

 char     outlet_number			[ 15];
 char     merchant_number		[ 15];
 char     outlet_name			[ 30];
 char     bank_code			[ 6];
 char     branch_code			[ 6];
 char     city_code			[ 5];
 char     region_code			[ 2];
 char     country_code			[ 3];
 char     status_code			[ 1];
 char     opening_date			[ 8];
 char     closing_date			[ 8];
 int      capture_mode			    ;
 char     mcc				[ 4];
 long     floor_limit			    ;
 char     currency_code			[ 3];
 char     sensitive_operation_record	[ 1];

} TSPtserv ;

#endif /** HPS_PTSERV_H **/
