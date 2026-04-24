#ifndef HPS_CENTER_H
#define HPS_CENTER_H

typedef struct SCenter
{

 char     center_code	[ 2];
 char     grouping_code	[ 1];
 char     network_code	[ 2];
 int      multi_country_option;
 char     country_code	[ 3];
 char     country_code_alpha	[ 3];
 char     currency_code	[ 3];
 char     currency_code_alpha	[ 3];

} TSCenter;

#endif /** HPS_CENTER_H **/
