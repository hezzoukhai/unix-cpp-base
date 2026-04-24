  /***********************************************************************
  ** nom	: ndcmsgcst.h						                        **
  ** desc	: definition des constantes pour les messages NDC	        **
  ** auteur	: Younes OUADI (20 Oct 1998)				                **
  **			(c) 1998 Higtech Payment Systems		                **
  ************************************************************************/

#ifndef HPS_NDCMSGCST_H
#define HPS_NDCMSGCST_H

/* message unsollicite */
#define US_CONFIG_ID			  3

/* message transaction request */
/* definition des offsets */
#define TR_COORD_NBR			  4
#define TR_TRACKII			      5
#define	TR_TRACKIII			      6
#define TR_OPERATION_CODE	  	  7
#define TR_AMOUNT			      8
#define TR_PIN				      9
#define TR_FIRST_PIN			      14
#define TR_SECOND_PIN			      15
#define TR_BUFFER_B			     10
#define TR_BUFFER_C			     11
#define TR_EMV_DATA			 16 /* Modif Nabil 14-16 */
#define TR_EMV_DATA_WN			 16  /*RJE02022014*/
#define TR_CCDM_DATA_WN			 1  /*MICR MUW09052010*/ 
#define TR_CASH_DEP_WN		         16  /* TBO19052011 */
#define TR_WBUFFER			     17

/* definition des langueurs */
#define TR_TRACK2_LEN			 39
#define TR_TRACK3_LEN			106
#define TR_OPERATION_CODE_LEN	  8
#define TR_AMOUNT_LEN			 12
#define TR_PIN_LEN			     32
#define TR_BUFFER_B_LEN			 32
#define TR_BUFFER_C_LEN			 32

/* defined in define.h
#define MAX_FIELD_LENGTH		512
*/

#endif /** HPS_NDCMSGCST_H **/
