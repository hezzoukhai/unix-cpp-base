  /***********************************************************************
  ** nom	: dblmsgcst.h						                        **
  ** desc	: definition des constantes pour les messages DIE BOLD    **
  ** auteur	: Younes OUADI (20 Oct 1998)				                **
  **			(c) 1998 Higtech Payment Systems		                **
  ************************************************************************/

#ifndef HPS_DBLMSGCST_H
#define HPS_DBLMSGCST_H

/* message unsollicite */
#define US_CONFIG_ID			  3

/* message transaction request */
/* definition des offsets */
#define TR_TRACKII			      5
#define	TR_TRACKIII			      6
#define TR_OPERATION_CODE	  	      7
#define TR_AMOUNT			      8
#define TR_PIN				      9
#define TR_CONFIRMATION			      13
/*BEGIN Adil HMAMI*/
/*#define TR_FIRST_PIN			      14*/
#define TR_FIRST_PIN			      10
/*#define TR_SECOND_PIN			      15*/
#define TR_SECOND_PIN			      09
/*END Adil HMAMI*/
#define TR_BUFFER_B			     10
#define TR_BUFFER_C			     11

#define TR_EMV_DATA                  14

/* definition des langueurs */
#define TR_TRACK2_LEN			 39
#define TR_TRACK3_LEN			106
#define TR_OPERATION_CODE_LEN	  8
#define TR_AMOUNT_LEN			 12
#define TR_PIN_LEN			     32
#define TR_BUFFER_B_LEN			 32
#define TR_BUFFER_C_LEN			 32

/*#define MAX_FIELD_LENGTH		112*/
#define MAX_FIELD_LENGTH	  	1024	

#endif /** HPS_DBLMSGCST_H **/
