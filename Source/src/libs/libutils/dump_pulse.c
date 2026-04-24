/***************************************************************************/
/*            UTILS_ECCF.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <globals.h>
#include <pulse.h>
#include <iso8583.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print
{
   char *description;
   char *value;
} t_tlv_print;


t_tlv_print  sTlvDataDesc[] = {
							       { "PDS_DES..............", "01"  },
							       { "PDS_FLAGS............", "02"  },
							       { "PDS_VER_NBR..........", "03"  },
							       { "PDS_SIGN_OFF_REASON..", "04"  },
							       { "PDS_REJ_MSG..........", "05"  },
							       { "PDS_REJ_REASON.......", "06"  },
							       { "PDS_TEST_PROD........", "07"  },
							       { "PDS_PINBLOCK_FORMAT..", "08"  },
							       { "PDS_CVM..............", "20"  },
							       { "PDS_CERT_SERIAL_NBR..", "40"  },
							       { "PDS_SECURITY_IND.....", "42"  },
							       { "PDS_UCAF.............", "43"  },
							       { "PDS_CVC_INVALID......", "87"  },
							       { "PDS_CVC_CHANGE.......", "88"  },
							       { "PDS_CVC_ERROR........", "89"  },
							       { "PDS_CVC2.............", "92"  },
							       { "PDS_MSG_ERR_IND......", "005" },
							       { "PDS_ENTITY_SELECTOR..", "601" },
							       { "PDS_REGION_LIST_DATA.", "602" },
							       { "PDS_CHARG_EFF_DATE...", "603" },
							       { "PDS_LAST_UPDATE_DATE.", "605" },
							       { "PDS_SERVICE_ID.......", "606" },
							       { "PDS_LIST_REG.........", "608" }
};

void PulseDumpIccFields (char *sIccData, int nIccLen);

void DumpPulse( char *data )
{
   char STRING_FIELD[LG_MAX];
   char sub_data[LG_MAX];
   char sWork[MAX_LINE_TRC];
   char sPdData[LG_MAX];
   char TI[2];
   int nLength;
   int i,j,k;
   TSPulseInfo msgInfo;
   TSTlvPulse  sTlvInfo;
   char       sTlvBuffer[LG_MAX];
   char       sLine[MAX_LINE_TRC];
   int        nList[]={200,210,220,230,430,420,422,520,530,810,800,620};
   int		  nRetCode;

   char CardTrunc[22 + 1];
   char CryptPart[22 + 1];

   TSPdInfo       pdInfo;
   TlvInfo        kTlvInfo;

   InitPulseDataInfo(&pdInfo);

   memset(CardTrunc, 0, sizeof(CardTrunc));
   memset(CryptPart, 0, sizeof(CryptPart));


  trace_event("Start DumpPulse()",PROCESSING);

	InitPulseInfo(&msgInfo);
  	AnalysePulse  ( data , &msgInfo);
    memset(sWork, 0, sizeof(sWork));

   trace_event("------------------------------------",TRACE);
   sprintf(sLine,"- M.T.I      : %04d" , msgInfo.nMsgType );
   trace_event(sLine,TRACE);

/* Control in message Type */

   nRetCode = NOK;
   for (i=0;i<sizeof(nList)/sizeof(int);i++)
   {

	if (msgInfo.nMsgType == nList[i])
	  {
		nRetCode = OK;
		break;
	  }
   }
   if(nRetCode != OK)
   {
	trace_event("Invalid Message ",FATAL);
    trace_event("End   DumpPulse(NOK)",TRACE);
	return;
   }

   trace_event("------------------------------------",TRACE);
   trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
   trace_event("------------------------------------",TRACE);

 for( i = 1 ; i < 127 ; i++)
 {
    switch (i + 1)
    {

	 case 2 :
		if (GetPulseField (i, &msgInfo , sWork, &nLength ) == SUCCES )
		{

            if (debug_option == 1)
           {
                sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
                trace_event(sLine,TRACE);
           }
			else
           {
                                TrunCard(sWork, CardTrunc);
                                        CryptCard(sWork, CryptPart);
                                        sprintf(sLine,"- FLD (%03d) : (%03d) : [%s][%s] " , i+1 , nLength ,CardTrunc,CryptPart );
                                        trace_event(sLine,TRACE);
           }
        }
                break;
	case 35:

        if ( GetPulseField(i, &msgInfo, sWork, &nLength ) == SUCCES )
        {
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
        }
       break;
    case 52 :
                if (GetPulseField (i, &msgInfo , sWork, &nLength ) == SUCCES )
           {
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
        }
       break;

    case 55 :
	   if (GetPulseField(i, &msgInfo, sWork, &nLength) == SUCCES) {
		   sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
		   trace_event(sLine,TRACE);
		   PulseDumpIccFields(sWork,nLength);
	   }
	   break;

    case 63:
       if (GetPulseField(i, &msgInfo, sWork, &nLength) == SUCCES) {
				sprintf(sLine, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLength,sWork);
				trace_event(sLine, TRACE);
/*
				AnalysePd(sWork, &pdInfo);
				memset(sPdData, 0, sizeof(sPdData));
				for (j = 0; j < 18; j++)
					if (GetPdSubField(j, &pdInfo, sPdData, &nLength) == SUCCES) {
						sprintf(sLine, "      > SUBFIELD (%02d) ....: [%s]"
							,j + 1, sPdData);
						trace_event(sLine, TRACE);
					}*/
			}
			break;
	/* start FZL 20220913 PLUTONL-4979*/
	case 125:
		if (GetPulseField(i, &msgInfo, sWork, &nLength) == SUCCES)
		{
			sprintf(sLine, "- FLD (%03d) : (%03d) ", i + 1, nLength);
			trace_event(sLine, TRACE);
			InitPulseKeyBlock(&kTlvInfo);
			AnalyseTlvInfo(sWork,nLength,&kTlvInfo);

			if( GetTlvInfoField (PLS_F110T_KP_KEY_BLOCK, &kTlvInfo, sWork, &nLength) == SUCCESS )
			{
				InitPulseTKPDataProp(&kTlvInfo);
				AnalyseTlvInfo(sWork, nLength, &kTlvInfo);
				sprintf(sLine,"- DS: [%s] ", PLS_F110T_KP_KEY_BLOCK );
				trace_event(sLine,TRACE);
				DumpTlvInfo(&kTlvInfo);
			}
		}
		break;
	/* start FZL 20220913 PLUTONL-4979*/
    default :
        if ( GetPulseField(i, &msgInfo, sWork, &nLength ) == SUCCES )
	{
          sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
          trace_event(sLine,TRACE);
	}
       break;
    }
 }
 trace_event( "-----------------------------------",TRACE);
 trace_event("End   DumpPulse()",TRACE);
}


void DumpPulse_struct( TSPulseInfo *Pulseinfo )
{
   char data[LG_MAX];
   memset(data, 0, sizeof(data));
   PulseBuildMsg ( data , Pulseinfo );
   DumpPulse ( data );
}



void PulseDumpIccFields (char *sIccData, int nIccLen)
{

	TSTagPulse       lTagPulse;
	typedef struct{
		char *sTagDesc;
		char *sTagValue;
		int  nPrintable;
	} ECCFTAG;

	ECCFTAG aIccPulseTag [] =
	{
		{"PULSE_TAG_TRANS_AMOUNT       ","9F02",1},
		{"PULSE_TAG_OTHER_AMOUNT       ","9F03",1},
		{"PULSE_TAG_APP_CRYPTOGRAM     ","9F26",1},
		{"PULSE_TAG_APP_ID             ","9F06",1},
		{"PULSE_TAG_APP_INTER_PROFILE  ","8200",1},
		{"PULSE_TAG_ATC                ","9F36",1},
		{"PULSE_TAG_APP_USAGE_CTRL     ","9F07",1},
		{"PULSE_TAG_CRYPTO_INFO_DATA   ","9F27",1},
		{"PULSE_TAG_CVM_RESULTS        ","9F34",1},
		{"PULSE_TAG_DED_FILE_NAME      ","8400",1},
		{"PULSE_TAG_IFD_SERIAL_NUM     ","9F1E",1},
		{"PULSE_TAG_ISS_APP_DATA       ","9F10",1},
		{"PULSE_TAG_ISS_AUTHEN_DATA    ","9100",1},
		{"PULSE_TAG_ISS_SCRIPT_RESULTS ","9F5B",1},
		{"PULSE_TAG_ISS_SCRIPT1        ","7100",1},
		{"PULSE_TAG_ISS_SCRIPT2        ","7200",1},
		{"PULSE_TAG_TERM_APP_VER_NUM   ","9F09",1},
		{"PULSE_TAG_TERM_CAP           ","9F33",1},
		{"PULSE_TAG_TERM_COUNTRY_CODE  ","9F1A",1},
		{"PULSE_TAG_TERM_TYPE          ","9F35",1},
		{"PULSE_TAG_TVR                ","9500",1},
		{"PULSE_TAG_TRANS_CATEG_CODE   ","9F53",1},
		{"PULSE_TAG_TRANS_CUR_CODE     ","5F2A",1},
		{"PULSE_TAG_TRANS_DATE         ","9A00",1},
		{"PULSE_TAG_TRANS_SEQ_NUM      ","9F41",1},
		{"PULSE_TAG_TRANS_TYPE         ","9C00",1},
		{"PULSE_TAG_UNPRED_NUMBER      ","9F37",1},
	};

	int  i = 0;
	int  nLen = 0;
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];

	memset(sWork, 0, sizeof(sWork));

	InitPulseIcTag (&lTagPulse);


	AnalysePulseIc (sIccData, nIccLen, &lTagPulse);

	for (i=0; i<(sizeof(aIccPulseTag)/sizeof(aIccPulseTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));

		if ( GetPulseIcTag (aIccPulseTag[i].sTagValue , &lTagPulse,sWork,&nLen) == SUCCESS)
		{
			if (debug_option != 1 && aIccPulseTag[i].nPrintable == NO_FLAG)
			{
				memset(sWork,'*',nLen);
			}
			sprintf(sLine,"     > [%s]|%s : [%03d] : [%s]", aIccPulseTag[i].sTagValue,aIccPulseTag[i].sTagDesc,nLen,sWork);
			trace_event(sLine,TRACE);
		}
	}

}