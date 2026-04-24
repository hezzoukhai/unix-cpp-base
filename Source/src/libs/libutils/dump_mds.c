/***************************************************************************/
/*            UTILS_ECCF.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <iso_mds.h>
#include <mds_tlv.h>
#include <iso8583.h>
#define SUCCES  0
#define ECHEC  -1

/*extern int debug_option = 0;*//*qualif20120109*/

typedef struct s_tlv_print
{
	char *description;
	char *value;
} t_tlv_print;


static t_tlv_print  sTlvDataDesc[] = {
	{ "PDS_DES..............", "01"  },
	{ "PDS_FLAGS............", "02"  },
	{ "PDS_VER_NBR..........", "03"  },
	{ "PDS_SIGN_OFF_REASON..", "04"  },
	{ "PDS_REJ_MSG..........", "05"  },
	{ "PDS_REJ_REASON.......", "06"  },
	{ "PDS_TEST_PROD........", "07"  },
	{ "PDS_PINBLOCK_FORMAT..", "08"  },
	{ "TAG_PEK..............", "11"  },
	{ "PDS_CVM..............", "20"  },
	{ "PDS_ACCEPTANCE_DATA..", "21"  },
	{ "MULTI_PUR_MER_IND....", "22"  }, /*IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
	{ "PDS_DEVICE_TYPE_IND..", "23"	},   	/*ACH20191219 */
	{ "PDS_WALLET_PROG_DATA.", "26"	},   	/*ACH20191219 */
	{ "PDS_TRX_ANALYSIS.....", "27"	},   	/*ACH20191219*/
	{ "PDS_TKN_TRX_ID.......", "30"	},   	/*ACH20191219 */
	{ "PDS_PAN_MAPPING_INFO.", "33"	}, 		/*ACH20191219*/
	{ "TAG_CERT_SERIAL_NBR..", "40"  },
	{ "TAG_CERT_AUTHN_DATA..", "41"  },
	{ "TAG_SECURITY_IND.....", "42"  },
	{ "TAG_UCAF.............", "43"  },
	{ "MDS_SEC_SERV_DATA....", "56"},    /* PLUTONL-5548 AN6773  Enh 2023Q2 AHA20221221*/
	{ "PDS_TRACEID..........", "63"  },		/*HAL20210111 PLUTONL-4154*/
	{ "AUTHENTICATION_DATA..", "66"}, 	/* ELKAM20230213 PLUTONL-5831 ENH23.Q2 AN 7090 */
	{ "TAG_IMPLIED_DECIMAL..", "70"  },
	{ "PDS_ON_BEHALF_SERV...", "71"  },/* ZKO 22072022 PLUTONL-4902 ENH 22.Q4 MCI AN 6039*/
	{ "TAG_ADD_PROC_INFO....", "74"  },		/*AMER20210925 PLUTONL-3612*/
	{ "PDS_PAYMENT_TRANS....", "77"  },		/*HAL20210111 PLUTONL-4154*/
	{ "TAG_AVS_REQ_OPTION...", "82"  },
	{ "TAG_AVS_RESPONSE.....", "83"  },
	{ "TAG_CVC_INVALID......", "87"  },
	{ "TAG_CVC_F22_CHANGE...", "88"  },
	{ "TAG_ERR_TRK_F61_TCC..", "89"  },
	{ "PDS_CVC2.............", "92"  },
	{ "PDS_AIR_TICK_NBR.....", "93"  },
	{ "PDS_PROMOTION_CODE...", "95"  },

};


void DumpMds( char *data )
{


	char		sWork[LG_MAX];
	char		TI[2];
	int			nLength;
	int			i,j,k;
	TSMdsInfo  msgInfo;

	char       sLine[MAX_LINE_TRC];
	int        nList[]={200,210,220,230,430,420,422,520,530,810,800};
	int        nRetCode;
	int        nF48HeadLen ;

	TSPdsMds   sPdsInfo;

	char CardTrunc[22 + 1];
	char CryptPart[22 + 1];
	TlvInfo   kTlvInfo; /* AHA20221004 PLUTONL-4980*/


	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(CryptPart, 0, sizeof(CryptPart));

	trace_event("Start DumpMds()",PROCESSING);


	InitMdsInfo(&msgInfo);
	AnalyseMds  ( data , &msgInfo);
	memset(sWork, 0, sizeof(sWork));

	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"- M.T.I      : %04d" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);

	sprintf(sLine,"- BIT MAP    : %02X %02X %02X %02X %02X %02X %02X %02X",
		(unsigned char)msgInfo.sBitMap [0], (unsigned char)msgInfo.sBitMap [1], (unsigned char)msgInfo.sBitMap [2], (unsigned char)msgInfo.sBitMap [3],
		(unsigned char)msgInfo.sBitMap [4],(unsigned char) msgInfo.sBitMap [5],(unsigned char) msgInfo.sBitMap [6], (unsigned char)msgInfo.sBitMap [7]);

	if ( 0x80 == ( (unsigned char) msgInfo.sBitMap [0] & 0x80 ) )
		sprintf(sLine+strlen(sLine)," %02X %02X %02X %02X %02X %02X %02X %02X ",
		(unsigned char)msgInfo.sBitMap [8], (unsigned char)msgInfo.sBitMap [9], (unsigned char)msgInfo.sBitMap [10], (unsigned char)msgInfo.sBitMap [11],
		(unsigned char)msgInfo.sBitMap [12],(unsigned char) msgInfo.sBitMap [13],(unsigned char) msgInfo.sBitMap [14], (unsigned char)msgInfo.sBitMap [15]);

	trace_event(sLine, TRACE);
	/* Control in message Type */

	nRetCode = NOK;
	for (i=0;i<sizeof(nList)/sizeof(int);i++)
	{
		/*  sprintf(sLine," Message Type Checking Is %d",nList[i]);
		trace_event(sLine,TRACE); */

		if (msgInfo.nMsgType == nList[i])
		{
			nRetCode = OK;
			break;
		}
	}
	if(nRetCode != OK)
	{
		trace_event("Invalid Message ",FATAL);
		trace_event("End   DumpMds(NOK)",TRACE);
		return;
	}

	trace_event("------------------------------------",TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
	trace_event("------------------------------------",TRACE);

	InitMdsPds(&sPdsInfo);

	for( i = 1 ; i <= 127 ; i++)
	{
		/*Begin MKB060309 PCI Norme Integration */
		switch (i + 1)
		{

		case 2:/*case MDS_CARD_NBR :*/
			if ( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%.80s] " , i+1 , nLength ,sWork );
				else
				{
					TrunCard(sWork, CardTrunc);
					CryptCard(sWork, CryptPart);
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s][%s] " , i+1 , nLength ,CardTrunc,CryptPart );
				}
			}
			trace_event(sLine,TRACE);
			break;

		case 35: /*case MDS_T2_DATA :*/
		case 45: /*case MDS_T1_DATA :*/
		case 52:/*case MDS_PIN_DATA :*/

			if ( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%.80s] " , i+1 , nLength ,sWork );
				else
					sprintf(sLine,"- FLD (%03d) : (%03d) : [**********************]" , i+1, nLength);
				trace_event(sLine,TRACE);
			}
			break;
			/*case 48 :*/	/*AMER20170301: F48 type is MDS_LLL_ALPHA*/
		case 48:
			if ( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				InitMdsPds(&sPdsInfo); /*HAL20210111 PLUTONL-4154*/ 

				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				if (nLength - 1 > 0)
				{
					AnalyseMdsPds(sWork+1, nLength-1, msgInfo.nMsgType, &sPdsInfo);
				}
				for (j=0; j<(sizeof(sTlvDataDesc)/sizeof(sTlvDataDesc[0])); j++)
				{
				   if(GetMdsPds (sTlvDataDesc[j].value , &sPdsInfo , sWork, &nLength)==SUCCESS )
					{
						sWork[nLength] = 0;
						if( memcmp(sTlvDataDesc[j].value, MDS_PDS_CVC2, 2) == 0)
						{
							if (debug_option == 1)
								sprintf ( sLine ,"       >Tag: '%.2s' : (%25s) : [%.400s]", /*PLUTONL-5548  Enh 2023Q2 AHA20221221*/
										sTlvDataDesc[j].value,
										sTlvDataDesc[j].description,
								sWork );
							else
								sprintf ( sLine ,"       >Tag: '%.s' : (%25s) : [****************]",
										sTlvDataDesc[j].value,
										sTlvDataDesc[j].description
								);
							trace_event(sLine,TRACE);
						}
						/*START PLUTONL-5812 AN6022  Enh 2023Q2  FZL20230202*/	
					    else if( memcmp(sTlvDataDesc[j].value, MDS_MULTI_PUR_MER_IND, 2) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
									sTlvDataDesc[j].value,
									sTlvDataDesc[j].description);
							trace_event(sLine,TRACE);
							
							InitMdsMultiPurposeMerDataTlvInfo(&kTlvInfo);
							AnalyseTlvInfo(sWork, nLength, &kTlvInfo);
							
							DumpTlvInfo(&kTlvInfo);
						}
					    /*END PLUTONL-5812 AN6022  Enh 2023Q2  FZL20230202*/	
						else
						{
							sprintf ( sLine ,"       >Tag: '%.2s' : (%25s) : [%.400s]",/*PLUTONL-5548  Enh 2023Q2 AHA20221221*/
									sTlvDataDesc[j].value,
									sTlvDataDesc[j].description,
								sWork );
							trace_event(sLine,TRACE);
						}
					}
				}

			}
			break;
		case 55 :
			if ( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[LG_MAX];
				unsigned char leftTag,rightTag;
				int  nLen;

				sprintf(sLine,"- FLD (%03d)   (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);

				for( j = 0 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					/*NAB1006*/
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
					/*nLen = sWork[j++];*/

					HexToAscii(sWork+j,sValue,nLen*2);


					if (debug_option != 1)
					{
						if (   memcmp (sTag, "5F34", 4) == 0 ||
							memcmp (sTag, "9F27", 4) == 0 ||
							memcmp (sTag, "57", 2) == 0  ||
							memcmp (sTag, "5A", 2) == 0)
							sprintf(sLine,"      > TAG (%.4s) ....: [***** Len %lu *****]" , sTag , strlen( sValue )   );
						else
							sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					}
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );

					trace_event(sLine,TRACE);

					j += nLen;
				}
				memset(sValue, 0, sizeof(sValue));

			}
			break;
			/* AHA20221004 PLUTONL-4980*/
			case MDS_ADDITIONAL_DATA_2 +1:
				if( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES)
				{
					sprintf(sLine,"- FLD (%03d) (%03d)    " , i+1, nLength );
					trace_event(sLine,TRACE);

					InitMdsAdditionalDataTlv(&kTlvInfo);
					AnalyseTlvInfo(sWork,nLength,&kTlvInfo);
					DumpTlvInfo(&kTlvInfo);
				}
			break;
			/* AHA20221004 PLUTONL-4980*/
		default :
			if ( GetMdsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%.400s] " , i+1 , nLength ,sWork );/*PLUTONL-5548  Enh 2023Q2 AHA20221221*/
				trace_event(sLine,TRACE);
			}
			break;
		}
		/*End MKB060309 PCI Norme Integration */
	}

	InitMdsPds(&sPdsInfo);
	InitMdsInfo(&msgInfo);
	memset(sWork, 0, sizeof(sWork));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(CryptPart, 0, sizeof(CryptPart));


	trace_event( "-----------------------------------",TRACE); /*EBE140506: Adding ,TRACE*/
	trace_event("End   DumpMds()",TRACE);



}


void DumpMds_struct( TSMdsInfo *Mdsinfo )
{
	char data[LG_MAX];
	memset(data, 0, sizeof(data));
	MdsBuildMsg ( data , Mdsinfo );
	DumpMds( data );
	memset(data, 0, sizeof(data));
}



void MdsDumpIccFields (char *sIccData, int nIccLen)
{

	TSTagMds       lTagMds;
	typedef struct{
		char *sTagDesc;
		char *sTagValue;
	} ECCFTAG;

	ECCFTAG aIccMdsTag [] =
	{
		{"ECCF_TAG_ISS_SCRIPT1         ","7100"},
		{"ECCF_TAG_ISS_SCRIPT2         ","7200"},
		{"ECCF_TAG_APP_INTER_PROFILE   ","8200"},
		{"ECCF_TAG_DED_FILE_NAME       ","8400"},
		{"ECCF_TAG_ISS_AUTHEN_DATA     ","9100"},
		{"ECCF_TAG_TVR                 ","9500"},
		{"ECCF_TAG_TRANS_DATE          ","9A00"},
		{"ECCF_TAG_TRANS_TYPE          ","9C00"},
		{"ECCF_TAG_TRANS_CUR_CODE      ","5F2A"},
		{"ECCF_TAG_TRANS_AMOUNT        ","9F02"},
		{"ECCF_TAG_OTHER_AMOUNT        ","9F03"},
		{"ECCF_TAG_TERM_APP_VER_NUM    ","9F09"},
		{"ECCF_TAG_ISS_APP_DATA        ","9F10"},
		{"ECCF_TAG_TERM_COUNTRY_CODE   ","9F1A"},
		{"ECCF_TAG_IFD_SERIAL_NUM      ","9F1E"},
		{"ECCF_TAG_APP_CRYPTOGRAM      ","9F26"},
		{"ECCF_TAG_CRYPTO_INFO_DATA    ","9F27"},
		{"ECCF_TAG_TERM_CAP            ","9F33"},
		{"ECCF_TAG_CVM                 ","9F34"},
		{"ECCF_TAG_TERM_TYPE           ","9F35"},
		{"ECCF_TAG_ATC                 ","9F36"},
		{"ECCF_TAG_UNPRED_NUMBER       ","9F37"},
		{"ECCF_TAG_TRANS_SEQ_NUM       ","9F41"},
		{"ECCF_TAG_TRANS_CATEG_CODE    ","9F53"},
	};

	int  i = 0;
	int  nLen = 0;
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];

	trace_event("Start MdsDumpIccFields()",PROCESSING);

	memset(sWork, 0, sizeof(sWork));

	InitMdsIcTag (&lTagMds);


	AnalyseMdsIc (sIccData, nIccLen, &lTagMds);

	trace_event("         Field                         | Len  |              Value",TRACE);

	for (i=0; i<(sizeof(aIccMdsTag)/sizeof(aIccMdsTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));

		if ( GetMdsIcTag (aIccMdsTag[i].sTagValue , &lTagMds,sWork,&nLen) == SUCCESS)
		{

			if (debug_option != 1)
			{
				if (   memcmp(aIccMdsTag[i].sTagValue, "5F34", 4) == 0 ||
					memcmp(aIccMdsTag[i].sTagValue, "9F27", 4) == 0 ||
					memcmp(aIccMdsTag[i].sTagValue, "5A", 2) == 0 ||
					memcmp(aIccMdsTag[i].sTagValue, "57", 2) == 0)
					sprintf(sLine," >> %s|%s : [%03d] : [********************]", aIccMdsTag[i].sTagValue,aIccMdsTag[i].sTagDesc,nLen);
				else
					sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccMdsTag[i].sTagValue,aIccMdsTag[i].sTagDesc,nLen,sWork);
			}
			else
				sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccMdsTag[i].sTagValue,aIccMdsTag[i].sTagDesc,nLen,sWork);

			trace_event(sLine,TRACE);
		}
	}



	trace_event("End   MdsDumpIccFields()",PROCESSING);

}