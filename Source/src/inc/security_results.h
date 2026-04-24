#ifndef HPS_SECURITY_RESULTS_H
#define HPS_SECURITY_RESULTS_H

typedef struct SSecurityResults
{
	char  CVV1_VERIFICATION_RESULT; 			/* Tag 'S1', length : '01' */
    /* 	    '0'     : à NO  `
			'1' 	: à YES `  */
           
	char  CVC2_VERIFICATION_RESULT;           /* Tag 'S2' , length : '01'*/
    /* 	    '0'     : à NO  `
			'1' 	: à YES ` */

	char  PIN_VERIFICATION_RESULT;            /* Tag 'S3' , length : '01'*/
    /* 	    '0'     : à NO  `
			'1' 	: à YES ` */

	char  CRYPTOGRAM_VERIFICATION_RESULT;     /* Tag 'S4' , length : '01'*/
    /* 	    '0'     : à NO  `
			'1' 	: à YES ` */

} TSecurityResults;

#define CVC1_POSITION 0
#define CVC2_POSITION 1
#define PIN_POSITION  2
#define CRPT_POSITION  4 /** MAW20050916 **/
#define ICVV_POSITION  18 	/**start Mehdi Elyajizi 1909206 : MYJ231007***/     
#define AUC_POSITION   19        
#define DAC_POSITION   20          /* Mehdi Elyajizi veification DAC ignoré fonctionnalité MasterCard et DAC non envoyé systématiquement */
#define DYNAMIC_NBR_POSITION  21


#endif  /** HPS_SECURITY_RESULTS_H  **/
