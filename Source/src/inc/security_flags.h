#ifndef HPS_SECURITY_FLAGS_H
#define HPS_SECURITY_FLAGS_H

#define PIN_METHOD 			0
#define PVV_METHOD 			1
#define OFFSET_METHOD 		2

#define AMX_CSC5			5		/* Magstrip CSC */
#define AMX_CSC4			4		/* CSC in the card front */
#define AMX_CSC3			3		/* CSC in the card back */
#define AMX_AEVV                        3               
#define AMX_ALGO_V2                     2

enum {
	SEC_FLAG_CHK_CVV1               = 0	,
	SEC_FLAG_CHK_CVV2               	,
	SEC_FLAG_CHK_PIN                	,
	SEC_FLAG_PIN_MTHD               	,
	SEC_FLAG_CHK_ARQC               	,
	SEC_FLAG_CHK_EXP_DT             	,
	SEC_FLAG_CHK_VAL_DT             	,
	SEC_FLAG_CHK_SERVICE_CD         	,
	SEC_FLAG_CHK_SERVICE_SETUP      	,
	SEC_FLAG_CHK_STOP_LIST          	,
	SEC_FLAG_CHK_LIMITS             	,
	SEC_FLAG_CHK_CARD               	,
	SEC_FLAG_CHK_BANK_ACC           	,
	SEC_FLAG_CHK_SA_ACC             	,
	SEC_FLAG_CHK_CLIENT             	,
	SEC_FLAG_CHK_AVS                	,
	SEC_FLAG_CHK_DOMAIN                	,
	SEC_FLAG_CHK_FRAUD                 	,
	SEC_FLAG_CHK_ICVV               	,
	SEC_FLAG_CHK_APP_USAGE_CTRL     	,
	SEC_FLAG_CHK_ATC                	,
	SEC_FLAG_CHK_DATA_AUTH_CD       	,
	SEC_FLAG_CHK_DYN_NBR            	,
	SEC_FLAG_CHK_FALLBACK           	,
	SEC_FLAG_CHK_CVC3               	,
	SEC_FLAG_CHK_AAV                	,
	SEC_FLAG_CHK_SCA           			,
	SEC_FLAG_CHK_DCVV2                  ,	
	SEC_FLAG_CHK_CAVV_DECISION_MATRIX	,	/*AEA20210906 PLUTONL-2595*/
	/*SEC_RFU_3                       	,*/ /*align security flags between unix and DB to get 32 like flags flag defined in PCRD_P7_CTRL_SECURITY*/
	SEC_CHK_CVV2_COUNTER                ,
	SEC_RFU_7                       	,
	SEC_RFU_8                       	,
	SEC_FLAG_LEN
};





typedef struct SSecurityFlags
{
	char  cvv1_verification_flag; 			/* Tag '0S1', length : '01' */
    /* 	    'N'     : � NO  `
			'Y' 	: � YES `  */
           
	char  cvc2_verification_flag;           /* Tag 'S2' , length : '01'*/
    /* 	    'N'     : � NO  `
			'Y' 	: � YES ` */

	char  pin_verification_flag;            /* Tag 'S3' , length : '01'*/
    /* 	    'N'     : � NO  `
			'Y' 	: � YES ` */

	char  pin_verification_method;          /* Tag 'S4' , length : '01'*/
    /*		'0' 	: � PIN Method `
			'1' 	: � PVV Method `
			'2' 	: � Offset Method ` */

	char  crypt_verification_flag;     /* Tag 'S4' , length : '01'*/
    /* 	    'N'     : � NO  `
			'Y' 	: � YES ` */

} TSecurityFlags;


#endif  /** HPS_SECURITY_FLAGS_H  **/
