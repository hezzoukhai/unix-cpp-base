  /***********************************************************************
  ** nom	: ndcopcode.h						**
  ** desc	: fichier de definition des constantes de		**
  **		codification du buffer Operation Code pour le		**
  **		protocole NDC						**
  ** auteur	: Younes OUADI (20 Oct 1998)				**
  **				(c) 1998 Hightech Payment Systems	**
  ***********************************************************************/
#ifndef HPS_NDCOPCODE_H
#define HPS_NDCOPCODE_H

/* codification des positions */
#define OC_LANGUAGE_POS					0
#define OC_FUNCTION_POS					1

#define OC_ACC_SEL						5
#define OC_CR_PAYMENT_OPTION			5				/* Credit Card Payment Option RJE 16032014*/


#define OC_CURRENCY_CODE_POS			6
#define OC_PAYMENT_DESTINATION_POS		6
#define OC_PERIOD_POS					6				/* may be used for start date/end date of statement request */

#define OC_PRINT_FLAG_POS				7

/*#define OC_DELIVERY_MODE_POS		7*/




#define UBP_START_OPCODE       'I'

/* position 6 values */
#define OC_P6_ACC_SELECTION				'A'			/* Account selection	*/
#define OC_P6_FEE_INQUIRY				'F'			/* Fee inquiry. P2-P5 is the trx type */


#define OC_FULL_AMOUNT			'F'
/* Credit Card Payment Option RJE 16032014*/
/* codification de la premier position : les langues */
#define	OC_ARABIC			'G'
#define OC_ENGLISH			'F'
#define OC_FRENSH			'A'

/* codification des comptes */
#define OC_ACCOUNT_1			'I'
#define OC_ACCOUNT_2			'H'
#define OC_ACCOUNT_3			'G'
#define OC_ACCOUNT_4			'F'
#define OC_ACCOUNT_5			'A'
#define OC_ACCOUNT_6			'B'
#define OC_ACCOUNT_7			'C'

/* codification de la septieme position : destinataire */
#define OC_NAME_1			'I'
#define OC_NAME_2			'H'
#define OC_NAME_3			'G'
#define OC_NAME_4			'F'

/* codification de la huitieme position : suite de la session */
#define OC_COUNTINUE			'D'
#define OC_EXIT				'F'

/* codification la cinquieme position */
#define OC_PERIOD_1			'I'
#define OC_PERIOD_2			'H'
#define OC_PERIOD_3			'G'
#define OC_PERIOD_4			'F'
#define OC_PERIOD_5			'A'
#define OC_PERIOD_6			'B'
#define OC_PERIOD_7			'C'
#define OC_PERIOD_8			'D'

/* codification des currency */
#define	OC_FIRST_CURRENCY		'A'
#define	OC_SECOND_CURRENCY		'B'
#define	OC_THIRD_CURRENCY		'C'
#define	OC_DEFAULT_CURRENCY		' '

/* codification des flag impression */
#define OC_PRINT			'A'
#define	OC_NO_PRINT			'B'

/* codification des flag Type Account Inter */
#define POS_AT                  5       /* 6 bug, confusion between accountselection oc and currency oc !!! RJE17062014 */
#define AT_SAVING              'I'  
#define AT_CHEKING             'H'  
#define AT_CREDIT              'B'

#endif /**** HPS_NDCOPCODE_H ****/
