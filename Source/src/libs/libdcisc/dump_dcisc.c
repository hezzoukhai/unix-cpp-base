/****************************************************************************/
/***************************************************************************/
/*            UTILS_DCISC.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  DCISC 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <string.h>
#include <iso_dcisc.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <globals.h>

#define SUCCES  0
#define ECHEC  -1


typedef struct {
	char *sTagDesc;
	char *sTagValue;
	int  nPrintable;
} t_icc_dcisc_tag;


/* CFE EBETODO: verifier quels tags sont present dans les spec et nettoyer*/
static t_icc_dcisc_tag tIccDciscTag [] =
{
	{"ISO_TAG_ISS_SCRIPT1         ","7100",1},
	{"ISO_TAG_ISS_SCRIPT2         ","7200",1},
	{"ISO_TAG_ISS_SCRIPT_CMD      ","8600",0},
	{"ISO_TAG_APP_INTER_PROFILE   ","8200",1},
	{"ISO_TAG_DED_FILE_NAME       ","8400",1},
	{"ISO_TAG_ICC_APP_ID          ","4F00",1},
	{"ISO_TAG_AUTHOR_RESP_CODE    ","8A00",1},
	{"ISO_TAG_ISS_AUTHEN_DATA     ","9100",1},
	{"ISO_TAG_TVR                 ","9500",1},
	{"ISO_TAG_TRANS_DATE          ","9A00",1},
	{"ISO_TAG_TRANS_TYPE          ","9C00",1},
	{"ISO_TAG_TRANS_CUR_CODE      ","5F2A",1},
	{"ISO_TAG_TRANS_AMOUNT        ","9F02",1},
	{"ISO_TAG_OTHER_AMOUNT        ","9F03",1},
	{"ISO_TAG_TERM_APP_VER_NUM    ","9F09",1},
	{"ISO_TAG_ISS_APP_DATA        ","9F10",1},
	{"ISO_TAG_TERM_COUNTRY_CODE   ","9F1A",1},
	{"ISO_TAG_IFD_SERIAL_NUM      ","9F1E",1},
	{"ISO_TAG_APP_CRYPTOGRAM      ","9F26",1},
	{"ISO_TAG_CRYPTO_INFO_DATA    ","9F27",1},
	{"ISO_TAG_TERM_CAP            ","9F33",1},
	{"ISO_TAG_CVM                 ","9F34",1},
	{"ISO_TAG_TERM_TYPE           ","9F35",1},
	{"ISO_TAG_ATC                 ","9F36",1},
	{"ISO_TAG_UNPRED_NUMBER       ","9F37",1},
	{"ISO_TAG_TRANS_SEQ_NUM       ","9F41",1},
	{"ISO_TAG_TRANS_CATEG_CODE    ","9F53",1},
	{"ISO_TAG_APP_SEQ_NUMBER      ","5F34",0},
	{"ISO_TAG_ARPC                ","9F54",0},
	{"ISO_TAG_APP_SELECTED        ","9F06",1},
	{"ISO_TAG_TRACK2              ","5700",0},
	{"ISO_TAG_PAN                 ","5A00",0},
	{"ISO_TAG_APP_NAME   	      ","9F12",0},
	{"ISO_TAG_APP_LABEL	          ","5000",0},
	{"ISO_TAG_SECUR_ATT	          ","8C00",0},
	{"ISO_TAG_FFI                 ","9F6E",1},
	{"ISO_TAG_TRACK_1_DATA	      ","9F1F",0},
	{"ISO_TAG_APP_USE_CONTROL     ","9F07",1},
};

/* Start CFE 180921018 */

void DumpDciscFileData( TSDciscInfo* msgInfo )
{
	TSTlvPrivate   tlvInfo;
	char            sLine[MAX_LINE_TRC];
	char            sLineBuffer[MAX_LINE_TRC];
	char			CardTrunc[22 + 1];
	char            sWork[LG_MAX];
	char 			sTag [ 4 + 1], sValue[LG_MAX];
	unsigned char 	leftTag, rightTag;
	int             nLength;
	int             iMapCtr,i, j, nOffset=0, k=0;
	int  			nLen;
	
	memset(sLine, '\0',sizeof(sLine));
	memset(sWork, '\0',sizeof(sWork));

	iMapCtr = 1;
	while ( ( msgInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;
	
	
	sprintf(sLine, "Start DumpDciscFileData()");
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"MESSAGE_DINERS_XML_FORMAT_START[%04d]" , msgInfo->nMsgType );
    trace_event(sLine,TRACE);
	
	sprintf(sLine,"<?xml version=\"1.0\"?>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<BaseMessage Description=\"Diners base message template\">");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgType Value=\"%04d\"/>" , msgInfo->nMsgType );
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgHeader Value=\"\"/>");
	trace_event(sLine,TRACE);
	
	
	for( i = 1 ; i <= iMapCtr * 64 ; i++)
	{
		memset(sLine , '\0', sizeof(sLine));
		memset(sWork , '\0', sizeof(sWork));
		
		if ( GetDciscField(i, msgInfo, sWork, &nLength ) == SUCCES )
		{
			switch(i)
			{
				case DCISC_ICC_DATA :
				
					sprintf(sLine," <Field Number=\"%03d\" Value=\"" , i+1);
					
					for(j = 0 ; j < nLength;)
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
					while(k < strlen(sLine))
				    {
						memset(sLineBuffer,0,sizeof(sLineBuffer));
						if((strlen(sLine)-k)>93)
							memcpy(sLineBuffer,sLine+k,93);
						else
							memcpy(sLineBuffer,sLine+k,strlen(sLine)-k);
					
						trace_event(sLineBuffer ,TRACE);
						k+=93;
				    }
					break;
			   	/* case DCISC_CPS_FIELDS : /* CFE * /
				     memset(sValue, 0, sizeof(sValue));
					 nLength = HexToAscii(sWork, sValue, nLength*2); 
					 sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1, sValue);
					 k=0;
				     while(k < strlen(sLine))
				     { 
						memset(sLineBuffer,0,sizeof(sLineBuffer));
						if((strlen(sLine)-k)>93)
							memcpy(sLineBuffer,sLine+k,93);
						else
							memcpy(sLineBuffer,sLine+k,strlen(sLine)-k);
					
						trace_event(sLineBuffer ,TRACE);
						k+=93;
				     }
					 break;  */ /* ELKAM20230217 PLUTONL-5914 */
				default:
					sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
					k=0;
					
				    while(k < strlen(sLine))
				    {
						memset(sLineBuffer,0,sizeof(sLineBuffer));
						if((strlen(sLine)-k)>93)
							memcpy(sLineBuffer,sLine+k,93);
						else
							memcpy(sLineBuffer,sLine+k,strlen(sLine)-k);
					
						trace_event(sLineBuffer ,TRACE);
						k+=93;
				    }
					break;		
			}
		}
	}
	
	sprintf(sLine,"</MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"</BaseMessage>");
	trace_event(sLine,TRACE);
	
    sprintf(sLine,"MESSAGE_Diners_XML_FORMAT_END[%04d]" , msgInfo->nMsgType );
	trace_event(sLine,TRACE);
	
	trace_event  ("------------------------------------",TRACE);
	sprintf(sLine, "End DumpDciscFileData()");
	trace_event  (sLine,TRACE);
	memset(sLine, '\0',sizeof(sLine));
	memset(sWork, '\0',sizeof(sWork));
	
}
/* End CFE */

void DumpDciscStruct(TSDciscInfo* msgInfo)
{

	char	sWork[BUF_LG_MAX];
	char	sLine[MAX_LINE_TRC];
	char	CardTrunc[22 + 1];
	int		nLength;
	int		iMapCtr,i,j,k;
	int		nPos = 0;
	
	char sTag [ 4 + 1], sValue[LG_MAX];
	unsigned char leftTag,rightTag;
	int  nLen;
	

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sWork,     0, sizeof(sWork));
	


	trace_event("Start DumpDciscStruct()",PROCESSING);
	

	iMapCtr = 1;
	while ( ( msgInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", msgInfo->sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);

	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"- M.T.I      : %04d" , msgInfo->nMsgType ); 
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
	trace_event("------------------------------------",TRACE);


	for( i = 1 ; i < 128 ; i++)
	{
		if ( GetDciscField(i, msgInfo, sWork, &nLength ) != SUCCES )
		{
			continue;
		}
		switch (i+1)
		{
		case 2: 
			if (debug_option == 1)
			{
				sprintf(sLine,"-- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);
			}
			else        
			{
				TrunCard(sWork, CardTrunc);
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,CardTrunc );
				trace_event(sLine,TRACE);
			}
			break;
		case 35: 
		case 45: 
		case 52: 
			if (debug_option == 1)
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);
			}
			else
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
				trace_event(sLine,TRACE);
			}
			break;	

		case 55 :

			sprintf(sLine,"- FLD (%03d)   (%03d)" , i+1, nLength );
			trace_event(sLine,TRACE);
			/* CFE J = 0 ==> EBETODO: why j = 3? */
			for( j = 0; j < nLength; )
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
						memcmp(sTag, "57",   2) == 0 || 
						memcmp(sTag, "9F1F", 4) == 0 || 
						memcmp(sTag, "5A",   2) == 0) 
						sprintf(sLine,"      > TAG (%.4s) ....: [***** Len %lu *****]" , sTag , strlen( sValue )   );
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
				}
				else
					sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );

				trace_event(sLine,TRACE);

				j += nLen;
			}
			break;
		/* case 62 : /* CFE * /
			 memset(sValue, 0, sizeof(sValue));
			 nLength = HexToAscii(sWork, sValue, nLength*2); 
			 sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sValue);
			 trace_event(sLine,TRACE);
			 break; */ /* ELKAM20230217 PLUTONL-5914 */
		default :
			sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
			trace_event(sLine,TRACE);
			break;
		}
	}

	trace_event("------------------------------------",TRACE);
	trace_event("End   DumpDcisc()",PROCESSING);
	
	trace_event  ("------------------------------------",TRACE);
	if (debug_option == 1) 
		DumpDciscFileData(msgInfo);
	memset(sLine, '\0',sizeof(sLine));
	memset(sWork, '\0',sizeof(sWork));
	return;


}


void dump_dcisc_buffer( char *data )
{
	TSDciscInfo msgInfo;
	InitDciscInfo(&msgInfo);
	AnalyseDcisc  ( data , &msgInfo);
	DumpDciscStruct(&msgInfo);

	return;
}




int PrintDciscIccSubFields (char *sIccData, int nIccLen)
{
	TSTagDcisc     DciscIccInfo;
	char           sWork[LG_MAX];
	char           sLine[MAX_LINE_TRC];
	int            i = 0;
	int            nLen = 0;

	memset(sWork, '\0', sizeof(sWork));
	

	InitDciscIcTag (&DciscIccInfo);
	AnalyseDciscIc (sIccData, nIccLen, &DciscIccInfo);

	for (i=0; i<(sizeof(tIccDciscTag)/sizeof(tIccDciscTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));

		if ( GetDciscIcTag (tIccDciscTag[i].sTagValue , &DciscIccInfo,sWork,&nLen) == SUCCESS)
		{
			if (debug_option != 1 && tIccDciscTag[i].nPrintable == NO_FLAG)
			{
				memset(sWork,'*',nLen);
			}
			sprintf(sLine,"     > [%s]|%s ....: [%03d] : [%s]", tIccDciscTag[i].sTagValue,tIccDciscTag[i].sTagDesc,nLen,sWork);
			trace_event(sLine,TRACE);
		}
	}
	return(OK);
}



void DciscDumpIccFields (char *sIccData, int nIccLen)
{

	trace_event("Start DciscDumpIccFields()",PROCESSING);
	trace_event("         Field                        |  Len  |              Value",TRACE);
	trace_event("-------------------------------------------------------------------",TRACE);
	PrintDciscIccSubFields (sIccData, nIccLen);
	trace_event("-------------------------------------------------------------------",TRACE);
	trace_event("End   DciscDumpIccFields()",PROCESSING);

}


