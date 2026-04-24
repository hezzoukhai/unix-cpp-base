  /************************************************************************
  **  nom	: atmstatus.h						**
  **  desc	: gestion des status pour GAB				**
  **  auteur	: Younes OUADI (99/08/22)				**
  **		(C) 1997-2001 : Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_ATMSTATUS_H
#define HPS_ATMSTATUS_H

/* - Connexion Status ------------------------------------------------- */
#define	CS_CONNECTED		"A"
#define	CS_DISCONNECTED		"I"

/* - Disconnect reason Code ------------------------------------------- */
#define	DISC_NETWORK			"0000"
#define	DISC_LOCAL_NW_INACTIVE		"0001"
#define	DISC_UNKOWN_ADD			"0002"
#define	DISC_FACILITIES_NOT_ALLOWD	"0003"
#define	DISC_OVERFLOW_NETWORK		"0004"
#define	DISC_REMOTE_NW_STOP		"0005"

/* - Service Flag ----------------------------------------------------- */
#define	SF_ONLINE		"IW"
#define	SF_OFF_LINE		"OS"
#define	SF_OFF_LINE_SUSPECTED	"OS"
#define	SF_CENTRAL_OUT_SERVICE	"OS"
#define	SF_OUT_SERVICE		"OS"

/* - Off Line Reason Code ---------------------------------------------- */
#define	OL_CENTRAL_ORDER				"0"
#define	OL_NETWORK_DISCONNECT			"1"
#define	OL_FILE_TRX_IN_PROGRESS			"2"
#define	OL_ADVICE_FILE_TRX_IN_PROGRESS	"3"
#define	OL_STP_LST_TRX_IN_PROGRESS		"4"
#define	OL_KEY_TRX_IN_PROGRESS			"5"
#define	OL_CASSETTES_FAULT				"6"
#define	OL_NO_NOTES						"7"
#define	OL_SAFE_BOX_FAULT				"8"
#define	OL_KEYBORD_FAULT				"A"
#define	OL_SCREEN_FAULT					"B"
#define	OL_CARD_READER_FAULT			"C"
#define	OL_SECURITY_FAULT				"D"
#define	OL_PRN_FAULT					"E"
#define	OL_STORAGE_FAULT				"F"
#define	OL_PARAM_FAULT					"G"
#define	OL_ATM_MAINT					"H"
#define	OL_UNKOWN						"X"

/*#define SF_IN_SERVICE           "0" *//* Ajout par Y.K le 19-02-00 */



#define ATM_TERM_EVT_INAB 	"INAB"		/* Disconnection Network 								*/

#define ATM_TERM_EVT_TCDE 	"TCDE"		/* Terminal error : Cash Dispenser Error				*/
#define ATM_TERM_EVT_TCDW	"TCDW"		/* Terminal warning/error : Cash Dispenser Error		*/

#define ATM_TERM_EVT_TCPE 	"TCPE"		/* Terminal error : Receipt printer fault				*/
#define ATM_TERM_EVT_TCPW 	"TCPW"		/* Terminal warning/error : Receipt printer fault		*/
#define ATM_TERM_EVT_TLPE 	"TLPE"		/* Terminal error : Log imprinter fault					*/
#define ATM_TERM_EVT_TLPW 	"TLPW"		/* Terminal warning/error : Log imprinter fault			*/

#define ATM_TERM_EVT_TCRF 	"TCRF"		/* Card Reader Off Service								*/
#define ATM_TERM_EVT_TCRW 	"TCRW"		/* Card Reader In Service(Warning)						*/

#define ATM_TERM_EVT_TRDY 	"TRDY"		/* Terminal ready										*/
#define ATM_TERM_EVT_TRIS 	"TRIS"		/* Terminal In Service									*/
#define ATM_TERM_EVT_TROS 	"TROS"		/* Terminal out of service								*/

#define ATM_TERM_EVT_TWCF 	"TWCF"		/* Log Printer Paper Error			??					*/
#define ATM_TERM_EVT_TWRT	"TWRT"		/* Cash retracted										*/
#define ATM_TERM_EVT_LEN	4

#endif /*** HPS_ATMSTATUS_H ***/
