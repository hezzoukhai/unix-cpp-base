#ifndef HPS_VISA_PUF_FIELDS_H
#define HPS_VISA_PUF_FIELDS_H

/* PUF: Privat Use Fields Field 126*/
#define VPUF_CUSTOMUER_NAME           0
#define VPUF_CUSTOMUER_ADDRESS		  1
#define VPUF_BILLER_ADDRESS           2
#define VPUF_BILLER_PHONE_NUMBER      3
#define VPUF_MER_ID                   4 /*HAL ENHS 2020Q2 ART 9.3.3*/
#define VPUF_CARDHOLDER_VSEC          5
#define VPUF_MERCHANT_VSEC            6
#define VPUF_TRX_ID_VSEC              7
#define VPUF_CAVV					  8
#define VPUF_CVV2                     9
#define VPUF_IGOTS                    10
#define VPUF_RECURRING                12 /* SLA 120213 Recurring Payment */
/*Start SLA051012 ENH12.2 Vme Trx, HAZ21052014 SWITCH MONITORING */
#define VPUF_ACCOUNT_RESULT           17
#define VPUF_DCC_INDICATOR            18/*ICH See PROD00039494*/
#define VPUF_ADD_AUTH_METHOD          19
#define VPUF_ADD_AUTH_RES_CODE        20
/*End SLA051012 ENH12.2 Vme Trx */
#endif /** HPS_VISA_PUF_FIELDS_H **/
