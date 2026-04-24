#include <define.h>
#include <dump.h>
#include <stdio.h>
#include <sms_define.h>
#include <sms_param.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <resources.h>
#include <iso8583.h>
#include <iso_sms.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <security_flags.h>
#include <iso_hps.h>
/**************************************************************************/
/* SmsToIsoPosMode()                                                   */
/* */
/* Converts Sms POS Entry Mode data (field 22) into an ISO format code */
/* Input:  Sms fields 18, 22, 25, & 60                                 */
/* Output: ISO field 22                                                 */
/**************************************************************************/
void
	SmsToIsoPosMode(char *Merchant,
	char *SmsPosMode,
	char *SmsPosCond,
	char *SmsAddPos,
	char *IsoPosMode,
	char *EcomAuthInfo,
	int	*nEcomInfoLen)
{

	char            sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start SmsToIsoPosMode(%s|%s|%s|%s)", Merchant, SmsPosMode, SmsPosCond, SmsAddPos);
	trace_event(sLine, PROCESSING);

	/* Card Data Entry Capability - Position 1 */
	if (memcmp(SmsAddPos + 1, "0", 1) == 0)
		memcpy(IsoPosMode, "0", 1);
	else if (memcmp(SmsAddPos + 1, "1", 1) == 0)
		memcpy(IsoPosMode, "1", 1);
	else if (memcmp(SmsAddPos + 1, "2", 1) == 0)
		memcpy(IsoPosMode, "2", 1);
	else if (memcmp(SmsAddPos + 1, "3", 1) == 0)
		memcpy(IsoPosMode, "3", 1);
	else if (memcmp(SmsAddPos + 1, "4", 1) == 0)
		memcpy(IsoPosMode, "4", 1);
	else if (memcmp(SmsAddPos + 1, "5", 1) == 0)
		memcpy(IsoPosMode, "5", 1);
	else if (memcmp(SmsAddPos + 1, "9", 1) == 0)
		memcpy(IsoPosMode, "1", 1);
	else
		memcpy(IsoPosMode, "0", 1);

	/* Cardholder Authentication Capability - Position 2 */
	if (memcmp(SmsPosCond, "57", 2) == 0)
		memcpy((IsoPosMode + 1), "1", 1);
	else
		memcpy((IsoPosMode + 1), "0", 1);

	/* Card Retention capability - Position 3 */
	if (memcmp(Merchant, "6011", 4) == 0)
		memcpy((IsoPosMode + 2), "1", 1);
	else
		memcpy((IsoPosMode + 2), "0", 1);

	/* Operating Environment - Position 4 */
	memcpy((IsoPosMode + 3), "0", 1);

	/* Cardholder Presence Indicator - Position 5 */
	if (memcmp(SmsPosCond, "01", 2) == 0)
		memcpy((IsoPosMode + 4), "1", 1);
	else if (memcmp(SmsPosCond, "05", 2) == 0)
		memcpy((IsoPosMode + 4), "0", 1);
	else if (memcmp(SmsPosCond, "08", 2) == 0)
		memcpy((IsoPosMode + 4), "2", 1);
	else if (memcmp(SmsPosCond, "51", 2) == 0)
		memcpy((IsoPosMode + 4), "4", 1);
	else if (memcmp(SmsPosCond, "55", 2) == 0)
		memcpy((IsoPosMode + 4), "3", 1);
	else if (memcmp(SmsPosCond, "59", 2) == 0)
		memcpy((IsoPosMode + 4), "5", 1);
	else
		memcpy((IsoPosMode + 4), "0", 1);

	/* Card Presence Indicator - Position 6 */
	if (memcmp(SmsPosCond, "00", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(SmsPosCond, "02", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(SmsPosCond, "03", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(SmsPosCond, "05", 2) == 0)
		memcpy((IsoPosMode + 5), "0", 1);
	else if (memcmp(SmsPosCond, "10", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(SmsPosCond, "57", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else if (memcmp(SmsPosCond, "71", 2) == 0)
		memcpy((IsoPosMode + 5), "1", 1);
	else
		memcpy((IsoPosMode + 5), "0", 1);

	/* Card Data Input Mode - Position 7 */
	if (memcmp(SmsPosMode, "00", 2) == 0)
		memcpy((IsoPosMode + 6), "0", 1);
	else if (memcmp(SmsPosMode, "01", 2) == 0)
		memcpy((IsoPosMode + 6), "1", 1);
	else if (memcmp(SmsPosMode, "02", 2) == 0)
		memcpy((IsoPosMode + 6), "2", 1);
	else if (memcmp(SmsPosMode, "03", 2) == 0)
		memcpy((IsoPosMode + 6), "3", 1);
	else if (memcmp(SmsPosMode, "04", 2) == 0)
		memcpy((IsoPosMode + 6), "4", 1);
	else if (memcmp(SmsPosMode, "05", 2) == 0)
		memcpy((IsoPosMode + 6), "5", 1);
	else if (memcmp(SmsPosMode, "06", 2) == 0)
		memcpy((IsoPosMode + 6), "7", 1);
	else if (memcmp(SmsPosMode, "90", 2) == 0)
		memcpy((IsoPosMode + 6), "W", 1);
	else if (memcmp(SmsPosMode, "95", 2) == 0)
		memcpy((IsoPosMode + 6), "J", 1);
        else if (memcmp(SmsPosMode, "10", 2) == 0) /*: AMA :*/
                memcpy((IsoPosMode + 6), "F", 1);

	else
		memcpy((IsoPosMode + 6), "0", 1);
	/* Gestion E-Commerce Trx */
	if ((strlen(SmsAddPos) > 9) 
		 && (memcmp(SmsPosCond, "59", 2) == 0))/*AMA*/
	{

		IsoPosMode[4] = '5';
		if(IsoPosMode[6] != 'F') /*IBO20221025 PLUTONL-5272*/
		{
			if (!memcmp(SmsAddPos + 8, "05", 2))
				IsoPosMode[6] = 'T';
			else if (!memcmp(SmsAddPos + 8, "06", 2))
				IsoPosMode[6] = 'S';
			else if (!memcmp(SmsAddPos + 8, "07", 2))
				IsoPosMode[6] = 'V';
			else if (!memcmp(SmsAddPos + 8, "08", 2))
				IsoPosMode[6] = 'U';
			else
				IsoPosMode[6] = '9';
		}

	}
	/* Cardholder Authentication Method - Position 8 */
	if (memcmp(SmsPosMode + 2, "1", 1) == 0)
		memcpy((IsoPosMode + 7), "1", 1);
	else
		memcpy((IsoPosMode + 7), "0", 1);

	/* Cardholder Authentication Entity - Position 9 */
	memcpy((IsoPosMode + 8), "0", 1);

	/* Rewrite capability - Position 10 */
	memcpy((IsoPosMode + 9), "0", 1);

	/* Terminal Output capability - Position 11 */
	memcpy((IsoPosMode + 10), "0", 1);

	/* PIN Entry capability - Position 12 */
	if (memcmp(SmsPosMode + 2, "2", 1) == 0)
		memcpy((IsoPosMode + 11), "0", 1);
	else if (memcmp(SmsPosMode + 2, "8", 1) == 0)
		memcpy((IsoPosMode + 11), "S", 1);
	else
		memcpy((IsoPosMode + 11), "1", 1);

	if ((strlen(SmsAddPos) > 9)
			 && (memcmp(SmsPosCond, "59", 2) == 0)) /*IBO20221025 PLUTONL-5272*/
	{
		if (!memcmp(SmsAddPos + 8, "05", 2))
			IsoPosMode[6] = P7_F048_P66_POS1_ECOM_AUTHENTICATED;
		else if (!memcmp(SmsAddPos + 8, "06", 2))
			IsoPosMode[6] = P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT;
		else if (!memcmp(SmsAddPos + 8, "07", 2))
			IsoPosMode[6] = P7_F048_P66_POS1_ECOM_SEC_CHAN;
		else if (!memcmp(SmsAddPos + 8, "08", 2))
			IsoPosMode[6] = P7_F048_P66_POS1_ECOM_UNSECURE;
		else
			IsoPosMode[6] = P7_F048_P66_POS1_ECOM;

		*nEcomInfoLen = 1;
	}


	sprintf(sLine, "End   SmsToIsoPosMode(%s)", IsoPosMode);
	trace_event(sLine, PROCESSING);
	return;
}
/*************************************************************************/
/*   SmsToIsoReasonCode ()                                               */
/*                                                                       */
/*   Function to convert SMS Puf Reason Code to ISO Reason Code          */
/*                                                                       */
/*   Input:  SmsCode: SMS Reason Code                                    */
/*   Output: IsoCode: ISO Reason Code                                    */
/*************************************************************************/
void  SmsToIsoReasonCode  ( char *SmsCode,
                            char *IsoCode)
{
   char v_sms_code    [ 5 ];
   char v_iso_code    [ 5 ];
   int i;
   typedef struct {
    char FirstCode    [ 5 ];
    char SecondCode    [ 5 ];
  } RSN_CODE1;
 
  RSN_CODE1 bRSC[] =
  {
    {"0013", "4657"},/*ICH for VCR*/    
    {"0030", "4555"},
    {"0062", "4652"},
    {"0074", "4536"},
    {"0076", "4651"},
    {"0082", "0032"}, /*ADD KS conf GB 050316 Pb conversion reason_code */
    {"0090", "4650"},
    {"0100", "7300"},
    {"0110", "7301"},
    {"0130", "7302"},
    {"0140", "7303"},
    {"0150", "7304"},
    {"0160", "7305"},
    {"0170", "7306"},
    {"0200", "7307"},
    {"0210", "7308"},
    {"0220", "7309"},
    {"0230", "7310"},
    {"0240", "7311"},
    {"5290", "7312"},
    {"2602", "4653"},
    {"2604", "4654"},
    {"2606", "4655"},
    {"2608", "4656"},
    {"2620", "4657"},
    {"2622", "4658"},
    {"2624", "4659"},
{"2674", "4536"},/*SWI200904 Sms 2009 Late Presentement*/
{"2690", "2020"},
    {"5300", "4661"}
  };

  trace_event("Start SmsToIsoReasonCode()", PROCESSING);

  memset (v_sms_code,   0,  sizeof (v_sms_code));
  memset (v_iso_code,   0,  sizeof (v_iso_code));

  memcpy (v_sms_code, SmsCode, 4);

/* Start IBO 01042009 : Initialaise the ISO reason code with the Sms SMS Reason code */
  memcpy (v_iso_code, v_sms_code, 4);
/* End   IBO 01042009 : Initialaise the ISO reason code with the Sms SMS Reason code */

  for (i = 0; i < sizeof(bRSC)/sizeof(bRSC[0]); i++)
   {
    if (!strcmp(bRSC[i].FirstCode, v_sms_code))
      memcpy(v_iso_code, bRSC[i].SecondCode, 4);
   }
 
   memcpy (IsoCode, v_iso_code, 4);

  trace_event("End SmsToIsoReasonCode()", PROCESSING);
  return;
} 

/*************************************************************************/
/*   IsoToSmsReasonCode ()                                               */
/*                                                                       */
/*   Function to convert ISO Reason Code to SMS Reason Code              */
/*                                                                       */
/*   Input:  IsoCode: ISO Reason Code                                    */
/*   Output: SmsCode: SMS Reason Code                                    */
/*************************************************************************/
void  IsoToSmsReasonCode ( char  *IsoCode,
                           char  *SmsCode)
{
   char v_iso_code    [ 5 ];
   char v_sms_code    [ 5 ];
   int i;
   typedef struct {
    char FirstCode    [ 5 ];
    char SecondCode    [ 5 ];
  } RSN_CODE;
  
  RSN_CODE aRSC[] =
  {
    {"1003", "2502"},
    {"1376", "2502"},
    {"4000", "2501"},
    {"4001", "2502"},
    {"4002", "2502"},
    {"4004", "2502"},
    {"4005", "2502"},
    {"4006", "2502"},
    {"4007", "2502"},
    {"4013", "2502"},
    {"4014", "2502"},
    {"4015", "2502"},
    {"4016", "2502"},
    {"4017", "2502"},
    {"4018", "2502"},
    {"4019", "2502"},
    {"4020", "2502"},
    {"4200", "2502"},
    {"4201", "2503"},
    {"4202", "2502"},
    {"4203", "2502"},
    {"4300", "2502"},
    {"4301", "2502"},
    {"4302", "2502"},
    {"4303", "2502"},
    {"4304", "2502"},
    {"4307", "2502"},
    {"4308", "2502"},
    {"4309", "2502"},
    {"4310", "2502"},
    {"4311", "2502"},
    {"4312", "2502"},
    {"4313", "2502"},
    {"4400", "2502"},
    {"4401", "2502"},
    {"4750", "2502"}
  };

  trace_event("Start IsoToSmsReasonCode()", PROCESSING);

  memset (v_iso_code,   0,  sizeof (v_iso_code));
  memset (v_sms_code,   0,  sizeof (v_sms_code));

  memcpy (v_iso_code,  IsoCode, 4); 
  memcpy (v_sms_code,"2502", 4); /* Default alue */
 
  for (i = 0; i < sizeof(aRSC)/sizeof(aRSC[0]); i++)
   {
    if (!strcmp(aRSC[i].FirstCode, v_iso_code))
      memcpy(v_sms_code, aRSC[i].SecondCode, 4);
   }
   
   memcpy (SmsCode, v_sms_code, 4);

   trace_event("End  IsoToSmsReasonCode(OK)", PROCESSING);
   return;
}

/*************************************************************************/
/*   IsoToSmsProcessingCode()                                            */
/*                                                                       */
/*   Function to convert ISO processing code to SMS processing code      */
/*                                                                       */
/*   Input:  IsoCode: ISO processing code                                */
/*   Output: SmsCode: SMS processing code                                */
/*************************************************************************/
void  IsoToSmsProcessingCode ( char  *IsoCode,
                               char  *SmsCode,
                               char  *sMcc)
{
   char    v_process_code    [7];
   char    v_posit1          [3];
   char    v_posit2          [3];
   char    v_posit3          [3];
   char    sLine             [MAX_LINE_TRC];

   int     v_trans_type;
   int     v_account_from;
   int     v_account_to;
   int     nMcc = 0;

   sprintf(sLine, "Start IsoToSmsProcessingCode(%6s)", IsoCode);
   trace_event(sLine, PROCESSING);

   memset(v_process_code, 0, sizeof(v_process_code));
   memset(v_posit1,       0, sizeof(v_posit1));
   memset(v_posit2,       0, sizeof(v_posit2));
   memset(v_posit3,       0, sizeof(v_posit3));

   v_trans_type   = 0;
   v_account_from = 0;
   v_account_to   = 0;

   memcpy(v_posit1, IsoCode,     2);
   memcpy(v_posit2, IsoCode + 2, 2);
   memcpy(v_posit3, IsoCode + 4, 2);

   v_trans_type   = atoi(v_posit1);
   v_account_from = atoi(v_posit2);
   v_account_to   = atoi(v_posit3);

   switch(v_trans_type)
   {
       case   1  : memcpy(v_process_code, "01", 2); break;
       case   2  : memcpy(v_process_code, "02", 2); break;
       case   3  : memcpy(v_process_code, "03", 2); break;
       case  11  : memcpy(v_process_code, "11", 2); break;
       case  17  : memcpy(v_process_code, "01", 2); break;
       case  18  : memcpy(v_process_code, "40", 2); break;
       case  19  : memcpy(v_process_code, "19", 2); break;
	   case  20  : memcpy(v_process_code, SMS_F003_P1_REFUND, SMS_F003_P1_LEN); break;	/*AMER20180314 PROD00054205 Enh18.Q2 Art 2.4*/	
       case  22  : memcpy(v_process_code, "22", 2); break;
       case  29  : memcpy(v_process_code, "29", 2); break;
       case  31  : memcpy(v_process_code, "30", 2); break;
       case  40  : memcpy(v_process_code, "40", 2); break;
	   /*Start AMER20180314 PROD00054205 Enh18.Q2 Art 8.1.1*/
	   case  91  : memcpy(v_process_code, SMS_F003_P1_PIN_CHANGE, SMS_F003_P1_LEN); break;
	   case  98  : memcpy(v_process_code, SMS_F003_P1_PIN_INBLOCK_PP_ACT, SMS_F003_P1_LEN); break;
	   /*End AMER20180314 PROD00054205 Enh18.Q2 Art 8.1.1*/
       default   : memcpy(v_process_code, "00", 2);
                   trace_event("Invalid processing code", ERROR);
   }

   switch(v_account_from)
   {

       case    0  : memcpy(v_process_code + 2, "00", 2); break;
       case   10  : memcpy(v_process_code + 2, "10", 2); break;
       case   20  : memcpy(v_process_code + 2, "20", 2); break;
       case   30  : memcpy(v_process_code + 2, "30", 2); break;
       default    : memcpy(v_process_code + 2, "00", 2);
                    trace_event("Invalid processing code", ERROR);
   }

   switch(v_account_to)
   {

       case    0  : memcpy(v_process_code + 4, "00", 2); break;
       case   10  : memcpy(v_process_code + 4, "10", 2); break;
       case   20  : memcpy(v_process_code + 4, "20", 2); break;
       case   30  : memcpy(v_process_code + 4, "30", 2); break; 
       default    : memcpy(v_process_code + 4, "00", 2);
                    trace_event("Invalid processing code", ERROR);
   }
   nMcc = atoi(sMcc);
   switch(nMcc)
   {

       case    6051  : memcpy(v_process_code , "11", 2); break;
       case    7995  : memcpy(v_process_code , "11", 2); break;
       case    4829  : memcpy(v_process_code , "11", 2); break;
   }

   memcpy(SmsCode, v_process_code, 6);
   SmsCode[6] = '\0';

   trace_event("End  IsoToSmsProcessingCode(OK)", PROCESSING);
   return;
 
}

/*************************************************************************/
/*   SmsToIsoProcessingCode()                                            */
/*                                                                       */
/*   Converts a SMS processing code into an ISO standard code            */
/*   Input : SmsCode : SMS processing code                               */
/*           Merchant: merchant type                                     */
/*   Output: IsoCode : corresponding ISO processing code                 */
/*************************************************************************/
void  SmsToIsoProcessingCode ( char  *SmsCode,
                               char  *Merchant,
                               char  *IsoCode)
{
   char    v_process_code  [7];
   char    v_posit_1_2     [3];
   char    v_posit_3_4     [3];
   char    v_posit_5_6     [3];

   int     v_trans_type;
   int     v_account_from;
   int     v_account_to;

   char    ligne[MAX_LINE_TRC];

   sprintf(ligne, "Start SmsToIsoProcessingCode(%.6s)", SmsCode);
   trace_event(ligne, PROCESSING);

   memset(v_process_code, 0, sizeof(v_process_code));
   memset(v_posit_1_2,    0, sizeof(v_posit_1_2));
   memset(v_posit_3_4,    0, sizeof(v_posit_3_4));
   memset(v_posit_5_6,    0, sizeof(v_posit_5_6));

   memcpy(v_posit_1_2, SmsCode,     2);
   memcpy(v_posit_3_4, SmsCode + 2, 2);
   memcpy(v_posit_5_6, SmsCode + 4, 2);

   v_trans_type   = atoi(v_posit_1_2);
   v_account_from = atoi(v_posit_3_4);
   v_account_to   = atoi(v_posit_5_6);

   switch(v_trans_type)
   {
    case   0 : memcpy(v_process_code, "00", 2); break;
    case   1 : if (memcmp(Merchant, "6011", 4) == 0)
                   memcpy(v_process_code, "01", 2);
               else
                   memcpy(v_process_code, "17", 2);
               break;
    case   2 : memcpy(v_process_code, "02", 2); break; /*NAB20182004   PROD00055720*/
    case  19 : memcpy(v_process_code, "19", 2); break;
	case 20:  memcpy(v_process_code, P7_F003_REFUND, P7_F003_P1_LEN); break;
    case  22 : memcpy(v_process_code, "22", 2); break;
	case	26	:
	case	28	: memcpy(v_process_code, P7_F003_PAYMENT, P7_F003_P1_LEN); break;
    case  29 : memcpy(v_process_code, "29", 2); break;
    case  30 : memcpy(v_process_code, "31", 2); break;
    case  40 : memcpy(v_process_code, "40", 2); break;
    default  : memcpy(v_process_code, "00", 2);
               trace_event("Invalid processing code", ERROR);
   }

   switch(v_account_from)
   {

    case    0 : memcpy(v_process_code + 2, "00", 2); break;
    case   10 : memcpy(v_process_code + 2, "10", 2); break;
    case   20 : memcpy(v_process_code + 2, "20", 2); break;
    case   30 : memcpy(v_process_code + 2, "30", 2); break;
    default   : memcpy(v_process_code + 2, "00", 2);
                trace_event("Invalid processing code", ERROR);
   }

   switch(v_account_to)
   {

    case    0 : memcpy(v_process_code + 4, "00", 2); break;
    case   10 : memcpy(v_process_code + 4, "10", 2); break;
    case   20 : memcpy(v_process_code + 4, "20", 2); break;
    case   30 : memcpy(v_process_code + 4, "30", 2); break;
    default   : memcpy(v_process_code + 4, "00", 2);
                trace_event("Invalid processing code", ERROR);
   }

   memcpy (IsoCode, v_process_code, 6);
   IsoCode [6] = '\0';

   trace_event ("End   SmsToIsoProcessingCode(OK)", PROCESSING);
   return;

}

/*************************************************************************/
/*   IsoToSmsPosMode()                                                   */
/*                                                                       */
/*   Converts an ISO POS Entry Mode Code (field 22) to a SMS mode        */
/*   Input:  ISO mode                                                    */
/*   Output: SMS mode                                                    */
/*************************************************************************/
void  IsoToSmsPosMode ( char  *IsoMode, 
                        char  *SmsMode)
{

   char    tmp_pos_entry   [5];


   trace_event("Start IsoToSmsPosMode()", PROCESSING);

   memset(tmp_pos_entry, 0, sizeof(tmp_pos_entry));

   switch (*(IsoMode + 6))
   {
       case   '0'  : memcpy(tmp_pos_entry, "00", 2); break;
/*       case   '2'  : memcpy(tmp_pos_entry, "02", 2); break;*/
       case   '2'  : memcpy(tmp_pos_entry, "90", 2); break;
       case   '5'  : memcpy(tmp_pos_entry, "05", 2); break;
       case   'J'  : memcpy(tmp_pos_entry, "95", 2); break;
       case   '8'  :
       case   'W'  : memcpy(tmp_pos_entry, "90", 2); break;
       case   'F'  : memcpy(tmp_pos_entry, "10", 2); break; /*: AMA :*/
       default     : memcpy(tmp_pos_entry, "00", 2);
                     trace_event("Invalid POS mode", ERROR);
   }

   switch (*(IsoMode + 11))
   {
       case   '1'  : 
       case   '4'  : 
       case   '5'  : 
       case   '6'  : 
       case   '7'  : 
       case   '8'  : 
       case   '9'  : 
       case   'A'  : 
       case   'B'  : /*memcpy(tmp_pos_entry + 2, "0", 1); break; */
       case   'C'  : 
       case   'T'  : memcpy(tmp_pos_entry + 2, "1", 1); break;
       default     : memcpy(tmp_pos_entry + 2, "0", 1);
                     trace_event("Invalid POS mode", ERROR);
   }
   
   memcpy(tmp_pos_entry + 3, "0", 1);

   memcpy(SmsMode, tmp_pos_entry, 4);
   SmsMode [4] = '\0';

   trace_event("End IsoToSmsPosMode(OK)", PROCESSING);
   return;
}

/**************************************************************************/
/*   IsoToSmsSecData()                                                    */
/*                                                                        */
/*   Converts ISO standard Security data (field 53) into SMS-compliant    */
/*   format.                                                              */
/*   Input:  IsoSecData: ISO field 53                                     */
/*   Output: SmsSecData: SMS security data                                */
/**************************************************************************/  
void  IsoToSmsSecData ( char  *IsoSecData,
                        char  *SmsSecData)
{
   int     v_offset = 0;
   char    ligne[80];

   sprintf(ligne, "Start IsoToSmsSecData(%.18s)", IsoSecData);
   trace_event(ligne, PROCESSING);

   /* Security Format Code */
   memcpy(SmsSecData + v_offset, "20", 2);
   v_offset += 2;

   /* Pin Encryption Algorithm Id */
   memcpy(SmsSecData + v_offset, "01", 2);
   v_offset += 2;

   /* Pin Block Format Code */
   if (memcmp(IsoSecData + 2, "14", 2) == 0)
       memcpy(SmsSecData + v_offset, "04", 2);
   else
   if (memcmp(IsoSecData + 2, "13", 2) == 0)
       memcpy(SmsSecData + v_offset, "03", 2);
   else
   if (memcmp(IsoSecData + 2, "12", 2) == 0)
       memcpy(SmsSecData + v_offset, "02", 2);
   else
       memcpy(SmsSecData + v_offset, "01", 2);

   v_offset += 2;

   /* Pin Zone Key Index */
   memcpy(SmsSecData + v_offset, "01" , 2);
   v_offset += 2;

   /* Reserved Positions */
   memcpy(SmsSecData + v_offset, "00000000",  6);


   sprintf(ligne, "End   IsoToSmsSecData(%.16s)", SmsSecData);
   trace_event(ligne, PROCESSING);
   return;
}



/*************************************************************************/
/*   BuildAddPosInfo()                                                   */
/*                                                                       */
/*   Builds a BASEI additional POS info field (60)                       */
/*   Input:  ISO POS mode                                                */
/*           Merchant type                                               */
/*           ISO processing code                                         */
/*   Output: BASEI field 60                                              */
/*************************************************************************/
void  BuildAddPosInfo (char  *IsoPosMode,
                       char  *MerchantType,
                       char  *IsoProcCode,
                       char  *SmsAddPosInfo)
{

   trace_event("Start BuildAddPosInfo()", PROCESSING);


   /* Terminal Type */
   if ( (*(IsoPosMode + 3) == '2'  || *(IsoPosMode + 3) == '4')  
         && (memcmp(MerchantType, "6011", 4) == 0) )  
      memcpy (SmsAddPosInfo, "2", 1);
   else 
      if (*(IsoPosMode + 3) == '5')
         memcpy (SmsAddPosInfo, "5", 1);
   else
      memcpy (SmsAddPosInfo, "0", 1);


   /* Terminal Entry Capability */
   if (*(IsoPosMode) == '1')    memcpy(SmsAddPosInfo + 1, "1", 1);
   else
      if (*(IsoPosMode) == '2')     memcpy(SmsAddPosInfo + 1, "2", 1);
   else
      if (*(IsoPosMode) == '3')     memcpy(SmsAddPosInfo + 1, "3", 1);
   else
      if (*(IsoPosMode) == '4')     memcpy(SmsAddPosInfo + 1, "4", 1);
   else
      if (*(IsoPosMode) == '9')     memcpy(SmsAddPosInfo + 1, "5", 1);
   else
      memcpy(SmsAddPosInfo + 1, "0", 1);


   /* Chip condition code */
   memcpy(SmsAddPosInfo + 2, "0", 1); 

   /* Reserved */
   memcpy(SmsAddPosInfo + 3, "0", 1); 

   /* Merchant Group Indicator */
   if (memcmp(IsoProcCode, "11", 2) == 0)     /* Quasi-cash */
       memcpy(SmsAddPosInfo + 4, "10", 2);
   else  /* ATM cash */
      if ((memcmp(IsoProcCode, "01", 2) == 0) && (memcmp(MerchantType, "6011", 4)  == 0))   
          memcpy(SmsAddPosInfo + 4, "09", 2);
   else  /* Other cash transaction */
      if (memcmp(IsoProcCode, "01", 2) == 0)   
          memcpy(SmsAddPosInfo + 4, "11", 2);
   else  /* Mail-Phone order */
      if ((*(IsoPosMode + 4) == '2') || (*(IsoPosMode + 4) == '3'))    
          memcpy(SmsAddPosInfo + 4, "06", 2);
   else  /* any other purchase */
          memcpy(SmsAddPosInfo + 4, "08", 2);
   
   memcpy (SmsAddPosInfo, "\0", 1);

   trace_event("End  BuildAddPosInfo()", PROCESSING);
   return;
}


void load_audit_trace (char  *target)
{

   char    trace_audit_temp[10];
   char    hhmmss_temp[7];
   char    hhss_temp[5];
   double  trace_audit_long;
   int     i, j;

   trace_event("Start load_audit_trace()",PROCESSING);
   memset(trace_audit_temp, 0 , 10);
   memset(hhmmss_temp,      0 , 7);
   memset(hhss_temp,        0 , 5);

   trace_audit_long = lrand48();
   sprintf(trace_audit_temp,"%06.0f",trace_audit_long);
   current_hour(hhmmss_temp);

   i = 0;
   j = 0;
   while ( (j < 6) && (i < 4) )
   {
       hhss_temp[i] = hhmmss_temp[j];
       i++; j++;
       if ( j == 2)  j = 4;
   }
   memcpy(target  , hhss_temp       , 4);
   memcpy(target+4, trace_audit_temp, 2);

   trace_event("End   load_audit_trace()",PROCESSING);
   return;
}

void SmsToIsoMoneyTransData(TSSmsInfo * SmsInfo, TlvInfo*   pkIsoMoneyTransfTlv, char* sBAI)
{
	char			sData[1024];
	char			sDOB[12];
	int				nLength;
	TlvInfo			kSmsTransSpecTlv;
	TlvInfo			kSmsAppIdentTlv;
	TlvInfo			kSmsSenderDataTlv;
	/*TlvInfo			kSmsAddSenderDataTlv;*/


	if (GetSmsField(SMS_TRANS_DESC, SmsInfo, sData, &nLength) != SUCCESS) 
	{
		return;
	}

	memset(sDOB,'\0',sizeof(sDOB));
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsAppIdentDataTlv(&kSmsAppIdentTlv);
	InitSmsSenderDataTlv(&kSmsSenderDataTlv);
	/*InitSmsAddSenderDataTlv(&kSmsAddSenderDataTlv);*/
	
	
	
	
	
	AnalyseTlvInfo(sData, nLength, &kSmsTransSpecTlv);

	if( GetTlvInfoField (SMS_F104_BUSINESS_APP_ID, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsAppIdentTlv);
	}
	if( GetTlvInfoField (SMS_F104_SENDER_DATA, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsSenderDataTlv);
	}
	/*
	if( GetTlvInfoField (SMS_F104_ADDITIONAL_SENDER_DATA, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsAddSenderDataTlv);
	}*/

	if( GetTlvInfoField (SMS_F104T57_BUSINESS_APP_ID, &kSmsAppIdentTlv, sData, &nLength) == SUCCESS )
	{
		memcpy(sBAI,sData,SMS_F104T57_LEN);
		sData[SMS_F104T57_LEN]='\0';

		if( memcmp(sData,SMS_F104T57_AA,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_ACC_2_ACC, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_PP,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_P_2_P, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_TU,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_PP_RELOAD, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_OG,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_GAMBLING_REPAY, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_CP,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_BILL_PAY, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_PD,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_PAYROLL, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_GD,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_GOV_2_C, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_MD,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_MER_REB, PAY_TYPE_LEN);
		}
		else if( memcmp(sData,SMS_F104T57_PG,SMS_F104T57_LEN) == 0 )
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_C_2_GOV, PAY_TYPE_LEN);
		}
		else
		{
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, pkIsoMoneyTransfTlv, PAY_TYPE_OTHER_PAY, PAY_TYPE_LEN);
		}


	}
	
	/*Start AEA20210712 Enh21.Q4 Art2.9*/
	if( GetTlvInfoField (SMS_F104T57_PAY_PURPOSE, &kSmsAppIdentTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_PAYMENT_PURPOSE, pkIsoMoneyTransfTlv, sData, nLength);
	}
	/*End AEA20210712 Enh21.Q4 Art2.9*/

	/*P7_F048S20_FUNDS_SOURCE*/

	if( GetTlvInfoField (SMS_F104T5F_SENDER_REF_NBR, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_REF_NBR, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_ACCOUNT_NO, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_ACCOUNT_NO, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_NAME, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_STREET_ADDR, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_STREET_ADDR, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_CITY_NAME, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_CITY_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_STATE, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_STATE, pkIsoMoneyTransfTlv, sData, nLength);
	}
	if( GetTlvInfoField (SMS_F104T5F_SENDER_COUNTRY, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_SENDER_COUNTRY, pkIsoMoneyTransfTlv, sData, nLength);
	}

	if( GetTlvInfoField (SMS_F104T5F_RECIPIENT_NAME, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_RECIPIENT_NAME, pkIsoMoneyTransfTlv, sData, nLength);
	}
	
	/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
	if( GetTlvInfoField (SMS_F104T5F_FUNDS_SOURCE, &kSmsSenderDataTlv, sData, &nLength) == SUCCESS )
	{
		AddTlvInfoField(P7_F048S20_FUNDS_SOURCE, pkIsoMoneyTransfTlv, sData, nLength);
	}
	/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/


	if (GetSmsField(SMS_ADTNL_DATA_PRIV, SmsInfo, sData, &nLength) == SUCCESS)
	{
		memcpy(sDOB,sData+11+2,2);
		memcpy(sDOB+2,sData+11,2);
		memcpy(sDOB+4,sData+11+4,2);
		AddTlvInfoField(P7_F048S20_SENDER_DOB, pkIsoMoneyTransfTlv, sDOB, 6);
	}


}

void SmsBuildAuthId(TSSmsInfo * SmsInfo, char* sAuthId)
{
	char	sFieldData[24];
	int		nLength;


	memcpy(sAuthId , ResourceId, 2);
	GetSmsField(SMS_AUDIT_NBR, SmsInfo, sFieldData, &nLength);
	memcpy(sAuthId + 2, sFieldData, P7_F011_LEN);
	GetSmsField(SMS_REFERENCE_NBR, SmsInfo, sFieldData, &nLength);
	memcpy(sAuthId + 2 + P7_F011_LEN, sFieldData, P7_F037_LEN);
	sAuthId[2+P7_F011_LEN+P7_F037_LEN]='\0';

}

/*PLUTONL-3660*/
/**************************************************************************/
void
	SmsToIsoAcceptorName(int  nIndexCtx,char *SmsAcceptName,char *IsoAcceptName)
{

	char            pSmsCodeAlpha[2 +1];
	char   			pIsoCodeAlpha[3 + 1]; 
	char            sLine[MAX_LINE_TRC];
	int             nRetCode;

	sprintf(sLine, "Start SmsToIsoAcceptorName[%s]", SmsAcceptName);
	trace_event(sLine, PROCESSING);
	
	memset (pSmsCodeAlpha,   0, sizeof(pSmsCodeAlpha));
	memset (pIsoCodeAlpha,   0, sizeof(pIsoCodeAlpha));

	memset(IsoAcceptName,' ',40);
	/*merchant acronym*/
	memcpy(IsoAcceptName,SmsAcceptName,24);
	/*city name*/
	memcpy(IsoAcceptName+24,SmsAcceptName+25,13);
	/*Country code alpha*/
	memcpy(pSmsCodeAlpha, SmsAcceptName+38, 2);
	
	nRetCode = GetCountryCodeAlphaVISA(nIndexCtx, pSmsCodeAlpha, pIsoCodeAlpha);

	if (nRetCode == OK)
	{
		memcpy(IsoAcceptName+37,pIsoCodeAlpha ,3);	
	}
	else
	{
		trace_event(">>>> Error Getting Alpha Country Code",ERROR);
	}
	IsoAcceptName[40]='\0';
	sprintf(sLine, "End   SmsToIsoAcceptorName[%s]", IsoAcceptName);
	trace_event(sLine, PROCESSING);
	return;
}

void SmsToIsoAddAmount(TSSmsInfo * SmsInfo, char* sIsoAddMount)
{
	int		i,j;
	int		nLength;
	int		nAddAmount;
	char	sLocAddAmount[20 + 1];
	char    NwAddAmount [256];
	char    currency [3 + 1];
	int     tmpFieldLen;
	char    sOtherAmnt [256];
	char    sProcessCode[6 + 1];
	char    sCashBackAmount [20 +1];

	memset(NwAddAmount,0,sizeof(NwAddAmount)); 
	memset(currency,0,sizeof(currency));
	memset(sOtherAmnt,0,sizeof(sOtherAmnt));
	memset(sProcessCode,0,sizeof(sProcessCode));
	memset(sCashBackAmount,0,sizeof(sCashBackAmount));

	j = 0;
	if (GetSmsField (SMS_ADTNL_AMOUNTS, SmsInfo,NwAddAmount, &tmpFieldLen) == SUCCESS) 
	{
	nLength = strlen(NwAddAmount);
	for(i=0,j=0; i < nLength; i+=20)
	{
		nAddAmount = NOK;
		memcpy(sLocAddAmount,NwAddAmount+i,20);

		if( memcmp(sLocAddAmount + 2,SMS_F054_AT_LG_BAL,SMS_F054_AT_LEN) == 0 ||
			memcmp(sLocAddAmount + 2,SMS_F054_AT_AVAILABLE,SMS_F054_AT_LEN) == 0 ||
			memcmp(sLocAddAmount + 2,SMS_F054_AT_ORIG_AMNT,SMS_F054_AT_LEN) == 0  )
		{
			nAddAmount = OK;
		}
		if( memcmp(sLocAddAmount + 2,"00",SMS_F054_AT_LEN) == 0 )
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
	if (GetSmsField (SMS_OTHER_AMOUNTS, SmsInfo,sOtherAmnt, &tmpFieldLen) == SUCCES) {
	 
	GetSmsField (SMS_TRANS_CRNCY, SmsInfo,currency, &tmpFieldLen);
	GetSmsField (SMS_PROC_CODE, SmsInfo,sProcessCode, &tmpFieldLen);	
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

int IsoToSmsAddAmount(char* IsoAddMount, char* NwAddAmount)
{

	strncpy(NwAddAmount,IsoAddMount,P7_F054_MAX_LEN);
	NwAddAmount[P7_F054_MAX_LEN]='\0';
	return OK;
}
/*Start SNO120416 */
int GetTableInd( TSIsoInfo *IsoInfo, char *sAutTabInd)
{

	char            slProcCode         [ 6 + 1 ] ;
	char            sMerchActv         [ 4 + 1 ] ;
	int             nLength                      ;
	char            sLine       [ MAX_LINE_TRC ] ;


	trace_event(" START     GetTableInd()", PROCESSING);

	memset( sAutTabInd, 0, 4);
	memset( slProcCode, 0, sizeof(slProcCode));
	memset( sMerchActv, 0, sizeof(sMerchActv));

	GetIsoField( ISO_PROC_CODE       , IsoInfo , slProcCode      , &nLength );
	sprintf(sLine, "   slProcCode  = [%s]", slProcCode);
	trace_event(sLine, PROCESSING);

	/*GetIsoField( ISO_MERCHANT_TYPE   , IsoInfo , sMerchActv      , &nLength ) ;
	sprintf(sLine, "   sMerchActv  = [%s]", sMerchActv);
	trace_event(sLine, PROCESSING);*/

	memcpy(sAutTabInd, "ASM", 3); /** For Pos Trx **/
	if( memcmp(slProcCode, "31", 2) == 0 || memcmp(slProcCode, "91", 2) == 0 )
			memcpy(sAutTabInd, "MSC", 3);
	
	
	sprintf(sLine, " END       GetTableInd(%s)", sAutTabInd);
	trace_event(sLine, PROCESSING);
	return (OK);

}

/*************************************************************************/
/* SmsToIsoAdtnlRespData()                                               */
/* 																		 */
/* Converts SMS Additional Response Data (field 44) into an ISO format   */
/* Input:  SMS Additional Response Data                                  */
/* ISO T2 data (field 35) presence flag                                  */
/* ISO Icc data (field 55) presence flag                                 */
/* ISO POS mode                                             			 */
/* Output: ISO field 48                                    				 */
/*************************************************************************/
/*AMER20210927 PLUTONL-3612*/
void 
	SmsToIsoAdtnlRespData(char *SmsAdtnlRespData,
	int  IsoT2DataPresence, 
	int  IsoIccDataPresence,
	char *IsoPosMode,
	TSTlvPrivate *TlvInfo)
{
	char			sSecurityResults[32 + 1];	 				
	char			sExtSecResDtl[P7_F048_P38_VISA_LEN + 1]; 
	char            FieldData1[2];
	int             nLength;	

	trace_event("Start SmsToIsoAdtnlRespData()", PROCESSING);
	
	memset(sSecurityResults  ,' ',sizeof(sSecurityResults )); 	
	memset(sExtSecResDtl  ,' ',sizeof(sExtSecResDtl )); 
	memset(FieldData1, 0, sizeof(FieldData1));		

	nLength = strlen(SmsAdtnlRespData);
	sExtSecResDtl[P7_F048_P38_NETWORK] = '1'; /* Network */  
	
	/* Subfield 44.1 Response Source/Reason Code*/
	PutTlvPrivate(EXTERNAL_RESP_SOURCE, TlvInfo, SmsAdtnlRespData, 1);

	/* Subfield 44.2 Address Verification Result Code*/
	if (nLength > 1 && SmsAdtnlRespData[1] != ' ') 
	{
		PutTlvPrivate(ADDRESS_VERIF_REP_TYPE_TAG, TlvInfo, SmsAdtnlRespData + 1, 1);

		switch (SmsAdtnlRespData[1])
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
		sExtSecResDtl[P7_F048_P38_VISA_CHK_AVS] = SmsAdtnlRespData[1];
	}

	/* Subfield 44.5 CVV/iCVV Results Code (requests and responses)*/

	if ((IsoPosMode[6] == '2' || IsoPosMode[6] == 'W') && IsoT2DataPresence == SUCCESS)
	{
		/* The authorization is magstripe && F35 is present  */
		if (nLength > 4)
		{
			switch (SmsAdtnlRespData[4])
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
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV1] = SmsAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, SmsAdtnlRespData + 4, 1);
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
			switch (SmsAdtnlRespData[4])
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
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV] = SmsAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, SmsAdtnlRespData + 4, 1);
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
			switch (SmsAdtnlRespData[4])
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
			sExtSecResDtl[P7_F048_P38_VISA_CHK_ICVV] = SmsAdtnlRespData[4];
			PutTlvPrivate(CVV_RESULT_CODE_TAG, TlvInfo, SmsAdtnlRespData + 4, 1);
		}
		else
		{
			sSecurityResults[SEC_FLAG_CHK_ICVV] = '4';
		}
	}

	/* Subfield 44.8 Card Authentication Results Code*/
	if (nLength > 8)
	{
		switch (SmsAdtnlRespData[8])
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
		sExtSecResDtl[P7_F048_P38_VISA_CHK_ARQC] = SmsAdtnlRespData[8];
	}
	else
	{
		sSecurityResults[SEC_FLAG_CHK_ARQC] = '4';
	}

	/* Subfield 44.10 CVV2 Result Code*/
	if (nLength > 10)
	{
		PutTlvPrivate(CVV2_RESULT_CODE_TAG, TlvInfo, SmsAdtnlRespData + 10, 1);

		switch (SmsAdtnlRespData[10])
		{
		case 'C':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = SmsAdtnlRespData[10];
			break;
		case 'D':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = SmsAdtnlRespData[10];
			break;
		case 'K':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = SmsAdtnlRespData[10];
			break;
		case 'L':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = SmsAdtnlRespData[10];
			break;
		case 'M':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '0';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = SmsAdtnlRespData[10];
			break;
		case 'N':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '1';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = SmsAdtnlRespData[10];
			break;
		case 'P':
			sSecurityResults[SEC_FLAG_CHK_DCVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_DCVV2] = SmsAdtnlRespData[10];
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = SmsAdtnlRespData[10];
			break;
		case 'S':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = SmsAdtnlRespData[10];
			break;
		case 'U':
			sSecurityResults[SEC_FLAG_CHK_CVV2] = '4';
			sExtSecResDtl[P7_F048_P38_VISA_CHK_CVV2] = SmsAdtnlRespData[10];
			break;
		}
	}

	/* Subfield 44.13 CAVV Results Code*/
	if (nLength > 14)
	{
		switch (SmsAdtnlRespData[14])
		{
		case ' ':
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'R';
			break;
		case '0':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '0';
			break;
		case '1':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '1';
			break;
		case '2':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '2';
			break;
		case '3':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '3';
			break;
		case '4':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '4';
			break;
		case '6':
			FieldData1[0] = P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART;
			break;
		case '7':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '7';
			break;
		case '8':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '8';
			break;
		case '9':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = '9';
			break;
		case 'A':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'A';
			break;
		case 'B':
			FieldData1[0] = SmsAdtnlRespData[14];
			sSecurityResults[SEC_FLAG_CHK_AAV] = 'B';
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

		sExtSecResDtl[P7_F048_P38_VISA_CHK_CAVV] = SmsAdtnlRespData[14];

		if (FieldData1[0] != '\0')
			PutTlvPrivate(AAV_RESULT, TlvInfo, FieldData1, 1);
	}
	else
	{
		sSecurityResults[SEC_FLAG_CHK_AAV] = 'R';
	}

	PutTlvPrivate(EXT_SECURITY_VER_RESULTS_TAG, TlvInfo, sSecurityResults, 32);
	PutTlvPrivate(EXT_SECURITY_RESULTS_DTL, TlvInfo, sExtSecResDtl, P7_F048_P38_VISA_LEN);
	
	trace_event("End SmsToIsoAdtnlRespData()", PROCESSING);

}
/**************************************************************************/
void
	IsoToSmsPosCondition(char *IsoMode,
	char *MerchantType,
	int acc_verification_only,
	char *SmsCond)
{
	trace_event("Start IsoToSmsPosCondition()", PROCESSING);

	/*Start AMER: Enh18.Q2 PROD00054205 Art 1.1: The code below is replacing the old one*/
	if (memcmp(MerchantType, MCC_AUTOMATED_CASH, P7_F018_LEN) == 0)	/* ATM transaction */
		memcpy(SmsCond, SMS_F025_CP_PIN, SMS_F025_LEN);
	else if(IsoMode[6] == P7_F022_P7_ECOM || IsoMode[6] == P7_F022_P7_ECOM_NON_AUTH_ATTEMPT || IsoMode[6] == P7_F022_P7_ECOM_AUTHENTICATED || IsoMode[6] == P7_F022_P7_ECOM_UNSECURE || IsoMode[6] == P7_F022_P7_ECOM_SEC_CHAN)	/*Ecom*/
		memcpy(SmsCond, SMS_F025_PUB_ECOM, SMS_F025_LEN);
	else if (IsoMode[4] == '1')	/* Cardholder not present */
		memcpy(SmsCond, SMS_F025_CH_NP, SMS_F025_LEN);
	else if ((IsoMode[4] == '0') && (IsoMode[5] == '0')) /* Cardholder present, card not present */
		memcpy(SmsCond, SMS_F025_CH_P_CRD_NP, SMS_F025_LEN);	
	else if ((IsoMode[4] == '5') || (IsoMode[4] == '2') || (IsoMode[4] == '3')) /* Mail order */
		memcpy(SmsCond, SMS_F025_MOTO_RECURRING, SMS_F025_LEN);	
	else if(acc_verification_only == 1)
		memcpy(SmsCond, SMS_F025_ACC_VERIF, SMS_F025_LEN);	
	else
		memcpy(SmsCond, SMS_F025_NORMAL_TRN, SMS_F025_LEN);
	/*End AMER Enh18.Q2 PROD00054205 Art 1.1*/


	trace_event("End  IsoToSmsPosCondition(OK)", PROCESSING);
	return OK;
}