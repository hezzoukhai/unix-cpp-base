/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>
#include <iso8583.h>
#include <iso_visa.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <visa_puf.h>
#include <visa_param.h>
#include <visa_define.h>
#include <visa_puf_fields.h>
#include <p7_common.h>
#include <p7_globals.h>

#include <iso8583.h>


#define SUCCES  0
#define ECHEC  -1

/*Start NAB23032020 PLUTONL-1435*/
/** AHA20170907 START dump under xml format **/
void DumpVisaXMLFormat( char *data )
{
	char sPufData[1024];
	char sCpsData[1024];
	char sWork[2048];
	char sLine[MAX_LINE_TRC];
	char sLine_Buffer[MAX_LINE_TRC];
	char TI[2];
	int nLength;
	int nFldType;
	int iMapCtr,i,j,k,nPos=0;
	TSVisaInfo msgInfo;
	TSCpsInfo cpsInfo;
	TSPufInfo pufInfo;
	TSVisaPufInfo VisapufInfo;

	/* Mehdi Elyajizi PCI */   	    
	char CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));
	/* Mehdi Elyajizi PCI */

	InitPufInfo(&pufInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitVisaInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);


	AnalyseVisa  ( data , &msgInfo);
	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", msgInfo.sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);


	sprintf(sLine, "-----------------------------------");
	trace_event(sLine, TRACE);
	sprintf(sLine,"MESSAGE_VISA_XML_FORMAT_START[%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<?xml version=\"1.0\"?>");
    trace_event(sLine,TRACE);
	
	sprintf(sLine,"<BaseMessage	Description=\"Log XML VISA \">");
    trace_event(sLine,TRACE);
    sprintf(sLine,"<MessageInfo>");
    trace_event(sLine,TRACE);
    sprintf(sLine,"<MsgType Value=\"%04d\"/>" , msgInfo.nMsgType );
    trace_event(sLine,TRACE);

	/*sprintf(sLine,"<MsgHeader Value=\"16010200FE0000000000000000000000000000000000\"/>" , msgInfo.nMsgType );
    trace_event(sLine,TRACE);*/

	for( i = 1 ; i <= iMapCtr * 64 ; i++)
	{
		memset(sWork, 0, sizeof(sWork));
		switch (i)
		{
		case 54 :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[LG_MAX];
				unsigned char leftTag,rightTag;
				int  nLen;

				sprintf(sLine,"<Field Number=\"%03d\" Value=\"" , i+1  );
				
				for( j = 3 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						leftTag=(unsigned char)sWork[j++];
						rightTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X%02X", leftTag, rightTag);
					}
					else
					{
						leftTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X", leftTag);
					}

					nLen = (unsigned char)sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);

					sprintf(sLine + strlen(sLine),"%s%02lX%s" , sTag ,strlen(sValue)/2, sValue );

					j += nLen;
				}
				sprintf(sLine + strlen(sLine),"\"/>" );
				
				k=0;
				while(k<strlen(sLine))
				{
					memset(sLine_Buffer,0,sizeof(sLine_Buffer));
					if((strlen(sLine)-k)>93)
						memcpy(sLine_Buffer,sLine+k,93);
					else
						memcpy(sLine_Buffer,sLine+k,strlen(sLine)-k);
					
					trace_event(sLine_Buffer ,TRACE);
					k=k+93;
				}
			}
			break;

		default :
			nFldType = GetVisaFieldType(i);

			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if( nFldType != VISA_FIX_BIN &&
					nFldType != VISA_VAR_BIN &&
					nFldType != VISA_VAR2_BIN &&   /*IZL16022022 PLUTONL-4379*/
					i!=61 &&
					i!=62 )
				{
					sprintf(sLine,"<Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				
					k=0;
					while(k<strlen(sLine))
					{
						memset(sLine_Buffer,0,sizeof(sLine_Buffer));
						if((strlen(sLine)-k)>93)
							memcpy(sLine_Buffer,sLine+k,93);
						else
							memcpy(sLine_Buffer,sLine+k,strlen(sLine)-k);
						
						trace_event(sLine_Buffer ,TRACE);
						k=k+93;
					}
				}
				else
				{
					nLength = HexToAscii(sWork, sPufData, nLength*2);
					
					sprintf(sLine,"<Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sPufData );
				
					k=0;
					while(k<strlen(sLine))
					{
						memset(sLine_Buffer,0,sizeof(sLine_Buffer));
						if((strlen(sLine)-k)>93)
							memcpy(sLine_Buffer,sLine+k,93);
						else
							memcpy(sLine_Buffer,sLine+k,strlen(sLine)-k);
						
						trace_event(sLine_Buffer ,TRACE);
						k=k+93;
					}
				}
			}

			break;
		}
	}
	
	sprintf(sLine," </MessageInfo>");
    trace_event(sLine,TRACE);
	
	sprintf(sLine," </BaseMessage>");
    trace_event(sLine,TRACE);
	sprintf(sLine,"MESSAGE_VISA_XML_FORMAT_END[%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);

	/**PA DSS*/
	memset(sPufData, 0, sizeof(sPufData));
	InitPufInfo(&pufInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitVisaInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);

}
/** AHA20170907 END dump under xml format **/

/*End HAL2402 PLUTONL-4391*/
static void DumpVerifData(char *sVerifnData, int nVerifDataLength)
{
	char sLine[MAX_LINE_TRC];
	char sData[1024];
	int nLength;
	TlvInfo kVisaTlvInfo;
	TlvInfo kVisaVerifDataTlv;

	InitB1VerifDataTlvInfo(&kVisaVerifDataTlv);

	AnalyseTlvInfo(sVerifnData, nVerifDataLength, &kVisaVerifDataTlv);

	if (GetTlvInfoField(B1_F123_ADDR_VERIF_DATA, &kVisaVerifDataTlv, sData, &nLength) == SUCCESS)
	{

		InitB1AddrVerifDataTlvInfo(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo);

		sprintf(sLine, "- DS: [%s] ", B1_F123_ADDR_VERIF_DATA);
		trace_event(sLine, TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}

	if (GetTlvInfoField(B1_F123_TOKEN_DATA, &kVisaVerifDataTlv, sData, &nLength) == SUCCESS)
	{

		InitB1TokenDataTlvInfo(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo);

		sprintf(sLine, "- DS: [%s] ", B1_F123_TOKEN_DATA);
		trace_event(sLine, TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
}
/*End HAL2402 PLUTONL-4391*/

/*End NAB23032020 PLUTONL-1435*/
static void DumpVisaTransSpecData(char* sTransSpecData, int nTransSpecLength)
{
	char			sLine[MAX_LINE_TRC];
	char			sData[1024];
	int				nLength;
	TlvInfo			kVisaTransSpecTlv;
	TlvInfo			kVisaAppIdentTlv;
	TlvInfo			kVisaSenderDataTlv;
	/*TlvInfo			kVisaAddSenderDataTlv;*/
	TlvInfo			kVisaPayFacTlv;	
	TlvInfo			kVisaRecurringPayTlv;	/*ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);
	InitVisaAppIdentDataTlv(&kVisaAppIdentTlv);
	InitVisaSenderDataTlv(&kVisaSenderDataTlv);
	/*InitVisaAddSenderDataTlv(&kVisaAddSenderDataTlv);*/
	InitVisaPayFacDataTlv(&kVisaPayFacTlv);
	InitVisaRecurringPayDataTlv(&kVisaRecurringPayTlv);	/*ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	


	AnalyseTlvInfo(sTransSpecData, nTransSpecLength, &kVisaTransSpecTlv);

	/* Start AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/
	if( GetTlvInfoField (B1_F104_PAY_FAC_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaPayFacTlv); 

		sprintf(sLine,"- DS: [%s] ",B1_F104_PAY_FAC_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaPayFacTlv);
	}
	/* End AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/

	if( GetTlvInfoField (B1_F104_BUSINESS_APP_ID, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaAppIdentTlv);

		sprintf(sLine,"- DS: [%s] ",B1_F104_BUSINESS_APP_ID);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaAppIdentTlv);
	}
	if( GetTlvInfoField (B1_F104_SENDER_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaSenderDataTlv);

		sprintf(sLine,"- DS: [%s] ",B1_F104_SENDER_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaSenderDataTlv);
	}
	/*
	if( GetTlvInfoField (B1_F104_ADDITIONAL_SENDER_DATA, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaAddSenderDataTlv);

		sprintf(sLine,"- DS: [%s] ",B1_F104_ADDITIONAL_SENDER_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaAddSenderDataTlv);
	}*/
	
	/*Start ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if( GetTlvInfoField (B1_F104_RECURRING_PAY, &kVisaTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kVisaRecurringPayTlv);

		sprintf(sLine,"- DS: [%s] ",B1_F104_RECURRING_PAY);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaRecurringPayTlv);
	}
	/*End ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

}
/* Start IBO20190726 PROD00070403 Enh19.Q4 9.1.3 & 9.1.4 */
static void DumpVisaElectComData(char* sElectComData, int nElectComLength)
{
	char			sLine[MAX_LINE_TRC];
	char			sData[1024];
	int				nLength;
	TlvInfo			kVisaElectComTlv;   		
	TlvInfo			kVisaTlvInfo;		
    BerInfo			kVisaBerInfo;	/*IBE PLUTONL-5640*/


	InitVisaElectComDataTlv(&kVisaElectComTlv);					
	
	
	AnalyseTlvInfo(sElectComData, nElectComLength, &kVisaElectComTlv);
	

	if( GetTlvInfoField (B1_F34_SUPP_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		/*start IBE PLUTONL-5640*/
		InitVisaSupplementalData(&kVisaBerInfo); 
		AnalyseBerInfo(sData, nLength, &kVisaBerInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_SUPP_DATA);
		trace_event(sLine,TRACE);

		DumpBerInfo(&kVisaBerInfo);
		/*end IBE PLUTONL-5640*/
	}

	if( GetTlvInfoField (B1_F34_EU_PSD2_SCA_CSC, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaEuPsd2ScaCsc(&kVisaTlvInfo); 
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_EU_PSD2_SCA_CSC);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	

	if( GetTlvInfoField (B1_F34_ACCEPTANCE_ENV_ADD_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaAccEnvAddDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_ACCEPTANCE_ENV_ADD_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	/*Start HAL PROD00073823 20202801 Enh Q2.2020 Art 2941*/
	if( GetTlvInfoField (B1_F34_AUTHEN_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{

		InitVisaEcomAddDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ", B1_F34_AUTHEN_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}

	/*END HAL PROD00073823 20202801 Enh Q2.2020 Art 2941*/
	/*Start PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
	if( GetTlvInfoField (B1_F34_ADD_SERVICE_REQUEST_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{

		InitVisaAddSrvReqDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_ADD_SERVICE_REQUEST_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	if( GetTlvInfoField (B1_F34_ADD_SERVICE_RESULT_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaAddSrvResDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_ADD_SERVICE_RESULT_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	if( GetTlvInfoField (B1_F34_DEVICE_PLATFORM_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaDevPlatformDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_DEVICE_PLATFORM_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);

	}
	if( GetTlvInfoField (B1_F34_DEVICE_COMMON_DATA, &kVisaElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaDevCommonDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F34_DEVICE_COMMON_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);

	}
	/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

	/*End PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/

}
/* End IBO20190726 PROD00070403 Enh19.Q4 9.1.3 & 9.1.4 */

/*Start PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
static void DumpVisaCustmRelatdData(char* sCustmRelatdData, int nCustmRelatdDataLen)
{
	char			sLine[MAX_LINE_TRC];
	char			sData[1024];
	int				nLength;
	TlvInfo			kCustmRelatdDataTlv;   		
	TlvInfo			kVisaTlvInfo;		


	InitVisaCustmRelatdDataTlv(&kCustmRelatdDataTlv);					
	AnalyseTlvInfo(sCustmRelatdData, nCustmRelatdDataLen, &kCustmRelatdDataTlv);

	if( GetTlvInfoField (B1_F056_ACCOUNT_OWNER_DATA, &kCustmRelatdDataTlv, sData, &nLength) == SUCCESS )
	{
		InitVisaAccOwnerDataTlv(&kVisaTlvInfo); 
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F056_ACCOUNT_OWNER_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	if ( GetTlvInfoField (B1_F056_CONTACT_INFO, &kCustmRelatdDataTlv, sData, &nLength) == SUCCESS) 
	{
		InitVisaContactInfoDataTlv(&kVisaTlvInfo); 
		AnalyseTlvInfo(sData, nLength, &kVisaTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",B1_F056_CONTACT_INFO);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kVisaTlvInfo);
	}
	/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

}

/*End  PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/

static void GetRejectCode(TSVisaHeader * reject_visa,char * RejectCode)
{
	RejectCode[0] = ((reject_visa->abRejectCode[0] >> 4) & 0x0F) + 0x30  ;
	RejectCode[1] = (reject_visa->abRejectCode[0] & 0x0F) + 0x30  ;
	RejectCode[2] = ((reject_visa->abRejectCode[1] >> 4) & 0x0F) + 0x30  ;
	RejectCode[3] = (reject_visa->abRejectCode[1] & 0x0F) + 0x30  ;
}

void DumpVisaData( char *data )
{
	char sPufData[1024];
	char sCpsData[1024];
	char sWork[2048];
	char sLine[MAX_LINE_TRC];
	char TI[2];
	int nLength;
	int nFldType;
	int iMapCtr,i,j,k,nPos=0;
	TSVisaInfo msgInfo;
	TSCpsInfo cpsInfo;
	TSPufInfo pufInfo;
	TSVisaPufInfo VisapufInfo;
	TlvInfo   kTlvInfo;
	TlvInfo   kTlvInfo2; /* En23Q2-Hotfix-PLUTONL-6119*/

	/* Mehdi Elyajizi PCI */
	char CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));
	/* Mehdi Elyajizi PCI */

	InitPufInfo(&pufInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitVisaInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);


	AnalyseVisa  ( data , &msgInfo);
	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char) msgInfo.sBitMap[i]);/*NAB11032020*/
		nPos += 3;
	}
	trace_event(sLine,TRACE);


	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- M.T.I      : [%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	sprintf(sLine,"- FLD (FIELD): LENGTH : DATA        ");
	trace_event(sLine,TRACE);
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine,TRACE);

	for( i = 1 ; i <= iMapCtr * 64 ; i++)
	{
		memset(sWork, 0, sizeof(sWork));
		switch (i)
		{
			/*Begin MKB060309 PCI Norme Integration */
		case VISA_CARD_NBR :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
				else
				{
					TrunCard(sWork, CardTrunc);
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength, CardTrunc );
				}
				trace_event(sLine,TRACE);
			}
			break;
		case 33:	/*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
				trace_event(sLine,TRACE);
				DumpVisaElectComData(sWork,nLength);
			}
			break;
		case VISA_T1_DATA :
		case VISA_T2_DATA :
		case VISA_PIN_DATA :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength, sWork);
				else
					sprintf(sLine,"- FLD (%03d)   (%03d)    [**********************]" , i+1, nLength);
				trace_event(sLine,TRACE);
			}
			break;
			/*End MKB060309 PCI Norme Integration */
		case 54 :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[LG_MAX];
				unsigned char leftTag,rightTag;
				int  nLen;

				sprintf(sLine,"- FLD (%03d)   (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);
				for( j = 3 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						leftTag=(unsigned char)sWork[j++];
						rightTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X%02X", leftTag, rightTag);
					}
					else
					{
						leftTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X  ", leftTag);
					}

					nLen = (unsigned char)sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);

					if (debug_option != 1)
					{
						if (  memcmp(sTag, "5F34", 4) == 0 ||
							memcmp(sTag, "9F27", 4) == 0 ||
							memcmp(sTag, "57", 2) == 0 ||
							memcmp(sTag, "5A", 2) == 0 ||
							memcmp(sTag, "C0", 2) == 0) 	/*AMER20180307 PROD00054205: Enh18.Q2 Art 8.1.1*/
							sprintf(sLine,"      > TAG (%.4s) ....: [***** Len %lu *****]" , sTag , strlen( sValue )   );
						else
							sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					}
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );

					trace_event(sLine,TRACE);

					j += nLen;
				}
			}
			break;
		
		/*Start PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
		/*case VISA_I_ORIG_DATA:	
		if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
			sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
			trace_event(sLine,TRACE);
			DumpVisaCustmRelatdData(sWork,nLength);
		}
		break;*/
		/*End PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
		case 61 :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);
				AnalyseCps    (  sWork  , &cpsInfo);
				memset(sCpsData, 0, sizeof(sCpsData));
				/*for( j = 0 ; j < 18 ; j++) ENH 11.1*/
				for( j = 0 ; j < 25 ; j++) /*ENH 11.1*/ /*  HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1 21->25*/
					if ( GetCpsSubField(j, &cpsInfo, sCpsData, &nLength ) == SUCCES )
					{
						sprintf(sLine,"      > SUBFIELD (%02d) ....: [%s]"
							, j+1 , sCpsData    );
						trace_event(sLine,TRACE);
					}
			}
			break;

		case 62 :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);
				AnalysePuf    (  sWork  , &pufInfo);
				memset(sPufData, 0, sizeof(sPufData));
				for( j = 0 ; j < 18 ; j++)
					if ( GetPufSubField(j, &pufInfo, sPufData, &nLength ) == SUCCES )
					{
						sprintf(sLine,"      > SUBFIELD (%02d) ....: [%s]"
							, j+1 , sPufData    );
						trace_event(sLine,TRACE);
					}
			}
			break;
		case 125 :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);
				AnalyseVisaPuf    (  sWork  , &VisapufInfo);
				memset(sPufData, 0, sizeof(sPufData));
				for( j = 0 ; j < 19 ; j++)
				{
					if ( GetVisaPufSubField(j, &VisapufInfo, sPufData, &nLength ) == SUCCES )
					{
						if(j == VPUF_CVV2)	/*AMER 20160727 PADSS Fix*/
						{
							if (debug_option == 1)
								sprintf(sLine,"      > SUBFIELD (%02d) ....: [%d|%s]" , j+1 , nLength, sPufData);
							else
								sprintf(sLine,"      > SUBFIELD (%02d) ..............: ***",j+1);
						}
						else
							sprintf(sLine,"      > SUBFIELD (%02d) ....: [%d|%s]"
							, j+1 , nLength,sPufData    );
						trace_event(sLine,TRACE);

					}
				}
			}
			break;
		case 103:	/*AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
				trace_event(sLine,TRACE);
				DumpVisaTransSpecData(sWork,nLength);
			}
			break;
        case 109:   /*FZL20220822 PLUTONL-4979*/
		    if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
			    sprintf(sLine,"- FLD (%03d)   (%03d) " , i+1, nLength);
				trace_event(sLine,TRACE);
				InitVisaAdditionalDataTlv(&kTlvInfo);
				AnalyseTlvInfo(sWork,nLength,&kTlvInfo);
				if( GetTlvInfoField (B1_F110T04_KEY_DATA, &kTlvInfo, sWork, &nLength) == SUCCESS )
				{
	                InitVisaKeyDataTlv(&kTlvInfo);
				    AnalyseTlvInfo(sWork, nLength, &kTlvInfo);
					sprintf(sLine,"- DS: [%s] ", B1_F110T04_KEY_DATA );
					trace_event(sLine,TRACE);
					DumpTlvInfo(&kTlvInfo);
				};
			}
		    break;

		/*HAL2402 PLUTONL-4391*/
		case 122:
			if (GetVisaField(i, &msgInfo, sWork, &nLength) == SUCCES)
			{
				sprintf(sLine, "- FLD (%03d)   (%03d)   ", i + 1, nLength);
				trace_event(sLine, TRACE);
				DumpVerifData(sWork, nLength);
			}
			break;
			/*HAL2402 PLUTONL-4391*/
		/*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
		case VISA_SUPPORT_INFO :
			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
			    sprintf(sLine,"- FLD (%03d)   (%03d) " , i+1, nLength);
				trace_event(sLine,TRACE);
				InitVisaSupportInfDataTlv(&kTlvInfo);
				AnalyseTlvInfo(sWork,nLength,&kTlvInfo);
				if( GetTlvInfoField (B1_F125_TOKEN_DEVICE, &kTlvInfo, sWork, &nLength) == SUCCESS )
				{
	                InitTokenDevice(&kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
				    AnalyseTlvInfo(sWork, nLength, &kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
					sprintf(sLine,"- DS: [%s] ", B1_F125_TOKEN_DEVICE );
					trace_event(sLine,TRACE);
					DumpTlvInfo(&kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
				};
				if( GetTlvInfoField (B1_F125_WALLET_PROVIDER, &kTlvInfo, sWork, &nLength) == SUCCESS )
				{
	                InitVisaWalletProviderDataTlv(&kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
				    AnalyseTlvInfo(sWork, nLength, &kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
					sprintf(sLine,"- DS: [%s] ", B1_F125_WALLET_PROVIDER );
					trace_event(sLine,TRACE);
					DumpTlvInfo(&kTlvInfo2);/* En23Q2-Hotfix-PLUTONL-6119*/
				};
				
			}
		    break;
		/*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
		default :
			nFldType = GetVisaFieldType(i);

			if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if( nFldType != VISA_FIX_BIN &&
					nFldType != VISA_VAR_BIN )
				{
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
					trace_event(sLine,TRACE);
				}
				else
				{
					nLength = HexToAscii(sWork, sPufData, nLength*2);
					sPufData[nLength]='\0'; /*IBO20221004 PLUTONL-5240*/
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sPufData);
					trace_event(sLine,TRACE);
				}
			}

			break;
		}
	}

	/**PA DSS*/
	memset(sPufData, 0, sizeof(sPufData));
	InitPufInfo(&pufInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitVisaInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);
	memset(sPufData, 0, sizeof(sPufData));
	memset(sCpsData, 0, sizeof(sCpsData));
	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));

}

void DumpVisa(data)
	char *data;
{
	char * texte;
	char   RejectCode[5];
	TSVisaHeader * header;
	TSVisaHeader * reject_visa;
	char sLine[MAX_LINE_TRC];
	trace_event("Start DumpVisa()",PROCESSING);

	header = ( TSVisaHeader *) data ;
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- H(Header field) : CONTENT");
	trace_event(sLine,TRACE);
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(01) Header Length.........: %02X",header->bHeaderLength );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(02) Header flag & format..: %02X", header->bHeaderFlag );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(03) Text Format...........: %02X",header->bTextFormat );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(04) Total Message Length..: %02X %02X",header->abMsgLength [0], header->abMsgLength [1]);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(05) Dest Station Id.......: %02X %02X %02X",header->abDestStationId [ 0],
		header->abDestStationId  [ 1], header->abDestStationId [ 2]);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(06) Src  Station Id.......: %02X %02X %02X",header->abSrcStationId [ 0],
		header->abSrcStationId  [ 1], header->abSrcStationId [ 2]);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(07) Round-Trip Ctrl Info..: %02X",header->bEchoInfo);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(08) Base I Flags..........: %02X %02X",header->abBaseIIFlags[0],  header->abBaseIIFlags[1]);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(09) Message Status Flags..: %02X %02X %02X",header->abMsgFlags[0],
		header->abMsgFlags[1] , header->abMsgFlags[2] );
	trace_event(sLine,TRACE);
	sprintf(sLine ,"- H(10) Batch Number..........: %02X",header->bBatchNumber);
	trace_event(sLine,TRACE);
	sprintf(sLine ,"- H(11) Reserved..............: %02X %02X %02X",header->abReserved [0] ,
		header->abReserved [1] , header->abReserved [2]);
	trace_event(sLine,TRACE);
	sprintf(sLine ,"- H(12) User Information......: %02X ",header->bUserInfo);
	trace_event(sLine,TRACE);

	if ( header->bHeaderLength >= 26 )
	{
		reject_visa = ( TSVisaHeader *) data;
		sprintf(sLine,"               MESSAGE WAS REJECTED");
		trace_event(sLine,TRACE);
		sprintf(sLine,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		trace_event(sLine,TRACE);
		GetRejectCode(reject_visa,RejectCode);
		sprintf(sLine,"------> Reject Code :    %.4s                ", RejectCode);
		trace_event(sLine,TRACE);
		sprintf(sLine,"-++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		trace_event(sLine,TRACE);

		if ( reject_visa->abBitMap [0] & 0x80 )
		{
			texte = data + reject_visa->bHeaderLength ;
			header = ( TSVisaHeader *) texte ;
			DumpVisaData(data);
		}
	}
	else
		DumpVisaData(data);

	/*Start NAB23032020  PLUTONL-1435*/

	/** AHA20170907 PROD00046623 START dump message in format XML */
	if (debug_option == 1)
		DumpVisaXMLFormat (data);
	/** AHA20170907 PROD00046623 END dump message in format XML */
	/*End NAB23032020   PLUTONL-1435*/

	trace_event("End   DumpVisa()",PROCESSING);
}
void DumpVisaStruct(TSVisaInfo  *VisaInfo)
{
	char sWork[LG_MAX];
	int  Length;

	trace_event("Start DumpVisaStruct()",TRACE);
	memset(sWork, 0, sizeof(sWork));

	Length = VisaBuildMsg (sWork + VISA_HDR_LEN, VisaInfo);
	VisaBuildHeader(RESPONSE , &(VisaInfo->sHeader), NULL, Length,StationId);
	memcpy(sWork, (char *) &(VisaInfo->sHeader), VISA_HDR_LEN);

	DumpVisaData (sWork);
	trace_event("End   DumpVisaStruct()",TRACE);

}

/**Start MKB221208 CCD*/
void VisaDumpIccFields (char *sIccData, int nIccLen)
{
	TSTagVisa       lTagVisa;
	typedef struct{
		char *sTagDesc;
		char *sTagValue;
	} VISATAG;

	VISATAG aIccVisaTag [] =
	{
		{"VISA_TAG_ISS_SCRIPT1         ","7100"},
		{"VISA_TAG_ISS_SCRIPT2         ","7200"},
		{"VISA_TAG_APP_INTER_PROFILE   ","8200"},
		{"VISA_TAG_DED_FILE_NAME       ","8400"},
		{"VISA_TAG_ISS_AUTHEN_DATA     ","9100"},
		{"VISA_TAG_TVR                 ","9500"},
		{"VISA_TAG_TRANS_DATE          ","9A00"},
		{"VISA_TAG_TRANS_TYPE          ","9C00"},
		{"VISA_TAG_TRANS_CUR_CODE      ","5F2A"},
		{"VISA_TAG_TRANS_AMOUNT        ","9F02"},
		{"VISA_TAG_OTHER_AMOUNT        ","9F03"},
		{"VISA_TAG_TERM_APP_VER_NUM    ","9F09"},
		{"VISA_TAG_ISS_APP_DATA        ","9F10"},
		{"VISA_TAG_TERM_COUNTRY_CODE   ","9F1A"},
		{"VISA_TAG_IFD_SERIAL_NUM      ","9F1E"},
		{"VISA_TAG_APP_CRYPTOGRAM      ","9F26"},
		{"VISA_TAG_CRYPTO_INFO_DATA    ","9F27"},
		{"VISA_TAG_TERM_CAP            ","9F33"},
		{"VISA_TAG_CVM                 ","9F34"},
		{"VISA_TAG_TERM_TYPE           ","9F35"},
		{"VISA_TAG_ATC                 ","9F36"},
		{"VISA_TAG_UNPRED_NUMBER       ","9F37"},
		{"VISA_TAG_TRANS_SEQ_NUM       ","9F41"},
		{"VISA_TAG_TRANS_CATEG_CODE    ","9F53"},
	};

	int  i = 0;
	int  nLen = 0;
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];

	trace_event("Start VisaDumpIccFields()",PROCESSING);

	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));


	InitVisaIcTag (&lTagVisa);


	AnalyseVisaIc (sIccData, nIccLen, &lTagVisa);

	trace_event("         Field                         | Len  |              Value",TRACE);

	for (i=0; i<(sizeof(aIccVisaTag)/sizeof(aIccVisaTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));


		if ( GetVisaIcTag (aIccVisaTag[i].sTagValue , &lTagVisa,sWork,&nLen) == SUCCESS)
		{

			if (debug_option != 1)
			{
				if (   memcmp(aIccVisaTag[i].sTagValue, "5F34", 4) == 0 ||
					memcmp(aIccVisaTag[i].sTagValue, "9F27", 4) == 0 ||
					memcmp(aIccVisaTag[i].sTagValue, "5A", 2) == 0 ||
					memcmp(aIccVisaTag[i].sTagValue, "57", 2) == 0)
					sprintf(sLine, " >> %s|%s : [%03d] : [********************]", aIccVisaTag[i].sTagValue, aIccVisaTag[i].sTagDesc, nLen);
				else
					sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccVisaTag[i].sTagValue, aIccVisaTag[i].sTagDesc, nLen, sWork);
			}
			else
				sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccVisaTag[i].sTagValue, aIccVisaTag[i].sTagDesc, nLen, sWork);
			trace_event(sLine,TRACE);
		}
		else {

			sprintf(sLine," -->> FIELD %s|%s NOT FOUND ", aIccVisaTag[i].sTagValue,aIccVisaTag[i].sTagDesc);
			trace_event(sLine,TRACE);
		}
	}


	trace_event("End VisaDumpIccFields()",PROCESSING);
}