#ifndef HPS_SMS_PUF_FIELDS_H
#define HPS_SMS_PUF_FIELDS_H

/* PUF: Privat Use Fields Field 126*/
#define SPUF_VPUF_MER_ID              4
#define SPUF_CARDHOLDER_VSEC          5
#define SPUF_MERCHANT_VSEC            6
#define SPUF_TRX_ID_VSEC              7
#define SPUF_CAVV                     8
#define SPUF_CVV2                     9
#define SPUF_SERVICES_INDICATOR       11
#define SPUF_POS_ENVIRONMENT          12
#define SPUF_MC_UCAF_COLLECTION_IND   14
#define SPUF_MC_UCAF_FIELD            15
/*Start SLA051012 ENH12.2 Vme Trx, HAZ21052014 SWITCH MONITORING */
#define SPUF_ACCOUNT_RESULT           17
#define SPUF_DCC_INDICATOR            18/*ICH See PROD00039494*/
#define SPUF_3DS_INDICATOR            19
/*End SLA051012 ENH12.2 Vme Trx */ 
#endif /** HPS_SMS_PUF_FIELDS_H **/
