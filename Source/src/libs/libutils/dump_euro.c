/***************************************************************************/
/*            UTILS_ECCF.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <iso_euro.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print
{
   char *description;
   char *value;
} t_tlv_print;


t_tlv_print  sPdsDataDesc[] = {
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


void DumpEuro( char *data )
{
   char STRING_FIELD[LG_MAX];
   char sub_data[LG_MAX];
   char sWork[LG_MAX];
   char TI[2];
   int nLength;
   int i,j,k;
   TSEuroInfo msgInfo;
   TSPdsEuro  sPdsInfo;
   char       sPdsBuffer[LG_MAX];
   char       sLine[MAX_LINE_TRC];
   int        nList[]={430,100,420,421,121,120,810,110,130,800};
   int		  nRetCode;


  trace_event("Start DumpEuro()",PROCESSING);

	InitEuroPds(&sPdsInfo);
	InitEuroInfo(&msgInfo);
  	AnalyseEuro  ( data , &msgInfo);
    memset(sWork, 0, sizeof(sWork));

   trace_event("------------------------------------",TRACE);
   sprintf(sLine,"- M.T.I      : %04d" , msgInfo.nMsgType ); 
   trace_event(sLine,TRACE);

/* Control in message Type */

   nRetCode = NOK;
   for (i=0;i<sizeof(nList)/sizeof(int);i++)
   {
/*
	  sprintf(sLine," Message Type Is %d",nList[i]);
      trace_event(sLine,TRACE);
*/
      if (msgInfo.nMsgType == nList[i])
	  {
		nRetCode = OK;
		break;
	  }
   }
   if(nRetCode != OK)
   {
	trace_event("Invalid Message ",FATAL);
    trace_event("End   DumpEuro(NOK)",TRACE);
	return;
   }

   trace_event("------------------------------------",TRACE);
   trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
   trace_event("------------------------------------",TRACE);

 for( i = 1 ; i < 127 ; i++)
 {
    switch (i + 1)
    {

    case 48 :
 
   if (GetEuroField (i, &msgInfo , sWork, &nLength ) == SUCCES )
   {
     InitEuroPds (&sPdsInfo);
     AnalyseEuroPds ( sWork + 4, nLength - 4 , msgInfo.nMsgType , &sPdsInfo);
     sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
     trace_event(sLine,TRACE);
     for (j=0; j<(sizeof(sPdsDataDesc)/sizeof(sPdsDataDesc[0])); j++)
            {
                if ( GetEuroPds(  sPdsDataDesc[j].value , &sPdsInfo, sub_data , &nLength) == SUCCES)
                {
                    sub_data[nLength] = 0;
                    sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPdsDataDesc[j].value,
                                        sPdsDataDesc[j].description,
                                        sub_data );
		    trace_event(sLine,TRACE);
                }
            }
      }

       break;


    default :
        if ( GetEuroField(i, &msgInfo, sWork, &nLength ) == SUCCES )
	{
          sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
          trace_event(sLine,TRACE);
	}
       break;
    }
 }
 trace_event( "-----------------------------------",TRACE); /*EBE140506: Adding ,TRACE*/
 trace_event("End   DumpEuro()",TRACE);
}

void EuroDumpIccFields (char *sIccData, int nIccLen)
{

TSTagEuro       lTagEuro;
typedef struct{
    char *sTagDesc;
    char *sTagValue;
} ECCFTAG;

ECCFTAG aIccEuroTag [] =
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

trace_event("Start EuroDumpIccFields()",PROCESSING);

memset(sWork, 0, sizeof(sWork));

InitEuroIcTag (&lTagEuro);


AnalyseEuroIc (sIccData, nIccLen, &lTagEuro);

trace_event("         Field                         | Len  |              Value",TRACE);

for (i=0; i<(sizeof(aIccEuroTag)/sizeof(aIccEuroTag[0])); i++)
{
	memset(sWork, 0, sizeof(sWork));

	if ( GetEuroIcTag (aIccEuroTag[i].sTagValue , &lTagEuro,sWork,&nLen) == SUCCESS) 
	{
		sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccEuroTag[i].sTagValue,aIccEuroTag[i].sTagDesc,nLen,sWork);
		trace_event(sLine,TRACE);
	}
}



trace_event("End   EuroDumpIccFields()",PROCESSING);

}
