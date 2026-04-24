#ifndef HPS_ISO_8583_H
#define HPS_ISO_8583_H


/*******Defines that should be in the std include files*****/

#define SIZE_KEY_TDES					256

#define HOST_PC_FUNDS_CONSULT			"31"

/* Function codes */

#define ISO_FC_INIT_REQUEST				"100"			/* Initial request						*/
#define ISO_FC_PRE_AUTH					"101"			/* Pre authorization					*/
#define ISO_FC_PRE_AUTH_INC				"107"			/* Pre authorization incremental		*//*SNO091115 */
#define ISO_FC_BAL_REQUEST				"108"			/* Balance request						*/
#define ISO_FC_AUTH_PREV				"201"			/* Authorized previously				*/
#define ISO_FC_PRE_AUTH_COMPL			"202"			/* Completion							*//*SNO091115 */
#define ISO_FC_BAL_INQUIRY				"281"			/* Balance Inquiry						*/
#define ISO_FC_ADD_RECORD				"301"			/* Add record							*/
#define ISO_FC_CHG_RECORD				"302"			/* Change record (partial update)		*/
#define ISO_FC_DEL_RECORD				"303"			/* Delete record						*/
#define ISO_FC_INQ_RECORD				"305"			/* Inquiry								*/
#define ISO_FC_FULL_REVERSAL			"400"			/* Full reversal						*/
#define ISO_FC_PARTIAL_REVERSAL			"402"			/* Partial reversal						*/
#define ISO_FC_SIGNON					"603"			/* Sign-on (BIN)						*/
#define ISO_FC_SIGNOFF					"604"			/* Sign-off (BIN)						*/
#define ISO_FC_ECHO						"609"			/* Echo test							*/
#define ISO_FC_KEY_EXCHG				"811"			/* Key change request					*/


#define ISO_MR_LATE_RESPONSE			"4006"
#define ISO_MR_INVALID_RESPONSE			"4020"
#define ISO_MR_TIME_OUT					"4200"

#define ISO_PC_PURCHASE					"00"			/* Achat de biens ou services				*/
#define ISO_PC_ATM_WITHDRAWAL			"01"			/* Retrait sur automate (DAB/GAB)			*/
#define ISO_PC_QUASICASH				"11"			/* Quasi-cash								*/
#define ISO_PC_CAPTURE_CARD				"14"			/* Capture de carte							*/
#define ISO_PC_WITHDRAWAL				"17"			/* Retrait au guichet						*/
#define ISO_PC_CREDIT					"20"			/* Crédit									*/
#define ISO_PC_FUNDS_CONSULT			"31"			/* Interrogation sur disponibilité de fonds */
#define ISO_PC_CARD_RETURN				"80"			/* Restitution de carte						*/
#define ISO_PC_FUNDS_TRANSFER_D			"29"			/* Transfert de fonds, débit				*/
#define ISO_PC_FUNDS_TRANSFER_C			"08"			/* Transfert de fonds, crédit				*/
#define ISO_PC_PME						"81"			/* Autorisation de chargement PME			*/


#define ISO_F024_LEN					3
#define ISO_F025_LEN					3


/* ISO Message  type identifiers */
#define MSG_TYPE_VERSION				0
#define MSG_TYPE_CLASS					1
#define MSG_TYPE_FUNCTION				2
#define MSG_TYPE_TRX_ORIGIN				3


#define MSG_TYPE_P2_AUT					'1'			/* authorization				*/
#define MSG_TYPE_P2_FIN					'2'			/* financial					*/
#define MSG_TYPE_P2_FILE				'3'			/* file action					*/
#define MSG_TYPE_P2_REV					'4'			/* reversal/chargeback			*/
#define MSG_TYPE_P2_REC					'5'			/* reconciliation				*/
#define MSG_TYPE_P2_ADM					'6'			/* administrative				*/
#define MSG_TYPE_P2_FEE					'7'			/* fee collection				*/
#define MSG_TYPE_P2_NET					'8'			/* network management			*/


#define MSG_TYPE_P3_REQ					'0'
#define MSG_TYPE_P3_REQ_RESP			'1'
#define MSG_TYPE_P3_ADV					'2'
#define MSG_TYPE_P3_ADV_RESP			'3'
#define MSG_TYPE_P3_NOTIF				'4'

#define MSG_TYPE_P4_ACQ					'0'
#define MSG_TYPE_P4_ACQ_REP				'1'
#define MSG_TYPE_P4_ISS					'2'
#define MSG_TYPE_P4_ISS_REP				'3'
#define MSG_TYPE_P4_OTHER				'4'
#define MSG_TYPE_P4_OTHER_REP			'5'
/**********************************************************/


#endif
