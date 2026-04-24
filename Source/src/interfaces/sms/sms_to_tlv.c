#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_cps.h>

#include <tlv_autho.h>
#include <tlv_private.h>
#include <sms_fields.h>
#include <sms_puf.h>
#include <security_data.h>
#include <sms_param.h>
#include <p7_define.h>

/**********************************************************************************************/
/*   Fonction qui convertit un message Sms en un message tlv              *********************/
/*   Usage unique pour les chargebacks, fees, et text messages            *********************/
/**********************************************************************************************/ 
int SmsToTlv (int nIndiceCtx, TSSmsInfo * SmsInfo, TSTlvAutho *tlvInfo)
{
	TSPufInfo      PufInfo;
	TSCpsInfo      CpsInfo;
	TSTlvPrivate   TlvInfo;

	char           sLine     [ MAX_LINE_TRC ];
	char           tmpField  [ BUF_LG_MAX ];
	char           tmpField1 [ BUF_LG_MAX ]; 
	char           tagName   [ 4 ];
	char	       szAcquirerBank[12];
	char           sMti      [ 4+1 ];
	int            i;
	int            nRetCode;
	int            tmpFieldLen;
	int            tmpFieldLen1;

	trace_event ( "Start SmsToTlv ()", PROCESSING );

	memset ( sMti , 0 , sizeof ( sMti ));
	memset ( tagName, 0, sizeof(tagName));
	memset ( szAcquirerBank, 0, sizeof(szAcquirerBank));

	sprintf ( sMti , "%04d", SmsInfo->nMsgType);

	PutTlvAutho ( AUTO_MSG_TYPE, tlvInfo, sMti, 4);
	PutTlvAutho ( AUTO_ACQUIRER_BANK, tlvInfo, AcquirerBank, 6);
	
	for ( i= 1; i< 127; i++)
	{
		memset (tmpField, 0, sizeof(tmpField));
		
		switch (i)
		{

		case SMS_PROC_CODE :
			
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				sprintf(tmpField1,    "%.3s002%.2s", PROC_TRANS_TYPE, tmpField );
				sprintf(tmpField1+8,  "%.3s002%.2s", PROC_ACCOUNT_FROM, tmpField+2);
				sprintf(tmpField1+16, "%.3s002%.2s", PROC_ACCOUNT_TO, tmpField+4);
				sprintf (tagName, "%.3d", i+1);
				PutTlvAutho (tagName, tlvInfo, tmpField1, 24);
			}
			break;

		case SMS_POS_CONDITION : 
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				PutTlvAutho (AUTO_POS_DATA, tlvInfo, tmpField, tmpFieldLen);
			}
			break;
		case SMS_TRANS_FEE:
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				PutTlvAutho (AUTO_TRANS_FEE, tlvInfo, tmpField, tmpFieldLen);
			} 
			break;
		case SMS_CPS_FIELDS : 

			memset ( tagName, 0, sizeof(tagName));
			InitCpsInfo (&CpsInfo);
			if (GetSmsField (i, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AnalyseCps (tmpField, &CpsInfo);
				InitTlvPrivate(&TlvInfo);

				/*if (GetCpsSubField (CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					
					sprintf (tagName, "%.3d", i+1);
					if (GetCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField1, &tmpFieldLen1) == SUCCESS)
					{
						memcpy (tmpField + 1, tmpField1, tmpFieldLen1);
						PutTlvAutho (tagName, tlvInfo, tmpField, tmpFieldLen1 + 1);
					}
					else
						PutTlvAutho (tagName, tlvInfo, tmpField, 1);
				}*/
				
				/*Start AMER20211004 PLUTONL-3579*/
				if (GetCpsSubField (CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
					AddTlvPrivate(CPS_ACI_TAG, &TlvInfo, tmpField, 1);
				
				if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
					AddTlvPrivate(CPS_TRANS_ID_TAG, &TlvInfo, tmpField, 15);

				tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);

				if(tmpFieldLen > 0)
					PutTlvAutho (AUTO_CPS_DATA, tlvInfo, tmpField, tmpFieldLen);
				/*End AMER20211004 PLUTONL-3579*/
				
			}
			break;

		case SMS_PRIV_USE_FIELDS :
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				InitPufInfo (&PufInfo);
				InitTlvPrivate (&TlvInfo);

				AnalysePuf (tmpField, &PufInfo);
				memset (tmpField,  0, sizeof (tmpField));
				/*memcpy (tmpField, "N01004", 6);*/




				/*GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField +6,  &tmpFieldLen);
				if (tmpFieldLen == 4)
					PutTlvAutho (AUTO_NETWORK_DATA, tlvInfo, tmpField, 10);
					*/
				/*Start ICH17012018*/
				if (GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
					PutTlvPrivate (NET_NETWORK_ID_TAG, &TlvInfo, tmpField, tmpFieldLen);

				if (GetPufSubField (PUF_CHARGEBACK_FLAGS, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
					PutTlvPrivate (NET_CHARGEBACK_FLAGS_TAG, &TlvInfo, tmpField, tmpFieldLen);

				tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);

				if(tmpFieldLen >0)
				PutTlvAutho (AUTO_NETWORK_DATA, tlvInfo, tmpField, tmpFieldLen);
				/*End ICH17012018*/

				if (GetPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
					PutTlvAutho (AUTO_MESSAGE_REASON_CODE, tlvInfo, tmpField, tmpFieldLen);

				memset (tmpField,  0, sizeof (tmpField));
				memset (tmpField1, 0, sizeof (tmpField1));
				if (GetPufSubField (PUF_STIP_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					memcpy (tmpField1, tmpField, 4);
				}
				else
					memcpy (tmpField1, "0000", 4);

				if (GetPufSubField (PUF_FPI, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					memcpy (tmpField1+4, tmpField, 3);
					PutTlvAutho (AUTO_UF_FILE_RECORD, tlvInfo, tmpField1, 7);
				}
				else
					PutTlvAutho (AUTO_UF_FILE_RECORD, tlvInfo, tmpField1, 4);
			}
			break;
		case SMS_ICC_DATA :
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				IsoIccToTlv(tmpField, tmpFieldLen, tmpField1, &tmpFieldLen1);
				if (tmpFieldLen > 0)
				{
					PutTlvAutho (AUTO_ICC_DATA, tlvInfo, tmpField1, tmpFieldLen1);
				}
			}
			break;
		default :
			sprintf (tagName, "%.3d", i+1);
			if (GetSmsField ( i, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
			{
				PutTlvAutho (tagName, tlvInfo, tmpField, tmpFieldLen);
			}
			break;

		} /* end switch */ 
	} /* end for */

	trace_event ( "End SmsToTlv ()", PROCESSING );
	return (OK);
}   
