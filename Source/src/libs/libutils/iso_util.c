#include <stdio.h>
#include <stdlib.h>

#include <iso_hps.h>
#include <tools.h>
#include <define.h>
#include <tlv_private.h>
#include <iso8583.h>
#include <p7_sid.h>

#include <queue.h>
#include <security_data.h>

#ifndef GIT_TAG 

#define GIT_TAG				"Undefined"
#define GIT_BRANCH			"Undefined"
#define GIT_COMMIT_HASH		"Undefined"

#endif

int BuildVisaReferenceNbr(char* sRefNbr, char* sXmit,char* sStan)
{

	char	szDate[32];
	char	szWork[32];
	int		nRetCode;

	memset(szDate, 0, sizeof(szDate));
	memset(szWork, 0, sizeof(szWork));
	
	current_date_yyyymmdd(szWork);
	memcpy(szDate, sXmit + 2, 2);
	memcpy(szDate + 2, sXmit, 2);
	memcpy(szDate + 4, szWork, 4);
	memset(szWork, 0, sizeof(szWork));
	nRetCode = convert_to_julian(szDate, szWork);

	sRefNbr[0] = szDate[7];
	memcpy(sRefNbr + 1, szWork, 3);
	memcpy(sRefNbr + 4, sXmit + 4, 2);
	memcpy(sRefNbr + 6, sStan, 6);

	sRefNbr[12]='\0';
	
	return OK;

}

/******************************************************************************/
/*AMER 20160719*/
/*int SetLocalYear(char* szLocalDateYYMMDD, char* szGmtDateYYMMDD)
{
	char    szGmtYear[12];
	char    szLocalYear[12];

	if( memcmp(szLocalDateYYMMDD + 2,"01",2) != 0 &&
		memcmp(szLocalDateYYMMDD + 2,"12",2) != 0)
	{
		memcpy(szLocalDateYYMMDD,szGmtDateYYMMDD,2);
		return OK;
	}



	if( memcmp(szLocalDateYYMMDD + 2, szGmtDateYYMMDD + 2,2) == 0 )
	{
		memcpy(szLocalDateYYMMDD,szGmtDateYYMMDD,2);
		return OK;
	}

	memset(szGmtYear,'\0',sizeof(szGmtYear));
	memset(szLocalYear,'\0',sizeof(szLocalYear));

	if( memcmp(szLocalDateYYMMDD + 2,"01",2) == 0 )
	{
		memcpy(szGmtYear,szGmtDateYYMMDD,2);
		sprintf(szLocalYear,"%.2d",atoi(szGmtYear) + 1 );
		memcpy(szLocalDateYYMMDD,szLocalYear,2);
		return OK;
	}
	memcpy(szGmtYear,szGmtDateYYMMDD,2);
	sprintf(szLocalYear,"%.2d",atoi(szGmtYear) - 1 );
	memcpy(szLocalDateYYMMDD,szLocalYear,2);

	return OK;
}*/
/*ICH20180104*/
int SetLocalYear(char* szMerchantDateYYMMDD, char* szGmtDateYYMMDD)
{
	char    szGmtYear[12];
	char    szLocalYear[12];
	char 	sFristMonthDigit[2];

	int 	nFristMonthDigit;


	memset(szGmtYear,'\0',sizeof(szGmtYear));
	memset(szLocalYear,'\0',sizeof(szLocalYear));
	memset(sFristMonthDigit,'\0',sizeof(sFristMonthDigit));



	if( memcmp(szMerchantDateYYMMDD + 2, szGmtDateYYMMDD + 2,2) == 0 )
	{
		memcpy(szMerchantDateYYMMDD,szGmtDateYYMMDD,2);
		return OK;
	}

	if( memcmp(szMerchantDateYYMMDD + 2,"01",2) != 0 &&
		memcmp(szMerchantDateYYMMDD + 2,"12",2) != 0)
	{
		memcpy(szMerchantDateYYMMDD,szGmtDateYYMMDD,2);
		return OK;
	}



	memcpy(szGmtYear,szGmtDateYYMMDD,2);

	memcpy(sFristMonthDigit,szGmtDateYYMMDD + 2,1);

	nFristMonthDigit= atoi(sFristMonthDigit);

	if( memcmp(szMerchantDateYYMMDD + 2,"01",2) == 0 )
	{
		if(nFristMonthDigit == 1)/* To manage gmt MM <01 */
			sprintf(szLocalYear,"%.2d",atoi(szGmtYear) + 1 );

		else /* To manage gmt MM >01 */
			sprintf(szLocalYear,"%.2d",atoi(szGmtYear));


		memcpy(szMerchantDateYYMMDD,szLocalYear,2);
		return OK;
	}


	/* To manage gmt MM=11 or 10  */
	if(nFristMonthDigit == 1)
		sprintf(szLocalYear,"%.2d",atoi(szGmtYear));
	else
		sprintf(szLocalYear,"%.2d",atoi(szGmtYear) - 1 );


	memcpy(szMerchantDateYYMMDD,szLocalYear,2);
	return OK;
}

/************************************************************************************/
/* MciToIsoMdesMemberDefData()                                             			*/
/* 																					*/
/* Converts CIS & MDS Member-Defined Data for MDES use (DE 124) into an ISO format	*/
/* Input:  CIS & MDS Member-Defined Data (DE 124)                               	*/
/* 		   ISO field 48   (Additional Data, Private)                           		*/
/* Output: ISO field 24   (Function Code)                                 			*/
/* 		   ISO field 48   (Additional Data, Private)                           		*/
/************************************************************************************/


int MciToIsoMdesMemberDefData(
	char *MdesMemberDefData,
	char *IsoFunctionCode,
	TSTlvPrivate *TlvPrivateInfo)
{
	TlvInfo isoDigitizationDataTlv;
	char FieldData[BUF_LG_MAX];
	int tmpFieldLen;
	int offset = 0;
	char sSpaceVar[165 + 1];

	memset(FieldData, 0, sizeof(FieldData));
	IniDigitDataTlv(&isoDigitizationDataTlv);
	tmpFieldLen = strlen(MdesMemberDefData);

	if (tmpFieldLen >= MCI_F124_MSG_TYPE_LEN)
	{
		if (memcmp(MdesMemberDefData, "TE", MCI_F124_MSG_TYPE_LEN) == 0)
		{
			/* Message Type */
			memcpy(IsoFunctionCode, P7_F024_TER, P7_F024_LEN);
			offset += MCI_F124_MSG_TYPE_LEN;

			/* Correlation ID */
			if (tmpFieldLen >= offset + MCI_F124_CORRELATION_ID_LEN)
				PutTlvPrivate(TRANSACTION_IDENTIFIER, TlvPrivateInfo, MdesMemberDefData + offset, MCI_F124_CORRELATION_ID_LEN);

			offset += MCI_F124_CORRELATION_ID_LEN;

			/* Primary Account Number Source */
			if (tmpFieldLen >= offset + MCI_F124_PAN_SOURCE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_PAN_SOURCE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_PAN_SOURCE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_PAN_SOURCE_LEN);

			offset += MCI_F124_PAN_SOURCE_LEN;

			/* Payment Application Instance ID */
			if (tmpFieldLen >= offset + MCI_F124_PAY_APP_INST_ID_LEN
				&& memcmp(MdesMemberDefData + offset,"                                                ",MCI_F124_PAY_APP_INST_ID_LEN) != 0)
				AddTlvInfoField(P7_F048S25_PAY_APP_INSTANCE_ID, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_PAY_APP_INST_ID_LEN);

			offset += MCI_F124_PAY_APP_INST_ID_LEN;

			/* Number of Active Tokens for the Primary Account Number */
			if (tmpFieldLen >= offset + MCI_F124_NBR_ACTIVE_TKNS_LEN
				&& memcmp(MdesMemberDefData + offset,"  ",MCI_F124_NBR_ACTIVE_TKNS_LEN) != 0 )
				AddTlvInfoField(P7_F048S25_NBR_ACTIVE_TKNS, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_NBR_ACTIVE_TKNS_LEN);

			offset += MCI_F124_NBR_ACTIVE_TKNS_LEN + MCI_F124_WP_ACC_ID_HASH_LEN + MCI_F124_CARDHLDR_NAME_LEN; /* Wallet Provider Account ID Hash && Cardholder Name are not mapped*/

			/* Token Type */
			if (tmpFieldLen >= offset + MCI_F124_TKN_TYPE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_TKN_TYPE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_TYPE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_TYPE_LEN);
		
		}
		else if (memcmp(MdesMemberDefData, "TA", 2) == 0)
		{
			/* Message Type */
			memcpy(IsoFunctionCode, P7_F024_TAR, P7_F024_LEN);
			offset += MCI_F124_MSG_TYPE_LEN;

			/* Correlation ID */
			if (tmpFieldLen >= offset + MCI_F124_CORRELATION_ID_LEN)
				PutTlvPrivate(TRANSACTION_IDENTIFIER, TlvPrivateInfo, MdesMemberDefData + offset, MCI_F124_CORRELATION_ID_LEN);

			offset += MCI_F124_CORRELATION_ID_LEN;

			/* Primary Account Number Source */
			if (tmpFieldLen >= offset + MCI_F124_PAN_SOURCE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_PAN_SOURCE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_PAN_SOURCE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_PAN_SOURCE_LEN);

			offset += MCI_F124_PAN_SOURCE_LEN;

			/* Payment Application Instance ID */
			if (tmpFieldLen >= offset + MCI_F124_PAY_APP_INST_ID_LEN
				&& memcmp(MdesMemberDefData + offset,"                                                ",MCI_F124_PAY_APP_INST_ID_LEN) != 0)
				AddTlvInfoField(P7_F048S25_PAY_APP_INSTANCE_ID, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_PAY_APP_INST_ID_LEN);

			offset += MCI_F124_PAY_APP_INST_ID_LEN;

			/* Device Source IP Address */
			if (tmpFieldLen >= offset + MCI_F124_DEVICE_SRC_IP_AD_LEN
				&& memcmp(MdesMemberDefData + offset,"            ",MCI_F124_DEVICE_SRC_IP_AD_LEN) != 0)
				AddTlvInfoField(P7_F048S25_DEVICE_SRC_IP_ADR, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_DEVICE_SRC_IP_AD_LEN);

			offset += MCI_F124_DEVICE_SRC_IP_AD_LEN + MCI_F124_WP_ACC_ID_HASH_LEN + MCI_F124_CARDHLDR_NAME_LEN; /* Wallet Provider Account ID Hash && Cardholder Name are not mapped*/

			/* Wallet Provider Tokenization Recommendation */
			if (tmpFieldLen >= offset + MCI_F124_WP_TKN_RECMD_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_WP_TKN_RECMD_LEN) != 0)
				AddTlvInfoField(P7_F048S25_WP_TKN_RECMD, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_WP_TKN_RECMD_LEN);

			offset += MCI_F124_WP_TKN_RECMD_LEN;

			/* Wallet Provider Tokenization Recommendation Standard Version */
			if (tmpFieldLen >= offset + MCI_F124_WP_TKN_RECMD_VERS_LEN
				&& memcmp(MdesMemberDefData + offset,"  ",MCI_F124_WP_TKN_RECMD_VERS_LEN) != 0)
				AddTlvInfoField(P7_F048S25_WP_TKN_RECMD_VERSION, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_WP_TKN_RECMD_VERS_LEN);

			offset += MCI_F124_WP_TKN_RECMD_VERS_LEN;

			/* Wallet Provider Device Score */
			if (tmpFieldLen >= offset + MCI_F124_WP_DEVICE_SCORE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_WP_DEVICE_SCORE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_WP_DEVICE_SCORE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_WP_DEVICE_SCORE_LEN);

			offset += MCI_F124_WP_DEVICE_SCORE_LEN;

			/* Wallet Provider Account Score */
			if (tmpFieldLen >= offset + MCI_F124_WP_ACCOUNT_SCORE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_WP_ACCOUNT_SCORE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_WP_ACCOUNT_SCORE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_WP_ACCOUNT_SCORE_LEN);

			offset += MCI_F124_WP_ACCOUNT_SCORE_LEN;

			/* Number of Active Tokens for the Primary Account Number */
			if (tmpFieldLen >= offset + MCI_F124_NBR_ACTIVE_TKNS_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_NBR_ACTIVE_TKNS_LEN) != 0)
				AddTlvInfoField(P7_F048S25_NBR_ACTIVE_TKNS, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_NBR_ACTIVE_TKNS_LEN);

			offset += MCI_F124_NBR_ACTIVE_TKNS_LEN + MCI_F124_WP_RECM_REAS_CD_LEN; /* Wallet Service Provider Tokenization Recommendation Reason Codes is not mapped*/

			/* Device Location */
			if (tmpFieldLen >= offset + MCI_F124_DEVICE_LOCT_LEN
				&& memcmp(MdesMemberDefData + offset,"         ",MCI_F124_DEVICE_LOCT_LEN) != 0)
				AddTlvInfoField(P7_F048S25_DEVICE_LOCATION, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_DEVICE_LOCT_LEN);

			offset += MCI_F124_DEVICE_LOCT_LEN;

			/* Mobile Number Last 4 Digits */
			if (tmpFieldLen >= offset + MCI_F124_MOB_NBR_LAST_DIG_LEN
				&& memcmp(MdesMemberDefData + offset,"    ",MCI_F124_MOB_NBR_LAST_DIG_LEN) != 0)
				AddTlvInfoField(P7_F048S25_MOB_NBR_LAST_DIGITS, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_MOB_NBR_LAST_DIG_LEN);

			offset += MCI_F124_MOB_NBR_LAST_DIG_LEN;

			/* Token Type */
			if (tmpFieldLen >= offset + MCI_F124_TKN_TYPE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_TKN_TYPE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_TYPE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_TYPE_LEN);
		
		}
		else if (memcmp(MdesMemberDefData, "AC", 2) == 0)
		{
			/* Message Type */
			memcpy(IsoFunctionCode, P7_F024_ACN, P7_F024_LEN);
			offset += MCI_F124_MSG_TYPE_LEN;

			/* Correlation ID */
			if (tmpFieldLen >= offset + MCI_F124_CORRELATION_ID_LEN)
				PutTlvPrivate(TRANSACTION_IDENTIFIER, TlvPrivateInfo, MdesMemberDefData + offset, MCI_F124_CORRELATION_ID_LEN);

			offset += MCI_F124_CORRELATION_ID_LEN;

			/* Activation Code */
			if (tmpFieldLen >= offset + MCI_F124_ACTIV_CODE_LEN
				&& memcmp(MdesMemberDefData + offset,"        ",MCI_F124_ACTIV_CODE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_ACTIVATION_CODE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_ACTIV_CODE_LEN);

			offset += MCI_F124_ACTIV_CODE_LEN;

			/* Activation Code Expiration Date and Time */
			if (tmpFieldLen >= offset + MCI_F124_ACTIV_CODE_EXP_DT_LEN
				&& memcmp(MdesMemberDefData + offset,"          ",MCI_F124_ACTIV_CODE_EXP_DT_LEN) != 0)
				AddTlvInfoField(P7_F048S25_ACTIVATION_CODE_EXP_DT, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_ACTIV_CODE_EXP_DT_LEN);

			offset += MCI_F124_ACTIV_CODE_EXP_DT_LEN;

			/* Consumer’s Activation Method Preference */
			memset(sSpaceVar, '\0',sizeof(sSpaceVar));
			memset(sSpaceVar, ' ',tmpFieldLen - offset);
			if (tmpFieldLen > offset
				&& memcmp(MdesMemberDefData + offset, sSpaceVar, tmpFieldLen - offset) != 0)
				AddTlvInfoField(P7_F048S25_CSMR_ACTIV_METHD_PREF, &isoDigitizationDataTlv, MdesMemberDefData + offset, tmpFieldLen - offset);
		
		}
		else if (memcmp(MdesMemberDefData, "TC", 2) == 0)
		{
			/* Message Type */
			memcpy(IsoFunctionCode, P7_F024_TCN, P7_F024_LEN);
			offset += MCI_F124_MSG_TYPE_LEN;

			/* Correlation ID */
			if (tmpFieldLen >= offset + MCI_F124_CORRELATION_ID_LEN)
				PutTlvPrivate(TRANSACTION_IDENTIFIER, TlvPrivateInfo, MdesMemberDefData + offset, MCI_F124_CORRELATION_ID_LEN);

			offset += MCI_F124_CORRELATION_ID_LEN;

			/* Number of Active Tokens for the Primary Account Number */
			if (tmpFieldLen >= offset + MCI_F124_NBR_ACTIVE_TKNS_LEN
				&& memcmp(MdesMemberDefData + offset,"  ",MCI_F124_NBR_ACTIVE_TKNS_LEN) != 0)
				AddTlvInfoField(P7_F048S25_NBR_ACTIVE_TKNS, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_NBR_ACTIVE_TKNS_LEN);

			offset += MCI_F124_NBR_ACTIVE_TKNS_LEN + MCI_F124_ISS_PRD_CONF_ID_LEN + MCI_F124_CONSUMER_LANG_LEN; /* Issuer Product Configuration ID && Consumer Language are not mapped*/

			/* Device Name */
			if (tmpFieldLen >= offset + MCI_F124_DEVICE_NAME_LEN
				&& memcmp(MdesMemberDefData + offset,"                    ",MCI_F124_DEVICE_NAME_LEN) != 0)
				AddTlvInfoField(P7_F048S25_DEVICE_NAME, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_DEVICE_NAME_LEN);
			offset += MCI_F124_DEVICE_NAME_LEN;

			/* Final Tokenization Decision*/
			if (tmpFieldLen >= offset + MCI_F124_FNL_TKN_DECISION_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_FNL_TKN_DECISION_LEN) != 0)
				AddTlvInfoField(P7_F048S25_FINAL_TKN_DECISION, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_FNL_TKN_DECISION_LEN);

			offset += MCI_F124_FNL_TKN_DECISION_LEN;

			/* Final Tokenization Decision Indicator */
			if (tmpFieldLen >= offset + MCI_F124_FNL_TKN_DECS_IND_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_FNL_TKN_DECS_IND_LEN) != 0)
				AddTlvInfoField(P7_F048S25_FINAL_TKN_DECISION_IND, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_FNL_TKN_DECS_IND_LEN);

			offset += MCI_F124_FNL_TKN_DECS_IND_LEN;

			/* T&C Identifier */
			if (tmpFieldLen >= offset + MCI_F124_TC_IDENTIFIER_LEN
				&& memcmp(MdesMemberDefData + offset,"                                ",MCI_F124_TC_IDENTIFIER_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TC_IDENTIFIER, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TC_IDENTIFIER_LEN);

			offset += MCI_F124_TC_IDENTIFIER_LEN;

			/* T&C Date and Time */
			if (tmpFieldLen >= offset + MCI_F124_TC_DT_LEN
				&& memcmp(MdesMemberDefData + offset,"          ",MCI_F124_TC_DT_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TC_DT, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TC_DT_LEN);

			offset += MCI_F124_TC_DT_LEN + MCI_F124_NBR_ACTV_ATTEMPTS_LEN; /* Number of Activation Attempts is not mapped*/

			/* Token Unique Reference */
			if (tmpFieldLen >= offset + MCI_F124_TKN_UNQ_REF_LEN
				&& memcmp(MdesMemberDefData + offset,"                                                ",MCI_F124_TKN_UNQ_REF_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_UNQ_REF, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_UNQ_REF_LEN);

			offset += MCI_F124_TKN_UNQ_REF_LEN;

			/* Primary Account Number Unique Reference */
			if (tmpFieldLen >= offset + MCI_F124_PAN_UNQ_REF_LEN
				&& memcmp(MdesMemberDefData + offset,"                                                ",MCI_F124_PAN_UNQ_REF_LEN) != 0)
				AddTlvInfoField(P7_F048S25_PAN_UNQ_REF, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_PAN_UNQ_REF_LEN);

			offset += MCI_F124_PAN_UNQ_REF_LEN;

			/* Token Type */
			if (tmpFieldLen >= offset + MCI_F124_TKN_TYPE_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_TKN_TYPE_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_TYPE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_TYPE_LEN);
		}
		else if (memcmp(MdesMemberDefData, "TV", 2) == 0)
		{
			/* Message Type */
			memcpy(IsoFunctionCode, P7_F024_TVN, P7_F024_LEN);
			offset += MCI_F124_MSG_TYPE_LEN;

			/* Correlation ID */
			if (tmpFieldLen >= offset + MCI_F124_CORRELATION_ID_LEN)
				PutTlvPrivate(TRANSACTION_IDENTIFIER, TlvPrivateInfo, MdesMemberDefData + offset, MCI_F124_CORRELATION_ID_LEN);

			offset += MCI_F124_CORRELATION_ID_LEN;

			/* Tokenization Event Indicator */
			if (tmpFieldLen >= offset + MCI_F124_TKN_EVENT_IND_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_TKN_EVENT_IND_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_EVENT_IND, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_EVENT_IND_LEN);

			offset += MCI_F124_TKN_EVENT_IND_LEN;

			/* Tokenization Event Reason Code */
			if (tmpFieldLen >= offset + MCI_F124_TKN_EVENT_RSN_CD_LEN
				&& memcmp(MdesMemberDefData + offset,"  ",MCI_F124_TKN_EVENT_RSN_CD_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_EVENT_RSN_CODE, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_EVENT_RSN_CD_LEN);

			offset += MCI_F124_TKN_EVENT_RSN_CD_LEN;

			/* Event Requestor */
			if (tmpFieldLen >= offset + MCI_F124_EVENT_REQUESTOR_LEN
				&& memcmp(MdesMemberDefData + offset," ",MCI_F124_EVENT_REQUESTOR_LEN) != 0)
				AddTlvInfoField(P7_F048S25_EVENT_REQUESTOR, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_EVENT_REQUESTOR_LEN);

			/*Start AEA20210712 Enh21.Q4 AN4409*/
			offset += MCI_F124_EVENT_REQUESTOR_LEN;

			/* Token Unique Reference */
			if (tmpFieldLen >= offset + MCI_F124_TKN_UNQ_REF_LEN
				&& memcmp(MdesMemberDefData + offset,"                                                ",MCI_F124_TKN_UNQ_REF_LEN) != 0)
				AddTlvInfoField(P7_F048S25_TKN_UNQ_REF, &isoDigitizationDataTlv, MdesMemberDefData + offset, MCI_F124_TKN_UNQ_REF_LEN);
			/*End AEA20210712 Enh21.Q4 AN4409*/

		
		}
		tmpFieldLen = BuildTlvInfoBuffer(FieldData, &isoDigitizationDataTlv);
		if (tmpFieldLen > 0)
			PutTlvPrivate(DIGITIZATION_DATA, TlvPrivateInfo, FieldData, tmpFieldLen);
	}
}
int ProcessOpt (int argc, char *argv[]) 
{
	int opt;
	while ((opt = getopt(argc, argv, "v")) != -1) 
    {
        switch (opt) 
		{
            case 'v':
        		fprintf(stderr, " >>>>> VERSION TAG:[%s]\n >>>>> BRANCH:[%s] \n >>>>> HASH:[%s]\n", GIT_TAG, GIT_BRANCH, GIT_COMMIT_HASH);
				return OK;
        	default: /* aq?aq */
            	fprintf(stderr, "Usage -v");
				return OK;
        }
    }
	return NOK;
}
 
/* ELKAM PLUTONL-3835 START */
/* This function has as role to manage the cases where 	*/
/* the interface visa could not write on LINE   		*/

int IntWriteLineExeption(int nIndexCtx, TSIsoInfo * IsoInfo) {

	int             Length;
	int             FieldLen;
	char 			sMsgType[4 + 1];
	char            FieldData[256];

	memset(sMsgType, '\0', sizeof(sMsgType)); 
	memset(FieldData, '\0', sizeof(FieldData)); 

	trace_event("Start IntWriteLineExeption()",PROCESSING);

	sprintf(sMsgType,"%.4d",IsoInfo->nMsgType);

	switch(sMsgType[MSG_TYPE_CLASS])
   	{
   	case MSG_TYPE_P2_AUT :
	case MSG_TYPE_P2_FIN :
		switch(sMsgType[MSG_TYPE_FUNCTION])
		{
		case MSG_TYPE_P3_REQ		:
			/*  1100 / 1200 */
			if (sMsgType[MSG_TYPE_CLASS]=='1') IsoInfo->nMsgType = 1110 ;	/* 1100/1101 -> 1110 */
			else IsoInfo->nMsgType = 1210 ;									/* 1200/1201 -> 1210 */

			ReplyIsoMsgToBal(nBalMsg, IsoInfo, 0, P7_F039_ISS_UNV);
			MsgFwdRepReplied(IsoInfo->msgId); 
			break;

		case MSG_TYPE_P3_REQ_RESP 	:
			/*	1110 / 1210 */
			if (GetIsoField ( ISO_RESPONSE_CODE, IsoInfo, FieldData, &FieldLen) != SUCCESS)
			{
				/* MALFUNCTION */
				trace_event("End   IntWriteLineExeption(NOK)",PROCESSING);
				return NOK;
			}
			if ( strcmp(FieldData ,"000") != 0 && strcmp(FieldData ,"002") != 0 && strcmp(FieldData ,"010") != 0)
			{
				/* Auth not accepted, no need to reversal */
				trace_event("End   IntWriteLineExeption(OK)",PROCESSING);
				return OK;
			}

			if (GenIsoRevProc(nIndexCtx , "ADM" , IsoInfo , P7_F025_UNABLE_TO_DELIVER_MSG ) != OK)
			{
				trace_event("End   IntWriteLineExeption(NOK)",PROCESSING);
				return NOK;
			}
			break;

		case MSG_TYPE_P3_ADV		:
			/*	1120 / 1220 */
			/* Nothing to do */
			break;

		case MSG_TYPE_P3_ADV_RESP 	:
			/*	1130 / 1230 */
			/* Nothing to do*/
			break;

		default :
			/* NOTIF CASE */
			/* Nothing to do*/
			break;
		}
		break;


	case MSG_TYPE_P2_REV :
		/* 14XX cases */
		/* No need to process Reversal req */
		break;
		
	default :
		/* other cases */
		break;
	}
	trace_event("End   IntWriteLineExeption(OK)",PROCESSING);
	return OK;
}
/* ELKAM PLUTONL-3835 END */

/* ELKAM PLUTONL-4061 START */

int IntWriteBalExeption(int nIndexCtx, TSIsoInfo * IsoInfo) {

	int             Length;
	int             FieldLen;
	char 			sMsgType[4 + 1];
	char            FieldData[256];
	char			sResource[SOURCE_RESOURCE_LEN + 1];
	char			sDestResource[DESTINATION_RESOURCE_LEN + 1];
	char            sSecurityData[128];

	memset(sMsgType, '\0', sizeof(sMsgType)); 
	memset(FieldData, '\0', sizeof(FieldData));
	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset(sDestResource, 0, sizeof(sDestResource));
	memset(sResource, 0, sizeof(sResource));

	trace_event("Start IntWriteBalExeption()",PROCESSING);
	sprintf(sMsgType,"%.4d",IsoInfo->nMsgType);

	switch(sMsgType[MSG_TYPE_CLASS])
   	{
   	case MSG_TYPE_P2_AUT :
	case MSG_TYPE_P2_FIN :
		switch(sMsgType[MSG_TYPE_FUNCTION])
		{
		case MSG_TYPE_P3_REQ		:
			/*  1100 / 1200 */
			/* Case processed after ForwardAutRequest, nothing to do*/
			break;

		case MSG_TYPE_P3_REQ_RESP 	:
			/*	1110 / 1210 */
			if (GetIsoField ( ISO_RESPONSE_CODE, IsoInfo, FieldData, &FieldLen) != SUCCESS)
			{
				/* MALFUNCTION */
				trace_event("End   IntWriteLineExeption(NOK)",ERROR);
				return NOK;
			}
			if ( strcmp(FieldData ,"000") != 0 && strcmp(FieldData ,"002") != 0 && strcmp(FieldData ,"010") != 0)
			{
				/* Auth not accepted, no need to reversal */
				trace_event("End   IntWriteLineExeption(OK)",PROCESSING);
				return OK;
			}
			
			GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &FieldLen);
			GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sDestResource, &Length);
			GetSecurityData(SOURCE_RESOURCE, sSecurityData, sResource, &Length);

			if (AutomaticIsoReversalProc(nIndexCtx , NORMAL_MODE , sResource , sDestResource , "ADM" , IsoInfo , P7_F025_UNABLE_TO_DELIVER_MSG ) != OK)
			{
				trace_event("End   IntWriteLineExeption(NOK)",ERROR);
				return NOK;
			}
			break;

		case MSG_TYPE_P3_ADV		:
			/*	1120 / 1220 */
			/* Nothing to do */
			break;

		case MSG_TYPE_P3_ADV_RESP 	:
			/*	1130 / 1230 */
			/* Nothing to do*/
			break;

		default :
			/* NOTIF CASE */
			/* Nothing to do*/
			break;
		}
		break;

	case MSG_TYPE_P2_REV :
		/* 14XX cases */
		/* No need to process Reversal req */
		break;
		
	default :
		/* other cases */
		break;
	}
	trace_event("End   IntWriteBalExeption(OK)",PROCESSING);
	return OK;
}
/* ELKAM PLUTONL-4061 END */

/* ELKAM20230217 PLUTONL-5914 START*/
/* ELKAM20221226 PLUTONL-4304 START */
/* 						IsoCompletionToReversal									
   For completion : Final amount in F04 , Original amount in F54.57 			
   For Reversal   : Final amount (replacement) in F30 , Original amount in F04 	
   Put F04+F05 	in F30 															
   Put F54.57 	in F04 															
   Put '402' 	in F24 															
   Do nothing if Original amount = Final amount									*/
int IsoCompletionToReversal(TSIsoInfo * sMsgIso,int* nSendReq)
{

	int				nCmpAmount,i,nRetCode =	NOK	;
    int             tmpFieldLen					;
    char			sFinalAmount	[  24 + 1 ]	;
    char			sOrigAmount 	[  12 + 1 ]	;
    char			sOldOrg 	    [  24 + 1 ]	;
    char            tmpField        [ 255 + 1 ] ;

	trace_event("Start IsoCompletionToReversal()" ,PROCESSING);
        
    memset ( sFinalAmount 		, '\0' , sizeof ( sFinalAmount	));
    memset ( sOrigAmount  		, '\0' , sizeof ( sOrigAmount	));
    memset ( sOldOrg 	    	, '\0' , sizeof ( sOldOrg		));
    memset ( tmpField 	    	, '\0' , sizeof ( tmpField		));

	nRetCode = GetIsoField(ISO_TRANS_AMOUNT, sMsgIso, sFinalAmount, &tmpFieldLen); /* From position 1 to position 12 the amount of transaction in transaction currency */
	if (nRetCode != OK)
	{
		trace_event(">>>> Error Getting ISO_TRANS_AMOUNT",ERROR);
		trace_event("End IsoCompletionToReversal(NOK)" ,ERROR);
		return( NOK );
	}

	/* nRetCode = GetIsoField(ISO_BILLING_AMOUNT, sMsgIso, sFinalAmount+12, &tmpFieldLen); /* from position 13 to position 24 the new amount of transaction expressed in the billing currency * /
	if (nRetCode != OK)
	{
		memcpy(sFinalAmount+12, "000000000000", 12);
	} */

	memcpy(sFinalAmount+12, "000000000000", 12);

	if (GetIsoField(ISO_ADTNL_AMOUNTS, sMsgIso, tmpField, &tmpFieldLen) == OK)
	{
		for(i=0; i < strlen(tmpField) ; i+=20)
		{
			if (memcmp(tmpField + i + P7_F054_AT_LEN, P7_F054_AT_ORIG_AMNT, P7_F054_ACCT_TYPE_LEN) == 0)
			{
				strncpy(sOrigAmount,tmpField+i+8,12);
			}
		}
	}

	/* trace_message(0,"sOldOrg = [%s]",sFinalAmount+12);
	trace_message(0,"sOrigAmount = [%s]",sOrigAmount); */

	if (strlen(sOrigAmount) == 0)
	{
		trace_event(">>>> Error Getting F54.57",ERROR);
		trace_event("End IsoCompletionToReversal(NOK)" ,ERROR);
		return( NOK );
	}

	nCmpAmount = strncmp(sFinalAmount, sOrigAmount, 12);

	if ( nCmpAmount == 0 )
	{
		*nSendReq = 0;
		trace_event(">> [sFinalAmount = sOrigAmount] -> Do nothing" ,PROCESSING);
		trace_event("End IsoCompletionToReversal(OK)" ,PROCESSING);
		return( OK ); /* Nothing to do */
	}
	else if ( nCmpAmount > 0 ) /* If Final amount greater than Original amount */
	{
		trace_event(">>>> Error : Final amount should not exceed preauthorization amount",ERROR);
		trace_event("End IsoCompletionToReversal(NOK)" ,ERROR);
		return( NOK );
	}

	sMsgIso->nMsgType = 1420 ;
	PutIsoField ( ISO_ORIGINAL_AMOUNTS, sMsgIso, sFinalAmount,				24  );
	PutIsoField ( ISO_TRANS_AMOUNT, 	sMsgIso, sOrigAmount, 				12  );
	PutIsoField ( ISO_FUNCTION_CODE, 	sMsgIso, P7_F024_PARTIAL_REVERSAL,	3   );

	trace_event("End IsoCompletionToReversal(OK)" ,PROCESSING);

	return OK;
}
/* ELKAM20221226 PLUTONL-4304 END */
/* ELKAM20230217 PLUTONL-5914 END*/