#ifndef HPS_AUTHO_PERIOD_H
#define HPS_AUTHO_PERIOD_H


typedef struct SAuthoPeriod
{

 char     period_code	[ 3];
 char     period_type	[ 1];
 int      period_value;

}TSAuthoPeriod;

#endif /** HPS_AUTHO_PERIOD_H **/
