#include <stdio.h>
#include <iso_jcc.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <define.h>


#include <globals.h> /* MJA PCI 220411 */

typedef struct s_tlv_print
{
   char *description;
   char *value;
} t_tlv_print;

/*YK220206:EMV*/ 
void JccDumpIccFields (char *sIccData, int nIccLen)
{

TSTagJcc       lTagJcc;
typedef struct{
    char *sTagDesc;
    char *sTagValue;
} JCCTAG;

JCCTAG aIccJccTag [] =
{
{"JCC_TAG_ISS_SCRIPT1         ","7100"},
{"JCC_TAG_ISS_SCRIPT2         ","7200"},
{"JCC_TAG_ISS_SCRIPT_CMD      ","8600"},
{"JCC_TAG_APP_INTER_PROFILE   ","8200"},
{"JCC_TAG_DED_FILE_NAME       ","8400"},
{"JCC_TAG_ICC_APP_ID          ","4F00"},
{"JCC_TAG_AUTHOR_RESP_CODE    ","8A00"},
{"JCC_TAG_ISS_AUTHEN_DATA     ","9100"},
{"JCC_TAG_TVR                 ","9500"},
{"JCC_TAG_TRANS_DATE          ","9A00"},
{"JCC_TAG_TRANS_TYPE          ","9C00"},
{"JCC_TAG_TRANS_CUR_CODE      ","5F2A"},
{"JCC_TAG_TRANS_AMOUNT        ","9F02"},
{"JCC_TAG_OTHER_AMOUNT        ","9F03"},
{"JCC_TAG_TERM_APP_VER_NUM    ","9F09"},
{"JCC_TAG_ISS_APP_DATA        ","9F10"},
{"JCC_TAG_TERM_COUNTRY_CODE   ","9F1A"},
{"JCC_TAG_IFD_SERIAL_NUM      ","9F1E"},
{"JCC_TAG_APP_CRYPTOGRAM      ","9F26"},
{"JCC_TAG_CRYPTO_INFO_DATA    ","9F27"},
{"JCC_TAG_TERM_CAP            ","9F33"},
{"JCC_TAG_CVM                 ","9F34"},
{"JCC_TAG_TERM_TYPE           ","9F35"},
{"JCC_TAG_ATC                 ","9F36"},
{"JCC_TAG_UNPRED_NUMBER       ","9F37"},
{"JCC_TAG_TRANS_SEQ_NUM       ","9F41"},
{"JCC_TAG_TRANS_CATEG_CODE    ","9F53"},
{"JCC_TAG_APP_SEQ_NUMBER      ","5F34"},
{"JCC_TAG_ARPC                ","9F54"},
{"JCC_TAG_SEC_PIN_DATA        ","C000"}, /* ACK PIN CHANGE MARCH2015 */
{"JCC_TAG_FFI                 ","9F6E"}, /*SWI06032015 ENH15.1*/
};

int  i = 0;
int  nLen = 0;
char sWork[LG_MAX];
char sLine[MAX_LINE_TRC];

trace_event("Start JccDumpIccFields()",PROCESSING);

memset(sWork, 0, sizeof(sWork));
memset(sLine, 0, sizeof(sLine));

InitJccIcTag (&lTagJcc);



AnalyseJccIc (sIccData, nIccLen, &lTagJcc);



trace_event("         Field                        |  Len  |              Value",TRACE);

for (i=0; i<(sizeof(aIccJccTag)/sizeof(aIccJccTag[0])); i++)
{
	memset(sWork, 0, sizeof(sWork));

	if ( GetJccIcTag (aIccJccTag[i].sTagValue , &lTagJcc,sWork,&nLen) == SUCCESS) 
	{
		sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccJccTag[i].sTagValue,aIccJccTag[i].sTagDesc,nLen,sWork);
		trace_event(sLine,TRACE);
	}
}



trace_event("End   JccDumpIccFields()",PROCESSING);

}
/*YK220206:EMV*/
void DumpBanks ( char *data )
{
   char sub_data[LG_MAX];
   char sWork[LG_MAX];
   char sLine[LG_MAX];

   char TI[2];
   int nLength;
   int i,j,k,iMapCtr,nPos=0;
   TSJccInfo msgInfo;
   TSTlvPrivate tlvInfo;

/** START MJA PCI 270411  */

   char CardTrunc[22 + 1];
   char CryptPart[22 + 1];
   memset(CardTrunc, 0, sizeof(CardTrunc));
   memset(CryptPart, 0, sizeof(CryptPart));

/** END MJA PCI 270411   */


   trace_event("Start DumpBanks()",PROCESSING);


   InitJccInfo(&msgInfo);

   AnalyseJcc  ( data , &msgInfo);

   trace_event("------------------------------------",TRACE);
   sprintf(sLine ,"- M.T.I      : %04d" , msgInfo.nMsgType ); 
   trace_event(sLine,TRACE);
   trace_event("------------------------------------",TRACE);

      iMapCtr = 1;
      nPos = 0;
      while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
        ++iMapCtr;

      memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
      for (i=0; i<iMapCtr * 8;i++)
      {
                sprintf(sLine + nPos," %02X", msgInfo.sBitMap[i]);
        nPos += 3;
      }
      trace_event(sLine,TRACE);

   trace_event("------------------------------------",TRACE);


   trace_event("- FLD (FIELD): (LEN) : CONTENT             ",TRACE);
   trace_event("------------------------------------",TRACE);

   memset(sWork, 0, sizeof(sWork));



 for( i = 1 ; i < 127 ; i++)
 {
    memset(sWork, 0, sizeof(sWork));
    switch (i+1)
    {

/** START MJA PCI 270411  **/

     case 2:

        if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES )
         {
          if (debug_option == 1)
           {
            sprintf(sLine,"-- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
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

        if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES )
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

     case 45:

        if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES ) {
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

       case 52:
        if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES )
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

      /** END MJA PCI 270411 **/
		    case 55 :       
	   if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES )
       {
       char sTag [ 4 + 1], sValue[LG_MAX];
       int  nLen,j;
	   unsigned char leftTag,rightTag;

          sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
          trace_event(sLine,TRACE);
          for( j = 0 ; j < nLength; )
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
             nLen = sWork[j++];

             HexToAscii(sWork+j,sValue,nLen*2);

			if (debug_option != 1)
			{
			 if ( memcmp(sTag, "5F34", 4) == 0 || memcmp(sTag, "9F27", 4) == 0 || memcmp(sTag, "57", 2) == 0)
				 sprintf(sLine,"      > TAG (%.4s) ....: [**** Len %lu ****]" , sTag  , strlen(sValue)   );
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
    default :
        if ( GetJccField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
			/*if(i+1 == 55)
			JccDumpIccFields (sWork, nLength);
			else
			*//*YK23032006:core dump when big script*/
			{
                        sprintf(sLine ,"- FLD (%03d) : (%03d) : [%s]" , i+1 , nLength, sWork );
   		        trace_event(sLine,TRACE);
   		        }
		}

       break;
    }
 }

   trace_event("------------------------------------",TRACE);
   trace_event("End   DumpBanks()",PROCESSING);
}

 
