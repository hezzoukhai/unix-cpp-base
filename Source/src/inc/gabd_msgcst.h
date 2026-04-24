  /***********************************************************************
  ** nom	: ndcmsgcst.h						                        **
  ** desc	: definition des constantes pour les messages GABD	        **
  ** auteur	: Younes OUADI (20 Oct 1998)				                **
  **			(c) 1998 Higtech Payment Systems		                **
  ************************************************************************/

#ifndef HPS_GABDMSGCST_H
#define HPS_GABDMSGCST_H

/* message unsollicite */
#define US_CONFIG_ID			  3

/* message transaction request */
/* definition des offsets */
#define TR_TRACKII			      5
#define	TR_TRACKIII			      6
#define TR_OPERATION_CODE	  	  7
#define TR_AMOUNT			      8
#define TR_PIN				      9
#define TR_BUFFER_B			     10
#define TR_BUFFER_C			     11

/* definition des langueurs */
#define TR_TRACK2_LEN			 39
#define TR_TRACK3_LEN			106
#define TR_OPERATION_CODE_LEN	  8
#define TR_AMOUNT_LEN			 12
#define TR_PIN_LEN			     32
#define TR_BUFFER_B_LEN			 32
#define TR_BUFFER_C_LEN			 32

#define MAX_FIELD_LENGTH		112

#endif /** HPS_GABDMSGCST_H **/
