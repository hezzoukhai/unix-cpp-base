#include <stdio.h>
#include <define.h>
#include <dump.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <p7_define.h>
#include <p7_sid.h>
#include <visa_define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <resources.h>
#include <iso8583.h>
#include <iso_cps.h>
#include <iso_visa.h>
#include <p7_sid.h>
#include <security_flags.h>
#include <visa_param.h>
#include <nw_raw_msg.h>
#include <security_data.h>
/*************************************************************************/
/* IsoToVisaProcessingCode()                                           */
/* */
/* Function to convert ISO processing code to VISA processing code     */
/* */
/* Input:  IsoCode: ISO processing code                                */
/* Output: VisaCode: VISA processing code                              */
/*************************************************************************/
void
	IsoToVisaProcessingCode(char *IsoCode,
	char *VisaCode,
	char *sMcc)
{
	char            v_process_code[7];
	char            v_posit1[3];
	char            v_posit2[3];
	char            v_posit3[3];

	int             v_trans_type;
	int             v_account_from;
	int             v_account_to;
	int             nMcc = 0;

	trace_event("Start IsoToVisaProcessingCode()", PROCESSING);

	memset(v_process_code, 0, sizeof(v_process_code));
	memset(v_posit1, 0, sizeof(v_posit1));
	memset(v_posit2, 0, sizeof(v_posit2));
	memset(v_posit3, 0, sizeof(v_posit3));
	v_trans_type = 0;
	v_account_from = 0;
	v_account_to = 0;

	memcpy(v_posit1, IsoCode, 2);
	memcpy(v_posit2, IsoCode + 2, 2);
	memcpy(v_posit3, IsoCode + 4, 2);

	v_trans_type = atoi(v_posit1);
	v_account_from = atoi(v_posit2);
	v_account_to = atoi(v_posit3);

	switch (v_trans_type) {
	case 0:
		memcpy(v_process_code, "00", 2);
		break;
	case 1:
		memcpy(v_process_code, "01", 2);
		break;
	/*case 3:				
		memcpy(v_process_code, "03", 2);*/	/*AMER PROD00054205: There is no Trx type "03" in the specs*/
		break;
	case 11:
		memcpy(v_process_code, "11", 2);
		break;
	case 17:
		memcpy(v_process_code, "01", 2);
		break;
	case 18:
		memcpy(v_process_code, "40", 2);
		break;
	case 20:								/*End AMER20180314 PROD00054205 Enh18.Q2 Art 2.4*/
		memcpy(v_process_code, B1_F003_P1_REFUND, B1_F003_P1_LEN);
		break;	
	case 31:
		memcpy(v_process_code, "30", 2);
		break;
	case 40:
		memcpy(v_process_code, "40", 2);
		break;
	case 50:
		memcpy(v_process_code, "50", 2);
		break;
	/*case 58:
		memcpy(v_process_code, "51", 2);
		break;
	case 59:
		memcpy(v_process_code, "52", 2);*/	/*AMER PROD00054205: There is no trx type neither "51" nor "52" in the specs*/
		break;
	/*Start AMER20180314 PROD00054205 Enh18.Q2 Art 8.1.1*/
	case 91:
		memcpy(v_process_code, B1_F003_P1_PIN_CHG_UNBLOCK, 2);
		break;
	case 98:
		memcpy(v_process_code, B1_F003_P1_PIN_UNBLOCK_PP_ACT, 2);
		break;
	/*End AMER20180314 PROD00054205 Enh18.Q2 Art 8.1.1*/
	default:
		memcpy(v_process_code, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	switch (v_account_from) {

	case 0:
		memcpy(v_process_code + 2, "00", 2);
		break;
	case 10:
		memcpy(v_process_code + 2, "10", 2);
		break;
	/*case 20:
		memcpy(v_process_code + 2, "20", 2);*/	/*AMER PROD00054205: There is no account type from "20" the specs*/
		break;
	case 30:
		memcpy(v_process_code + 2, "30", 2);
		break;
	case 38:
		memcpy(v_process_code + 2, "00", 2);
		break;
	case 40:
		memcpy(v_process_code + 2, "40", 2);
		break;
	case 50:
		memcpy(v_process_code + 2, "00", 2); /*HAL 20221901 PLUTONL-4188 from +4 to +2 because we are in v_account_from*/
		break;
	default:
		memcpy(v_process_code + 2, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	switch (v_account_to) {

	case 0:
		memcpy(v_process_code + 4, "00", 2);
		break;
	case 10:
		memcpy(v_process_code + 4, "10", 2);
		break;
	/*case 20:
		memcpy(v_process_code + 4, "20", 2);*/	/*AMER PROD00054205: There is no account type from "20" the specs*/
		break;
	case 30:
		memcpy(v_process_code + 4, "30", 2);
		break;
	case 38:
		memcpy(v_process_code + 4, "00", 2);
		break;
	case 40:
		memcpy(v_process_code + 4, "40", 2);
		break;
	case 50:
		memcpy(v_process_code + 4, "00", 2);
		break;
	default:
		memcpy(v_process_code + 4, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}
	/*nMcc = atoi(sMcc);		/*AMER PROD00054205: Don't need to check the MCC to send quasi-cash trx type. The information already exists in ISS F3.1 = "11"* /
	switch (nMcc) {

	case 6051:
		memcpy(v_process_code, "11", 2);
		break;
	case 7995:
		memcpy(v_process_code, "11", 2);
		break;
	case 4829:
		memcpy(v_process_code, "11", 2);
		break;
	}*/
	memcpy(VisaCode, v_process_code, 6);

	trace_event("End  IsoToVisaProcessingCode(OK)", PROCESSING);
	return;

}



/*************************************************************************/
/* VisaToIsoProcessingCode()                                           */
/* */
/* Converts a VISA processing code into an ISO standard code           */
/* Input:  VisaCode: VISA processing code                              */
/* Merchant: merchant type                                     */
/* Output: IsoCode:  corresponding ISO processing code                 */
/*************************************************************************/
void
	VisaToIsoProcessingCode(char *VisaCode,
	char *Merchant,
	char *IsoCode)
{
	char            v_process_code[7];
	char            v_posit_1_2[3];
	char            v_posit_3_4[3];
	char            v_posit_5_6[3];

	int             v_trans_type;
	int             v_account_from;
	int             v_account_to;

	char            ligne[80];

	sprintf(ligne, "Start VisaToIsoProcessingCode(%.6s)", VisaCode);
	trace_event(ligne, PROCESSING);

	memset(v_process_code, 0, sizeof(v_process_code));
	memset(v_posit_1_2, 0, sizeof(v_posit_1_2));
	memset(v_posit_3_4, 0, sizeof(v_posit_3_4));
	memset(v_posit_5_6, 0, sizeof(v_posit_5_6));

	memcpy(v_posit_1_2, VisaCode, 2);
	memcpy(v_posit_3_4, VisaCode + 2, 2);
	memcpy(v_posit_5_6, VisaCode + 4, 2);

	v_trans_type = atoi(v_posit_1_2);
	v_account_from = atoi(v_posit_3_4);
	v_account_to = atoi(v_posit_5_6);

	switch (v_trans_type) {
	case 0: memcpy(v_process_code, P7_F003_PURCHASE, P7_F003_P1_LEN);break;
	case 1:
		if (memcmp(Merchant, "6011", 4) == 0)
			memcpy(v_process_code, P7_F003_WITHDRAWAL, P7_F003_P1_LEN);
		else
			memcpy(v_process_code, P7_F003_CASH_ADVANCE, P7_F003_P1_LEN);
		break;
	case 3: memcpy(v_process_code, "03", 2); break;
	case 11: memcpy(v_process_code, P7_F003_QUASICASH, P7_F003_P1_LEN); break;
	case 20: memcpy(v_process_code, P7_F003_REFUND, P7_F003_P1_LEN); break;
	case 10: memcpy(v_process_code, P7_F003_ACC_FUNDING, P7_F003_P1_LEN); break;/*HAL PROD00073823 20202401 Enh Q2.2020 Art 8.1.2*/  	   

	/*EBEcase  26 : memcpy(v_process_code, "26", 2); break;*/ /* ENH 12.2 SLA 130912 Fast Funds */		
	case 26:
	case 28:memcpy(v_process_code, P7_F003_PAYMENT, P7_F003_P1_LEN);			break;
	case 30:memcpy(v_process_code, P7_F003_BAL_INQ, P7_F003_P1_LEN);			break;
	case 40:memcpy(v_process_code, P7_F003_ACC_TRANSFER, P7_F003_P1_LEN);		break;
	case 50:memcpy(v_process_code, P7_F003_BILL_PAYMENT, P7_F003_P1_LEN);		break;
	case 51:memcpy(v_process_code, "58", 2);break;
	case 52:memcpy(v_process_code, "59", 2);break;
	case 70:memcpy(v_process_code, P7_F003_PIN_CHANGE, P7_F003_P1_LEN);			break;
	case 72:memcpy(v_process_code, P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN);		break;
	default:
		memcpy(v_process_code, P7_F003_PURCHASE, P7_F003_P1_LEN);
		trace_event("Invalid processing code", ERROR);
	}

	switch (v_account_from) {

	case 0:
		memcpy(v_process_code + 2, "00", 2);
		break;
	case 10:
		memcpy(v_process_code + 2, "10", 2);
		break;
	case 20:
		memcpy(v_process_code + 2, "20", 2);
		break;
	case 30:
		memcpy(v_process_code + 2, "30", 2);
		break;
	case 40:
		memcpy(v_process_code + 2, "40", 2);
		break;
	case 64:
		memcpy(v_process_code + 2, "62", 2);
		break;
	default:
		memcpy(v_process_code + 2, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	switch (v_account_to) {

	case 0:
		memcpy(v_process_code + 4, "00", 2);
		break;
	case 10:
		memcpy(v_process_code + 4, "10", 2);
		break;
	case 20:
		memcpy(v_process_code + 4, "20", 2);
		break;
	case 30:
		memcpy(v_process_code + 4, "30", 2);
		break;
	case 40:
		memcpy(v_process_code + 4, "40", 2);
		break;
	case 64:
		memcpy(v_process_code + 4, "62", 2);
		break;
	default:
		memcpy(v_process_code + 4, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	memcpy(IsoCode, v_process_code, 6);
	IsoCode[6] = '\0';

	trace_event("End   VisaToIsoProcessingCode(OK)", PROCESSING);
	return;

}



/*************************************************************************/
/* IsoToVisaPosMode()                                                  */
/* */
/* Converts an ISO POS Entry Mode Code (field 22) to a BASEI mode      */
/* Input:  ISO mode                                                    */
/* Output: BASEI mode                                                  */
/*************************************************************************/
void
	IsoToVisaPosMode(char *IsoMode,
	char *VisaMode)
{
	trace_event("Start IsoToVisaPosMode()", PROCESSING);

	switch (IsoMode[6]) {
	case P7_F022_P7_UNSPECIFIED:
		memcpy(VisaMode, "00", 2);
		break;
	case P7_F022_P7_MANUAL:
		memcpy(VisaMode, "01", 2);
		break;
	case P7_F022_P7_MAG:
		memcpy(VisaMode, "90", 2);
		break;
	case P7_F022_P7_BARCODE:
		memcpy(VisaMode, "03", 2);
		break;
	/*case P7_F022_P7_OCR:
		memcpy(VisaMode, "04", 2);*/		/*AMER PROD00054205: "04" is Reserved for future use*/
		break;
	case P7_F022_P7_ICC:
		memcpy(VisaMode, "05", 2);
		break;
	case P7_F022_P7_PAN_NETRY:
		memcpy(VisaMode, "01", 2);
		break;
	case P7_F022_P7_FALLBACK:  /* BURGAN ONSITE CERTIF MKB/AHA20171002 */ /* BURGAN_04 */
		memcpy(VisaMode, "90", 2);
		break;
	case P7_F022_P7_ECOM:
	case P7_F022_P7_ECOM_NON_AUTH_ATTEMPT:
	case P7_F022_P7_ECOM_AUTHENTICATED:
	case P7_F022_P7_ECOM_UNSECURE:
	case P7_F022_P7_ECOM_SEC_CHAN:
		memcpy(VisaMode, "01", 2);
		break;
	case P7_F022_P7_TRACK1_READ:
		memcpy(VisaMode, "90" /*"06"*/, 2);		/*AMER PROD00054205: The value "06" is Reserved for future use. We should use "90" instead of "06"*/
		break;
	case P7_F022_P7_PAN_AUTO_ENTRY_CL_ICC: /* BURGAN ONSITE CERTIF MKB/AHA20171002 */ /* BURGAN_05 */
		memcpy(VisaMode, "07", 2);
		break;
	case P7_F022_P7_PAN_AUTO_ENTRY_CL_MAG:
		memcpy(VisaMode, "91", 2);
		break;
	case P7_F022_P7_ICC_CVV_UNREL:
		memcpy(VisaMode, "95", 2);
		break;
	case P7_F022_P7_TRACK_UNALT:
		memcpy(VisaMode, "90", 2);
		break;
	case P7_F022_P7_CRED_ON_FILE:				/* AMA */
		memcpy(VisaMode, "10", 2);
		break;
	default:  
		memcpy(VisaMode, "00", 2);   
		break;
	}

	switch (IsoMode[11]) {
	case '0':
		memcpy(VisaMode + 2, "2", 1);
		break;
	case '1':
		memcpy(VisaMode + 2, "0", 1);
		break;
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
		memcpy(VisaMode + 2, "1", 1);
		break;
	case 'S':
		memcpy(VisaMode + 2, "8", 1);
		break;
	case 'T':
		memcpy(VisaMode + 2, "1", 1);
		break;
	default:
		memcpy(VisaMode + 2, "0", 1);
	}

	memcpy(VisaMode + 3, "0", 1);
	VisaMode[4] == '\0';

	trace_event("End IsoToVisaPosMode(OK)", PROCESSING);
	return;
}


/**************************************************************************/
/* VisaToIsoPosMode()                                                   */
/* */
/* Converts VISA POS Entry Mode data (field 22) into an ISO format code */
/* Input:  VISA fields 18, 22, 25, & 60                                 */
/* Output: ISO field 22                                                 */
/**************************************************************************/
void
	VisaToIsoPosMode(char *Merchant,
	char *VisaPosMode,
	char *VisaPosCond,
	char *VisaAddPos,
	char *IsoPosMode,
	char *EcomAuthInfo,
	int	*nEcomInfoLen )
{

	char            sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start VisaToIsoPosMode(%s|%s|%s|%s)", Merchant, VisaPosMode, VisaPosCond, VisaAddPos);
	trace_event(sLine, PROCESSING);

	/* Card Data Entry Capability - Position 1 */
	if (memcmp(VisaAddPos + 1, "0", 1) == 0)
		memcpy(IsoPosMode, "0", 1);
	else if (memcmp(VisaAddPos + 1, "1", 1) == 0)
		memcpy(IsoPosMode, "1", 1);
	else if (memcmp(VisaAddPos + 1, "2", 1) == 0)
		memcpy(IsoPosMode, "2", 1);
	else if (memcmp(VisaAddPos + 1, "3", 1) == 0)
		memcpy(IsoPosMode, "3", 1);
	else if (memcmp(VisaAddPos + 1, "4", 1) == 0)
		memcpy(IsoPosMode, "4", 1);
	else if (memcmp(VisaAddPos + 1, "5", 1) == 0)
		memcpy(IsoPosMode, "5", 1);
	else if (memcmp(VisaAddPos + 1, "9", 1) == 0)
		memcpy(IsoPosMode, "1", 1);
	else
		memcpy(IsoPosMode, "0", 1);

	/* Cardholder Authentication Capability - Position 2 */
	if (memcmp(VisaPosCond, "57", 2) == 0)
		memcpy((IsoPosMode + 1), "1", 1);
	else
		memcpy((IsoPosMode + 1), "0", 1);

	/* Card Retention capability - Position 3 */
	if (memcmp(Merchant, "6011", 4) == 0)
		memcpy((IsoPosMode + 2), "1", 1);
	else
		memcpy((IsoPosMode + 2), "0", 1);

	/* Operating Environment - Position 4 */
	/*memcpy((IsoPosMode + 3), "0", 1);*/

	/*Start IBO20221129 PLUTONL-5456*/
	if (memcmp(VisaAddPos + 1, "1", 1) == 0) /*No terminal Used*/
		IsoPosMode[3] = '0';
	else
	{
		if (memcmp(VisaPosCond, "00", 2) == 0) /*Face to face*/
			IsoPosMode[3] = '1';
		else
		{
			switch (VisaAddPos[0])
			{
				case '5': /*Unattended customer terminal*/
					IsoPosMode[3] = '5'; break;
				case '2':  /*ATM or UCAT*/
					IsoPosMode[3] = 'S'; break;
				case '3':  /*UCAT*/
					IsoPosMode[3] = '2'; break;
				default:
					IsoPosMode[3] = '9'; break;
			}
		}
	}
	/*End IBO20221129 PLUTONL-5456*/

	/* Cardholder Presence Indicator - Position 5 */
	if (memcmp(VisaPosCond, "01", 2) == 0)
		memcpy((IsoPosMode + 4), "1", 1);
	else if (memcmp(VisaPosCond, "05", 2) == 0)
		memcpy((IsoPosMode + 4), "0", 1);
	else if (memcmp(VisaPosCond, "08", 2) == 0)
		memcpy((IsoPosMode + 4), "2", 1);
	else if (memcmp(VisaPosCond, "51", 2) == 0)
		memcpy((IsoPosMode + 4), "4", 1);
	else if (memcmp(VisaPosCond, "55", 2) == 0)
		memcpy((IsoPosMode + 4), "3", 1);
	else if (memcmp(VisaPosCond, "59", 2) == 0)
		memcpy((IsoPosMode + 4), "5", 1);
	else
		memcpy((IsoPosMode + 4), "0", 1);

	/* Card Presence Indicator - Position 6 */
	if (memcmp(VisaPosCond, "00", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(VisaPosCond, "02", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(VisaPosCond, "03", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(VisaPosCond, "05", 2) == 0)
		memcpy((IsoPosMode + 5), "0", 1);
	else if (memcmp(VisaPosCond, "10", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(VisaPosCond, "57", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(VisaPosCond, "71", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else
		memcpy((IsoPosMode + 5), "0", 1);

	/* Card Data Input Mode - Position 7 */
        if (memcmp(VisaAddPos + 2, "1", 1) == 0 || memcmp(VisaAddPos + 2, "2", 1) == 0)   /*AMER 20160608 Fallback*/
                memcpy(IsoPosMode+6, "8", 1);
	else if (memcmp(VisaPosMode, "00", 2) == 0)
		memcpy((IsoPosMode + 6), "0", 1);
	else if (memcmp(VisaPosMode, "01", 2) == 0)
		memcpy((IsoPosMode + 6), "1", 1);
	else if (memcmp(VisaPosMode, "02", 2) == 0)
		memcpy((IsoPosMode + 6), "2", 1);
	else if (memcmp(VisaPosMode, "03", 2) == 0)
		memcpy((IsoPosMode + 6), "3", 1);
	else if (memcmp(VisaPosMode, "04", 2) == 0)
		memcpy((IsoPosMode + 6), "4", 1);
	else if (memcmp(VisaPosMode, "05", 2) == 0)
		memcpy((IsoPosMode + 6), "5", 1);
	else if (memcmp(VisaPosMode, "06", 2) == 0)
		memcpy((IsoPosMode + 6), "7", 1);
	else if (memcmp(VisaPosMode, "90", 2) == 0)
		memcpy((IsoPosMode + 6), "W", 1);
	else if (memcmp(VisaPosMode, "95", 2) == 0)
		memcpy((IsoPosMode + 6), "J", 1);
    else if (memcmp(VisaPosMode, "91", 2) == 0) /* Ajout NAK 280616*/
		memcpy((IsoPosMode + 6), "A", 1);
    else if (memcmp(VisaPosMode, "07", 2) == 0)
		memcpy((IsoPosMode + 6), "M", 1);
	else if (memcmp(VisaPosMode, "10", 2) == 0) /*:: AMA-220816::*/
		memcpy((IsoPosMode + 6), "F", 1);
	else
		memcpy((IsoPosMode + 6), "0", 1);
	/* Gestion E-Commerce Trx */
	if ((strlen(VisaAddPos) > 9)
		&& (memcmp(VisaPosCond, "59", 2) == 0))/**ADD MOU20160315**/
	{

		IsoPosMode[4] = '5';
		if(IsoPosMode[6] != 'F') /*IBO20221025 PLUTONL-5272*/
		{
			if (!memcmp(VisaAddPos + 8, "05", 2))
				IsoPosMode[6] = 'T';
			else if (!memcmp(VisaAddPos + 8, "06", 2))
				IsoPosMode[6] = 'S';
			else if (!memcmp(VisaAddPos + 8, "07", 2))
				IsoPosMode[6] = 'V';
			else if (!memcmp(VisaAddPos + 8, "08", 2))
				IsoPosMode[6] = 'U';
			else
				IsoPosMode[6] = '9';
		}
	}
	/* Cardholder Authentication Method - Position 8 */
	/*NAB12032020 PLUTONL-1287 */
	/*if (memcmp(VisaPosMode + 2, "1", 1) == 0)
		memcpy((IsoPosMode + 7), "1", 1);
	else
		memcpy((IsoPosMode + 7), "0", 1);*/
	if (memcmp(VisaAddPos + 10, "0", 1) == 0)      /*  Not authenticated*/ 
		memcpy((IsoPosMode + 7), "0", 1);
	else if (memcmp(VisaAddPos + 10, "1", 1) == 0)/*SIGNATURE -Manual signature verification */ 
		memcpy((IsoPosMode + 7), "5", 1);
	else if (memcmp(VisaAddPos + 10, "2", 1) == 0)/*PIN*/ 
		memcpy((IsoPosMode + 7), "1", 1);
	else
		memcpy((IsoPosMode + 7), "0", 1);/* Not authenticated*/

	/* Cardholder Authentication Entity - Position 9 */
	memcpy((IsoPosMode + 8), "0", 1);

	/* Rewrite capability - Position 10 */
	memcpy((IsoPosMode + 9), "0", 1);

	/* Terminal Output capability - Position 11 */
	memcpy((IsoPosMode + 10), "0", 1);

	/* PIN Entry capability - Position 12 */
	if (memcmp(VisaPosMode + 2, "2", 1) == 0)
		memcpy((IsoPosMode + 11), "0", 1);
	else if (memcmp(VisaPosMode + 2, "8", 1) == 0)
		memcpy((IsoPosMode + 11), "S", 1);
	else
		memcpy((IsoPosMode + 11), "1", 1);

	if ((strlen(VisaAddPos) > 9)
		&& (memcmp(VisaPosCond, "59", 2) == 0)) /*IBO20221025 PLUTONL-5272*/
	{
		if (!memcmp(VisaAddPos + 8, "05", 2))
			EcomAuthInfo[0] = P7_F048_P66_POS1_ECOM_AUTHENTICATED;
		else if (!memcmp(VisaAddPos + 8, "06", 2))
			EcomAuthInfo[0] = P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT;
		else if (!memcmp(VisaAddPos + 8, "07", 2))
			EcomAuthInfo[0] = P7_F048_P66_POS1_ECOM_SEC_CHAN;
		else if (!memcmp(VisaAddPos + 8, "08", 2))
			EcomAuthInfo[0] = P7_F048_P66_POS1_ECOM_UNSECURE;
		else
			EcomAuthInfo[0] = P7_F048_P66_POS1_ECOM;

		*nEcomInfoLen = 1;
	}
	IsoPosMode[12] = '\0';

	sprintf(sLine, "End   VisaToIsoPosMode(%s)", IsoPosMode);
	trace_event(sLine, PROCESSING);
	return;
}

/*PLUTONL-3660*/
/**************************************************************************/
void
	VisaToIsoAcceptorName(int  nIndexCtx,char *VisaAcceptName,char *IsoAcceptName)
{

	char            pVisaCodeAlpha[2 +1];
	char   			pIsoCodeAlpha[3 + 1]; 
	char            sLine[MAX_LINE_TRC];
	int             nRetCode;

	sprintf(sLine, "Start VisaToIsoAcceptorName[%s]", VisaAcceptName);
	trace_event(sLine, PROCESSING);
	
	memset (pVisaCodeAlpha,   0, sizeof(pVisaCodeAlpha));
	memset (pIsoCodeAlpha,   0, sizeof(pIsoCodeAlpha));

	memset(IsoAcceptName,' ',40);
	/*merchant acronym*/
	memcpy(IsoAcceptName,VisaAcceptName,24);
	/*city name*/
	memcpy(IsoAcceptName+24,VisaAcceptName+25,13);
	/*Country code alpha*/
	memcpy(pVisaCodeAlpha, VisaAcceptName+38, 2);
	nRetCode = GetCountryCodeAlphaVISA(nIndexCtx, pVisaCodeAlpha, pIsoCodeAlpha);

	if (nRetCode == OK)
	{
		memcpy(IsoAcceptName+37,pIsoCodeAlpha ,3);	
	}
	else
	{
		trace_event(">>>> Error Getting Alpha Country Code",ERROR);
	}
	IsoAcceptName[40]='\0';
	sprintf(sLine, "End   VisaToIsoAcceptorName[%s]", IsoAcceptName);
	trace_event(sLine, PROCESSING);
	return;
}


/**************************************************************************/
/* IsoToVisaPosCondition()                                              */
/* */
/* Converts an ISO standard POS condition code into a VISA code.        */
/* Input:  IsoMode: ISO POS Entry Mode                                  */
/* MerchantType: merchant type                                  */
/* Output: VisaCond: VISA POS condition code                            */
/**************************************************************************/
void
	IsoToVisaPosCondition(char *IsoMode,
	char *MerchantType,
	int acc_verification_only,
	char *VisaCond)
{
	trace_event("Start IsoToVisaPosCondition()", PROCESSING);

	/*Start AMER: Enh18.Q2 PROD00054205 Art 1.1: The code below is replacing the old one*/
	if (memcmp(MerchantType, MCC_AUTOMATED_CASH, P7_F018_LEN) == 0)	/* ATM transaction */
		memcpy(VisaCond, B1_F025_CP_PIN, B1_F025_LEN);
	else if(IsoMode[6] == P7_F022_P7_ECOM || IsoMode[6] == P7_F022_P7_ECOM_NON_AUTH_ATTEMPT || IsoMode[6] == P7_F022_P7_ECOM_AUTHENTICATED || IsoMode[6] == P7_F022_P7_ECOM_UNSECURE || IsoMode[6] == P7_F022_P7_ECOM_SEC_CHAN)	/*Ecom*/
		memcpy(VisaCond, B1_F025_PUB_ECOM, B1_F025_LEN);
	else if (IsoMode[4] == '1')	/* Cardholder not present */
		memcpy(VisaCond, B1_F025_CH_NP, B1_F025_LEN);
	else if ((IsoMode[4] == '0') && (IsoMode[5] == '0')) /* Cardholder present, card not present */
		memcpy(VisaCond, B1_F025_CH_P_CRD_NP, B1_F025_LEN);	
	else if ((IsoMode[4] == '5') || (IsoMode[4] == '2') || (IsoMode[4] == '3')) /* Mail order */
		memcpy(VisaCond, B1_F025_MOTO_RECURRING, B1_F025_LEN);	
	else if(acc_verification_only == 1)
		memcpy(VisaCond, B1_F025_ACC_VERIF, B1_F025_LEN);	
	else
		memcpy(VisaCond, B1_F025_NORMAL_TRN, B1_F025_LEN);
	/*End AMER Enh18.Q2 PROD00054205 Art 1.1*/

	/*if (memcmp(MerchantType, "6011", 4) == 0)	/* ATM transaction * /
		memcpy(VisaCond, "02", 2);
	else if (IsoMode[4] == '1')	/* Cardholder not present * /
		memcpy(VisaCond, "01", 2);
	else if (IsoMode[1] == '1')	/* PIN * /
		/* * memcpy (VisaCond, "57", 2); * * /
		memcpy(VisaCond, "00", 2);
	else
		/* Customer present, card not present * /
		if ((IsoMode[4] == '0') && (IsoMode[5] == '0'))
			memcpy(VisaCond, "05", 2);
		else
			/* Mail order * /
			if ((IsoMode[4] == '5') || (IsoMode[4] == '2') || (IsoMode[4] == '3'))
				memcpy(VisaCond, "08", 2);
			else
				/* Authorized customer * /
				if (IsoMode[4] == '4')
					memcpy(VisaCond, "51", 2);
				else
					/* Phone order * /
					if (IsoMode[4] == '2')
						memcpy(VisaCond, "55", 2);
					else if ((IsoMode[3] == '2') ||
						(IsoMode[3] == '4') ||
						(IsoMode[3] == 'T') ||
						(IsoMode[3] == '5'))
						memcpy(VisaCond, "02", 2);
					else
						memcpy(VisaCond, "00", 2);

					if ((memcmp(MerchantType, "5961", 4) == 0) || (memcmp(MerchantType, "7922", 4) == 0))
						memcpy(VisaCond, "08", 2);
					/* E-Comm MNG * /
					switch (IsoMode[6]) {
					case 'S':
					case 'U':
					case 'T':
					case 'V':
						memcpy(VisaCond, "59", 2);
					}*/

					trace_event("End  IsoToVisaPosCondition(OK)", PROCESSING);
					return;
}



/**************************************************************************/
/* IsoToVisaSecData()                                                   */
/* */
/* Converts ISO standard Security data (field 53) into VISA-compliant   */
/* format.                                                              */
/* Input:  IsoSecData: ISO field 53                                     */
/* Output: VisaSecData: VISA security data                              */
/**************************************************************************/
void
	IsoToVisaSecData(char *IsoSecData,
	char *VisaSecData)
{
	int             v_offset = 0;

	trace_event("Start  IsoToVisaSecData()", PROCESSING);

	/* Security Format Code */
	memcpy(VisaSecData + v_offset, "20", 2);
	v_offset += 2;

	/* Pin Encryption Algorithm Id */
	memcpy(VisaSecData + v_offset, "01", 2);
	v_offset += 2;

	/* Pin Block Format Code */
	if (memcmp(IsoSecData + 2, "01", 2) == 0)
		memcpy(VisaSecData + v_offset, "01", 2);
	else if (memcmp(IsoSecData + 2, "10", 2) == 0)
		memcpy(VisaSecData + v_offset, "01", 2);
	else if (memcmp(IsoSecData + 2, "11", 2) == 0)
		memcpy(VisaSecData + v_offset, "05", 2);
	else
		memcpy(VisaSecData + v_offset, "01", 2);

	v_offset += 2;

	/* Pin Zone Key Index */
	memcpy(VisaSecData + v_offset, "01", 2);
	v_offset += 2;

	/* Pin Data Type : pin is present in field 52  */
	memcpy(VisaSecData + v_offset, "00", 2);
	v_offset += 2;

	/* Reserved Positions */
	memcpy(VisaSecData + v_offset, "000000", 6);


	trace_event("End  IsoToVisaSecData()", PROCESSING);
	return;
}



/*************************************************************************/
/* BuildAddPosInfo()                                                   */
/* */
/* Builds a BASEI additional POS info field (60)                       */
/* Input:  ISO POS mode                                                */
/* Merchant type                                               */
/* ISO processing code                                         */
/* Output: BASEI field 60                                              */
/*************************************************************************/
void
	BuildAddPosInfo(char *IsoPosMode,
	char *MerchantType,
	char *IsoProcCode,
	char *RecurringFlag,
	char *sAddAuthInfo,
	char *sPosExtData,/*PLUTONL-3326*/
	char *VisaAddPosInfo,
	TSIsoInfo * IsoInfo)
{

	char            sLine[MAX_LINE_TRC];
	char            tmpField[LG_MAX];
	int             tmpFieldLen;

	TSTlvPrivate    kTlvInfo;	/*AEA20210712 Enh21.Q4 Art10.1.11*/

	trace_event("Start BuildAddPosInfo()", PROCESSING);
	sprintf(sLine, "IsoPosMode : %s", IsoPosMode);
	trace_event(sLine, PROCESSING);
	sprintf(sLine, "MerchantType : %s", MerchantType);
	trace_event(sLine, PROCESSING);
	sprintf(sLine, "IsoProcCode : %s", IsoProcCode);
	trace_event(sLine, PROCESSING);

	InitTlvPrivate(&kTlvInfo);	/*AEA20210712 Enh21.Q4 Art10.1.11*/

	/*Start AMER20180314 PROD00054205 Enh18.Q2 Art 1.1*/
	/* Field 60.1 ï¿½Position 1, Terminal Type */
	if (memcmp(MerchantType, "6011", 4) == 0)
		memcpy(VisaAddPosInfo, "2", 1);
	else if ((IsoPosMode [3] == '2')||(IsoPosMode [3] == '4')||(IsoPosMode [3] == '5')||(IsoPosMode [3] == 'S')||(IsoPosMode [3] == 'T')||(IsoPosMode [3] == 'U')||(IsoPosMode [3] == 'V'))
		memcpy(VisaAddPosInfo, "3", 1);
	else if (IsoPosMode[3] == '5')
		memcpy(VisaAddPosInfo, "5", 1);
	/*PLUTONL-3326*/
	else if (sPosExtData[P7_F048_P61_MPOS_IND_OFST]==P7_F048_P61_POS21_MPOS_TERM)/*Dedicated mPOS Terminal with PCI compliant dongle [with or without keypad].*/
	{
		memcpy(VisaAddPosInfo, "9", 1);
	}
    /*PLUTONL-3326*/
	else
		memcpy(VisaAddPosInfo, "0", 1);
	
	/* Field 60.2 ï¿½Position 2, Terminal Entry Capability*/
	if ((IsoPosMode[0]) == '1')
		memcpy(VisaAddPosInfo + 1, "1", 1);
	else if ((IsoPosMode[0]) == '2' || IsoPosMode[0] == '7')
		memcpy(VisaAddPosInfo + 1, "2", 1);
	else if ((IsoPosMode[0]) == '3')
		memcpy(VisaAddPosInfo + 1, "3", 1);
	else if ((IsoPosMode[0]) == '4')
		memcpy(VisaAddPosInfo + 1, "4", 1);
	else if ((IsoPosMode[0]) == '5' || IsoPosMode[0] == '8' || IsoPosMode[0] == '9')
		memcpy(VisaAddPosInfo + 1, "5", 1);
	else if ((IsoPosMode[0]) == 'M')
		memcpy(VisaAddPosInfo + 1, "5", 1);/*ZKO:08112018; visa certif*/
	else if ((IsoPosMode[0]) == '6')
		memcpy(VisaAddPosInfo + 1, "9", 1);
	else
		memcpy(VisaAddPosInfo + 1, "0", 1);
	
	
	/* Field 60.3 ï¿½Position 3, Chip Condition Codes */
	if (IsoPosMode[6] == '8')  /*  BURGAN ONSITE CERTIF MKB/AHA20171002 */ /* BURGAN_06 */
		memcpy(VisaAddPosInfo + 2, "1", 1);
	else if (IsoPosMode[6] == '5')
	{
		if(GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) != SUCCESS)
			memcpy(VisaAddPosInfo + 2, "1", 1);
		else
			memcpy(VisaAddPosInfo + 2, "0", 1);
	}
	else if (IsoPosMode[4] != '5' 
	&& IsoPosMode[0] != '5'
	&& IsoPosMode[0] != '8'/*ZKO26112018*/
	&& IsoPosMode[0] != '9'/*ZKO26112018*/
	&& IsoPosMode[0] != 'M' /*ZKO26112018*/)   /* Si trx non E-com or Chip Only 2 Pos a envoyee */
	{	
		VisaAddPosInfo[2] = '\0';
		sprintf(sLine, "...000 VisaAddPosInfo = [%s]", VisaAddPosInfo);
		trace_event(sLine, PROCESSING);

		trace_event("End  BuildAddPosInfo()", PROCESSING);
		return;
	} 
	else
		memcpy(VisaAddPosInfo + 2, "0", 1);

	/* Field 60.4 ï¿½Position 4, Special Condition Indicator */
	/*VisaAddPosInfo[3] = '0';*/
	
	/*AMER20190219 PROD00067537: Enh19.Q2 Art2.1*/
	if(memcmp(IsoProcCode, P7_F003_QUASICASH, P7_F003_P1_LEN) == 0)
	{
		if(sAddAuthInfo[P7_F048_P64_CRYPTO_CURR_OFST] == '1')
			VisaAddPosInfo[3] = '7';	/*Purchase of crypto-currency*/
		else 
			VisaAddPosInfo[3] = '8';	/*Quasi-cash*/
	}  
	else
		VisaAddPosInfo[3] = '0';
	/*End AMER20190219 PROD00067537: Enh19.Q2 Art2.1*/
	
	/* Field 60.5 Positions 5 6, Merchant Group Indicator ï¿½Not Applicable. */
	memcpy(VisaAddPosInfo + 4, "00", 2);
	
	/* PROD00037016: Visa requires Chip Transaction Indicator to be 1 when ICC data is sent and not '0'*/
	if(GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		memcpy(VisaAddPosInfo + 6, "1", 1);
	else
		memcpy(VisaAddPosInfo + 6, "0", 1);

	/* Field 60.7 ï¿½Position 8, Chip Card Authentication Reliability Indicator */
	memcpy(VisaAddPosInfo + 7, "0", 1);

	/* Field 60.8 ï¿½Positions 9 10, Mail/Phone/Electronic Commerce and Payment Indicator */
	if (IsoPosMode[4] == '2' || IsoPosMode[4] == '3')
		memcpy(VisaAddPosInfo + 8, "01", 2);
	else if (RecurringFlag[0] == 'R')
		memcpy(VisaAddPosInfo + 8, "02", 2);
	else if (RecurringFlag[0] == 'I')
		memcpy(VisaAddPosInfo + 8, "03", 2);
	else if (IsoPosMode[6] == '9' || IsoPosMode[6] == 'V')
		memcpy(VisaAddPosInfo + 8, "07", 2);
	else if (IsoPosMode[6] == 'S')
		memcpy(VisaAddPosInfo + 8, "06", 2);
	else if (IsoPosMode[6] == 'T')
		memcpy(VisaAddPosInfo + 8, "05", 2);
	else if (IsoPosMode[6] == 'U')
		memcpy(VisaAddPosInfo + 8, "08", 2);
	else
		memcpy(VisaAddPosInfo + 8, "00", 2);

	/* Field 60.9 ï¿½Position 11, Cardholder ID Method Indicator */
	if (IsoPosMode[7] == '5')
		VisaAddPosInfo[10] = '1';
	else if (IsoPosMode[7] == '1')
		VisaAddPosInfo[10] = '2';
	else if (IsoPosMode[4] == '2' || IsoPosMode[4] == '3' || IsoPosMode[4] == '5')
		VisaAddPosInfo[10] = '4';
	else
		VisaAddPosInfo[10] = '0';
	
	/* Field 60.10/Position 12: Additional Authorization Indicators */
	VisaAddPosInfo[11] = '0';
	/*Start AEA20210712 Enh21.Q4 Art10.1.11*/
	if(GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);
		if (GetTlvPrivate( POS_DATA_EXT_TAG, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			if( tmpField[0] == '1')
			{
				if(GetIsoField(ISO_FUNCTION_CODE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS && memcmp(tmpField, P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN) == 0)
					VisaAddPosInfo[11] = '3';
				else
					VisaAddPosInfo[11] = '1';
			}
			else
			{
				if(GetIsoField(ISO_FUNCTION_CODE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS && memcmp(tmpField, P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN) == 0)
					VisaAddPosInfo[11] = '2';
				else
					VisaAddPosInfo[11] = '0';
			}
		}
	}
	/*End AEA20210712 Enh21.Q4 Art10.1.11*/

	/*End AMER20180314 PROD00054205 Enh18.Q2 Art 1.1*/
	

	/*Old mapping*/
	/* Terminal Type * /
	if ((IsoPosMode[3] == '2' || IsoPosMode[3] == '4')
		&& (memcmp(MerchantType, "6011", 4) == 0))
		memcpy(VisaAddPosInfo, "2", 1);
	else if ((IsoPosMode [3] == '2')||(IsoPosMode [3] == '4')||(IsoPosMode [3] == '5')||(IsoPosMode [3] == 'S')||(IsoPosMode [3] == 'T')||(IsoPosMode [3] == 'U')||(IsoPosMode [3] == 'V'))
		memcpy(VisaAddPosInfo, "3", 1);
	else if (IsoPosMode[4] == '5')
		memcpy(VisaAddPosInfo, "5", 1);
	else
		memcpy(VisaAddPosInfo, "0", 1);


	/* Terminal Entry Capability * /
	if ((IsoPosMode[0]) == '1')
		memcpy(VisaAddPosInfo + 1, "1", 1);
	else if ((IsoPosMode[0]) == '2')
		memcpy(VisaAddPosInfo + 1, "2", 1);
	else if ((IsoPosMode[0]) == '3')
		memcpy(VisaAddPosInfo + 1, "3", 1);
	else if ((IsoPosMode[0]) == '4')
		memcpy(VisaAddPosInfo + 1, "4", 1);
	else if ((IsoPosMode[0]) == '9')
		memcpy(VisaAddPosInfo + 1, "5", 1);
	else if ((IsoPosMode[0]) == '8')
		memcpy(VisaAddPosInfo + 1, "5", 1);
	else if ((IsoPosMode[0]) == '7')
		memcpy(VisaAddPosInfo + 1, "2", 1);
	else if ((IsoPosMode[0]) == '6')
		memcpy(VisaAddPosInfo + 1, "9", 1);
	else
		memcpy(VisaAddPosInfo + 1, "0", 1);


	/* Chip condition code * /
	/** ************************ ** /
	if ( '5' == IsoPosMode[6]
	&& GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) != SUCCESS
		) {
			/** Fall Back ** /
			memcpy(VisaAddPosInfo + 2, "1", 1);
	} else if ('8' == IsoPosMode[6])  /*  BURGAN ONSITE CERTIF MKB/AHA20171002 * / /* BURGAN_06 * /
		memcpy(VisaAddPosInfo + 2, "1", 1);

	else if ('5' == IsoPosMode[6])
		memcpy(VisaAddPosInfo + 2, "0", 1);

	/* Si trx non E-com or Chip Only 2 Pos a envoye * /
	else if (IsoPosMode[4] != '5' && IsoPosMode[0] != '5') {
		VisaAddPosInfo[2] = '\0';
		sprintf(sLine, "...000 VisaAddPosInfo = [%s]", VisaAddPosInfo);
		trace_event(sLine, PROCESSING);

		trace_event("End  BuildAddPosInfo()", PROCESSING);
		return;

	} else
		memcpy(VisaAddPosInfo + 2, "0", 1);

	/* Reserved * /
	memcpy(VisaAddPosInfo + 3, "0", 1);

	/* Merchant Group Indicator * /
	if (memcmp(IsoProcCode, "11", 2) == 0)	/* Quasi-cash * /
		memcpy(VisaAddPosInfo + 4, "10", 2);
	else
		/* ATM cash * /
		if ((memcmp(IsoProcCode, "01", 2) == 0) &&
			(memcmp(MerchantType, "6011", 4) == 0))
			memcpy(VisaAddPosInfo + 4, "09", 2);
		else
			/* Other cash transaction * /
			if (memcmp(IsoProcCode, "01", 2) == 0 || memcmp(IsoProcCode, "17", 2) == 0)
				memcpy(VisaAddPosInfo + 4, "11", 2);
			else
				/* Mail-Phone order * /
				if ((IsoPosMode[4] == '5') || (IsoPosMode[4] == '2') || (IsoPosMode[4] == '3'))
					memcpy(VisaAddPosInfo + 4, "06", 2);
				else			/* any other purchase * /
					memcpy(VisaAddPosInfo + 4, "08", 2);

	/* VSDC 7 * /
	/* PROD00037016: Visa requires Chip Transaction Indicator to be 1 when ICC data is sent and not '0'* /
	if( GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS )
		memcpy(VisaAddPosInfo + 6, "1", 1);
	else
		memcpy(VisaAddPosInfo + 6, "0", 1);

	if ((IsoPosMode[6] == '5'))
		memcpy(VisaAddPosInfo + 6, "1", 1);
	/* Pos 8 * /
	memcpy(VisaAddPosInfo + 7, "0", 1);
	/* Pos 9 And 10 * /
	if ((IsoPosMode[4] == '5'))
		memcpy(VisaAddPosInfo + 8, "00", 2);
	switch (IsoPosMode[6]) {
	case '9':
		memcpy(VisaAddPosInfo + 8, "00", 2);
		break;
	case 'S':
		memcpy(VisaAddPosInfo + 8, "06", 2);
		break;
	case 'T':
		memcpy(VisaAddPosInfo + 8, "05", 2);
		break;
	case 'U':
		memcpy(VisaAddPosInfo + 8, "08", 2);
		break;
	case 'V':
		memcpy(VisaAddPosInfo + 8, "07", 2);
		break;
	default:
		memcpy(VisaAddPosInfo + 8, "00", 2);
		break;
	}
	/* Chip Trx * /
	if ((IsoPosMode[0] == '5') || (IsoPosMode[6] == '5')) {
		VisaAddPosInfo[0] = '0';
		VisaAddPosInfo[1] = '5';
		if (IsoPosMode[6] == '2') {	/* Capbilite Trminal Chip But
									* Transction was only Track
									* II Provided * /
			VisaAddPosInfo[2] = '2';
			VisaAddPosInfo[3] = '0';
			VisaAddPosInfo[4] = '\0';
		} else {
			VisaAddPosInfo[2] = '0';
			VisaAddPosInfo[6] = '1';
			VisaAddPosInfo[7] = '0';
			VisaAddPosInfo[8] = '\0';
		}
	}
	if (
		'5' == IsoPosMode[6]
	&& GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) != SUCCESS
		) {
			/** Fall Back ** /
			memcpy(VisaAddPosInfo + 2, "2", 1);
	}
	VisaAddPosInfo[10] = '\0';*/
	sprintf(sLine, "... 001 VisaAddPosInfo = [%s]", VisaAddPosInfo);
	trace_event(sLine, PROCESSING);

	trace_event("End  BuildAddPosInfo()", PROCESSING);
	return;
}

/*************************************************************************/
/* IsoToVisaOrigData()                                                 */
/* */
/* Converts ISO Original Data Elements (field 56) into a VISA format   */
/* (field 90).                                                         */
/* Input:  ISO Original Data Elements                                  */
/* Output: VISA ORIGINAL Data Elements                                 */
/*************************************************************************/
void
	IsoToVisaOrigData(char *IsoOrigData,
	char *VisaOrigData)
{
	char            AcqId[11 + 1];
	char            buffer[10];
	int             AcqIdLen;

	fprintf(stderr, "\nISO Original Data = %35s", IsoOrigData);

	memset(AcqId, 0, sizeof(AcqId));
	memset(buffer, 0, sizeof(buffer));

	/* original message type */
	memcpy(buffer, IsoOrigData, 4);
	sprintf(VisaOrigData, "%04d", atoi(buffer) - 1000);

	/* original stan */
	memcpy(VisaOrigData + 4, IsoOrigData + 4, 6);

	/* original transmission date & time */
	memcpy(VisaOrigData + 10, IsoOrigData + 12, 10);

	/* original acquirer Id */
	memcpy(VisaOrigData + 20, IsoOrigData + 24, 11);

	/* filler */
	memcpy(VisaOrigData + 31, "00000000000", 11);

}


/*************************************************************************/
/* VisaToIsoOrigData()                                                 */
/* */
/* Converts VISA Original Data Elements (field 90) into an ISO format  */
/* (field 56).                                                         */
/* Input:  VISA Original Data Elements                                 */
/* VISA Acquirer Id                                            */
/* Output: ISO ORIGINAL Data Elements                                  */
/*************************************************************************/
void
	VisaToIsoOrigData(char *VisaOrigData,
	char *VisaAcquirerId,
	char *IsoOrigData)
{
	char            buffer[10];


	memset(buffer, 0, sizeof(buffer));

	/* original message type */
	memcpy(buffer, VisaOrigData, 4);
	sprintf(IsoOrigData, "%4d", atoi(buffer) + 1000);

	/* original stan */
	memcpy(IsoOrigData + 4, VisaOrigData + 4, 6);

	/* original transmission date & time */
	memcpy(IsoOrigData + 10, "00", 2);
	memcpy(IsoOrigData + 12, VisaOrigData + 10, 10);

	/* original acquirer Id */
	sprintf(IsoOrigData + 22, "%02lu", strlen(VisaAcquirerId));
	memcpy(IsoOrigData + 24, VisaOrigData + 20, 11);

}
/*void VisaToIsoMoneyTransData(TSVisaInfo * VisaInfo, TlvInfo*   pkIsoMoneyTransfTlv, char* sBAI)*/
void VisaToIsoMoneyTransData(TSVisaInfo * VisaInfo, TlvInfo*   pkIsoMoneyTransfTlv, TlvInfo*  pkIsoPayFacTlv, char* sBAI)  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/ 
{
	char			sData[1024];
	char			sDOB[12];
	int				nLength;
	TlvInfo			kVisaTransSpecTlv;
	TlvInfo			kVisaAppIdentTlv;
	TlvInfo			kVisaSenderDataTlv;
	/*TlvInfo			kVisaAddSenderDataTlv;*/
	char 			sSenderRecipientData[B1_F056T03_9F1F_LEN + 1];  /*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/  
    TlvInfo			kVisaPayFacTlv;	     /*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/
	TlvInfo			kVisaCustmRelatdDataTlv;    /*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/
	TlvInfo			kVisaCustomerIdDataTlv;		/*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/
    
	if (GetVisaField(VISA_TRANS_DESC, VisaInfo, sData, &nLength) != SUCCESS) 
	{
		return;
	}

	memset(sDOB,'\0',sizeof(sDOB));
	memset(sSenderRecipientData,'\0',sizeof(sSenderRecipientData)); /*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/  
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);
	InitVisaAppIdentDataTlv(&kVisaAppIdentTlv);
	InitVisaSenderDataTlv(&kVisaSenderDataTlv);
	/*InitVisaAddSenderDataTlv(&kVisaAddSenderDataTlv);*/
	InitVisaPayFacDataTlv(&kVisaPayFacTlv);     /*IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/

	AnalyseTlvInfo(sData, nLength, &kVisaTransSpecTlv);
	/*Start IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/
	if( GetTlvInfoField (B1_F104_PAY_FAC_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaPayFacTlv);
	}
	/*End IZL 20220121 PLUTONL-4015 ENH 22Q2 VISA Art3.2*/
	if( GetTlvInfoField (B1_F104_BUSINESS_APP_ID, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaAppIdentTlv);
	}
	if( GetTlvInfoField (B1_F104_SENDER_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaSenderDataTlv);
	}
	/*
	if( GetTlvInfoField (B1_F104_ADDITIONAL_SENDER_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaAddSenderDataTlv);
	}
	*/
	if( GetTlvInfoField (B1_F104T57_BUSINESS_APP_ID, &kVisaAppIdentTlv, sData, &nLength) == SUCCESS )
	{
		memcpy(sBAI,sData,B1_F104T57_LEN);
		sData[B1_F104T57_LEN]='\0';
		if( memcmp(sData,B1_F104T57_AA,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_ACC_2_ACC, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_PP,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_P_2_P, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_TU,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_PP_RELOAD, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_OG,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_GAMBLING_REPAY, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_CP,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_BILL_PAY, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_PD,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_PAYROLL, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_GD,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_GOV_2_C, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_MD,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_MER_REB, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,B1_F104T57_PG,B1_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_C_2_GOV, PAY_TYPE_LEN);
		}
		else
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_OTHER_PAY, PAY_TYPE_LEN);
		}


	}
	
	/* Start AEA20210712 Enh21.Q4 Art2.9*/
	if( GetTlvInfoField (B1_F104T57_PAY_PURPOSE, &kVisaAppIdentTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_PAYMENT_PURPOSE, pkIsoMoneyTransfTlv, sData, nLength);
	}
	/* End AEA20210712 Enh21.Q4 Art2.9*/

	/*P7_F048S20_FUNDS_SOURCE*/

	if( GetTlvInfoField (B1_F104T5F_SENDER_REF_NBR, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_REF_NBR, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_ACCOUNT_NO, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_ACCOUNT_NO, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_NAME, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_STREET_ADDR, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_STREET_ADDR, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_CITY_NAME, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_CITY_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_STATE, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_STATE, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (B1_F104T5F_SENDER_COUNTRY, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_COUNTRY, pkIsoMoneyTransfTlv, sData, nLength);
	}

	if( GetTlvInfoField (B1_F104T5F_RECIPIENT_NAME, &kVisaSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_RECIPIENT_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	
	if (GetVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, sData, &nLength) == SUCCESS)
	{
		memcpy(sDOB,sData+11+2,2);
		memcpy(sDOB+2,sData+11,2);
		memcpy(sDOB+4,sData+11+4,2);
		AddTlvInfoField(P7_F048S20_SENDER_DOB, pkIsoMoneyTransfTlv, sDOB, 6);
	}

	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
	if( GetVisaField (VISA_I_ORIG_DATA, VisaInfo, sData, &nLength ) == SUCCESS)
	{
		InitVisaCustmRelatdDataTlv(&kVisaCustmRelatdDataTlv);		
		AnalyseTlvInfo(sData, nLength, &kVisaCustmRelatdDataTlv); 
		InitVisaCustomerIdDataTlv(&kVisaCustomerIdDataTlv);    
		if( GetTlvInfoField (B1_F056_CUSTOMER_ID_DATA, &kVisaCustmRelatdDataTlv, sData, &nLength) == SUCCESS )   
		{   
			AnalyseTlvInfo(sData, nLength, &kVisaCustomerIdDataTlv);
			GetTlvInfoField (B1_F056T03_DATA_SENDER_RECIP, &kVisaCustomerIdDataTlv, sSenderRecipientData, &nLength);
			if( memcmp(sSenderRecipientData,B1_F056T03_9F1F_05,B1_F056T03_9F1F_LEN) == 0 )
			{
				if( GetTlvInfoField (B1_F056T03_ID_TYPE_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_SENDER_ID_TYPE_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
				
				if( GetTlvInfoField (B1_F056T03_ID_SUBTYPE_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_SENDER_ID_SUBTYPE_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
				if( GetTlvInfoField (B1_F056T03_ACQ_POPULATED_ID_VAL, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_SENDER_ID, pkIsoMoneyTransfTlv, sData, nLength);
				}
				
				if( GetTlvInfoField (B1_F056T03_CUSTOMER_ID_ISS_CNTRY_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_SENDER_ID_ISS_CNTRY_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
			}
			else if( memcmp(sSenderRecipientData,B1_F056T03_9F1F_06,B1_F056T03_9F1F_LEN) == 0 )
			{
				if( GetTlvInfoField (B1_F056T03_ID_TYPE_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_RECEIVER_ID_TYPE_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
				
				if( GetTlvInfoField (B1_F056T03_ID_SUBTYPE_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_RECEIVER_ID_SUBTYPE_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
				
				if( GetTlvInfoField (B1_F056T03_ACQ_POPULATED_ID_VAL, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S20_RECEIVER_ID, pkIsoMoneyTransfTlv, sData, nLength);
				}
				
				if( GetTlvInfoField (B1_F056T03_CUSTOMER_ID_ISS_CNTRY_CODE, &kVisaCustomerIdDataTlv, sData, &nLength) == SUCCESS )  /*IZL 17022022 PLUTONL-4387 ENH 22.Q2 VISA Art3.2*/
				{
					AddTlvInfoField(P7_F048S20_RECEIVER_ID_ISS_COUNTRY_CODE, pkIsoMoneyTransfTlv, sData, nLength);
				}
			}
		}
		/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
		/*if( GetTlvInfoField (B1_F104T56_TRANSFER_SERVC_FOR_OCT, &kVisaPayFacTlv, sData, &nLength) == SUCCESS )*/
		if( GetTlvInfoField (B1_F104T56_ACC_LEGAL_BUSINESS_NAME, &kVisaPayFacTlv, sData, &nLength) == SUCCESS ) /*ZKO 05042022 PLUTONL-4550 ENH 22.Q2 VISA Art3.2*/
		{
			/*AddTlvInfoField(P7_F048P21_TRANSF_SERVC_PROVID_NAME, pkIsoPayFacTlv, sData, nLength);*/
			AddTlvInfoField(P7_F048P21_ACC_LEGAL_BUSINESS_NAME, pkIsoPayFacTlv, sData, nLength); /*ZKO 05042022 PLUTONL-4550 ENH 22.Q2 VISA Art3.2*/	
		}

		if( GetTlvInfoField (B1_F104T56_PAY_FACILITATOR, &kVisaPayFacTlv, sData, &nLength) == SUCCESS )
		{
			AddTlvInfoField(P7_F048P21_PAY_FACILITATOR_NAME, pkIsoPayFacTlv, sData, nLength);
		}	
		/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
	}
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
}



void VisaBuildAuthId(TSVisaInfo * VisaInfo, char* sAuthId)
{
	char	sFieldData[24];
	int		nLength;

	memcpy(sAuthId , ResourceId, 2);
	GetVisaField(VISA_AUDIT_NBR, VisaInfo, sFieldData, &nLength);
	memcpy(sAuthId + 2, sFieldData, P7_F011_LEN);
	GetVisaField(VISA_REFERENCE_NBR, VisaInfo, sFieldData, &nLength);
	memcpy(sAuthId + 2 + P7_F011_LEN, sFieldData, P7_F037_LEN);
	sAuthId[2+P7_F011_LEN+P7_F037_LEN]='\0';

}

	/* START MOD CFE PROD00045718 20170828*/
void VisaToIsoAddAmount(TSVisaInfo * VisaInfo, char* sIsoAddMount)
{
	int		i,j;
	int		nLength;
	int		nAddAmount;
	char	sLocAddAmount[20 + 1];
	char    sCashBackAmount [20 +1];
	int     tmpFieldLen1;	
	char    NwAddAmount [256];
	char    sOtherAmnt [256];
	char    currency [3 + 1];
	char    sProcessCode[6 + 1];
	int     tmpFieldLen;

	memset(NwAddAmount,0,sizeof(NwAddAmount)); 
	memset(sOtherAmnt,0,sizeof(sOtherAmnt));
	memset(currency,0,sizeof(currency));
	memset(sProcessCode,0,sizeof(sProcessCode));
	memset(sCashBackAmount,0,sizeof(sCashBackAmount));
	
	j = 0;
	if (GetVisaField (VISA_ADTNL_AMOUNTS, VisaInfo,NwAddAmount, &tmpFieldLen) == SUCCESS) 
	{
	nLength = strlen(NwAddAmount);
	for(i=0; i < nLength; i+=20)
	{
		nAddAmount = NOK;
		memcpy(sLocAddAmount,NwAddAmount+i,20);

		if( memcmp(sLocAddAmount + 2,B1_F054_AT_LG_BAL,B1_F054_AT_LEN) == 0 ||
			memcmp(sLocAddAmount + 2,B1_F054_AT_AVAILABLE,B1_F054_AT_LEN) == 0 ||
			memcmp(sLocAddAmount + 2,B1_F054_AT_ORIG_AMNT,B1_F054_AT_LEN) == 0  ||
			memcmp(sLocAddAmount + 2,B1_F054_AT_CUMUL_AMNT,B1_F054_AT_LEN) == 0)	/*AMER20170426: PROD00041736*/
		{
			nAddAmount = OK;
		}
		if( memcmp(sLocAddAmount + 2,"00",B1_F054_AT_LEN) == 0 )
		{
			memcpy(sLocAddAmount + 2,P7_F054_AT_UNSPECIFIED,P7_F054_AT_LEN);
			nAddAmount = OK;
		}
		if( nAddAmount == OK )
		{
			memcpy(sIsoAddMount + j, sLocAddAmount,20);
			j+=20;
		}
	}
	}
	if (GetVisaField (VISA_OTHER_AMOUNTS, VisaInfo,sOtherAmnt, &tmpFieldLen) == SUCCES) {
	 
	 	trace_event(" MOD CFE ==> champ 61 ", TRACE);
	GetVisaField (VISA_TRANS_CRNCY, VisaInfo,currency, &tmpFieldLen);
	GetVisaField (VISA_PROC_CODE, VisaInfo,sProcessCode, &tmpFieldLen);	
	memcpy(sCashBackAmount, sProcessCode + 2,2); 
	memcpy(sCashBackAmount + 2, "40",2); /* cashback amount */
	memcpy(sCashBackAmount + 4, currency,3);
	memcpy(sCashBackAmount + 7, "D",1);
	memcpy(sCashBackAmount + 8, sOtherAmnt,12);
	memcpy(sIsoAddMount + j, sCashBackAmount,20);	
	}
	j+=20;
	
	sIsoAddMount[j]='\0';
				
}

int IsoToVisaAddAmount(char* IsoAddMount, char* NwAddAmount)
{
	/*Start AMER20171123 PROD00049633*/ 
	int i= 0;
	int nLength = strlen(IsoAddMount);
	char sLine[MAX_LINE_TRC];
	
	while( i < nLength)
	{
		if(memcmp(IsoAddMount + i, P7_F054_ACCT_TYPE_SA, P7_F054_ACCT_TYPE_LEN) == 0)
		{
			memcpy(IsoAddMount + i, B1_F054_ACCT_TYPE_CREDIT_ACCT, B1_F054_ACCT_TYPE_LEN);
		}
		
		i+= 20;
	}
	/*End AMER20171123 PROD00049633*/ 	

	strncpy(NwAddAmount,IsoAddMount,P7_F054_MAX_LEN);
	NwAddAmount[P7_F054_MAX_LEN]='\0';
	return OK;
}
/*************************************************************************/
/* VisaToIsoAdtnlRespData()                                              */
/* */
/* Converts VISA Additional Response Data (field 44) into an ISO format  */
/* Input:  VISA Additional Response Data                                 */
/* ISO T2 data (field 35) presence flag                                 */
/* ISO Icc data (field 55) presence flag                                */
/* ISO POS mode                                             			 */
/* Auth type : Request (0) or Response (1)                                             			 */
/* Output: ISO field 48                                    				 */
/*************************************************************************/
void 
	VisaToIsoAdtnlRespData(char *VisaAdtnlRespData,
	int  IsoT2DataPresence, 
	int  IsoIccDataPresence,
	char *IsoPosMode,
	int  AuthType,
	TSTlvPrivate *TlvInfo)
{
	char			sSecurityResults[32 + 1];	 				
	char			sExtSecResDtl[P7_F048_P38_VISA_LEN + 1]; 
	char            FieldData1[2];
	int             nLength;	
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1]; 
	int 			nFieldLen;

	trace_message(TRACE,"Start VisaToIsoAdtnlRespData", PROCESSING);

	memset(sSecurityResults  ,' ',sizeof(sSecurityResults )); 	
	memset(sExtSecResDtl  ,' ',sizeof(sExtSecResDtl )); 
	memset(FieldData1, 0, sizeof(FieldData1));		
	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);/*HAL PROD00074189 20202801 Enh Q2.2020*/

	nLength = strlen(VisaAdtnlRespData);
	sExtSecResDtl[P7_F048_P38_NETWORK] = '1'; /* Network */  
	
	/* Subfield 44.1 Response Source/Reason Code*/
	PutTlvPrivate(EXTERNAL_RESP_SOURCE, TlvInfo, VisaAdtnlRespData, 1);

	/* Subfield 44.2 Address Verification Result Code*/
	if (nLength > 1 && VisaAdtnlRespData[1] != ' ') 
	{
		PutTlvPrivate(ADDRESS_VERIF_REP_TYPE_TAG, TlvInfo, VisaAdtnlRespData + 1, 1);

		switch (VisaAdtnlRespData[1])
		{
		case 'A':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'A';
			break;
		case 'B':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'B';
			break;
		case 'C':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'B';
			break;
		case 'D':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'X';
			break;
		case 'F':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'X';
			break;
		case 'G':
			sSecurityResults[SEC_FLAG_CHK_AVS] = '0';
			break;
		case 'I':
			sSecurityResults[SEC_FLAG_CHK_AVS] = '0';
			break;
		case 'M':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'X';
			break;
		case 'N':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'N';
			break;
		case 'P':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'W';
			break;
		case 'R':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'R';
			break;
		case 'U':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'U';
			break;
		case 'Y':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'X';
			break;
		case 'Z':
			sSecurityResults[SEC_FLAG_CHK_AVS] = 'W';
			break;
		}
		sExtSecResDtl[P7_F048_P38_VISA_CHK_AVS] = VisaAdtnlRespData[1];
	}

	/* Subfield 44.5 CVV/iCVV Results Code (requests and responses)*/

	if ((IsoPosMode[6] == '2' || IsoPosMode[6] == 'W') && IsoT2DataPresence == SUCCESS)
	{
		/* The authorization is magstripe && F35 is present  */
		if (nLength > 4)
		{
			switch (VisaAdtnlRespData[4])
			{
			case ' ':
				sSecurityResults[SEC_FLAG_CHK_CVV1] = '4';
				break;
			case '1':
				sSecurityResults[SEC_FLAG_CHK_CVV1] = '1';
				break;
			case '2':
				sSecurityResults[SEC_FLAG_CHK_CVV1] = '0';
				break;
			}
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV1] = VisaAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, VisaAdtnlRespData + 4, 1);
		}
		else
		{
			sSecurityResults[SEC_FLAG_CHK_CVV1] = '4';
		}
	}
	else if (IsoPosMode[6] == 'A' && IsoT2DataPresence == SUCCESS)
	{
		/*The authorization is contactless magstripe && F35 is present  */
		if (nLength > 4)
		{
			switch (VisaAdtnlRespData[4])
			{
			case ' ':
				sSecurityResults[SEC_FLAG_CHK_CVC3] = '4';
				break;
			case '1':
				sSecurityResults[SEC_FLAG_CHK_CVC3] = '1';
				break;
			case '2':
				sSecurityResults[SEC_FLAG_CHK_CVC3] = '0';
				break;
			}
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV] = VisaAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, VisaAdtnlRespData + 4, 1);
		}
		else
		{
			sSecurityResults[SEC_FLAG_CHK_CVC3] = '4';
		}
	}
	else if ((IsoPosMode[6] == '5' || IsoPosMode[6] == 'M' || IsoPosMode[6] == 'J') && IsoIccDataPresence == SUCCESS)
	{
		/*  the authorization is EMV  && F55 is present */
		if (nLength > 4)
		{
			switch (VisaAdtnlRespData[4])
			{
			case ' ':
				sSecurityResults[SEC_FLAG_CHK_ICVV] = '4';
				break;
			case '1':
				sSecurityResults[SEC_FLAG_CHK_ICVV] = '1';
				break;
			case '2':
				sSecurityResults[SEC_FLAG_CHK_ICVV] = '0';
				break;
			}
			sExtSecResDtl[P7_F048_P38_VISA_CHK_ICVV] = VisaAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, VisaAdtnlRespData + 4, 1);
		}
		else
		{
			sSecurityResults[SEC_FLAG_CHK_ICVV] = '4';
		}
	}

	/* Subfield 44.8 Card Authentication Results Code*/
	if (nLength > 8)
	{
		switch (VisaAdtnlRespData[8])
		{
		case ' ':
			sSecurityResults[SEC_FLAG_CHK_ARQC] = '4';
			break;
		case '1':
			sSecurityResults[SEC_FLAG_CHK_ARQC] = '1';
			break;
		case '2':
			sSecurityResults[SEC_FLAG_CHK_ARQC] = '0';
			break;
		}
		sExtSecResDtl[P7_F048_P38_VISA_CHK_ARQC] = VisaAdtnlRespData[8];
	}
	else
	{
		sSecurityResults[SEC_FLAG_CHK_ARQC] = '4';
	}

	/* Subfield 44.10 CVV2 Result Code*/
	if (nLength > 10)
	{
		PutTlvPrivate(CVV2_RESULT_CODE_TAG, TlvInfo, VisaAdtnlRespData + 10, 1);

		switch (VisaAdtnlRespData[10])
		{
		case 'C':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = VisaAdtnlRespData[10];
			break;
		case 'D':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = VisaAdtnlRespData[10];
			break;
		case 'K':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = VisaAdtnlRespData[10];
			break;
		case 'L':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = VisaAdtnlRespData[10];
			break;
		case 'M':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = VisaAdtnlRespData[10];
			break;
		case 'N':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = VisaAdtnlRespData[10];
			break;
		case 'P':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = VisaAdtnlRespData[10];
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = VisaAdtnlRespData[10];
			break;
		case 'S':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = VisaAdtnlRespData[10];
			break;
		case 'U':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = VisaAdtnlRespData[10];
			break;
		}
	}

	/* Subfield 44.13 CAVV Results Code*/
	if (nLength > 14)
	{
		switch (VisaAdtnlRespData[14])
		{
		case ' ':
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'R';
			break;
		case '0':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '0';
			break;
		case '1':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '1';
			break;
		case '2':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '2';
			break;
		case '3':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '3';
			break;
		case '4':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '4';
			break;
		case '7':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '7';
			break;
		case '8':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '8';
			break;
		case '9':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '9';
			break;
		case 'A':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'A';
			break;
		case 'B':
			FieldData1[0] = VisaAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'B';
			break;
		case '6':
			FieldData1[0] = P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART;
			break;
		case 'C':
			FieldData1[0] = P7_F048_P54_CAVV_NOT_VALIDATED_ATTE;
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'N';
			break;
		case 'D':
			FieldData1[0] = P7_F048_P54_CAVV_NOT_VALIDATED_AUTH;
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'O';
			break;
		}

		sExtSecResDtl[P7_F048_P38_VISA_CHK_CAVV] = VisaAdtnlRespData[14];

		if (FieldData1[0] != '\0')
			PutTlvPrivate(AAV_RESULT, TlvInfo, FieldData1, 1);

		if (AuthType == 1 && VisaAdtnlRespData[14] == '0')
		{
		/*Start HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10 */
			if(GetTlvPrivate(ECOM_AUTH_INFO, TlvInfo, sEcomAuthInfo, &nFieldLen)==SUCCESS)
			{
				sEcomAuthInfo[4] = '0';
			}
			else
			{
				memcpy(sEcomAuthInfo,"    0",5);
				nFieldLen = 5;
			}
			PutTlvPrivate(ECOM_AUTH_INFO, TlvInfo, sEcomAuthInfo, nFieldLen);
		}
		/*End HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10 */
		}
	else
	{
		sSecurityResults[SEC_FLAG_CHK_AAV] = 'R';
	}

	PutTlvPrivate(EXT_SECURITY_VER_RESULTS_TAG, TlvInfo, sSecurityResults, 32);
	PutTlvPrivate(EXT_SECURITY_RESULTS_DTL, TlvInfo, sExtSecResDtl, P7_F048_P38_VISA_LEN);
	
	trace_message(TRACE,"End VisaToIsoAdtnlRespData", PROCESSING);
	
}
/******************************************************************************************/
/******************************************************************************************/
void LogVisaMessage(int  nIndexCtx, TSVisaInfo * visaMsgInfo, TSIsoInfo * isoMsgInfo, E_NW_MSG_CAT eMsgCat)
{
	char			sKey[36 + 1];
	char			sRefNbr[12 + 1];
	char			sIntStan[6 + 1];
	char			sExtStan[6 + 1];
	char 			sRoutingCode[6 + 1];
	char 			sCaptureCode[6 + 1];
	int				nRetCode,Length;
	char        	sBuffer[LG_MAX];
	char 			sSecurityData [128 + 1];
	char            tmpField[1024];
	TSTlvPrivate    TlvInfo;

	memset (sBuffer, 0, sizeof(sBuffer));
	memset (sKey, 			'\0', sizeof(sKey));
	memset (sRefNbr, 		'\0', sizeof(sRefNbr));
	memset (sIntStan, 		'\0', sizeof(sIntStan));
	memset (sExtStan, 		'\0', sizeof(sExtStan));
	memset (sRoutingCode,	'\0', sizeof(sRoutingCode));
	memset (sCaptureCode,	'\0', sizeof(sCaptureCode));
	memset (sSecurityData, 0, sizeof(sSecurityData));
	memset (tmpField, 0, sizeof(tmpField));

	GetVisaField(VISA_REFERENCE_NBR, visaMsgInfo, sRefNbr, &Length);
	GetIsoField(ISO_AUDIT_NBR, isoMsgInfo, sIntStan, &Length);
	GetVisaField(VISA_AUDIT_NBR, visaMsgInfo, sExtStan, &Length);

	/* START ELKAM 07-12-2021 PLUTONL-3917 */

	/* switch (eMsgCat)
	{
		case INC_REQ:
		case OUTG_RESP:
			InitTlvPrivate (&TlvInfo);
			if (GetIsoField (ISO_ADTNL_DATA_PRIV, isoMsgInfo, tmpField, &Length) == SUCCESS)
			{
				AnalyseTlvPrivate (tmpField, Length, &TlvInfo);
				GetTlvPrivate(ROUTING_CODE, &TlvInfo, sRoutingCode, &Length);
			}
			memcpy(sCaptureCode, ResourceId, 6);
			break;
		case OUTG_REQ:
		case INC_RESP:
			if (GetIsoField(ISO_SECURITY_DATA, isoMsgInfo, sSecurityData, &Length) == SUCCESS)
			{
				GetSecurityData(SOURCE_RESOURCE, sSecurityData, sCaptureCode, &Length);
				GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sRoutingCode, &Length);
			}

			break;
	} */

	if (GetIsoField(ISO_SECURITY_DATA, isoMsgInfo, sSecurityData, &Length) == SUCCESS)
	{
		GetSecurityData(SOURCE_RESOURCE, sSecurityData, sCaptureCode, &Length);
		GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sRoutingCode, &Length);
	}

	/* STAND IN if Routing code = 000000 */
	if (strcmp(sRoutingCode, "000000") == 0 ) {
		strcpy(sRoutingCode,"000001");
	}
	/* END ELKAM 07-12-2021 PLUTONL-3917 */


	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, visaMsgInfo);
	memcpy(sBuffer, (char *) &(visaMsgInfo->sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;
	BuildKeyRawMsg(sRefNbr, sIntStan, sExtStan, sCaptureCode, sRoutingCode, sKey);
	nRetCode = LogNetworkMessage(nIndexCtx, eMsgCat, ISO8583_VISA_BASE1, sKey, sBuffer, Length);
}
