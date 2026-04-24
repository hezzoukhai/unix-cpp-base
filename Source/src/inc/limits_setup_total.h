
#ifndef HPS_LIMITS_SETUP_TOTAL_H
#define HPS_LIMITS_SETUP_TOTAL_H

typedef struct SLimitsSetupTotal
{

 char     limit_index	[ 4];
 char     currency_code	[ 3];
 double   total_daily_amnt;
 int      total_daily_nbr;
 double   total_per_amnt;
 int      total_per_nbr;
}
TSLimitsSetupTotal;

#endif  /** HPS_LIMITS_SETUP_TOTAL_H **/
