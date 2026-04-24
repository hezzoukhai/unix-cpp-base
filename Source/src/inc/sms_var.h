#ifndef HPS_SMS_VAR_H
#define HPS_SMS_VAR_H

char PlusPMC            [ 6 + 1 ];
char sAbrevBankName     [15 + 1 ];
char sAcquirerCountry   [ 3 + 1 ];
char szAcquirerId       [11 + 1 ];
char sForwarderCountry  [ 3 + 1 ];

int nOutgResponse;
int nOutgTimeOut;

#define OUTG_RESPONSE      0
#define OUTG_TIMEOUT       1


#endif /*** HPS_SMS_VAR_H ****/
